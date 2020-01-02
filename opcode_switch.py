import csv


#read in opcodes create switch for c++
#case 0x00: std::cout << "NOP\n"; break;

beg = "switch (code) {\n"
end = "\tdefault: std::cout << \"***Instruction not recognized*** \" << HEX(code) << \"\\n\"; break; \n}\n"

out_file = "switch.txt"
file_name = "istr_set.csv" 

data = []
with open(file_name, 'r') as f:
    reader = csv.reader(f)
    data = list(list(rec) for rec in csv.reader(f, delimiter='|')) #reads csv into a list of lists


flag_dict = {
    "NZ": "~FLAGZERO",
    "Z": "FLAGZERO",
    "NC": "~FLAGCARRY",
    "C": "FLAGCARRY",
}

#remove spaces, commas, generate hex rows
pdata = []
k = []
l = []
for j,row in enumerate(data):
    new_row = []
    for i, r in enumerate(row):

        if j == 0:
            if r == '0': continue
            y = str(r).replace(' ','').replace(',','')
            k.append(y)

        elif i == 0:
            l.append(r.strip())

        else:
            r = str(r)
            r = r.replace(',', ' ')
            new_row.append(r)
    if j != 0:    
        pdata.append(new_row)

#generate hexes
hexes = []
for i in l:
    row = []
    for j in k:
        val = i[0] + j[-1]
        row.append(val)
    hexes.append(row)

