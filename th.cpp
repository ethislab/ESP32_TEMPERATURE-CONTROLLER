
#include <arduino.h>

// LOOKUP Table for 0-100 deg C -5k thermistor
PROGMEM const float C_th5k[] = {-0.00491691,0.186419,0.379876,0.575498,0.773331,0.973425,1.17583,1.38059,1.58777,1.79741,2.00958,2.22433,2.44172,2.66182,2.88469,3.11039,3.339,3.57059,3.80523,4.043,4.28398,4.52825,4.77589,5.02701,5.28169,5.54003,
5.80213,6.06809,6.33802,6.61205,6.89028,7.17284,7.45987,7.75149,8.04785,8.3491,8.65539,8.96688,9.28374,9.60616,9.93431,10.2684,10.6086,10.9552,11.3084,11.6684,12.0355,12.41,12.7921,13.1821,13.5804,13.9873,14.4031,14.8283,15.2632,15.7083,16.164,
16.6308,17.1093,17.5999,18.1034,18.6204,19.1515,19.6975,20.2592,20.8375,21.4333,22.0478,22.6819,23.337,24.0144,24.7155,25.442,26.1957,26.9786,27.7929,28.641,29.5257,30.4501,31.4177,32.4325,33.4991,34.6226,35.809,37.0654,38.4,39.8224,41.3443,42.9796,
44.7454,46.6627,48.758,51.0653,53.6286,56.5072,59.7827,63.5718,68.049,73.4912,80.3714,89.8162};

#define RMIN (16330)
#define RMAX (458.2)
#define NSEGTH 100       // = number of sections in table
#define RSEG 158.72      // = (RMAX-RMIN)/NSEGTH =  RSEG resistence per segment



/***************************************************************/
/**resistance to temperature function 5K thermistor 0-100 deg C**/
float T_th5k (float r) {
  float t;
  int i;
  i=(RMIN-r)/RSEG;       // determine which coefficients to use
  if (i<0)               // if input is under-range..
    i=0;                 // ..then use lowest coefficients
  else if (i>NSEGTH-1)     // if input is over-range..
    i=NSEGTH-1;            // ..then use highest coefficients
  //t = C_th5k[i]+((RMIN-RSEG*i)-r)*(C_th5k[i+1]-C_th5k[i])/RSEG;
  t = pgm_read_float(C_th5k+i)+((RMIN-RSEG*i)-r)*(pgm_read_float(C_th5k+i+1)-pgm_read_float(C_th5k+i))/RSEG;
  return (t);
}
