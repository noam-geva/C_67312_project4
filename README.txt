project 4 workplan. today is december 6th, deadline is december 17th.

definations:
	1)hash-function: like in DAST
	needs to be easy to calculate
	minimal collisions
	
	2)hash-map: basicilly same as hash-table in DAST.

	3)load-factor: LF = M/CAP (0 < CAP is the current max capacity of the vector, and 0 <= M is the current number the vector holds)
	  suprimum||infimum LF: when M reaches a certain ratio to CAP then we wish to change 
	  CAP and the vector size so to not waste memory but still be able to aloocate everything needed.
	  
/** CAP is some lg_2 (2^k for some natural k) so 1 <= CAP is always true
		h(x) = x MOD CAP. for computing resons we will use h(x) = x & (CAP-1)
		PAY ATENTION!! - its a single &, which means its bit-wise and.     */
		
/** we will use open hashing on collision. to do so we will implement a
	struct Vector that can hold N number of elements for n-1 collisions    */
	
Vector - implimantation will be done in Vector.c	
/** MUST: it needs to be a generic vector*/

HashMap - mplimantation will be done in HashMap.c	
/** MUST: it needs to be a generic programming*/

Pair interface - pair is God Given. it structure is { Key : Value}. we will use it to hash a value with its key.
/** note that i do not implement or change Pair.c ot Pair.h*/

Hash.h - simple hash function for later testing.

submition - ex4.tar = {vector.c, HashMap.c}
/** NO h files !!!!!!!!!!!!*/
