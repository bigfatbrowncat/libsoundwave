/*
  Copyright (c) 2007-2009, The Musepack Development Team
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the following
  disclaimer in the documentation and/or other materials provided
  with the distribution.

  * Neither the name of the The Musepack Development Team nor the
  names of its contributors may be used to endorse or promote
  products derived from this software without specific prior
  written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <mpc/mpcdec.h>
#include "internal.h"
#include "huffman.h"
#include "mpc_bits_reader.h"

const mpc_uint32_t Cnk[MAX_ENUM / 2][MAX_ENUM] =
{
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
	{0, 0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120, 136, 153, 171, 190, 210, 231, 253, 276, 300, 325, 351, 378, 406, 435, 465},
	{0, 0, 0, 1, 4, 10, 20, 35, 56, 84, 120, 165, 220, 286, 364, 455, 560, 680, 816, 969, 1140, 1330, 1540, 1771, 2024, 2300, 2600, 2925, 3276, 3654, 4060, 4495},
	{0, 0, 0, 0, 1, 5, 15, 35, 70, 126, 210, 330, 495, 715, 1001, 1365, 1820, 2380, 3060, 3876, 4845, 5985, 7315, 8855, 10626, 12650, 14950, 17550, 20475, 23751, 27405, 31465},
	{0, 0, 0, 0, 0, 1, 6, 21, 56, 126, 252, 462, 792, 1287, 2002, 3003, 4368, 6188, 8568, 11628, 15504, 20349, 26334, 33649, 42504, 53130, 65780, 80730, 98280, 118755, 142506, 169911},
	{0, 0, 0, 0, 0, 0, 1, 7, 28, 84, 210, 462, 924, 1716, 3003, 5005, 8008, 12376, 18564, 27132, 38760, 54264, 74613, 100947, 134596, 177100, 230230, 296010, 376740, 475020, 593775, 736281},
	{0, 0, 0, 0, 0, 0, 0, 1, 8, 36, 120, 330, 792, 1716, 3432, 6435, 11440, 19448, 31824, 50388, 77520, 116280, 170544, 245157, 346104, 480700, 657800, 888030, 1184040, 1560780, 2035800, 2629575},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 9, 45, 165, 495, 1287, 3003, 6435, 12870, 24310, 43758, 75582, 125970, 203490, 319770, 490314, 735471, 1081575, 1562275, 2220075, 3108105, 4292145, 5852925, 7888725},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 10, 55, 220, 715, 2002, 5005, 11440, 24310, 48620, 92378, 167960, 293930, 497420, 817190, 1307504, 2042975, 3124550, 4686825, 6906900, 10015005, 14307150, 20160075},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 11, 66, 286, 1001, 3003, 8008, 19448, 43758, 92378, 184756, 352716, 646646, 1144066, 1961256, 3268760, 5311735, 8436285, 13123110, 20030010, 30045015, 44352165},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 12, 78, 364, 1365, 4368, 12376, 31824, 75582, 167960, 352716, 705432, 1352078, 2496144, 4457400, 7726160, 13037895, 21474180, 34597290, 54627300, 84672315},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 13, 91, 455, 1820, 6188, 18564, 50388, 125970, 293930, 646646, 1352078, 2704156, 5200300, 9657700, 17383860, 30421755, 51895935, 86493225, 141120525},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 14, 105, 560, 2380, 8568, 27132, 77520, 203490, 497420, 1144066, 2496144, 5200300, 10400600, 20058300, 37442160, 67863915, 119759850, 206253075},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 15, 120, 680, 3060, 11628, 38760, 116280, 319770, 817190, 1961256, 4457400, 9657700, 20058300, 40116600, 77558760, 145422675, 265182525},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 16, 136, 816, 3876, 15504, 54264, 170544, 490314, 1307504, 3268760, 7726160, 17383860, 37442160, 77558760, 155117520, 300540195},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 17, 153, 969, 4845, 20349, 74613, 245157, 735471, 2042975, 5311735, 13037895, 30421755, 67863915, 145422675, 300540195}
};

const mpc_uint8_t Cnk_len[MAX_ENUM / 2][MAX_ENUM] =
{
	{0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	{0, 0, 2, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9},
	{0, 0, 0, 2, 4, 5, 6, 6, 7, 7, 8, 8, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 13, 13},
	{0, 0, 0, 0, 3, 4, 6, 7, 7, 8, 9, 9, 10, 10, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 15, 16},
	{0, 0, 0, 0, 0, 3, 5, 6, 7, 8, 9, 10, 11, 11, 12, 13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18},
	{0, 0, 0, 0, 0, 0, 3, 5, 7, 8, 9, 10, 11, 12, 13, 13, 14, 15, 15, 16, 16, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20},
	{0, 0, 0, 0, 0, 0, 0, 3, 6, 7, 9, 10, 11, 12, 13, 14, 15, 15, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 21, 22, 22},
	{0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 8, 9, 11, 12, 13, 14, 15, 16, 17, 17, 18, 19, 19, 20, 21, 21, 22, 22, 23, 23, 23, 24},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 6, 8, 10, 11, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 21, 22, 23, 23, 24, 24, 25, 25},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 9, 10, 12, 13, 15, 16, 17, 18, 19, 20, 21, 21, 22, 23, 24, 24, 25, 25, 26, 26},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 9, 11, 13, 14, 15, 17, 18, 19, 20, 21, 22, 23, 23, 24, 25, 26, 26, 27, 27},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 9, 11, 13, 15, 16, 17, 19, 20, 21, 22, 23, 24, 25, 25, 26, 27, 28, 28},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 10, 12, 14, 15, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 27, 28, 29},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 7, 10, 12, 14, 16, 17, 19, 20, 21, 23, 24, 25, 26, 27, 28, 28, 29},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 10, 12, 14, 16, 18, 19, 21, 22, 23, 25, 26, 27, 28, 29, 30},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 8, 10, 13, 15, 17, 18, 20, 21, 23, 24, 25, 27, 28, 29, 30}

};

const mpc_uint32_t Cnk_lost[MAX_ENUM / 2][MAX_ENUM] =
{
	{0, 0, 1, 0, 3, 2, 1, 0, 7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	{0, 0, 1, 2, 6, 1, 11, 4, 28, 19, 9, 62, 50, 37, 23, 8, 120, 103, 85, 66, 46, 25, 3, 236, 212, 187, 161, 134, 106, 77, 47, 16},
	{0, 0, 0, 0, 6, 12, 29, 8, 44, 8, 91, 36, 226, 148, 57, 464, 344, 208, 55, 908, 718, 508, 277, 24, 1796, 1496, 1171, 820, 442, 36, 3697, 3232},
	{0, 0, 0, 0, 3, 1, 29, 58, 2, 46, 182, 17, 309, 23, 683, 228, 1716, 1036, 220, 3347, 2207, 877, 7529, 5758, 3734, 1434, 15218, 12293, 9017, 5363, 1303, 29576},
	{0, 0, 0, 0, 0, 2, 11, 8, 2, 4, 50, 232, 761, 46, 1093, 3824, 2004, 7816, 4756, 880, 12419, 6434, 31887, 23032, 12406, 65292, 50342, 32792, 12317, 119638, 92233, 60768},
	{0, 0, 0, 0, 0, 0, 1, 4, 44, 46, 50, 100, 332, 1093, 3187, 184, 4008, 14204, 5636, 26776, 11272, 56459, 30125, 127548, 85044, 31914, 228278, 147548, 49268, 454801, 312295, 142384},
	{0, 0, 0, 0, 0, 0, 0, 0, 28, 8, 182, 232, 332, 664, 1757, 4944, 13320, 944, 15148, 53552, 14792, 91600, 16987, 178184, 43588, 390776, 160546, 913112, 536372, 61352, 1564729, 828448},
	{0, 0, 0, 0, 0, 0, 0, 0, 7, 19, 91, 17, 761, 1093, 1757, 3514, 8458, 21778, 55490, 5102, 58654, 204518, 33974, 313105, 1015577, 534877, 1974229, 1086199, 4096463, 2535683, 499883, 6258916},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 9, 36, 309, 46, 3187, 4944, 8458, 16916, 38694, 94184, 230358, 26868, 231386, 789648, 54177, 1069754, 3701783, 1481708, 6762211, 2470066, 13394357, 5505632},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 62, 226, 23, 1093, 184, 13320, 21778, 38694, 77388, 171572, 401930, 953086, 135896, 925544, 3076873, 8340931, 3654106, 13524422, 3509417, 22756699, 2596624},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 50, 148, 683, 3824, 4008, 944, 55490, 94184, 171572, 343144, 745074, 1698160, 3931208, 662448, 3739321, 12080252, 32511574, 12481564, 49545413, 5193248},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 37, 57, 228, 2004, 14204, 15148, 5102, 230358, 401930, 745074, 1490148, 3188308, 7119516, 16170572, 3132677, 15212929, 47724503, 127314931, 42642616},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 23, 464, 1716, 7816, 5636, 53552, 58654, 26868, 953086, 1698160, 3188308, 6376616, 13496132, 29666704, 66353813, 14457878, 62182381, 189497312},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 344, 1036, 4756, 26776, 14792, 204518, 231386, 135896, 3931208, 7119516, 13496132, 26992264, 56658968, 123012781, 3252931, 65435312},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 208, 220, 880, 11272, 91600, 33974, 789648, 925544, 662448, 16170572, 29666704, 56658968, 113317936, 236330717, 508019104},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 103, 55, 3347, 12419, 56459, 16987, 313105, 54177, 3076873, 3739321, 3132677, 66353813, 123012781, 236330717}
};

static const mpc_uint8_t log2_[32] =
{ 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6};

static const mpc_uint8_t log2_lost[32] =
{ 0, 1, 0, 3, 2, 1, 0, 7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 31};

mpc_int32_t mpc_bits_golomb_dec(mpc_bits_reader * r, const mpc_uint_t k)
{
	unsigned int l = 0;
	unsigned int code = r->buff[0] & ((1 << r->count) - 1);

	while( code == 0 ) {
		l += r->count;
		r->buff++;
		code = r->buff[0];
		r->count = 8;
	}

	while( ((1 << (r->count - 1)) & code) == 0 ) {
		l++;
		r->count--;
	}
	r->count--;

	while( r->count < k ) {
		r->buff++;
		r->count += 8;
		code = (code << 8) | r->buff[0];
	}

	r->count -= k;

	return (l << k) | ((code >> r->count) & ((1 << k) - 1));
}

mpc_uint32_t mpc_bits_log_dec(mpc_bits_reader * r, mpc_uint_t max)
{
	mpc_uint32_t value = 0;
	if (max == 0)
		return 0;
	if (log2_[max - 1] > 1)
		value = mpc_bits_read(r, log2_[max - 1] - 1);
	if (value >= log2_lost[max - 1])
		value = ((value << 1) | mpc_bits_read(r, 1)) - log2_lost[max - 1];
	return value;
}

unsigned int mpc_bits_get_size(mpc_bits_reader * r, mpc_uint64_t * p_size)
{
	unsigned char tmp;
	mpc_uint64_t size = 0;
	unsigned int ret = 0;

	do {
		tmp = mpc_bits_read(r, 8);
		size = (size << 7) | (tmp & 0x7F);
		ret++;
	} while((tmp & 0x80));

	*p_size = size;
	return ret;
}

int mpc_bits_get_block(mpc_bits_reader * r, mpc_block * p_block)
{
	int size = 2;

	p_block->size = 0;
	p_block->key[0] = mpc_bits_read(r, 8);
	p_block->key[1] = mpc_bits_read(r, 8);

	size += mpc_bits_get_size(r, &(p_block->size));

	if (p_block->size >= size) // check if the block size doesn't conflict with the header size
		p_block->size -= size;

	return size;
}



