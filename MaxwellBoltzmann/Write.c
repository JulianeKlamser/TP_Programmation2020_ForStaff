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
    double vx = R * cos(Phi); //gaussian distributed random number vx
    double vy = R * sin(Phi); //gaussian distributed random number vy -> vx and vy are INDEPENDENT!
    return sqrt( vx*vx + vy*vy ); //random number following the Maxwell-Boltzmann distribution
}

double MaxWellBotlzmannFast(double StandDev){
    // StandDev = sqrt( k_B T / mass )
    double Y = - log ( drand48() );
    return StandDev * sqrt( 2. * Y ); //random number following the Maxwell-Boltzmann distribution
}

int main(){
  srand48(time(NULL)); //seed random number generator

  // We want to test MaxWellBotlzmann(double StandDev) and MaxWellBotlzmannFast(double StandDev)
  // So we sample random numbers, store them in a histogram
  // normalize the histogram and then we compare with the teory!
    
  // We choose our parameters
  double kT = 4., mass = 1.0, StandDev = sqrt(kT / mass);
    
  // initialize Histograms and paramters of the Histogram
  double Histogram1[Bins], Histogram2[Bins];//Bins is defined in line 7
  for (int i = 0; i < Bins; ++i) {// initialize Hist with 0
      Histogram1[i] = 0;
      Histogram2[i] = 0;
  }
  double MaxVelocityCutOff= 5*StandDev; // an estimate for the maximum velocity
  double BinWidth = MaxVelocityCutOff/Bins;

  // Fill the histogram
  double xValue, yValue;
  int BinIndex;
  for (int l=0; l<50000;l++){//Fill the histogram by sampling a lot of random numbers
    xValue =  MaxWellBotlzmann( StandDev ); // sample random number
    BinIndex = xValue/BinWidth; // find the corresponding bin (discretize range of arguments)
    if (BinIndex < Bins){ Histogram1[ BinIndex ] += 1;}
      
    xValue =  MaxWellBotlzmannFast( StandDev ); // sample random number
    BinIndex = xValue/BinWidth; // find the corresponding bin (discretize range of arguments)
    if (BinIndex < Bins){ Histogram2[ BinIndex ] += 1;}
  }

  // Normalize the histogram, such that sum_{i = 0}^{Bins-1} Histogram[ i ] * BinWidth = 1.0
  // In the end, a probability is always normalized ! ;)
  double Sum1 = 0.0, Sum2 = 0.0;
  for (int i = 0; i < Bins; ++i) {
      Sum1 += Histogram1[i];
      Sum2 += Histogram2[i];
  }
  Sum1 *= BinWidth;
  Sum2 *= BinWidth;
  for (int i = 0; i < Bins; ++i) {
      Histogram1[i] /= Sum1;
      Histogram2[i] /= Sum2;
      
  }
  // end of normalization. now the "integral" over Histogram is 1, i.e. probability is normalized
    
  // write the Histogram to a data file in a two column format
  // x y\newline
  FILE *fp;
  // First Histogram
  fp = fopen("./MaxwellBoltzmannHistogram1.dat", "w"); //open file
  for (int i = 0; i < Bins; ++i) {
      xValue = (0.5+i)*BinWidth;
      yValue = Histogram1[i];
      fprintf(fp, "%f %f\n", xValue, yValue);//writing data into file.
  }
  fclose(fp);//close File
    
  // Second Histogram
  fp = fopen("./MaxwellBoltzmannHistogram2.dat", "w"); //open file
  for (int i = 0; i < Bins; ++i) {
    xValue = (0.5+i)*BinWidth;
    yValue = Histogram2[i];
    fprintf(fp, "%f %f\n", xValue, yValue);//writing data into file.
  }
  fclose(fp);//close File

  // Write the theory to a file!
  fp = fopen("./MaxwellBoltzmannTheory.dat", "w"); //open file
  for (int i = 0; i < Bins; ++i) {
      xValue = (0.5+i)*BinWidth;
      yValue = xValue/ pow( StandDev, 2) * exp(  -pow( xValue, 2) / (2.* pow( StandDev, 2) ) );
      fprintf(fp, "%f %f\n", xValue, yValue);//writing data into file.
  }
  fclose(fp);//close File
    
  printf("\n\nI produced three files :\n1) MaxwellBoltzmannHistogram1.dat\n2) MaxwellBoltzmannHistogram2.dat\n3) MaxwellBoltzmannTheory.dat\n\nYou can plot the data with the command\n\ngnuplot GnuplotScript.plt\n\n");
  
  return 0;
}




