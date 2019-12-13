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
                row = f"\tcase 0x{hex}: std::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << ' ' << HEX(gbdata.buffer[pc+2]) << \"\\n\"; opbytes = 3; break;\n"

            elif 'n' in r or 'e' in r:
                #2 byte row, (n)
                row = f"\tcase 0x{hex}: std::cout << \" {opcode} \" << HEX(gbdata.buffer[pc+1]) << \"\\n\"; opbytes = 2; break;\n"
            else:

                if 'XOR' in opcode and len(opcode.split()[1]) == 1:
                    func = f" logXor(reg.A, reg.{opcode[-1]});"
                elif 'OR' in opcode and len(opcode.split()[1]) == 1:
                    func = f" logOr(reg.A, reg.{opcode.split()[1]});"
                elif 'AND' in opcode and len(opcode.split()[1]) == 1:
                    func = f" logAnd(reg.A, reg.{opcode.split()[1]});"
                elif 'NOP' in opcode:
                    func = f""
                elif 'INC' in opcode and len(opcode.split()[1]) == 1:
                    func = f" inc(reg.{opcode.split()[1]});"
                elif 'DEC' in opcode and len(opcode.split()[1]) == 1:
                    func = f" dec(reg.{opcode.split()[1]});"

                elif 'LD' in opcode and len(opcode.split()[1]) == 1 and len(opcode.split()[2]) == 1:
                    func = f" load(reg.{opcode.split()[1]}, reg.{opcode.split()[2]});"

                else:
                    opcode = opcode + " | NOT IMPLEMENTED"

                #1 byte row
                row = f"\tcase 0x{hex}: std::cout << \" {opcode} \\n\"; {func} break;\n"

            print(row)
            f.write(row)
    f.write(end)







