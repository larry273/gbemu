#include "graphics.h"

graphics::graphics(memory *gbMem, QObject *parent) : QObject(parent){
    mem = gbMem;
    scanLineCount = 456;

    //initalize colors
    greyShades[0x00] = QColor(255, 255, 255);
    greyShades[0x01] = QColor(198, 198, 198);
    greyShades[0x02] = QColor(127, 127, 127);
    greyShades[0x03] = QColor(0, 0, 0);
    greyShades[0x04] = QColor(255, 0, 0); //red error

    //initializte images
    bgPixels = QImage(160, 144, QImage::Format_ARGB32);
    bgPixels.fill(QColor(255, 255, 255));

    spritePixels = QImage(160, 144, QImage::Format_ARGB32);
    spritePixels.fill(QColor(255, 255, 255));


}

void graphics::updateScanline(int cycles){
    //144 visible lines, 153 total scanlines, 7 are invisible
    //456 cpu clock cycles to draw one line

    //update lcd mode
    LCDStatus();

    if (lcdEnabled()){
        scanLineCount -= cycles;
    }
    else {
        return;
    }

    //draw the entire scan line after 456 cycles have passed
    if (scanLineCount <= 0){
        //increment scan line and read
        mem->smallMem[0x44]++; //cant use write as it will reset
        uint8_t line = mem->read(0xFF44);
        scanLineCount = 456;

        //std::cout << "CURRENT SCANLINE: " << HEX(line) << "\n";

        if (line == 144){
            //interrupt
            emit frameCompleted();
        }
        else if (line > 153){
            mem->smallMem[0x44] = 0; //reset to zero
        }
        else if (line < 144){
            drawScanLine();
        }
    }
}

void graphics::drawScanLine(){
    uint8_t lcdReg = mem->read(0xFF40);

    //bit 0 set
    if (lcdReg & 0x01){
        renderTiles();
    }

    //bit 1 set
    if (lcdReg & 0x02) {
        renderSprites();
    }
}

/* LCD Register
 * 7 - lcd enable
 * 6 - window tile identity loc
 * 5 - window enable
 * 4 - bg & window tile data
 * 3 - bg tile identity loc
 * 2 - sprite size
 * 1 - sprite display enable
 * 0 - bg display enable
 */

//render the bg and the window tiles for the frame
void graphics::renderTiles(){

    uint8_t scrollY = mem->read(0xFF42) ;
    uint8_t scrollX = mem->read(0xFF43) ;
    uint8_t windowY = mem->read(0xFF4A) ;
    uint8_t windowX = mem->read(0xFF4B) - 7;

    uint8_t lcdReg = mem->read(0xFF41);
    const uint8_t scanLine = mem->read(0xFF44);
    uint16_t tileBase;
    int offset = 0; //128 for signed section

    bool renderWindow = false;
    int identityBit = 0x08; //bit 3 for bg
    //bit 5 window enabled?
    if (lcdReg & 0x20){
        if (windowY <= scanLine){
            renderWindow = true;
            identityBit = 0x40; // test bit 6 for window
        }
    }

    //bit 4 test for tile mem location
    /*
    if (lcdReg & 0x10){
        tileBase = 0x8000; //unsigned bytes
    }
    else{
        tileBase = 0x8800; //signed bytes
        offset = 128;
    }
    */
    //FIX
    tileBase = 0x8000;

    uint16_t identityLoc;
    //test bit 6/3 for window/bg identity loc
    if (lcdReg & identityBit){
        identityLoc = 0x9C00;
    }
    else{
        identityLoc = 0x9800;
    }

    int yPos;
    if (!renderWindow){
        yPos = scanLine + scrollY;
    }
    else{
        yPos = scanLine - windowY;
    }

    int tileRow = uint8_t(yPos / 8) * 32;
    //draw scanline pixels
    for (int x = 0; x < 160; x++){
        int xPos = x + scrollX;
        if (renderWindow && x >= windowX){
            xPos = x - windowX;
        }

        int tileCol = xPos / 8;
        uint16_t tileAddr = identityLoc + tileCol + tileRow;

        uint16_t tileLoc;
        if (renderWindow){
            int8_t tileNum = int8_t(mem->read(tileAddr));
             tileLoc = tileBase + ((tileNum + offset) * 16);
        }
        else {
            uint8_t tileNum = uint8_t(mem->read(tileAddr));
             tileLoc = tileBase + ((tileNum + offset) * 16);
        }

        int line = (yPos % 8) * 2;
        uint8_t byte1 = mem->read(tileLoc + line);
        uint8_t byte2 = mem->read(tileLoc + line + 1);

        //determine current bit in tile bytes
        int colorBit = abs((xPos % 8) - 7);

        //get color number from 2 tile bytes
        int colorNumber = (byte2 >> colorBit) & 0x1;
        colorNumber <<= 1;
        colorNumber |= (byte1 >> colorBit) & 0x1;

        //bg and window palette 0xFF47
        QColor color = getColour(colorNumber, 0xFF47);
        bgPixels.setPixel(x, scanLine, color.rgb());
    }
}

