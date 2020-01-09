#ifndef VIDEO_H
#define VIDEO_H

#include "memory.h"
#include <QColor>
#include <QImage>
#include <QObject>


class graphics : public QObject
{
    Q_OBJECT
/* 256x256 total screen, 160x144 viewable
 * or 32x32 tiles (8x8 each)
 * scrollx, scrolly upper left loc of background image
 *
 * vram background tile 32 rows of 32 bytes - contains # of a tile displayed
 * 8000-8FFF case 1: unsigned
 * 8800-97FF case 2: signed -128 to 127
 *
 * window over background
 * wndposx-7, wndposy location
 */

// colors
// white: 0b11, darkgrey 0b10, lightgrey 0b01, black 0b00

/* Tile (8x8)
 * 1-3 represent colors
 * 2 bytes per line
 * sample: .33333..
 * bits:   01111100
 *         01111100 hex: 0x7C 7C
 *
 * 00000000
 * 11000110
 * 22...22.
 */

public:
    explicit graphics(memory *gbMem, QObject *parent = 0);

    //update line of lcd
    void updateScanline(int cycles);
    //render static background/window tiles
    void renderTiles();
    //render sprites (characters) on background
    void renderSprites();

    QImage sendFrame();


signals:
    void frameCompleted();

private:

    memory *mem;
    //get color from 2 bits and palette address
    QColor getColour(uint8_t bits, uint16_t paletteAddr);
    int scanLineCount;

    const int TILESIZE = 16;

    //status of lcd
    void LCDStatus();
    //lcd enabled
    bool lcdEnabled();
    void drawScanLine();

    QImage bgPixels;
    QImage spritePixels;

    QColor greyShades[5];

};

#endif // VIDEO_H
