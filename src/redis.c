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

#include <redismodule.h>
#include <redis_namespace.h>

#define SAFE(f) if(f == REDISMODULE_ERR) return REDISMODULE_ERR

int fptw_version(CTX *ctx, STR **argv, int argc) {
	(void)argv;
	if (argc != 1) return RedisModule_WrongArity(ctx);
	r_replywithsimplestring(ctx, VERSION);
	return REDISMODULE_OK;
}

// main entrypoint symbol
int RedisModule_OnLoad(CTX *ctx) {
	if (RedisModule_Init(ctx, "fptw", 1, REDISMODULE_APIVER_1) ==
	    REDISMODULE_ERR)
		return REDISMODULE_ERR;

	// if (RedisModule_CreateCommand(ctx, "fptw.version",
	//                               fptw_version, "",0,0,0) == REDISMODULE_ERR)
	// 	return REDISMODULE_ERR;	
	SAFE(r_createcmd(ctx, "fptw.version", fptw_version, "",0,0,0));


	return REDISMODULE_OK;
}
