# -*- coding: utf-8 -*-

import os
import struct
from distutils.dir_util import mkpath

DICT = {
    0xab: 'g',
    0xaa: 'f',
    0xb4: 'x',
    0xe3: '/',
    0xbf: 's',
    0xa4: 'h',
    0xa3: 'o',
    0xbe: 'r',
    0xb8: 't',
    0xad: 'a',
    0xa9: 'e',
    0xaf: 'c',
    0xa5: 'i',
    0xa2: 'n',
    0xa8: 'd',
    0xa7: 'k',
    0xe2: '.',
    0xbb: 'w',
    0xba: 'v',
    0xfd: '1',
    0xa1: 'm',
    0xbc: 'p',
    0xa0: 'l',
    0xb9: 'u',
    0xb6: 'z',
    0x8e: 'B',
    0xae: 'b',
    0xa6: 'j',
    0x94: 'X',
    0x9C: 'P',
    0xf4: '8',
    0xfe: '2',
    0xf8: '4',
    0xfc: '0',
    0x93: '_',
    0xff: '3',
    0xb5: 'y',
    0x83: 'O',
    0x89: 'E',
    0xf9: '5',
    0xf5: '9',
    0x88: 'D',
    0xe1: '-',
    0x9f: 'S',
    0x80: 'L',
    0xfa: '6',
    0xfb: '7',
    0x8a: 'F',
    0x8f: 'C',
    0x85: 'I',
}

HEADER_STRUCT_SIZE = 88


def decode(s):
    res = ""
    for i in [ord(k) for k in s]:
        if i == 0xCC:
            return res
        res += DICT[i] if i in DICT else '#'
    return res


with open('../ChickenInvaders2.dat', 'rb') as f:
    HEADER_LEN = struct.unpack('=L', f.read(4))[0]
    print HEADER_LEN
    HEADER_DATA = f.read(HEADER_LEN * HEADER_STRUCT_SIZE)
    for i in xrange(0, HEADER_LEN * 88, 88):
        name = decode(HEADER_DATA[i: i + 80])
        print name
        f.seek(struct.unpack('=L', HEADER_DATA[i + 80: i + 84])[0])
        length = struct.unpack('=L', HEADER_DATA[i + 84: i + 88])[0]
        path = os.path.join('/tmp/img/', name)
        if not os.path.exists(os.path.split(path)[0]):
            mkpath(os.path.split(path)[0])
        with open(path, 'wb') as res_file:
            res_file.write(f.read(length))
