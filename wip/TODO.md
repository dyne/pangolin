# TODO notes for work in progress

Cuckoo filter implementation using hashdict as base?
in any case the hashes should be Meiyan and xxHash32
https://www.strchr.com/hash_functions


Randomization of the ephids order should be done via C pointers, we
can use Sandra Sattolo's cycle algo:

```c
void sattoloCycle(void** arr,int count){
	int i,j;
	void* temp;

	if(count<2)
		return;
	for(i=count-1;i>=1;i--){
		j = rand()%i;
		temp = arr[j];
		arr[j] = arr[i];
		arr[i] = temp;
	}
}
```