with open(out_file, 'w') as f:
    f.write(beg)
    for j, row in enumerate(pdata):
        for i, r in enumerate(row):
            hex = hexes[j][i]

            opcode = r.strip()
            func = ""

            
            if r == ' ':
                continue
            elif 'nn' in r:
                #3 byte row, (nn)
                if 'LD' in opcode and opcode.split()[1] == '(nn)':
                    reg = opcode.split()[2]
                    row = (f"\tcase 0x{hex}:{{\n" 
                    "\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];\n"
                    f"\t\tload( joinByte(val1, val2), reg.{reg});\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; break;\n\t}}\n")

                elif 'LD' in opcode and opcode.split()[2] == '(nn)':
                    reg = opcode.split()[1]
                    row = (f"\tcase 0x{hex}:{{\n" 
                    "\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];\n"
                    f"\t\tload(reg.{reg}, joinByte(val1,val2) );\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; break;\n\t}}\n")

                elif opcode == 'LD SP nn':
                    row = (f"\tcase 0x{hex}:{{\n" 
                    "\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];\n"
                    f"\t\tstd::cout << \" {opcode} \\n\"; loadSP(val1, val2); opbytes = 3; break;\n\t}}\n")


                elif 'LD' in opcode:
                    regs = opcode.split()[1]
                    row = (f"\tcase 0x{hex}:{{\n" 
                    "\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];\n"
                    f"\t\tload(reg.{regs[0]}, reg.{regs[1]}, val1, val2);\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; break;\n\t}}\n")
                
                #call operations
                elif 'CALL nn' in opcode:
                    row = (f"\tcase 0x{hex}:{{\n" 
                    "\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];\n"
                    f"\t\tcall(val1, val2);\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; break;\n\t}}\n")
                elif 'CALL' in opcode:
                    flag = flag_dict[opcode.split()[1]]
                    row = (f"\tcase 0x{hex}:{{\n" 
                    f"\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];\n"
                    f"\t\tbool jump = call(val1, val2, {flag});\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; if (jump) {{isJump = true ;}} break;\n\t}}\n") 


                #jump operations
                elif 'JP' in opcode and len(opcode.split()) == 2:
                    row = (f"\tcase 0x{hex}:{{\n" 
                    "\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2]; \n"
                    f"\t\tjp( joinByte(val1,val2) );\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; isJump = true ; break;\n\t}}\n")    
                elif 'JP' in opcode:
                    flag = flag_dict[opcode.split()[1]]
                    row = (f"\tcase 0x{hex}:{{\n" 
                    f"\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2]; \n"
                    f"\t\tbool jump = jp( joinByte(val1,val2), {flag});\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; if (jump) {{isJump = true ;}} break;\n\t}}\n") 

                else:
                    opcode = opcode + " | NOT IMPLEMENTED "
                    row = f"\tcase 0x{hex}: \n\t\tstd::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << \"\\n\"; opbytes = 3; break;\n"

            elif 'n' in r or 'e' in r:
                
                #2 byte row, (n)
                if 'LD' in opcode and len(opcode.split()[1]) == 1 and len(opcode.split()[2]) == 1:
                    row = (f"\tcase 0x{hex}:{{\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << \"\\n\"; opbytes = 2;\n"
                    f"\t\tload(reg.{opcode.split()[1]}, gbdata.buffer[pc+1]);"
                    "break;\n\t}\n")
                elif len(opcode.split()) == 2 and 'n' in r: #add,adc,sub,sbc,xor,or,cp
                    name = opcode.split()[0].lower()
                    if 'or' in name or 'and' in name:
                        name = 'log' + name.capitalize()

                    row = (f"\tcase 0x{hex}:{{\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << \"\\n\"; opbytes = 2;\n"
                    f"\t\t{name}(reg.A, gbdata.buffer[pc+1]);"
                    "break;\n\t}\n")
                elif 'LD (HL) n' == opcode:
                    row = (f"\tcase 0x{hex}:{{\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << \"\\n\"; opbytes = 2;\n"
                    f"\t\tload( pairReg(reg.H, reg.L ), gbdata.buffer[pc+1]);"
                    "break;\n\t}\n")
                elif 'LD HL SP+e' == opcode:
                    row = (f"\tcase 0x{hex}:{{\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << \"\\n\"; opbytes = 2;\n"
                    f"\t\tloadSP2Reg( reg.H, reg.L); addByte2SP(gbdata.buffer[pc+1]); "
                    "break;\n\t}\n")

                #ldh 
                elif 'LDH A (n)' == opcode:
                    row = (f"\tcase 0x{hex}:{{\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << \"\\n\"; opbytes = 2;\n"
                    f"\t\t ldh_F0(gbdata.buffer[pc+1]);"
                    "break;\n\t}\n")
                elif 'LDH (n) A' == opcode:
                    row = (f"\tcase 0x{hex}:{{\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << \"\\n\"; opbytes = 2;\n"
                    f"\t\t ldh_E0(gbdata.buffer[pc+1]);"
                    "break;\n\t}\n")


                #jr operations
                elif 'JR' in opcode and len(opcode.split()) == 2:
                    row = (f"\tcase 0x{hex}:{{\n"
                    "\t\tint8_t val1 = gbdata.buffer[pc+1]; \n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << \"\\n\"; opbytes = 2;\n"
                    f"\t\tjr(val1);"
                    "break;\n\t}\n")
                elif 'JR' in opcode:
                    flag = flag_dict[opcode.split()[1]]
                    row = (f"\tcase 0x{hex}:{{\n"
                    "\t\tint8_t val1 = gbdata.buffer[pc+1]; \n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << \"\\n\"; opbytes = 2;\n"
                    f"\t\tjr(val1, {flag});"
                    "break;\n\t}\n")
                elif 'ADD SP e' in opcode:
                    row = (f"\tcase 0x{hex}:{{\n"
                    "\t\tint8_t val1 = gbdata.buffer[pc+1]; \n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << \"\\n\"; opbytes = 2;\n"
                    f"\t\t addByte2SP(val1);"
                    "break;\n\t}\n")

                else:
                    opcode = opcode + " | NOT IMPLEMENTED "
                    row = f"\tcase 0x{hex}: std::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << \"\\n\"; opbytes = 2; break;\n"

            else:
                

                if 'XOR' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" logXor(reg.A, reg.{opcode[-1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" logXor(reg.A, pairRegData(reg.{reg1}, reg.{reg2}));"
                
                elif 'OR' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" logOr(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" logOr(reg.A, pairRegData(reg.{reg1}, reg.{reg2}));"

                elif 'AND' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" logAnd(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" logAnd(reg.A, pairRegData(reg.{reg1}, reg.{reg2}));"

                elif 'NOP' in opcode or 'HALT' in opcode:
                    func = f"" 
                
                #Increments
                elif 'INC SP' == opcode:
                    func = " incSP();"
                elif 'DEC SP' == opcode:
                    func = " decSP();"
                elif 'INC' in opcode and len(opcode.split()[1]) == 1:
                    func = f" inc(reg.{opcode.split()[1]});"
                elif 'DEC' in opcode and len(opcode.split()[1]) == 1:
                    func = f" dec(reg.{opcode.split()[1]});"
                
                #inc/dec pair
                elif 'INC' in opcode and opcode.split()[1] != "(HL)":
                    regs = opcode.split()[1]
                    func = f" inc(reg.{regs[0]}, reg.{regs[1]});"
                elif 'DEC' in opcode and opcode.split()[1] != "(HL)":
                    regs = opcode.split()[1]
                    func = f" dec(reg.{regs[0]}, reg.{regs[1]});"
                
                elif 'INC' in opcode:
                    func = f" inc(reg.A, true);"
                
                
                elif 'DEC' in opcode:
                    func = f" dec(reg.A, true);"


                elif 'ADD HL SP' in opcode:
                    func = "addSP2Reg(reg.H, reg.L);"     

                elif 'ADD' in opcode and (len(opcode.split()[1]) == 1 or len(opcode.split()[1]) == 4):
                    if len(opcode.split()[1]) == 1:
                        func = f" add(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" add(reg.A, pairRegData(reg.{reg1}, reg.{reg2}));"

                elif 'ADD' in opcode and len(opcode.split()[1]) == 2 and len(opcode.split()[2]) == 2:
                    regs1 = opcode.split()[1]
                    regs2 = opcode.split()[2]
                    func = f" add(reg.{regs1[0]}, reg.{regs1[1]}, reg.{regs2[0]}, reg.{regs2[1]});"

                elif 'SUB' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" sub(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" sub(reg.A, pairRegData(reg.{reg1}, reg.{reg2}));"

                elif 'ADC' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" adc(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" adc(reg.A, pairRegData(reg.{reg1}, reg.{reg2}));"

                elif 'SBC' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" sbc(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" sbc(reg.A, pairRegData(reg.{reg1}, reg.{reg2}));"

                elif 'CP ' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" cp(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" cp(reg.A, pairRegData(reg.{reg1}, reg.{reg2}));"
                
                #load operations
                elif 'LD' in opcode: 
                    if len(opcode.split()[1]) == 1 and len(opcode.split()[2]) == 1:
                        func = f" load(reg.{opcode.split()[1]}, reg.{opcode.split()[2]});"
                    elif len(opcode.split()[1]) == 4: #(XX) second
                        regFirst = opcode.split()[1][1]
                        regSec = opcode.split()[1][2]
                        regThird = opcode.split()[2]
                        func = f" load(pairReg(reg.{regFirst}, reg.{regSec}), reg.{regThird});"
                    elif len(opcode.split()[2]) == 4: #(XX) last
                        regFirst = opcode.split()[1]
                        regSec = opcode.split()[2][1]
                        regThird = opcode.split()[2][2]
                        func = f" load(reg.{regFirst}, pairReg(reg.{regSec}, reg.{regThird}));"
                    elif opcode.split()[1] == '(C)':
                        func = f" load( joinByte(0x00, reg.C), reg.A );"
                    elif opcode.split()[2] == '(C)':
                        func = f" load( reg.A, mem.read(joinByte(0x00, reg.C)) ); "

                    #load with additions and subtractions
                    elif 'LD (HL+) A' == opcode:
                        func = " load(pairReg(reg.H, reg.L), reg.A); inc(reg.A, true);"
                    elif 'LD A (HL+)' == opcode:
                        func = " load(reg.A, pairReg(reg.H, reg.L)); inc(reg.A, true);"
                    elif 'LD (HL-) A' == opcode:
                        func = " load(pairReg(reg.H, reg.L), reg.A); dec(reg.A, true);"
                    elif 'LD A (HL-)' == opcode:
                        func = " load(reg.A, pairReg(reg.H, reg.L) ); dec(reg.A, true);"




                    elif 'LD SP HL' == opcode:
                        func = " loadSP( reg.H, reg.L);"

                    

                    else:
                        opcode = opcode + " | NOT IMPLEMENTED"
                    
                #stack operations
                elif 'PUSH' in opcode:
                    regs = opcode.split()[1]
                    func = f" push(reg.{regs[0]}, reg.{regs[1]});"
                elif 'POP' in opcode:
                    regs = opcode.split()[1]
                    func = f" pop(reg.{regs[0]}, reg.{regs[1]});"
                
                
                


                #returns
                elif 'RETI' in opcode:
                    func = " reti();"
                elif 'RET' == opcode:
                    func = " ret(); isJump = true ;"
                elif 'RET' in opcode :
                    flag = flag_dict[opcode.split()[1]]

                    row = (f"\tcase 0x{hex}:{{\n" 
                    f"\t\tbool jump = ret( {flag});\n"
                    f"\t\tstd::cout << \" {opcode} \\n\"; if (jump) {{isJump = true ;}} break;\n\t}}\n") 
                    
                    print(row)
                    f.write(row)
                    continue

                elif 'EI' == opcode:
                    func = " ei();"
                elif 'DI' == opcode:
                    func = " di();"
                elif 'CPL' == opcode:
                    func = " cpl();"
                elif 'SCF' == opcode:
                    func = " scf();"
                elif 'CCF' == opcode:
                    func = " ccf();"
                
                elif 'CB op' in opcode: #jump to prebyte decoder
                    func = ' decodePreByte(gbdata.buffer[pc+1]); opbytes = 2;'

                #rotates
                elif 'RLCA' in opcode:
                    func = " rlca();"
                elif 'RLA' in opcode:
                    func = " rla();"
                elif 'RRCA' in opcode:
                    func = " rrca();"
                elif 'RRA' in opcode:
                    func = " rra();"

                
                
                #rst
                elif 'RST' in opcode:
                    byte = opcode.split()[1]
                    func = f" rst({byte});"

                #jp HL
                elif 'JP' in opcode:
                    func = ' jp( pairReg(reg.H, reg.L) );'

                elif 'DAA' == opcode:
                    func = " daa();"
                elif 'stop' == opcode:
                    func = " stop();"


                else:
                    opcode = opcode + " | NOT IMPLEMENTED"

                #1 byte row
                row = f"\tcase 0x{hex}: std::cout << \" {opcode} \\n\"; {func} break;\n"

            print(row)
            f.write(row)
    f.write(end)







