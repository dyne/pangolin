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
#include <test/positives_20k.h>

spec("DP-3T") {
	before() {
		printf("version: %s\n",VERSION);
		printf("platform: %s\n", PLATFORM);
	}
	after() {
		// 
	}
	describe("lowcost") {
		static uint8_t sk[32];
		static const beacons_t *b;
		static uint8_t *e;
		static uint32_t epd = (24*60)/15;

		// should be const, reused for tests
		static beacons_t ephids;
		static positives_t positives;

		before() {
			gcinit(&head);
			memset(sk,0x0,32);
		}
		after_each() {
			//
		}
		after() {
			free_beacons((beacons_t*)b);
			gcfree(&head); // free all strings
		}

		it("should use 'Broadcast key'") check( compare(hex32(BK), "Broadcast key", BKLEN) );
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
			b = alloc_beacons(sk, hex32(BK), BKLEN, epd);
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
		it("should match first positive SK generated EphID") {
			static const beacons_t *ske;
			ske = alloc_beacons( //hex32(ZERO32),
				hex32("984544bc997859dc250b664da0bc7ff55d7d1aa2d5bb7e8c4d0f0d17312437e9"),
				// SK number 9073 in the positives.h vector list
				hex32(BK), BKLEN, epd);
		}
		it("should match EphIDs with key 984544bc...") {
			ephids.ttl = 14;
			ephids.data = hex32("c1e0654b89518e4a321f7b33ba408987"
			                    "60063595836a97b3e2e69fa9f51dc45c"
			                    "6a9668c9246b6ee8b80140a32bf846e3"
			                    "f9ee1f679bdb8b6607fb3102df2af513"
			                    "11fec0ff20ed86e3b27fe78166dbc165"
			                    "dabee67c21c78d3804cff34805585b74"
			                    "5ba2607b92b20a826725d182150e5148"
			                    "4e6729728d82fd7b5c1b118c9d751851"
			                    "45d14c4efb1ff88a2e563fda605397e4"
			                    "43e9dc955644bc20daf8316ec26a6aac" // matches from 9845...
			                    "6434f413781ffe243e055e1265a5af5b"
			                    "a51b3af34af8052b9f80239eda5dcd67"
			                    "680c04683805b91c8b49b7f246897193"
			                    "fc1222872b066ee0d62a166badf0a591");
			positives.count = 4;
			positives.data
				= hex32("984544bc997859dc250b664da0bc7ff55d7d1aa2d5bb7e8c4d0f0d17312437e9"
				        "be72bb64705835411286c011c6c6da7cab6b78e4713dfe12538e3c685e8a7ea9"
				        "1d28890e653790c0d337affec887375718dda447e135cc7e5ef32aa19e045a52"
				        "213bb73bd4936375d02c4b72eb2b5ed6ac57318d8f7da7359a75d403ac1d8398");
			struct dictionary *dic = match_positives((const positives_t*)&positives,
			                                         (const beacons_t*)&ephids);

			dic_find(dic, "984544bc997859dc250b664da0bc7ff55d7d1aa2d5bb7e8c4d0f0d17312437e9", 32);
			check( *dic->value == 5 )
		}
		it("should compute a match over 20k keys with epoch 15 minutes (96 moments)") {
			positives.count = 20000;
			positives.data = hex32(positive_sks); // vectors from positives.h
			struct timespec *watch = stopwatch_go();
			struct dictionary *dic = match_positives(&positives, &ephids);
			double exectime = stopwatch_lap(watch);
			free(watch);
			dic_find(dic, "984544bc997859dc250b664da0bc7ff55d7d1aa2d5bb7e8c4d0f0d17312437e9", 32);
			check( *dic->value == 5 );
		}
	}
}
