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
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/hmac.h>

#include <codec.h>

#define SK_LEN 32
#define SHA256_LEN 32
#define EPHID_LEN 16
#define EPOCH_LEN 15 // In minutes
#define EPOCHS_PER_DAY (((24 * 60) / EPOCH_LEN) + 1)
#define RETENTION_PERIOD (1) // In days
#define MAX_EPHIDS (RETENTION_PERIOD * EPOCHS_PER_DAY)

#define MAX_STDOUT 256

const uint8_t BROADCAST_KEY[32] = "Broadcast key";
const uint32_t BROADCAST_KEY_LEN = 13;

static uint8_t SKT_0[SK_LEN];

#define NZ(x) if(x) fprintf(stderr,"Error in %s\n",__func__)

void dp3t_random(uint8_t *buf, int len) {
	int i = 0;
	for (i = 0; i < len; i++)
		buf[i] = (uint8_t) 0x0;
	// mockup zero random
}

static uint8_t EPHIDS_LOCAL[EPOCHS_PER_DAY][EPHID_LEN];


uint8_t *dp3t_get_skt_0(void)
{
	dp3t_random(SKT_0, SK_LEN);
	return SKT_0;
}


/*
 *   This function creates the next key in the chain of SK_t's.
 *   It is called either for the daily rotation.
 */
void dp3t_get_skt_1(const uint8_t *skt_0, uint8_t *skt_1)
{
	int ret;
	wc_Sha256 sha;
	ret = wc_InitSha256(&sha);
	assert(ret == 0);
	ret = wc_Sha256Update(&sha, skt_0, SK_LEN);
	assert(ret == 0);
	wc_Sha256Final(&sha, skt_1);
	wc_Sha256Free(&sha);
}

static char outbuf[MAX_STDOUT];

static void print_hex(const uint8_t *x, int len)
{
	NZ(len>MAX_STDOUT);
	bin2hex(outbuf,(char*)x,len);
	puts((const char*)outbuf);
	// putc('\n', stdout);
}

static void print_ephid(const uint8_t *x)
{
	print_hex(x, EPHID_LEN);
}

static void print_sk(const uint8_t *x)
{
	print_hex(x, SK_LEN);
}

void dp3t_print_ephids(void)
{
	int i;
	for (i = 0; i < EPOCHS_PER_DAY; i++) {
		printf("[ %03d ] ", i);
		print_ephid(EPHIDS_LOCAL[i]);
	}
}

// static const uint8_t zeroes = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void dp3t_create_ephids(const uint8_t *skt_0)
{
	Aes aes;
	Hmac hmac;
	uint8_t prf[SK_LEN];
	register int i;
	uint8_t zeroes[EPHID_LEN];
	memset(zeroes, 0, EPHID_LEN);
	printf("SK0: ");
	print_sk(skt_0);

	/* PRF */
	wc_HmacInit(&hmac, NULL, INVALID_DEVID); 
	wc_HmacSetKey(&hmac, WC_SHA256, skt_0, SK_LEN);
	wc_HmacUpdate(&hmac, BROADCAST_KEY, BROADCAST_KEY_LEN);
	wc_HmacFinal(&hmac, prf);
	printf("  PRF: ");
	print_sk(prf);

	/* PRG */
	wc_AesInit(&aes, NULL, INVALID_DEVID);
	wc_AesSetKeyDirect(&aes, prf, 32, zeroes, AES_ENCRYPTION);
	for(i = 0; i < EPOCHS_PER_DAY; i++)
		wc_AesCtrEncrypt(&aes, EPHIDS_LOCAL[i], zeroes, EPHID_LEN); 
	dp3t_print_ephids();
	wc_HmacFree(&hmac);
	wc_AesFree(&aes);
}


uint8_t *dp3t_get_ephid(int epoch)
{
	return EPHIDS_LOCAL[epoch];
}


#ifdef MODULE_WOLFCRYPT_TEST
extern int wolfcrypt_test(void* args);
static int wolftest(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	wolfcrypt_test(NULL);
	return 0;
}
#endif

int main(int argc, char **argv) {
	/* dp3t */
// #ifdef DEBUG
// 	wolfSSL_Debugging_ON();
// #endif

	dp3t_random(SKT_0, SK_LEN);
	dp3t_create_ephids(SKT_0);
	exit(0);
}
