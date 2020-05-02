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

#include <src/codec.h>

////////////////////////////////////////////////////////////////////
// very simple linked list for garbage collection of dynamic strings
struct node { void *data;	struct node *next; };
static struct node *head;
static void gcinit(struct node **head) {
	*head = malloc(sizeof(struct node));
	(*head)->data = NULL;
	(*head)->next = NULL; }
#define gcalloc(s) gcadd(&head, malloc(s));
static void *gcadd(struct node **head, void *data) {
	struct node *current = *head;
	struct node *tmp = current;
	if(current->next) do {
			tmp = current;
			current = current->next;
		} while (current);
	struct node *new = malloc(sizeof(struct node));
	new->next = NULL;
	new->data = data;
	tmp->next = new;
	return(new->data);
}
static void gcfree(struct node **head) {
	struct node *current = *head;
	struct node *tmp;
	do  { if(current->data) free(current->data);
		tmp = current;
		current = current->next;
		free(tmp);
	} while (current);
}
// end of auxiliary "garbage collection" for dynamic strings
////////////////////////////////////////////////////////////

// binary conversions (passthrough)
static char *hex(const char *src) {
	char *o = (char*)gcalloc(strlen(src));
	hex2bin(o, src);
	return(o);
}
static char *hex32(const char *src) {
	char *o = (char*)gcalloc(32);
	hex2binlen(o, src, 32);
	return(o);
}
static char *bin32(const uint8_t *src) {
	char *o = (char*)gcalloc(64+1);
	bin2hex(o, src, 32);
	return(o);
}
static char *bin16(const uint8_t *src) {
	char *o = (char*)gcalloc(32+1);
	bin2hex(o, src, 16);
	return(o);
}

// print to console
#define H32(p) printf("%s\n", p);
#define B16(p) printf("%s\n", bin16(p))
#define B32(p) printf("%s\n", bin32(p))

// spy functions to print and passthrough
static const uint8_t *spy16(const uint8_t *src) { B16(src); return(src); }
static const uint8_t *spy32(const uint8_t *src) { B32(src); return(src); }

// compare two values
static const int compare(uint8_t *left, uint8_t *right, size_t len) {
	assert(! (left == right)); // avoid comparing buffer with itself
	return(memcmp(left, right, len) == 0);
}


// timing execution
#include <time.h>
static struct timespec *stopwatch_go() {
	struct timespec *ts = malloc(sizeof(struct timespec));
	if( clock_gettime( CLOCK_REALTIME, ts) == -1 ) {
		perror( "clock gettime" );
		exit( EXIT_FAILURE ); }
	return(ts); }
static double stopwatch_lap(struct timespec *ts) {
	struct timespec lap;
	double accum;
	if( clock_gettime( CLOCK_REALTIME, &lap) == -1 ) {
		perror( "clock gettime" );
		exit( EXIT_FAILURE ); }
	return(( lap.tv_sec - ts->tv_sec )
	       + ( lap.tv_nsec - ts->tv_nsec )
	       / 10000000L);
}


