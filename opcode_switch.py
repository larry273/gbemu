import csv


#read in opcodes create switch for c++
#case 0x00: std::cout << "NOP\n"; break;

beg = "switch (*code) {\n"
end = "\tdefault: std::cout << \"***Instruction not recognized*** \" << HEX(*code) << \"\\n\"; break; \n}\n"

out_file = "switch.txt"
file_name = "istr_set.csv" 

data = []
with open(file_name, 'r') as f:
    reader = csv.reader(f)
    data = list(list(rec) for rec in csv.reader(f, delimiter='|')) #reads csv into a list of lists

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
                    f"\t\tload((val1 >> 8) + val2, reg.{reg});\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; break;\n\t}}\n")

                elif 'LD' in opcode and opcode.split()[2] == '(nn)':
                    reg = opcode.split()[1]
                    row = (f"\tcase 0x{hex}:{{\n" 
                    "\t\tuint8_t val1 = gbdata.buffer[pc+1]; uint8_t val2 = gbdata.buffer[pc+2];\n"
                    f"\t\tload(reg.{reg}, (val1 >> 8) + val2);\n"
                    f"\t\tstd::cout << \" {opcode} \" << HEX(val1) << ' ' << HEX(val2) << \"\\n\"; opbytes = 3; break;\n\t}}\n")

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
                        func = f" logXor(reg.A, pairData(reg.{reg1}, reg.{reg2}));"
                
                elif 'OR' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" logOr(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" logOr(reg.A, pairData(reg.{reg1}, reg.{reg2}));"

                elif 'AND' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" logAnd(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" logAnd(reg.A, pairData(reg.{reg1}, reg.{reg2}));"

                elif 'NOP' in opcode or 'HALT' in opcode:
                    func = f"" 
                elif 'INC' in opcode and len(opcode.split()[1]) == 1:
                    func = f" inc(reg.{opcode.split()[1]});"
                elif 'DEC' in opcode and len(opcode.split()[1]) == 1:
                    func = f" dec(reg.{opcode.split()[1]});"
                elif 'ADD' in opcode and (len(opcode.split()[1]) == 1 or len(opcode.split()[1]) == 4):
                    if len(opcode.split()[1]) == 1:
                        func = f" add(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" add(reg.A, pairData(reg.{reg1}, reg.{reg2}));"

                elif 'SUB' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" sub(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" sub(reg.A, pairData(reg.{reg1}, reg.{reg2}));"

                elif 'ADC' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" adc(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" adc(reg.A, pairData(reg.{reg1}, reg.{reg2}));"

                elif 'SBC' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" sbc(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" sbc(reg.A, pairData(reg.{reg1}, reg.{reg2}));"

                elif 'CP ' in opcode:
                    if len(opcode.split()[1]) == 1:
                        func = f" cp(reg.A, reg.{opcode.split()[1]});"
                    else:
                        reg1 = opcode.split()[1][1]
                        reg2 = opcode.split()[1][2]
                        func = f" cp(reg.A, pairData(reg.{reg1}, reg.{reg2}));"
                
                elif 'LD' in opcode: 
                    if len(opcode.split()[1]) == 1 and len(opcode.split()[2]) == 1:
                        func = f" load(reg.{opcode.split()[1]}, reg.{opcode.split()[2]});"
                    elif len(opcode.split()[1]) == 4: #(XX)
                        regFirst = opcode.split()[1][1]
                        regSec = opcode.split()[1][2]
                        regThird = opcode.split()[2]
                        func = f" load(pairReg(reg.{regFirst}, reg.{regSec}), reg.{regThird});"
                    elif len(opcode.split()[2]) == 4: #(XX)
                        regFirst = opcode.split()[1]
                        regSec = opcode.split()[2][1]
                        regThird = opcode.split()[2][2]
                        func = f" load(reg.{regFirst}, pairReg(reg.{regSec}, reg.{regThird}));"

                #stack operations
                elif 'PUSH' in opcode:
                    regs = opcode.split()[1]
                    func = f" push(reg.{regs[0]}, reg.{regs[1]});"
                elif 'POP' in opcode:
                    regs = opcode.split()[1]
                    func = f" pop(reg.{regs[0]}, reg.{regs[1]});"
                elif 'DEC SP' == opcode:
                    func = " decSP();"
                elif 'INC SP' == opcode:
                    func = " incSP();"

                else:
                    opcode = opcode + " | NOT IMPLEMENTED"

                #1 byte row
                row = f"\tcase 0x{hex}: std::cout << \" {opcode} \\n\"; {func} break;\n"

            print(row)
            f.write(row)
    f.write(end)







