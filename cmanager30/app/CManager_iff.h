/*
**
** Contact Manager - An Amiga Centralized Bookmarks System
**
** Copyright 1999-2006 by Simone [wiz] Tellini https://tellini.info/
** Copyright 2003-2006 by Alfonso [alfie] Ranieri <alforan@tin.it>
** All Rights Are Reserved.
**
** Destributed Under The Terms Of The LGPL II
**
**
**/

#ifdef USE_CMANAGER_COLORS
const ULONG CManager_colors[96] =
{
	0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,
	0x00000000,0x00000000,0x00000000,
	0x0e0e0e0e,0x0e0e0e0e,0x0e0e0e0e,
	0x1c1c1c1c,0x1c1c1c1c,0x1c1c1c1c,
	0x29292929,0x29292929,0x29292929,
	0x39393939,0x39393939,0x39393939,
	0x49494949,0x49494949,0x49494949,
	0x5a5a5a5a,0x5a5a5a5a,0x5a5a5a5a,
	0x6a6a6a6a,0x6a6a6a6a,0x6a6a6a6a,
	0x7a7a7a7a,0x7a7a7a7a,0x7a7a7a7a,
	0x9a9a9a9a,0x9a9a9a9a,0x9a9a9a9a,
	0xb8b8b8b8,0xb8b8b8b8,0xb8b8b8b8,
	0xc6c6c6c6,0xc6c6c6c6,0xc6c6c6c6,
	0xd4d4d4d4,0xd4d4d4d4,0xd4d4d4d4,
	0xe3e3e3e3,0xe3e3e3e3,0xe3e3e3e3,
	0xf1f1f1f1,0xf1f1f1f1,0xf1f1f1f1,
	0xffffffff,0xffffffff,0xffffffff,
	0x52525252,0x66666666,0x94949494,
	0x7f7f7f7f,0x89898989,0xc2c2c2c2,
	0xadadadad,0xadadadad,0xf0f0f0f0,
	0xdbdbdbdb,0x75757575,0x41414141,
	0xdfdfdfdf,0xbabababa,0x45454545,
	0xefefefef,0xe7e7e7e7,0x14141414,
	0x38383838,0x88888888,0x46464646,
	0x56565656,0xb1b1b1b1,0x5a5a5a5a,
	0x75757575,0xdbdbdbdb,0x6e6e6e6e,
	0x75757575,0x5d5d5d5d,0x44444444,
	0x9f9f9f9f,0x85858585,0x72727272,
	0xc9c9c9c9,0xadadadad,0x9f9f9f9f,
	0x91919191,0x36363636,0x82828282,
	0xaaaaaaaa,0x68686868,0xa2a2a2a2,
	0xc2c2c2c2,0x99999999,0xc2c2c2c2,
};
#endif

#define CMANAGER_WIDTH        64
#define CMANAGER_HEIGHT       64
#define CMANAGER_DEPTH         5
#define CMANAGER_COMPRESSION   1
#define CMANAGER_MASKING       2

#ifdef USE_CMANAGER_HEADER
const struct BitMapHeader CManager_header =
{ 64,64,278,146,5,2,1,0,0,22,22,64,64 };
#endif

