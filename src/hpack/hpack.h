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

#ifndef _HPACK_H_
#define _HPACK_H_

void bit_copy(unsigned char *dest, const uint32_t src,
                const uint32_t dest_bit_offset, const unsigned char src_bit_len);

unsigned char *huffman_encode(const unsigned char *value,
                            const unsigned int val_len, unsigned int *len);
char *huffman_decode(const unsigned char *value, const unsigned char len);

#endif