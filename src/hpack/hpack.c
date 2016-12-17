/*
 * Copyright (C) 2016 Florin Petriuc. All rights reserved.
 * Initial release: Florin Petriuc <petriuc.florin@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

#include "../libfirebase.h"

unsigned char get_bit_at(const unsigned char v, const unsigned char pos)
{
    unsigned char ret = 0;

    ret = (v >> (8 - pos - 1)) & 1;

    return ret;
}

char *huffman_decode(const unsigned char *value, const unsigned char len)
{
    uint32_t char_encoded = 0;

    uint16_t i = 0;
    uint16_t vIdx;

    unsigned char bits_used = 0;

    char decoded_str[768];

    char *ret;

    for(vIdx = 0; vIdx < len * 8; ++vIdx)
    {
        char_encoded = (char_encoded << 1) | get_bit_at(value[vIdx / 8], vIdx & 7);

        ++bits_used;

        if(i == sizeof(decoded_str))
        {
            break;
        }

        switch(bits_used)
        {
            case 5:
            {
                if(char_encoded >= 0x0 && char_encoded <= 0x2)
                {
                    decoded_str[i++] = char_encoded - 0x0 + 48;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3)
                {
                    decoded_str[i++] = 97;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x4)
                {
                    decoded_str[i++] = 99;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x5)
                {
                    decoded_str[i++] = 101;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x6)
                {
                    decoded_str[i++] = 105;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7)
                {
                    decoded_str[i++] = 111;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x8 && char_encoded <= 0x9)
                {
                    decoded_str[i++] = char_encoded - 0x8 + 115;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 6:
            {
                if(char_encoded == 0x14)
                {
                    decoded_str[i++] = 32;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x15)
                {
                    decoded_str[i++] = 37;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x16 && char_encoded <= 0x18)
                {
                    decoded_str[i++] = char_encoded - 0x16 + 45;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x19 && char_encoded <= 0x1f)
                {
                    decoded_str[i++] = char_encoded - 0x19 + 51;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x20)
                {
                    decoded_str[i++] = 61;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x21)
                {
                    decoded_str[i++] = 65;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x22)
                {
                    decoded_str[i++] = 95;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x23)
                {
                    decoded_str[i++] = 98;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x24)
                {
                    decoded_str[i++] = 100;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x25 && char_encoded <= 0x27)
                {
                    decoded_str[i++] = char_encoded - 0x25 + 102;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x28 && char_encoded <= 0x2a)
                {
                    decoded_str[i++] = char_encoded - 0x28 + 108;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x2b)
                {
                    decoded_str[i++] = 112;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x2c)
                {
                    decoded_str[i++] = 114;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x2d)
                {
                    decoded_str[i++] = 117;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 7:
            {
                if(char_encoded == 0x5c)
                {
                    decoded_str[i++] = 58;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x5d && char_encoded <= 0x72)
                {
                    decoded_str[i++] = char_encoded - 0x5d + 66;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x73)
                {
                    decoded_str[i++] = 89;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x74 && char_encoded <= 0x75)
                {
                    decoded_str[i++] = char_encoded - 0x74 + 106;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x76)
                {
                    decoded_str[i++] = 113;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x77 && char_encoded <= 0x7b)
                {
                    decoded_str[i++] = char_encoded - 0x77 + 118;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 8:
            {
                if(char_encoded == 0xf8)
                {
                    decoded_str[i++] = 38;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xf9)
                {
                    decoded_str[i++] = 42;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfa)
                {
                    decoded_str[i++] = 44;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfb)
                {
                    decoded_str[i++] = 59;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfc)
                {
                    decoded_str[i++] = 88;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfd)
                {
                    decoded_str[i++] = 90;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 10:
            {
                if(char_encoded >= 0x3f8 && char_encoded <= 0x3f9)
                {
                    decoded_str[i++] = char_encoded - 0x3f8 + 33;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3fa && char_encoded <= 0x3fb)
                {
                    decoded_str[i++] = char_encoded - 0x3fa + 40;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fc)
                {
                    decoded_str[i++] = 63;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 11:
            {
                if(char_encoded == 0x7fa)
                {
                    decoded_str[i++] = 39;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fb)
                {
                    decoded_str[i++] = 43;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fc)
                {
                    decoded_str[i++] = 124;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 12:
            {
                if(char_encoded == 0xffa)
                {
                    decoded_str[i++] = 35;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xffb)
                {
                    decoded_str[i++] = 62;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 13:
            {
                if(char_encoded == 0x1ff8)
                {
                    decoded_str[i++] = 0;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1ff9)
                {
                    decoded_str[i++] = 36;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1ffa)
                {
                    decoded_str[i++] = 64;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1ffb)
                {
                    decoded_str[i++] = 91;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1ffc)
                {
                    decoded_str[i++] = 93;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1ffd)
                {
                    decoded_str[i++] = 126;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 14:
            {
                if(char_encoded == 0x3ffc)
                {
                    decoded_str[i++] = 94;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffd)
                {
                    decoded_str[i++] = 125;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 15:
            {
                if(char_encoded == 0x7ffc)
                {
                    decoded_str[i++] = 60;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffd)
                {
                    decoded_str[i++] = 96;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffe)
                {
                    decoded_str[i++] = 123;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 19:
            {
                if(char_encoded == 0x7fff0)
                {
                    decoded_str[i++] = 92;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fff1)
                {
                    decoded_str[i++] = 195;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fff2)
                {
                    decoded_str[i++] = 207;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 20:
            {
                if(char_encoded == 0xfffe6)
                {
                    decoded_str[i++] = 128;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0xfffe7 && char_encoded <= 0xfffe8)
                {
                    decoded_str[i++] = char_encoded - 0xfffe7 + 130;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffe9)
                {
                    decoded_str[i++] = 162;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffea)
                {
                    decoded_str[i++] = 184;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffeb)
                {
                    decoded_str[i++] = 194;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffec)
                {
                    decoded_str[i++] = 224;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffed)
                {
                    decoded_str[i++] = 226;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 21:
            {
                if(char_encoded == 0x1fffdc)
                {
                    decoded_str[i++] = 153;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1fffdd)
                {
                    decoded_str[i++] = 161;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1fffde)
                {
                    decoded_str[i++] = 167;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1fffdf)
                {
                    decoded_str[i++] = 172;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x1fffe0 && char_encoded <= 0x1fffe1)
                {
                    decoded_str[i++] = char_encoded - 0x1fffe0 + 177;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1fffe2)
                {
                    decoded_str[i++] = 179;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1fffe3)
                {
                    decoded_str[i++] = 209;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x1fffe4 && char_encoded <= 0x1fffe5)
                {
                    decoded_str[i++] = char_encoded - 0x1fffe4 + 216;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1fffe6)
                {
                    decoded_str[i++] = 227;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x1fffe7 && char_encoded <= 0x1fffe8)
                {
                    decoded_str[i++] = char_encoded - 0x1fffe7 + 229;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 22:
            {
                if(char_encoded == 0x3fffd2)
                {
                    decoded_str[i++] = 129;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3fffd3 && char_encoded <= 0x3fffd5)
                {
                    decoded_str[i++] = char_encoded - 0x3fffd3 + 132;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffd6)
                {
                    decoded_str[i++] = 136;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffd7)
                {
                    decoded_str[i++] = 146;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffd8)
                {
                    decoded_str[i++] = 154;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffd9)
                {
                    decoded_str[i++] = 156;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffda)
                {
                    decoded_str[i++] = 160;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3fffdb && char_encoded <= 0x3fffdc)
                {
                    decoded_str[i++] = char_encoded - 0x3fffdb + 163;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3fffdd && char_encoded <= 0x3fffde)
                {
                    decoded_str[i++] = char_encoded - 0x3fffdd + 169;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffdf)
                {
                    decoded_str[i++] = 173;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffe0)
                {
                    decoded_str[i++] = 178;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffe1)
                {
                    decoded_str[i++] = 181;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3fffe2 && char_encoded <= 0x3fffe4)
                {
                    decoded_str[i++] = char_encoded - 0x3fffe2 + 185;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3fffe5 && char_encoded <= 0x3fffe6)
                {
                    decoded_str[i++] = char_encoded - 0x3fffe5 + 189;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffe7)
                {
                    decoded_str[i++] = 196;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffe8)
                {
                    decoded_str[i++] = 198;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffe9)
                {
                    decoded_str[i++] = 228;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3fffea && char_encoded <= 0x3fffeb)
                {
                    decoded_str[i++] = char_encoded - 0x3fffea + 232;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 23:
            {
                if(char_encoded == 0x7fffd8)
                {
                    decoded_str[i++] = 1;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fffd9)
                {
                    decoded_str[i++] = 135;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7fffda && char_encoded <= 0x7fffde)
                {
                    decoded_str[i++] = char_encoded - 0x7fffda + 137;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fffdf)
                {
                    decoded_str[i++] = 143;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fffe0)
                {
                    decoded_str[i++] = 147;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7fffe1 && char_encoded <= 0x7fffe4)
                {
                    decoded_str[i++] = char_encoded - 0x7fffe1 + 149;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fffe5)
                {
                    decoded_str[i++] = 155;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7fffe6 && char_encoded <= 0x7fffe7)
                {
                    decoded_str[i++] = char_encoded - 0x7fffe6 + 157;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7fffe8 && char_encoded <= 0x7fffe9)
                {
                    decoded_str[i++] = char_encoded - 0x7fffe8 + 165;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fffea)
                {
                    decoded_str[i++] = 168;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7fffeb && char_encoded <= 0x7fffec)
                {
                    decoded_str[i++] = char_encoded - 0x7fffeb + 174;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7fffed)
                {
                    decoded_str[i++] = 180;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7fffee && char_encoded <= 0x7fffef)
                {
                    decoded_str[i++] = char_encoded - 0x7fffee + 182;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffff0)
                {
                    decoded_str[i++] = 188;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffff1)
                {
                    decoded_str[i++] = 191;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffff2)
                {
                    decoded_str[i++] = 197;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffff3)
                {
                    decoded_str[i++] = 231;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffff4)
                {
                    decoded_str[i++] = 239;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 24:
            {
                if(char_encoded == 0xffffea)
                {
                    decoded_str[i++] = 9;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xffffeb)
                {
                    decoded_str[i++] = 142;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0xffffec && char_encoded <= 0xffffed)
                {
                    decoded_str[i++] = char_encoded - 0xffffec + 144;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xffffee)
                {
                    decoded_str[i++] = 148;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xffffef)
                {
                    decoded_str[i++] = 159;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffff0)
                {
                    decoded_str[i++] = 171;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffff1)
                {
                    decoded_str[i++] = 206;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffff2)
                {
                    decoded_str[i++] = 215;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xfffff3)
                {
                    decoded_str[i++] = 225;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0xfffff4 && char_encoded <= 0xfffff5)
                {
                    decoded_str[i++] = char_encoded - 0xfffff4 + 236;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 25:
            {
                if(char_encoded == 0x1ffffec)
                {
                    decoded_str[i++] = 199;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x1ffffed)
                {
                    decoded_str[i++] = 207;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x1ffffee && char_encoded <= 0x1ffffef)
                {
                    decoded_str[i++] = char_encoded - 0x1ffffee + 234;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 26:
            {
                if(char_encoded >= 0x3ffffe0 && char_encoded <= 0x3ffffe1)
                {
                    decoded_str[i++] = char_encoded - 0x3ffffe0 + 192;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3ffffe2 && char_encoded <= 0x3ffffe4)
                {
                    decoded_str[i++] = char_encoded - 0x3ffffe2 + 200;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffffe5)
                {
                    decoded_str[i++] = 205;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffffe6)
                {
                    decoded_str[i++] = 210;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffffe7)
                {
                    decoded_str[i++] = 213;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3ffffe8 && char_encoded <= 0x3ffffe9)
                {
                    decoded_str[i++] = char_encoded - 0x3ffffe8 + 218;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffffea)
                {
                    decoded_str[i++] = 238;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffffeb)
                {
                    decoded_str[i++] = 240;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x3ffffec && char_encoded <= 0x3ffffed)
                {
                    decoded_str[i++] = char_encoded - 0x3ffffec + 242;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffffee)
                {
                    decoded_str[i++] = 255;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 27:
            {
                if(char_encoded >= 0x7ffffde && char_encoded <= 0x7ffffdf)
                {
                    decoded_str[i++] = char_encoded - 0x7ffffde + 204;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7ffffe0 && char_encoded <= 0x7ffffe1)
                {
                    decoded_str[i++] = char_encoded - 0x7ffffe0 + 211;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffffe2)
                {
                    decoded_str[i++] = 214;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7ffffe3 && char_encoded <= 0x7ffffe5)
                {
                    decoded_str[i++] = char_encoded - 0x7ffffe3 + 221;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x7ffffe6)
                {
                    decoded_str[i++] = 241;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7ffffe7 && char_encoded <= 0x7ffffeb)
                {
                    decoded_str[i++] = char_encoded - 0x7ffffe7 + 244;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0x7ffffec && char_encoded <= 0x7fffff0)
                {
                    decoded_str[i++] = char_encoded - 0x7ffffec + 250;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 28:
            {
                if(char_encoded >= 0xfffffe2 && char_encoded <= 0xfffffe8)
                {
                    decoded_str[i++] = char_encoded - 0xfffffe2 + 2;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0xfffffe9 && char_encoded <= 0xfffffea)
                {
                    decoded_str[i++] = char_encoded - 0xfffffe9 + 11;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0xfffffeb && char_encoded <= 0xffffff2)
                {
                    decoded_str[i++] = char_encoded - 0xfffffeb + 14;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded >= 0xffffff3 && char_encoded <= 0xffffffb)
                {
                    decoded_str[i++] = char_encoded - 0xffffff3 + 23;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xffffffc)
                {
                    decoded_str[i++] = 127;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xffffffd)
                {
                    decoded_str[i++] = 220;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0xffffffe)
                {
                    decoded_str[i++] = 249;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;

            case 30:
            {
                if(char_encoded == 0x3ffffffc)
                {
                    decoded_str[i++] = 10;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffffffd)
                {
                    decoded_str[i++] = 13;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3ffffffe)
                {
                    decoded_str[i++] = 22;

                    char_encoded = 0;
                    bits_used = 0;
                }
                else if(char_encoded == 0x3fffffff)
                {
                    decoded_str[i++] = 0;

                    char_encoded = 0;
                    bits_used = 0;
                }
            }
            break;
        }
    }

    ret = env_malloc(i + 1);
    memcpy(ret, decoded_str, i);
    ret[i] = '\0';

    return ret;
}

void bit_copy(unsigned char *dest, const uint32_t src,
                const uint32_t dest_bit_offset, const unsigned char src_bit_len)
{
    uint32_t src_moved;
    uint32_t dest_byte;

    unsigned char *src_array;

    src_moved = src << (32 - src_bit_len - (dest_bit_offset & 7));

    src_array = (unsigned char *)&src_moved;

    dest_byte = (dest_bit_offset >> 3);

    dest[dest_byte] |= src_array[3];
    dest[dest_byte + 1] |= src_array[2];
    dest[dest_byte + 2] |= src_array[1];
    dest[dest_byte + 3] |= src_array[0];
}

unsigned char *huffman_encode(const unsigned char *value,
                            const unsigned int val_len, unsigned int *len)
{
    unsigned int l;

    unsigned char idx;
    unsigned char final_mask;

    unsigned char encoded_str[255];
    unsigned char char_val;

    unsigned char *ret;

    unsigned short int bit_offset = 0;

    memset(encoded_str, 0, sizeof(encoded_str));

    for(idx = 0; idx < val_len && idx < sizeof(encoded_str); ++idx)
    {
        char_val = value[idx];

        if(char_val == 0)
        {
            bit_copy(encoded_str, 0x1ff8, bit_offset, 13);
            bit_offset += 13;
        }
        else if(char_val == 1)
        {
            bit_copy(encoded_str, 0x7fffd8, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val >= 2 && char_val <= 8)
        {
            bit_copy(encoded_str, 0xfffffe2 + char_val - 2, bit_offset, 28);
            bit_offset += 28;
        }
        else if(char_val == 9)
        {
            bit_copy(encoded_str, 0xffffea, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val == 10)
        {
            bit_copy(encoded_str, 0x3ffffffc, bit_offset, 30);
            bit_offset += 30;
        }
        else if(char_val >= 11 && char_val <= 12)
        {
            bit_copy(encoded_str, 0xfffffe9 + char_val - 11, bit_offset, 28);
            bit_offset += 28;
        }
        else if(char_val == 13)
        {
            bit_copy(encoded_str, 0x3ffffffd, bit_offset, 30);
            bit_offset += 30;
        }
        else if(char_val >= 14 && char_val <= 21)
        {
            bit_copy(encoded_str, 0xfffffeb + char_val - 14, bit_offset, 28);
            bit_offset += 28;
        }
        else if(char_val == 22)
        {
            bit_copy(encoded_str, 0x3ffffffe, bit_offset, 30);
            bit_offset += 30;
        }
        else if(char_val >= 23 && char_val <= 31)
        {
            bit_copy(encoded_str, 0xffffff3 + char_val - 23, bit_offset, 28);
            bit_offset += 28;
        }
        else if(char_val == 32)
        {
            bit_copy(encoded_str, 0x14, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val >= 33 && char_val <= 34)
        {
            bit_copy(encoded_str, 0x3f8 + char_val - 33, bit_offset, 10);
            bit_offset += 10;
        }
        else if(char_val == 35)
        {
            bit_copy(encoded_str, 0xffa, bit_offset, 12);
            bit_offset += 12;
        }
        else if(char_val == 36)
        {
            bit_copy(encoded_str, 0x1ff9, bit_offset, 13);
            bit_offset += 13;
        }
        else if(char_val == 37)
        {
            bit_copy(encoded_str, 0x15, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 38)
        {
            bit_copy(encoded_str, 0xf8, bit_offset, 8);
            bit_offset += 8;
        }
        else if(char_val == 39)
        {
            bit_copy(encoded_str, 0x7fa, bit_offset, 11);
            bit_offset += 11;
        }
        else if(char_val >= 40 && char_val <= 41)
        {
            bit_copy(encoded_str, 0x3fa + char_val - 40, bit_offset, 10);
            bit_offset += 10;
        }
        else if(char_val == 42)
        {
            bit_copy(encoded_str, 0xf9, bit_offset, 8);
            bit_offset += 8;
        }
        else if(char_val == 43)
        {
            bit_copy(encoded_str, 0x7fb, bit_offset, 11);
            bit_offset += 11;
        }
        else if(char_val == 44)
        {
            bit_copy(encoded_str, 0xfa, bit_offset, 8);
            bit_offset += 8;
        }
        else if(char_val >= 45 && char_val <= 47)
        {
            bit_copy(encoded_str, 0x16 + char_val - 45, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val >= 48 && char_val <= 50)
        {
            bit_copy(encoded_str, 0x0 + char_val - 48, bit_offset, 5);
            bit_offset += 5;
        }
        else if(char_val >= 51 && char_val <= 57)
        {
            bit_copy(encoded_str, 0x19 + char_val - 51, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 58)
        {
            bit_copy(encoded_str, 0x5c, bit_offset, 7);
            bit_offset += 7;
        }
        else if(char_val == 59)
        {
            bit_copy(encoded_str, 0xfb, bit_offset, 8);
            bit_offset += 8;
        }
        else if(char_val == 60)
        {
            bit_copy(encoded_str, 0x7ffc, bit_offset, 15);
            bit_offset += 15;
        }
        else if(char_val == 61)
        {
            bit_copy(encoded_str, 0x20, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 62)
        {
            bit_copy(encoded_str, 0xffb, bit_offset, 12);
            bit_offset += 12;
        }
        else if(char_val == 63)
        {
            bit_copy(encoded_str, 0x3fc, bit_offset, 10);
            bit_offset += 10;
        }
        else if(char_val == 64)
        {
            bit_copy(encoded_str, 0x1ffa, bit_offset, 13);
            bit_offset += 13;
        }
        else if(char_val == 65)
        {
            bit_copy(encoded_str, 0x21, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val >= 66 && char_val <= 87)
        {
            bit_copy(encoded_str, 0x5d + char_val - 66, bit_offset, 7);
            bit_offset += 7;
        }
        else if(char_val == 88)
        {
            bit_copy(encoded_str, 0xfc, bit_offset, 8);
            bit_offset += 8;
        }
        else if(char_val == 89)
        {
            bit_copy(encoded_str, 0x73, bit_offset, 7);
            bit_offset += 7;
        }
        else if(char_val == 90)
        {
            bit_copy(encoded_str, 0xfd, bit_offset, 8);
            bit_offset += 8;
        }
        else if(char_val == 91)
        {
            bit_copy(encoded_str, 0x1ffb, bit_offset, 13);
            bit_offset += 13;
        }
        else if(char_val == 92)
        {
            bit_copy(encoded_str, 0x7fff0, bit_offset, 19);
            bit_offset += 19;
        }
        else if(char_val == 93)
        {
            bit_copy(encoded_str, 0x1ffc, bit_offset, 13);
            bit_offset += 13;
        }
        else if(char_val == 94)
        {
            bit_copy(encoded_str, 0x3ffc, bit_offset, 14);
            bit_offset += 14;
        }
        else if(char_val == 95)
        {
            bit_copy(encoded_str, 0x22, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 96)
        {
            bit_copy(encoded_str, 0x7ffd, bit_offset, 15);
            bit_offset += 15;
        }
        else if(char_val == 97)
        {
            bit_copy(encoded_str, 0x3, bit_offset, 5);
            bit_offset += 5;
        }
        else if(char_val == 98)
        {
            bit_copy(encoded_str, 0x23, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 99)
        {
            bit_copy(encoded_str, 0x4, bit_offset, 5);
            bit_offset += 5;
        }
        else if(char_val == 100)
        {
            bit_copy(encoded_str, 0x24, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 101)
        {
            bit_copy(encoded_str, 0x5, bit_offset, 5);
            bit_offset += 5;
        }
        else if(char_val >= 102 && char_val <= 104)
        {
            bit_copy(encoded_str, 0x25 + char_val - 102, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 105)
        {
            bit_copy(encoded_str, 0x6, bit_offset, 5);
            bit_offset += 5;
        }
        else if(char_val >= 106 && char_val <= 107)
        {
            bit_copy(encoded_str, 0x74 + char_val - 106, bit_offset, 7);
            bit_offset += 7;
        }
        else if(char_val >= 108 && char_val <= 110)
        {
            bit_copy(encoded_str, 0x28 + char_val - 108, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 111)
        {
            bit_copy(encoded_str, 0x7, bit_offset, 5);
            bit_offset += 5;
        }
        else if(char_val == 112)
        {
            bit_copy(encoded_str, 0x2b, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val == 113)
        {
            bit_copy(encoded_str, 0x76, bit_offset, 7);
            bit_offset += 7;
        }
        else if(char_val == 114)
        {
            bit_copy(encoded_str, 0x2c, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val >= 115 && char_val <= 116)
        {
            bit_copy(encoded_str, 0x8 + char_val - 115, bit_offset, 5);
            bit_offset += 5;
        }
        else if(char_val == 117)
        {
            bit_copy(encoded_str, 0x2d, bit_offset, 6);
            bit_offset += 6;
        }
        else if(char_val >= 118 && char_val <= 122)
        {
            bit_copy(encoded_str, 0x77 + char_val - 118, bit_offset, 7);
            bit_offset += 7;
        }
        else if(char_val == 123)
        {
            bit_copy(encoded_str, 0x7ffe, bit_offset, 15);
            bit_offset += 15;
        }
        else if(char_val == 124)
        {
            bit_copy(encoded_str, 0x7fc, bit_offset, 11);
            bit_offset += 11;
        }
        else if(char_val == 125)
        {
            bit_copy(encoded_str, 0x3ffd, bit_offset, 14);
            bit_offset += 14;
        }
        else if(char_val == 126)
        {
            bit_copy(encoded_str, 0x1ffd, bit_offset, 13);
            bit_offset += 13;
        }
        else if(char_val == 127)
        {
            bit_copy(encoded_str, 0xffffffc, bit_offset, 28);
            bit_offset += 28;
        }
        else if(char_val == 128)
        {
            bit_copy(encoded_str, 0xfffe6, bit_offset, 20);
            bit_offset += 20;
        }
        else if(char_val == 129)
        {
            bit_copy(encoded_str, 0x3fffd2, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val >= 130 && char_val <= 131)
        {
            bit_copy(encoded_str, 0xfffe7 + char_val - 130, bit_offset, 20);
            bit_offset += 20;
        }
        else if(char_val >= 132 && char_val <= 134)
        {
            bit_copy(encoded_str, 0x3fffd3 + char_val - 132, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 135)
        {
            bit_copy(encoded_str, 0x7fffd9, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 136)
        {
            bit_copy(encoded_str, 0x3fffd6, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val >= 137 && char_val <= 141)
        {
            bit_copy(encoded_str, 0x7fffda + char_val - 137, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 142)
        {
            bit_copy(encoded_str, 0xffffeb, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val == 143)
        {
            bit_copy(encoded_str, 0x7fffdf, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val >= 144 && char_val <= 145)
        {
            bit_copy(encoded_str, 0xffffec + char_val - 144, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val == 146)
        {
            bit_copy(encoded_str, 0x3fffd7, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 147)
        {
            bit_copy(encoded_str, 0x7fffe0, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 148)
        {
            bit_copy(encoded_str, 0xffffee, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val >= 149 && char_val <= 152)
        {
            bit_copy(encoded_str, 0x7fffe4 + char_val - 149, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 153)
        {
            bit_copy(encoded_str, 0x1fffdc, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 154)
        {
            bit_copy(encoded_str, 0x3fffd8, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 155)
        {
            bit_copy(encoded_str, 0x7fffe5, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 156)
        {
            bit_copy(encoded_str, 0x3fffd9, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val >= 157 && char_val <= 158)
        {
            bit_copy(encoded_str, 0x7fffe6 + char_val - 157, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 159)
        {
            bit_copy(encoded_str, 0xffffef, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val == 160)
        {
            bit_copy(encoded_str, 0x3fffda, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 161)
        {
            bit_copy(encoded_str, 0x1fffdd, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 162)
        {
            bit_copy(encoded_str, 0xfffe9, bit_offset, 20);
            bit_offset += 20;
        }
        else if(char_val >= 163 && char_val <= 164)
        {
            bit_copy(encoded_str, 0x3fffdb + char_val - 163, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val >= 165 && char_val <= 166)
        {
            bit_copy(encoded_str, 0x7fffe8 + char_val - 165, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 167)
        {
            bit_copy(encoded_str, 0x1fffde, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 168)
        {
            bit_copy(encoded_str, 0x7fffea, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val >= 169 && char_val <= 170)
        {
            bit_copy(encoded_str, 0x3fffdd + char_val - 169, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 171)
        {
            bit_copy(encoded_str, 0xfffff0, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val == 172)
        {
            bit_copy(encoded_str, 0x1fffdf, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 173)
        {
            bit_copy(encoded_str, 0x3fffdf, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val >= 174 && char_val <= 175)
        {
            bit_copy(encoded_str, 0x7fffeb + char_val - 174, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val >= 176 && char_val <= 177)
        {
            bit_copy(encoded_str, 0x1fffe0 + char_val - 176, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 178)
        {
            bit_copy(encoded_str, 0x3fffe0, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 179)
        {
            bit_copy(encoded_str, 0x1fffe2, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 180)
        {
            bit_copy(encoded_str, 0x7fffed, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 181)
        {
            bit_copy(encoded_str, 0x3fffe1, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val >= 182 && char_val <= 183)
        {
            bit_copy(encoded_str, 0x7fffee + char_val - 182, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 184)
        {
            bit_copy(encoded_str, 0xfffea, bit_offset, 20);
            bit_offset += 20;
        }
        else if(char_val >= 185 && char_val <= 187)
        {
            bit_copy(encoded_str, 0x3fffe2 + char_val - 185, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 188)
        {
            bit_copy(encoded_str, 0x7ffff0, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val >= 189 && char_val <= 190)
        {
            bit_copy(encoded_str, 0x3fffe5 + char_val - 189, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 191)
        {
            bit_copy(encoded_str, 0x7ffff1, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val >= 192 && char_val <= 193)
        {
            bit_copy(encoded_str, 0x3ffffe0 + char_val - 192, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val == 194)
        {
            bit_copy(encoded_str, 0xfffeb, bit_offset, 20);
            bit_offset += 20;
        }
        else if(char_val == 195)
        {
            bit_copy(encoded_str, 0x7fff1, bit_offset, 19);
            bit_offset += 19;
        }
        else if(char_val == 196)
        {
            bit_copy(encoded_str, 0x3fffe7, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 197)
        {
            bit_copy(encoded_str, 0x7ffff2, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 198)
        {
            bit_copy(encoded_str, 0x3fffe8, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val == 199)
        {
            bit_copy(encoded_str, 0x1ffffec, bit_offset, 25);
            bit_offset += 25;
        }
        else if(char_val >= 200 && char_val <= 202)
        {
            bit_copy(encoded_str, 0x3ffffe2 + char_val - 200, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val >= 203 && char_val <= 204)
        {
            bit_copy(encoded_str, 0x7ffffde + char_val - 203, bit_offset, 27);
            bit_offset += 27;
        }
        else if(char_val == 205)
        {
            bit_copy(encoded_str, 0x3ffffe5, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val == 206)
        {
            bit_copy(encoded_str, 0xfffff1, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val == 207)
        {
            bit_copy(encoded_str, 0x1ffffed, bit_offset, 25);
            bit_offset += 25;
        }
        else if(char_val == 208)
        {
            bit_copy(encoded_str, 0x7fff2, bit_offset, 19);
            bit_offset += 19;
        }
        else if(char_val == 209)
        {
            bit_copy(encoded_str, 0x1fffe3, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 210)
        {
            bit_copy(encoded_str, 0x3ffffe6, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val >= 211 && char_val <= 212)
        {
            bit_copy(encoded_str, 0x7ffffe0 + char_val - 211, bit_offset, 27);
            bit_offset += 27;
        }
        else if(char_val == 213)
        {
            bit_copy(encoded_str, 0x3ffffe7, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val == 214)
        {
            bit_copy(encoded_str, 0x7ffffe2, bit_offset, 27);
            bit_offset += 27;
        }
        else if(char_val == 215)
        {
            bit_copy(encoded_str, 0xfffff2, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val >= 216 && char_val <= 217)
        {
            bit_copy(encoded_str, 0x1fffe4 + char_val - 216, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val >= 218 && char_val <= 219)
        {
            bit_copy(encoded_str, 0x3ffffe8 + char_val - 218, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val == 220)
        {
            bit_copy(encoded_str, 0xffffffd, bit_offset, 28);
            bit_offset += 28;
        }
        else if(char_val >= 221 && char_val <= 223)
        {
            bit_copy(encoded_str, 0x7ffffe3 + char_val - 221, bit_offset, 27);
            bit_offset += 27;
        }
        else if(char_val == 224)
        {
            bit_copy(encoded_str, 0xfffec, bit_offset, 20);
            bit_offset += 20;
        }
        else if(char_val == 225)
        {
            bit_copy(encoded_str, 0xfffff3, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val == 226)
        {
            bit_copy(encoded_str, 0xfffed, bit_offset, 20);
            bit_offset += 20;
        }
        else if(char_val == 227)
        {
            bit_copy(encoded_str, 0x1fffe6, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 228)
        {
            bit_copy(encoded_str, 0x3fffe9, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val >= 229 && char_val <= 230)
        {
            bit_copy(encoded_str, 0x1fffe7 + char_val - 229, bit_offset, 21);
            bit_offset += 21;
        }
        else if(char_val == 231)
        {
            bit_copy(encoded_str, 0x7ffff3, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val >= 232 && char_val <= 233)
        {
            bit_copy(encoded_str, 0x3fffea + char_val - 232, bit_offset, 22);
            bit_offset += 22;
        }
        else if(char_val >= 234 && char_val <= 235)
        {
            bit_copy(encoded_str, 0x1ffffee + char_val - 234, bit_offset, 25);
            bit_offset += 25;
        }
        else if(char_val >= 236 && char_val <= 237)
        {
            bit_copy(encoded_str, 0xfffff4 + char_val - 236, bit_offset, 24);
            bit_offset += 24;
        }
        else if(char_val == 238)
        {
            bit_copy(encoded_str, 0x3ffffea, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val == 239)
        {
            bit_copy(encoded_str, 0x7ffff4, bit_offset, 23);
            bit_offset += 23;
        }
        else if(char_val == 240)
        {
            bit_copy(encoded_str, 0x3ffffeb, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val == 241)
        {
            bit_copy(encoded_str, 0x7ffffe6, bit_offset, 27);
            bit_offset += 27;
        }
        else if(char_val >= 242 && char_val <= 243)
        {
            bit_copy(encoded_str, 0x3ffffec + char_val - 242, bit_offset, 26);
            bit_offset += 26;
        }
        else if(char_val >= 244 && char_val <= 248)
        {
            bit_copy(encoded_str, 0x7ffffe7 + char_val - 244, bit_offset, 27);
            bit_offset += 27;
        }
        else if(char_val == 249)
        {
            bit_copy(encoded_str, 0xffffffe, bit_offset, 28);
            bit_offset += 28;
        }
        else if(char_val >= 250 && char_val <= 254)
        {
            bit_copy(encoded_str, 0x7ffffec + char_val - 250, bit_offset, 27);
            bit_offset += 27;
        }
        else if(char_val == 255)
        {
            bit_copy(encoded_str, 0x3ffffee, bit_offset, 26);
            bit_offset += 26;
        }

        env_logWrite(LOG_TYPE_INFO, "Encoded %02hhx, bit_offset is %hu", 2, char_val, bit_offset);
    }

    final_mask = 0;

    l = bit_offset >> 3;
    if(bit_offset & 7)
    {
        idx = 8 - (bit_offset & 7);

        while(idx)
        {
            final_mask |= (1 << (idx - 1));

            --idx;
        }

        ++l;
    }

    env_logWrite(LOG_TYPE_INFO, "Final number of bits is %u, mask is %hhu", 2, bit_offset, final_mask);

    ret = env_malloc(l);
    memcpy(ret, encoded_str, l);
    *len = l;

    ret[l - 1] |= final_mask;

    return ret;
}