import csv


#read in opcodes create switch for c++
#case 0x00: std::cout << "NOP\n"; break;

beg = "switch (bitCode) {\n"
end = "\tdefault: std::cout << \"***Instruction not recognized*** \" << HEX(bitCode) << \"\\n\"; break; \n}\n"

out_file = "bit_switch.txt"
file_name = "bit_istr_set.csv" 

bit_dict = {
    "0": "BITZERO",
    "1": "BITONE",
    "2": "BITTWO",
    "3": "BITTHREE",
    "4": "BITFOUR",
    "5": "BITFIVE",
    "6": "BITSIX",
    "7": "BITSEVEN"

}

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
            else:
                if 'RLC' in opcode and len(opcode.split()[1]) == 1:
                    reg = opcode.split()[1]
                    func = f" rlc(reg.{reg});"
                
                elif 'RRC' in opcode and len(opcode.split()[1]) == 1:
                    reg = opcode.split()[1]
                    func = f" rrc(reg.{reg});"
                
                elif 'SRL' in opcode and len(opcode.split()[1]) == 1:
                    reg = opcode.split()[1]
                    func = f" srl(reg.{reg});"
                
                elif 'RL' in opcode and len(opcode.split()[1]) == 1:
                    reg = opcode.split()[1]
                    func = f" rl(reg.{reg});"

                elif 'RR' in opcode and len(opcode.split()[1]) == 1:
                    reg = opcode.split()[1]
                    func = f" rr(reg.{reg});"

                elif 'SLA' in opcode and len(opcode.split()[1]) == 1:
                    reg = opcode.split()[1]
                    func = f" sla(reg.{reg});"

                elif 'SRA' in opcode and len(opcode.split()[1]) == 1:
                    reg = opcode.split()[1]
                    func = f" sra(reg.{reg});"

                elif 'SWAP' in opcode and len(opcode.split()[1]) == 1:
                    reg = opcode.split()[1]
                    func = f" swap(reg.{reg});"
                
                elif 'BIT' in opcode and len(opcode.split()[2]) == 1:
                    reg = opcode.split()[2]
                    bit = bit_dict[opcode.split()[1]]
                    func = f" bit({bit}, reg.{reg});"

                elif 'RES' in opcode and len(opcode.split()[2]) == 1:
                    reg = opcode.split()[2]
                    bit = bit_dict[opcode.split()[1]]
                    func = f" res({bit}, reg.{reg});"

                elif 'SET' in opcode and len(opcode.split()[2]) == 1:
                    reg = opcode.split()[2]
                    bit = bit_dict[opcode.split()[1]]
                    func = f" set({bit}, reg.{reg});"
                
                elif len(opcode.split()) == 2:
                    ff = opcode.split()[0].lower()
                    func = f" {ff}(reg.A, true);"

                elif len(opcode.split()) > 2:
                    ff = opcode.split()[0].lower()
                    bit = bit_dict[opcode.split()[1]]
                    func = f" {ff}({bit}, reg.A, true);"

                else:
                    opcode = opcode + " | NOT IMPLEMENTED"

       
                #1 byte row
                row = f"\tcase 0x{hex}: std::cout << \" {opcode} \\n\"; {func} break;\n"

            print(row)
            f.write(row)
    f.write(end)