QImage graphics::sendFrame(){
    //QImage test = QImage("/home/larry/Documents/gbemu/gbemulator/image.jpg");
    return bgPixels;
}

void graphics::renderSprites(){

}

//update lcd display operation modes and check interrupts
void graphics::LCDStatus(){
    uint8_t lcdStatus = mem->read(0xFF41);
    //lcd not enabled
    if (!lcdEnabled()){
        scanLineCount = 456;
        mem->write(0xFF44, 0);
        //set V-BLANK mode
        lcdStatus = (lcdStatus & 0xFC) | 0x01;
        mem->write(0xFF41, lcdStatus);
        return;
    }

    uint8_t currentLine = mem->read(0xFF44);
    uint8_t currentMode = lcdStatus & 0x3; //lcd mode LSB 2 bits
    bool doInterrupt = false;

    int mode = 0;
    //mode operation
    //Search Sprites - first 80 cycles
    //Transfer data - next 172 cycles
    //H-Blank - remaining cycles
    //on mode change, lcd interrupt TODO

    //frame completed, reset to V-BLANK
    if (currentLine >= 144){
        //VBLANK = 01
        mode = 1;
        lcdStatus = (lcdStatus & 0xFD) | 0x01;
        doInterrupt = (lcdStatus >> 4) & 0x01; //check bit 4 set
    }

    //drawing frame modes
    else{
        //search sprite mode 10
        if(scanLineCount >= (456-80)) {
            mode = 2;
            lcdStatus = (lcdStatus & 0xFE) | 0x02;
            doInterrupt = (lcdStatus >> 5) & 0x01; //check bit 5 set
        }
        //transfer data mode 11
        else if (scanLineCount >= (456-80-172)){
            mode = 3;
            lcdStatus = (lcdStatus | 0x03);
        }
        //h-blank mode 00
        else{
            mode = 0;
            lcdStatus = (lcdStatus & 0xFC);
            doInterrupt = (lcdStatus >> 3) & 0x01; //check bit 5 set
        }
    }

    if (doInterrupt && (mode != currentMode)){
        //do interrupt
    }

    //coincidence flag
    if (currentLine == mem->read(0xFF45)){
        //set to bit 2
        lcdStatus = lcdStatus | 0x04;
        //if bit 6 is set
        if (lcdStatus & 0x40){
            //do interrupt
        }
    }
    else{
        //set to bit 2 to 0
        lcdStatus = lcdStatus & 0xFB;
    }

    mem->write(0xFF41, lcdStatus);
}

//
QColor graphics::getColour(uint8_t bits, uint16_t paletteAddr){
    uint8_t palette = mem->read(paletteAddr);
    //bg pallette 0xFF47
    //sprite pallete 0xFF48 or 0xFF49
    //7-6 11, 5-4 10, 3-2 01, 2-1 00
    uint8_t color;

    switch (bits) {
    case 0x0:
        color = palette & 0x03;
        break;
    case 0x01:
        color = (palette >> 2) & 0x03;
        break;
    case 0x02:
        color = (palette >> 4) & 0x03;
        break;
    case 0x03:
        color = palette >> 6;
        break;
    default:
        //error red pixel
        color = 0x04;
        break;
    }
    return greyShades[color];
}


bool graphics::lcdEnabled(){
    //test lcd register bit 7, set
    return  (mem->read(0xFF40) & 0x80);
}

