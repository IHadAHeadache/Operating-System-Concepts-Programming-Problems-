#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 100000
int main(int argc, char* argv[])
{
int i;
int pi = 0;
omp_lock_t writelock; omp_init_lock(&writelock);
	#pragma omp parallel for
	for (i = 0; i < N; ++i){
	double width = random() / (double)RAND_MAX;
	double length = random() / (double)RAND_MAX;
		if(width*width + length*length < 1)
		{
			omp_set_lock(&writelock);
			++pi;
			omp_unset_lock(&writelock);
		}
	}
	printf("pi = %f\n",(double)pi/N*4);
	return 0;
}

