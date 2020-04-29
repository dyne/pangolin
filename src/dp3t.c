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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/hmac.h>

#include <dp3t.h>

#ifdef JEMALLOC
#include <jemalloc/jemalloc.h>
#else
#include <stdlib.h>
#endif

#ifndef EMBEDDED
#define XXMALLOC malloc
#define XXFREE free
#endif

#define SK_LEN 32
#define EPHID_LEN 16

#define SAFE_ALLOC(p) if( p==NULL ) { \
	fprintf(stderr,"ERROR %s %s\n", __func__, strerror(errno)); exit(1); }

// zero nonce, one ephid long
const uint8_t zero16[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint8_t zero32[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

char *create_key(WC_RNG* rng) { return NULL; }

// renew the SK in place (reuse input buffer)
int renew_key(uint8_t *sk) { 
	wc_Sha256 sha;
	uint8_t *skn = XXMALLOC(SK_LEN);
	assert( skn );
	assert( wc_InitSha256(&sha) == 0);
	assert( wc_Sha256Update(&sha, sk, SK_LEN) == 0);
	wc_Sha256Final(&sha, skn);
	wc_Sha256Free(&sha);
	memcpy(sk, skn, SK_LEN);
	XXFREE(skn);
	return(0);
}

// epd = epochs per day = ((24 * 60) / ttl in minutes) +1
// memory allocated = epd * (EPHID_LEN +1)
beacons_t *alloc_beacons(const uint8_t *sk, const char *bk, uint32_t num) {
	Aes aes;
	Hmac hmac;
	beacons_t *beacons;
	assert(num > 0);
	beacons = XXMALLOC(sizeof(beacons_t));
	SAFE_ALLOC( beacons );
	beacons->data = XXMALLOC(num <<4 ); // * EPHID_LEN 16
	SAFE_ALLOC( beacons->data );
	beacons->num = num;
	uint8_t prf[32];

	/* PRF */
	wc_HmacInit(&hmac, NULL, INVALID_DEVID); 
	wc_HmacSetKey(&hmac, WC_SHA256, sk, SK_LEN);
	wc_HmacUpdate(&hmac, (const byte*)bk, strlen(bk));
	wc_HmacFinal(&hmac, prf);
	wc_HmacFree(&hmac);

	wc_AesInit(&aes, NULL, INVALID_DEVID);
	wc_AesSetKeyDirect(&aes, prf, 32, zero16, AES_ENCRYPTION);
	register uint8_t *p = beacons->data;
	register uint32_t i;
	for(i=0; i<num; i++, p+=16)
		wc_AesCtrEncrypt(&aes, p, zero16, 16); 
	wc_AesFree(&aes);
	return(beacons);
	// TODO: randomize
}

void free_beacons(beacons_t *b) {
	assert(b);
	assert(b->num > 0);
	assert(b->data);
	XXFREE(b->data);
	XXFREE(b);
}

uint8_t *get_beacon(beacons_t *b, uint32_t num) {
	assert(b);
	assert(num < b->num);
	return(&b->data[num<<4]); // multiply by 16 = EPHID_LEN
}

struct dictionary *match_positive_beacons(beacons_t *ephids, positives_t *sks, 
                                          const char *bk, uint32_t num) {
	struct dictionary *dic = dic_new(0);
	// initial buffer allocation: number of ephids / 8
	Hmac hmac;
	Aes aes;
	wc_HmacInit(&hmac, NULL, INVALID_DEVID); 
	wc_AesInit(&aes, NULL, INVALID_DEVID);

	uint32_t bklen = strlen(bk);
	uint8_t prf[32];
	uint8_t *sk;
	register uint32_t i, ii, iii;
	for(i=0;i<sks->num;i++) {
		// calculate PRF of current posisk
		sk = &sks->data[i<<5]; // multiply by 32 = SK_LEN
		wc_HmacSetKey(&hmac, WC_SHA256, sk, 32);
		wc_HmacUpdate(&hmac, (const byte*)bk, bklen);
		wc_HmacFinal(&hmac, prf);
		wc_AesSetKeyDirect(&aes, prf, 32, zero16, AES_ENCRYPTION);
		// calculate ephids of the current posisk
		uint8_t skeph[16];
		for(ii=0; ii<num; ii++) {
			wc_AesCtrEncrypt(&aes, skeph, zero16, 16); 
			for(iii=0; iii<ephids->num; iii++) {
				if( memcmp(skeph, &ephids->data[iii<<4], 16) ==0) {
					dic_add(dic, sk, 32); *dic->value+=1;
				}
			}
		}
	}

	wc_AesFree(&aes);
	wc_HmacFree(&hmac);
	return(dic);
}
