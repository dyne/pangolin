/* The Fastest Proximity Tracing in the West (FPTW)
 * aka the Secret Pangolin Code
 *
 * Copyright (C) 2020 Dyne.org foundation
 * designed, written and maintained by Daniele Lacamera and Denis Roio
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef __DP3T_H__
#define __DP3T_H__

#include <inttypes.h>
#include <hashdict.h>

// simple offset structure of num elements sized EPHID_LEN bytes
typedef struct {
	uint32_t ttl;                //< ttl in minutes
	uint8_t *data;              //< data offset
	char *broadcast;          //< broadcast key
	uint32_t broadcast_len;  //< broadcast key length
} beacons_t;

typedef struct {
	uint8_t *data;    //< data offset
	uint32_t count; //< number of sk objects stored in data
} positives_t;

int renew_key(uint8_t *sk);

const beacons_t *alloc_beacons(const uint8_t *sk, const char *bk, uint32_t bklen, uint32_t num);
void       free_beacons(const beacons_t *b);
uint8_t   *get_beacon(const beacons_t *b, uint32_t num);

struct dictionary *match_positives(const positives_t *sks, const beacons_t *ephids);


#endif
