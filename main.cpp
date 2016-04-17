#include "height.cpp"


int main( int argc, char *argv[ ] )
{

	int XMAX = 5;
	int XMIN = 0;
	int YMAX = 5;
	int YMIN = 0;


	int NUMS[] = {10,100,500,1000};
	int size_NUMS_arr = sizeof(NUMS) / sizeof(int);

	printf("REIMANN TERMS\tSECONDS\tTHREADS\n");
	//try each quanitity of subdivisions
	for(int i = 0; i < size_NUMS_arr; i++) {
		for(int t = 1; t <= 4; t = t * 2) {
			float volume = 0;
			double start_time = omp_get_wtime();
	
			
			omp_set_num_threads(t);
			#pragma omp parallel for default(none), reduction(+:volume)
			for(int x = 0; x < NUMS[i]; x ++) {
				for(int y = 0; y < NUMS[i]; y++) {
	
					float fullTileArea = (  ( (XMAX-XMIN)/(float)(NUMS[i]-1) )  *  ( ( YMAX - YMIN )/(float)(NUMS[i]-1) )  );
					
					//calculate the half-sized area for edge cases
					if(x == 0 || x == NUMS[i] - 1) {
						fullTileArea = fullTileArea / 2.;
						if( y == 0 || y == NUMS[i] - 1) {
							fullTileArea = fullTileArea / 2.;
						}
					}	

					volume += fullTileArea * Height(x, y);
				}
			}

			double end_time = omp_get_wtime();
			double calculation_time = end_time - start_time;
			int n_calculations = NUMS[i] * NUMS[i]
	
			printf("%d\t%d\t%d", n_calculations, calculation_time, t);


		}
	}
}