#ifdef USE_CMANAGER_BODY
const UBYTE CManager_body[2010] = {
0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,
0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,
0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,
0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,
0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xfc,0x00,0x02,0x07,0x00,
0x00,0xfc,0x00,0x02,0x03,0x00,0x00,0xfc,0x00,0x02,0x03,0x00,0x00,0xfc,0x00,
0x02,0x04,0x80,0x00,0xf9,0x00,0xfc,0x00,0x02,0x1c,0xc0,0x00,0xfc,0x00,0x02,
0x0f,0x40,0x00,0xfc,0x00,0x02,0x08,0x40,0x00,0xfc,0x00,0x02,0x10,0x00,0x00,
0xf9,0x00,0xfc,0x00,0x02,0x6b,0x20,0x00,0xfc,0x00,0x02,0x1a,0xc0,0x00,0xfc,
0x00,0x02,0x16,0x00,0x00,0xfc,0x00,0x02,0x61,0x20,0x00,0xf9,0x00,0xfd,0x00,
0x03,0x06,0xbf,0xc0,0x00,0xfd,0x00,0x03,0x0d,0x8b,0xa0,0x00,0xfd,0x00,0x03,
0x02,0x77,0xa0,0x00,0xfd,0x00,0x03,0x0f,0xff,0x00,0x00,0xf9,0x00,0xfd,0x00,
0x03,0x05,0xe7,0xf0,0x00,0xfd,0x00,0x03,0x23,0x90,0x80,0x00,0xfd,0x00,0x03,
0x1f,0xff,0x00,0x00,0xfd,0x00,0x03,0x3f,0xff,0x90,0x00,0xf9,0x00,0xfd,0x00,
0x03,0x3f,0xf9,0x30,0x00,0xfd,0x00,0x03,0x06,0x31,0xd0,0x00,0xfd,0x00,0x03,
0x1f,0xfd,0xd0,0x00,0xfd,0x00,0x03,0x3f,0xfe,0x80,0x00,0xf9,0x00,0xfe,0x00,
0x04,0x01,0x1a,0xfd,0xe8,0x00,0xfd,0x00,0x03,0xb2,0xf1,0x90,0x00,0xfd,0x00,
0x03,0x7d,0xf8,0x00,0x00,0xfe,0x00,0x04,0x01,0xff,0xfa,0xc8,0x00,0xfc,0x00,
0x02,0x04,0x00,0x00,0xfe,0x00,0x04,0x09,0x8f,0xdb,0x88,0x00,0xfe,0x00,0x04,
0x08,0xdb,0xd4,0xf8,0x00,0xfe,0x00,0x04,0x0d,0xe7,0xd9,0x88,0x00,0xfe,0x00,
0x04,0x03,0xff,0xda,0x40,0x00,0xfc,0x00,0x02,0x24,0x00,0x00,0xff,0x00,0x05,
0x02,0x06,0x7f,0xfa,0xc0,0x00,0xfe,0x00,0x04,0x01,0xdf,0xf1,0xf0,0x00,0xfe,
0x00,0x04,0x01,0x3f,0xfe,0x28,0x00,0xff,0x00,0x05,0x03,0xfb,0xff,0xff,0x40,
0x00,0xfc,0x00,0x02,0x07,0x00,0x00,0xff,0x00,0x05,0x2d,0xe3,0xff,0xe9,0x80,
0x00,0xff,0x00,0x05,0xfc,0xbe,0x7f,0xeb,0x98,0x00,0xff,0x00,0x05,0x0c,0x7f,
0xff,0xee,0x00,0x00,0xff,0x00,0x05,0xf3,0xf9,0xff,0xef,0x64,0x00,0xfc,0x00,
0x02,0x13,0x00,0x00,0x07,0x00,0x03,0x5c,0x9a,0xbf,0x7e,0x80,0x00,0x07,0x00,
0x01,0xc5,0x18,0x7f,0xfa,0xac,0x00,0xff,0x00,0x05,0x3d,0xe2,0x7f,0xff,0xd4,
0x00,0x07,0x00,0x03,0xfd,0xfd,0xff,0xfe,0x20,0x00,0xff,0x00,0x05,0x02,0x00,
0x00,0x03,0x00,0x00,0x07,0x01,0x92,0x85,0x7a,0xfb,0x35,0x12,0x00,0x07,0x00,
0x1b,0x05,0x3a,0xff,0xf4,0x5c,0x00,0x07,0x00,0x1f,0xfa,0xf0,0xff,0xf7,0x10,
0x00,0x07,0x01,0xe1,0xff,0xfd,0xff,0xf6,0xa2,0x00,0xfc,0x00,0x02,0x09,0x00,
0x00,0x07,0x05,0xf9,0xb7,0xfe,0xf9,0xbe,0xa8,0x00,0x07,0x03,0x04,0xf3,0xf3,
0xbf,0xfd,0x9c,0x00,0x07,0x02,0x03,0x0f,0xff,0x7f,0xff,0xe0,0x00,0x07,0x04,
0x03,0xff,0xfc,0xff,0xfe,0x12,0x00,0xfc,0x00,0x02,0x01,0x80,0x00,0x07,0x0b,
0xff,0xbf,0x7e,0x7d,0xfa,0x1c,0x00,0x07,0x0c,0x00,0x7f,0xf8,0x1f,0xfa,0x22,
0x00,0x07,0x08,0x01,0xff,0xfd,0x3f,0xfb,0x02,0x00,0x07,0x00,0x03,0xff,0xfe,
0xff,0xfb,0xd8,0x00,0xfc,0x00,0x02,0x04,0x80,0x00,0x07,0x0f,0x05,0x77,0xf9,
0xbf,0xdf,0xe2,0x00,0x07,0x00,0xfa,0xff,0xf8,0xff,0xfe,0x1e,0x00,0x07,0x08,
0x03,0xff,0xfd,0xff,0xff,0x3c,0x00,0x07,0x00,0x01,0xff,0xfe,0x7f,0xff,0xc9,
0x00,0xfb,0x00,0x01,0x80,0x00,0x07,0x07,0xfc,0x3f,0xfa,0x0d,0xfd,0x0e,0x00,
0x07,0x08,0x02,0xff,0xfc,0xdf,0xfd,0x59,0x00,0x07,0x08,0x01,0xff,0xfe,0xbf,
0xfd,0x81,0x00,0x07,0x00,0x01,0xff,0xff,0x7f,0xfd,0xac,0x00,0xfc,0x00,0x02,
0x02,0x40,0x00,0x07,0x07,0xff,0x3f,0xde,0xff,0xef,0xfb,0x80,0xff,0x00,0x05,
0xff,0xfd,0xcf,0xff,0xfc,0x00,0xff,0x00,0x05,0xff,0xfe,0x5f,0xff,0x8e,0x00,
0x07,0x08,0x01,0xff,0xff,0x3f,0xff,0xa4,0x80,0xfb,0x00,0x01,0x40,0x00,0x07,
0x0f,0xfa,0x3d,0xfc,0x7e,0xe4,0xff,0x00,0x07,0x04,0x05,0x7f,0xfc,0xdf,0xfc,
0xbc,0x80,0x07,0x00,0x01,0xff,0xfe,0xff,0xfc,0xd0,0x80,0x07,0x08,0x00,0xff,
0xff,0x3f,0xfc,0xe6,0x00,0xfc,0x00,0x02,0x03,0x00,0x00,0x07,0x07,0xfe,0x3f,
0xee,0xde,0x77,0x8e,0x40,0x07,0x04,0x01,0x5f,0xff,0x3f,0xff,0xc4,0x80,0x07,
0x04,0x00,0xff,0xff,0x5f,0xff,0xe7,0x00,0xff,0x00,0xff,0xff,0x03,0xbf,0xff,
0xf2,0x40,0xf9,0x00,0x07,0x03,0xff,0xde,0xff,0x9f,0x7e,0x34,0x00,0xff,0x00,
0x05,0x3f,0xff,0x1f,0xfe,0x6c,0xc0,0xff,0x00,0x05,0x7f,0xff,0xdf,0xfe,0x62,
0x40,0x07,0x04,0x00,0xff,0xff,0xbf,0xfe,0x73,0x00,0xfc,0x00,0x02,0x01,0x80,
0x00,0x07,0x07,0xfc,0x1f,0xf7,0xef,0xfb,0xdd,0xc0,0x07,0x00,0x03,0xff,0xff,
0xef,0xff,0xe0,0x80,0xff,0x00,0xfd,0xff,0x01,0xf9,0x00,0x07,0x04,0x00,0x7f,
0xff,0x9f,0xff,0xf3,0x20,0xf9,0x00,0x07,0x01,0xf1,0x0f,0x67,0xff,0xbf,0x1f,
0x40,0x07,0x00,0x0e,0xbf,0xff,0xcf,0xff,0x30,0x00,0x07,0x02,0x00,0x7f,0xff,
0xaf,0xff,0x33,0x20,0xff,0x00,0x05,0x7f,0xff,0xdf,0xff,0x39,0x80,0xfb,0x00,
0x01,0xc0,0x00,0x07,0x01,0xff,0xce,0x7e,0xff,0xfd,0xb1,0x30,0x07,0x02,0x00,
0x3f,0xfe,0xd7,0xff,0xb2,0x50,0x07,0x02,0x00,0x3f,0xfe,0xff,0xff,0xbc,0x90,
0xff,0x00,0x05,0x7f,0xfe,0xcf,0xff,0xb9,0x80,0xfe,0x00,0x04,0x01,0x00,0x00,
0x40,0x00,0x07,0x01,0xff,0x0f,0xfb,0xc3,0xdf,0xda,0xe0,0xff,0x00,0x05,0xf7,
0xfb,0xd9,0xff,0xd9,0xc0,0xff,0x00,0x05,0x7f,0xfb,0xd7,0xff,0xd9,0xd0,0x07,
0x02,0x00,0x3f,0xfb,0xef,0xff,0xdc,0x80,0xfe,0x00,0x04,0x04,0x00,0x00,0x20,
0x00,0x07,0x02,0xfd,0x07,0xef,0xcd,0xfc,0xa6,0xf0,0x07,0x01,0x03,0xdf,0xef,
0xa9,0xff,0xaf,0xd0,0xff,0x00,0x05,0x3f,0xef,0xdb,0xff,0xa8,0x00,0x07,0x02,
0x00,0x3f,0xef,0xe7,0xff,0xac,0xc8,0xfe,0x00,0x04,0x10,0x00,0x00,0x50,0x00,
0x07,0x01,0xf3,0xf7,0xb8,0xcb,0xe4,0xd2,0xd8,0x07,0x00,0x0c,0x1f,0xbc,0x9b,
0xff,0xd7,0xe8,0x07,0x01,0x00,0x1f,0xbc,0xdf,0xff,0xd7,0x68,0xff,0x00,0x05,
0x3f,0xbc,0xe7,0xff,0xd4,0xc0,0xfe,0x00,0x04,0x43,0x00,0x00,0x28,0x00,0x07,
0x01,0xff,0xd6,0xfc,0x77,0xe7,0xac,0xd0,0x07,0x01,0x00,0x2e,0xfc,0x48,0xff,
0xab,0x88,0x07,0x01,0x00,0x3e,0xfc,0x6b,0xff,0xaf,0xc8,0xff,0x00,0x05,0x1e,
0xfc,0x77,0xff,0xaf,0xe0,0xff,0x00,0x05,0x01,0x03,0x80,0x00,0x50,0x00,0x07,
0x01,0xff,0xdb,0xc4,0x44,0x7f,0x41,0x98,0xff,0x00,0x05,0xef,0xe4,0x65,0xff,
0xde,0x60,0xff,0x00,0x05,0x1f,0xe4,0x7c,0xff,0xdf,0x48,0x07,0x01,0x00,0x1f,
0xe4,0x73,0xff,0xdf,0x80,0xfe,0x00,0x04,0x1b,0x80,0x00,0x20,0x00,0x07,0x00,
0x7c,0xfe,0x22,0x65,0xf7,0xa6,0xb8,0x07,0x00,0x03,0x03,0x22,0x4f,0xff,0xcb,
0x08,0x07,0x00,0x80,0x0f,0x22,0x6d,0xff,0xff,0xc8,0xff,0x00,0x05,0x1f,0x22,
0x73,0xff,0xfc,0x00,0xfe,0x00,0x01,0xdd,0x80,0xfe,0x00,0x07,0x00,0xff,0xe3,
0x03,0xe9,0xfc,0x38,0x68,0x07,0x00,0x80,0x1f,0x03,0xe4,0x7e,0xa4,0x50,0x07,
0x00,0x80,0x1f,0x03,0xf5,0xff,0x77,0xb0,0xff,0x00,0x05,0x0f,0x03,0xfb,0xff,
0xf8,0x08,0xfe,0x00,0x00,0xfc,0xfd,0x00,0x07,0x00,0x7f,0xdd,0x13,0x0b,0xf4,
0x81,0x60,0xff,0x00,0x05,0x17,0x13,0x97,0x7e,0x8c,0xc0,0xff,0x00,0x05,0x2f,
0x13,0x9f,0x79,0xd3,0xc0,0x07,0x00,0x80,0x0f,0x13,0x98,0xff,0xe0,0x20,0xfe,
0x00,0x01,0xec,0x60,0xfe,0x00,0x07,0x00,0xbe,0x09,0x9f,0x97,0xc1,0xe2,0x80,
0x07,0x00,0x41,0x13,0x9f,0x8a,0xeb,0xd6,0x00,0xff,0x00,0x05,0xe7,0x9f,0x96,
0xf6,0x8e,0x00,0x07,0x00,0x80,0x0f,0x9f,0x99,0xff,0x01,0x80,0xfe,0x00,0xff,
0x60,0xfe,0x00,0x07,0x00,0x3b,0x99,0xf1,0x9a,0xd1,0xca,0x00,0x07,0x00,0x47,
0x9b,0xf9,0x9c,0x6f,0x58,0x00,0x07,0x00,0x40,0x6f,0xf9,0x92,0x9a,0x38,0x00,
0xff,0x00,0x05,0x07,0xf9,0x9d,0xfc,0x06,0x00,0xfe,0x00,0x01,0x06,0x60,0xfe,
0x00,0x07,0x00,0x7b,0xda,0x58,0xc0,0xb1,0x68,0x00,0x07,0x00,0x47,0xd9,0xd8,
0xc4,0xcf,0x20,0x00,0x07,0x00,0x40,0x27,0xd8,0xc6,0x68,0xe0,0x00,0xff,0x00,
0x05,0x07,0xd8,0xc5,0xf0,0x18,0x00,0xfe,0x00,0x01,0x27,0x38,0xfe,0x00,0x07,
0x00,0x7f,0xed,0x88,0xd0,0xa4,0x20,0x00,0x07,0x00,0x03,0xe8,0x88,0xd1,0x1c,
0x80,0x00,0xff,0x00,0x05,0x13,0x88,0xdd,0x23,0x80,0x00,0x07,0x00,0x40,0x07,
0x88,0xde,0xc0,0x60,0x00,0xfe,0x00,0x01,0x77,0x20,0xfe,0x00,0x07,0x00,0x3c,
0x5d,0x4c,0xc8,0x70,0x00,0x00,0x07,0x00,0x03,0xe5,0x4c,0x85,0x12,0x00,0x00,
0x07,0x00,0x20,0x07,0x4c,0xfc,0x8e,0x00,0x00,0xff,0x00,0x05,0x03,0x4c,0xfe,
0x01,0x00,0x00,0xfe,0x00,0x00,0xb3,0xfd,0x00,0x07,0x00,0x3c,0xef,0xa7,0xeb,
0xa8,0x00,0x00,0x07,0x00,0x23,0x16,0x66,0xd4,0x60,0x00,0x00,0x07,0x00,0x20,
0x01,0xe7,0x34,0x18,0x00,0x00,0xff,0x00,0x05,0x03,0xe7,0xf8,0x04,0x00,0x00,
0xfe,0x00,0x00,0x18,0xfd,0x00,0x07,0x00,0x3f,0xed,0x3e,0x7f,0xe0,0x00,0x00,
0xff,0x00,0x05,0x12,0xb6,0x80,0xc0,0x00,0x00,0xff,0x00,0x05,0x01,0xb9,0xd1,
0x20,0x00,0x00,0x07,0x00,0x20,0x03,0xbf,0xe0,0x10,0x00,0x00,0xfe,0x00,0x00,
0x40,0xfd,0x00,0x07,0x00,0x1f,0xf7,0xee,0x7d,0x80,0x00,0x00,0xff,0x00,0x05,
0x0a,0xca,0x5f,0x80,0x00,0x00,0x07,0x00,0x10,0x03,0xf7,0x03,0x80,0x00,0x00,
0xff,0x00,0x05,0x01,0xff,0x80,0x40,0x00,0x00,0xf9,0x00,0x04,0x00,0x1f,0xe7,
0x4e,0x73,0xfe,0x00,0x04,0x00,0x10,0x1b,0xdf,0xf6,0xfe,0x00,0x04,0x00,0x10,
0x00,0xba,0x0e,0xfe,0x00,0xff,0x00,0x02,0x01,0xfc,0x01,0xfe,0x00,0xf9,0x00,
0x04,0x00,0x0f,0xc2,0xc9,0xc8,0xfe,0x00,0xff,0x00,0x02,0x05,0x7f,0xc8,0xfe,
0x00,0xff,0x00,0x02,0x38,0xe8,0x38,0xfe,0x00,0x04,0x00,0x10,0x01,0xf0,0x04,
0xfe,0x00,0xf9,0x00,0x04,0x00,0x0f,0xac,0x94,0x30,0xfe,0x00,0xff,0x00,0x02,
0x4f,0xac,0x60,0xfe,0x00,0x04,0x00,0x08,0x31,0xa3,0xe0,0xfe,0x00,0xfe,0x00,
0x01,0x40,0x10,0xfe,0x00,0xf9,0x00,0x04,0x00,0x0f,0x98,0xde,0x40,0xfe,0x00,
0x03,0x00,0x08,0x5f,0x3d,0xfd,0x00,0x03,0x00,0x08,0x20,0x03,0xfd,0x00,0xfd,
0x00,0x00,0xc0,0xfe,0x00,0xf9,0x00,0x03,0x00,0x07,0xb3,0x66,0xfd,0x00,0xff,
0x00,0x01,0x7c,0xfc,0xfd,0x00,0xfe,0x00,0x00,0x1c,0xfd,0x00,0x03,0x00,0x08,
0x00,0x02,0xfd,0x00,0xf9,0x00,0x03,0x00,0x0f,0x8f,0x88,0xfd,0x00,0x03,0x00,
0x04,0x71,0xa0,0xfd,0x00,0xfe,0x00,0x00,0x60,0xfd,0x00,0x03,0x00,0x08,0x00,
0x18,0xfd,0x00,0xf9,0x00,0x03,0x00,0x03,0xfa,0x20,0xfd,0x00,0x03,0x00,0x04,
0x06,0x80,0xfd,0x00,0x03,0x00,0x04,0x01,0x80,0xfd,0x00,0xfe,0x00,0x00,0x60,
0xfd,0x00,0xf9,0x00,0x03,0x00,0x03,0xf8,0x80,0xfd,0x00,0xff,0x00,0x00,0x0a,
0xfc,0x00,0xff,0x00,0x00,0x06,0xfc,0x00,0x03,0x00,0x04,0x01,0x80,0xfd,0x00,
0xf9,0x00,0x02,0x00,0x01,0xd0,0xfc,0x00,0x02,0x00,0x02,0x28,0xfc,0x00,0xff,
0x00,0x00,0x18,0xfc,0x00,0xff,0x00,0x00,0x04,0xfc,0x00,0xf9,0x00,0x02,0x00,
0x01,0x20,0xfc,0x00,0x02,0x00,0x02,0xa0,0xfc,0x00,0x02,0x00,0x02,0x60,0xfc,
0x00,0xff,0x00,0x00,0x10,0xfc,0x00,0xf9,0x00,0x02,0x00,0x02,0xc0,0xfc,0x00,
0x02,0x00,0x01,0x80,0xfc,0x00,0x02,0x00,0x01,0x80,0xfc,0x00,0x02,0x00,0x02,
0x40,0xfc,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,
0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,
0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,
0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,
0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,0xf9,0x00,
 };
#endif
