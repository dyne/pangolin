#include <inttypes.h>
#include <assert.h>
#include <string.h>

#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/hmac.h>

#include <codec.h>
#include <bdd-for-c.h>
// #include <dp3t.h>

#define SK_LEN 32
#define BK "Broadcast key"
#define EPHID_LEN 16

// test vectors from issue #62
#define PRF "d59d48e21935f3389e3bd3eb02cf66989190b7b09ed6c0a4b9616f49455c4f9a"
#define ephid0 "8fd521e6c47060efcbfdb9b801c30743"

static char buffer[64];
static char *hex(const char *src) {
	hex2bin(buffer, src);
	return buffer;
}

static const int compare(char *left, char *right, size_t len) {
	assert(! (left == right)); // avoid comparing buffer with itself
	return(strncmp((const char *)left, (const char*)right, len) == 0);
}

spec("DP-3T") {
	char SK0[SK_LEN];
	memset(SK0,0x0,SK_LEN);
	context("lowcost") {
		byte prf[SK_LEN];
		it("should use 'Broadcast key'") check(strcmp(BK, "Broadcast key")==0);
		it("should have zero derived PRF matching d59d48e21935f338...") {
			Hmac hmac;
			wc_HmacInit(&hmac, NULL, INVALID_DEVID);
			wc_HmacSetKey(&hmac, WC_SHA256, (byte*)SK0, SK_LEN);
			wc_HmacUpdate(&hmac, (byte*)BK, strlen(BK));
			wc_HmacFinal(&hmac, (byte*)prf);
			check( compare(prf, hex(PRF), SK_LEN) );
			wc_HmacFree(&hmac);
		}

		it("should have zero derived EphID matching 8fd521e6c47060ef...") {
			Aes aes;
			char out[EPHID_LEN];
			wc_AesInit(&aes, NULL, INVALID_DEVID);
			wc_AesSetKeyDirect(&aes, prf, 32, SK0, AES_ENCRYPTION);
			wc_AesCtrEncrypt(&aes, out, SK0, EPHID_LEN); 
			check( compare(out, hex(ephid0), EPHID_LEN) );
			wc_AesFree(&aes);
		}
	}
}
