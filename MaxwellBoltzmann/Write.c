#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#define Bins 100

double MaxWellBotlzmann(double StandDev){
    // StandDev = sqrt( k_B T / mass )
    double Phi = drand48()*2.*M_PI;
    double Y = - log ( drand48() );
    double R = StandDev * sqrt( 2. * Y );
    double x = R * cos(Phi);
    double y = R * sin(Phi);
    return sqrt( x*x + y*y );
}

int main(){
  srand48(time(NULL)); //seed random number generator

  // We want to test MaxWellBotlzmann(double StandDev)
  // So we sample random numbers, store them in a histogram
  // normalize the histogram and then we compare with the teory!
    
  // initialize Histogram and paramters of the Histogram
  double Histogram[Bins];//Bins is defined in line 7
  for (int i = 0; i < Bins; ++i) { Histogram[i] = 0; }// initialize Hist with 0
  double CutOff= 10; // cut_off in x direction
  double BinWidth = CutOff/Bins;

  // Fill the histogram
  double xValue, yValue;
  double kT = 4., mass = 1.0, StandDev = sqrt(kT / mass);
  int BinIndex;
  for (int l=0; l<50000;l++){//Fill the histogram
    xValue =  MaxWellBotlzmann( StandDev ); // sample random number
    BinIndex = xValue/BinWidth; // find the corresponding bin (discretize range of arguments)
    if (BinIndex < Bins){ Histogram[ BinIndex ] += 1;}
  }

  //Normalize the histogram, such that sum_{i = 0}^{Bins-1} Histogram[ i ] * BinWidth = 1.0
  double Sum = 0.0;
  for (int i = 0; i < Bins; ++i) { Sum += Histogram[i]; }
  Sum *= BinWidth;
  for (int i = 0; i < Bins; ++i) { Histogram[i] /= Sum; }
  //end of normalization. now the "integral" over Histogram is 1, i.e. probability is normalized
    
  // write the Histogram to a data file in a two column format
  // x y\newline
  FILE *fp;
  fp = fopen("./MaxwellBoltzmannHistogram.dat", "w"); //open file
  for (int i = 0; i < Bins; ++i) {
      xValue = i*BinWidth;
      yValue = Histogram[i];
      fprintf(fp, "%f %f\n", xValue, yValue);//writing data into file.
  }
  fclose(fp);//close File

  // Write the theory to a file!
  fp = fopen("./MaxwellBoltzmannTheory.dat", "w"); //open file
  for (int i = 0; i < Bins; ++i) {
      xValue = i*BinWidth;
      yValue = xValue/ pow( StandDev, 2) * exp(  -pow( xValue, 2) / (2.* pow( StandDev, 2) ) );
      fprintf(fp, "%f %f\n", xValue, yValue);//writing data into file.
  }
  fclose(fp);//close File
    
  printf("\n\nI produced two files :\n1) MaxwellBoltzmannHistogram.dat\n2) MaxwellBoltzmannTheory.dat\n\nYou can plot the data with the command\n\ngnuplot GnuplotScript.plt\n\n");
  
  return 0;
}




