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

#include <inttypes.h>
#include <assert.h>
#include <string.h>

#include <src/dp3t.h>
// #include <src/codec.h>

#include <test/bdd-for-c.h>
#include <test/aux.h>
#include <test/vectors.h>

#define SK_LEN 32
#define EPHID_LEN 16

spec("DP-3T") {
	static char BK[] = "Broadcast key";

	before() {
		printf("version: %s\n",VERSION);
		printf("platform: %s\n", PLATFORM);
		printf("broadcast: %s\n", BK);
	}
	after() {
	}
	describe("lowcost") {
		static uint8_t sk[32];
		static beacons_t *b;
		static uint8_t *e;
		static uint32_t epd = (24*60)/15;
		before() {
			gcinit(&head);
			memset(sk,0x0,32);
		}
		after() {
			free_beacons(b);
			gcfree(&head);
		}
		it("should use 'Broadcast key'") check( compare(BK, "Broadcast key", strlen(BK)) );
		it("should have zero derived SK1 matching 66687aadf862bd77...") {
			renew_key(sk);
			check( compare(sk, hex32(SK1), 32) );
		}
		it("should have zero derived SK2 matching 2b32db6c2c0a6235...") {
			renew_key(sk);
			check( compare(sk, hex32(SK2), 32) );
		}
		it("should allocate memory and generate beacons") {
			memset(sk,0x0,32);
			b = alloc_beacons(sk, BK, epd);
			check( b != NULL ); 
		}
		it("should have zero derived EphID 1 matching 8fd521e6c47060ef...") {
			e = get_beacon(b, 0);
			check( compare(e, hex32(ephid0), 16), "Invalid ephid: %s",bin32(e) );
		}
		it("should have zero derived EphID 8 matching c3db7c504dd6172d...") {
			e = get_beacon(b, 8);
			check( compare(e, hex32(ephid8), 16) );
		}
	}
}
