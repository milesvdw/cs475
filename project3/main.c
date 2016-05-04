#include <stdio.h>
#include <omp.h>
#include <math.h>
struct s
{
	float value;
	int pad[NUM];
} Array[4];


int main() {


	unsigned int someBigNumber = 1000000000;	// if > 4B, use "long unsigned int"

	for(int NUMT = 1; NUMT <= 4; NUMT = NUMT * 2) {
		double start_time = omp_get_wtime();
		omp_set_num_threads( NUMT );

		#pragma omp parallel for
		for( int i = 0; i < 4; i++ )
		{
			unsigned int sum = 0;
			for( unsigned int j = 0; j < someBigNumber; j++ )
			{
				sum = sum + 2.;
			}
			Array[i].value = sum;
		}
		double end_time = omp_get_wtime();
		double spent_time = end_time - start_time;
		double performance = someBigNumber * 4. / spent_time ;
		printf("%f,", performance);
	}
	printf("\n");
}
