#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
const float GRAIN_GROWS_PER_MONTH =		8.0;
const float ONE_DEER_EATS_PER_MONTH =		0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;
const float AMP_PRECIP_PER_MONTH =		6.0;
const float RANDOM_PRECIP =			2.0;
const float ONE_DRAGON_EATS_PER_MONTH =         10.0;
const float GRAIN_CARRYING_CAPACITY =           2000.0;
const float AVG_TEMP =				50.0;
const float AMP_TEMP =				20.0;
const float RANDOM_TEMP =			10.0;


int	NowYear = 2016;		// 2016 - 2021
int	NowMonth = 0;		// 0 - 11

float	NowPrecip = 0.;		// inches of rain per month
float	NowTemp = 0.;		// temperature this month
float	NowGrainHeight = 30000.;		// grain height in inches
int	NowDeer = 1000;		// number of deer in the current population
int     NowDragons = 1;


float Ranf( float low, float high )
{
        float r = (float) rand( );               // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

void GrainDeer() {
	while(NowYear < 2022) {
		int newDeer;
		if (NowDeer * ONE_DEER_EATS_PER_MONTH > NowGrainHeight) {
			newDeer = NowGrainHeight / ONE_DEER_EATS_PER_MONTH;
		} else  {
			newDeer = NowDeer * 1.01 + 10;
		}
	
		newDeer = newDeer - NowDragons * ONE_DRAGON_EATS_PER_MONTH;
		
		if(newDeer < 0) newDeer = 0;
		#pragma omp barrier
	
		NowDeer = newDeer;
	
		#pragma omp barrier
		#pragma omp barrier
	}
	return;
}

void Grain() {
	while(NowYear < 2022) {
		float tempFactor = exp(- ((NowTemp - AVG_TEMP)/10.) * (((NowTemp - AVG_TEMP)/10.)));
		float precipFactor =  exp(- ((NowPrecip - AVG_PRECIP_PER_MONTH)/10.) * (((NowPrecip - AVG_PRECIP_PER_MONTH)/10.)));
	
		int newHeight = NowGrainHeight * (tempFactor * precipFactor + 1) + GRAIN_GROWS_PER_MONTH;
		if(newHeight > GRAIN_CARRYING_CAPACITY) {
			newHeight = newHeight * Ranf(.7,.9);
		}
		newHeight -= (float)NowDeer * ONE_DEER_EATS_PER_MONTH;
		if(newHeight < 0) newHeight = 0;
		#pragma omp barrier
		NowGrainHeight = newHeight;
		#pragma omp barrier
		#pragma omp barrier
	}
}

void Watcher() {
	printf("Month,Precipitation,Temperature,Grain Height,Deer Population,Dragon Population\n");
	
	while(NowYear < 2022) {
		float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

		float temp = AVG_TEMP - AMP_TEMP * cos( ang );


		float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	
		float newPrecip = precip + Ranf( -RANDOM_PRECIP, RANDOM_PRECIP );
		
		float newTemp = temp + Ranf( -RANDOM_TEMP, RANDOM_TEMP );
		
		#pragma omp barrier
	
		NowPrecip = newPrecip;
	
		NowTemp = newTemp;

		if( NowPrecip < 0. )
	
			NowPrecip = 0.;
	
		#pragma omp barrier
	
		//print results
		printf("%d,%f,%f,%f,%d,%d\n",NowMonth + (NowYear - 2016)*12,NowPrecip,(5./9.)*(NowTemp-32),NowGrainHeight/100.,NowDeer,NowDragons);
	
		
       		if(NowMonth == 11) {
       		        NowMonth = 0;
       		        NowYear = NowYear + 1;
	       	} else {
       	        	NowMonth = NowMonth + 1;
       		}
		#pragma omp barrier
	}
}

void Dragons() {
	while(NowYear < 2022) {
		int newDragons = NowDragons;
		//dragons only breed once each year, if there's enough to eat
		if((NowMonth == 0) && (NowDeer * ONE_DRAGON_EATS_PER_MONTH > NowDragons) && (NowDeer > 100)) {
			newDragons = NowDragons + Ranf(1.,3.);
		}
		if((NowMonth != 0) && (NowDeer < NowDragons * ONE_DRAGON_EATS_PER_MONTH)) {
			newDragons = NowDeer / ONE_DRAGON_EATS_PER_MONTH;
		}
		if(newDragons < 0) newDragons = 0;
		#pragma omp barrier
		NowDragons = newDragons;
		#pragma omp barrier
		#pragma omp barrier
	}
}

int main() {
	srand(time(NULL));
	NowDragons = 2;
        omp_set_num_threads( 4 );
        #pragma omp parallel sections
        {
                #pragma omp section
                {
                        GrainDeer( );
                }

                #pragma omp section
                {
                        Grain( );
                }
                #pragma omp section
                {
                        Watcher( );
                }

                #pragma omp section
                {
                        Dragons( );
                }
        }       // implied barrier -- all functions must return in order to allow any of them to get past here
}

