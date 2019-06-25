import sys
if len(sys.argv) > 1:
    file_name = sys.argv[1]


POLYNOMIAL = 0x1021
PRESET = 0


def _initial(c):
    crc = 0
    c = c << 8
    for j in range(8):
        if (crc ^ c) & 0x8000:
            crc = (crc << 1) ^ POLYNOMIAL
        else:
            crc = crc << 1
        c = c << 1
    return crc


_tab = [_initial(i) for i in range(256)]


def _update_crc(crc, c):
    cc = 0xff & c

    tmp = (crc >> 8) ^ cc
    crc = (crc << 8) ^ _tab[tmp & 0xff]
    crc = crc & 0xffff

    return crc


def crc(str):
    crc = PRESET
    for c in str:
        crc = _update_crc(crc, ord(c))
    return crc


def crcb(i):
    crc = PRESET
    for c in i:
        crc = _update_crc(crc, c)
    return crc


if file_name != None:
    f = open(file_name)
    content = f.read()
    content = content.replace('\n', '\t')
    #print(content.replace('\t',""))
    string_list = content.split('\t')
    byte_list = []
    for string in string_list:
        byte_list.append(int(string, 16))
    checksum = crcb(byte_list[0:2])
    temp = []
    for byte in byte_list[0:2]:
        temp.append(str(hex(byte)))
    txt = '{\n' + ', '.join(temp) + '\n}'
    print(txt)
    print(len(byte_list[0:2]))
    print(hex(checksum))
    f.close()
