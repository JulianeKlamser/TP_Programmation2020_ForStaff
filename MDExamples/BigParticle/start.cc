#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>

#include "Graphics.h"  // Contains the definition of "Particule" and "Graphics"

double getDifference(double a, double b){
    return a - b;
}

enum  col_type {
  bottom,
  right,
  top,
  left,
  animation,
  particle
}; //different types of collision

typedef struct { // a structure describing each collision -- one might want an array of Events
  enum col_type type;
  int ia, ib;
  double time;
} Event;

void initparticles( Particle *p, int np, double Radius, double ParticleMass, double Lmax_x, double Lmax_y, double Lmin_x, double Lmin_y){
  int i;
  double Dist;
    
  p[0].Mass = 5.;
  p[0].Rad = 1.;
  p[0].x = Lmin_x +2*p[0].Rad + (Lmax_x-Lmin_x-2*p[0].Rad)*drand48(); //random positions for intial condition
  p[0].y = Lmin_y +2*p[0].Rad + (Lmax_y-Lmin_y-2*p[0].Rad)*drand48();
  p[0].vx = 2*drand48()-1;// choose random speeds too using drand48();
  p[0].vy = 2*drand48()-1;
    
  int Pold, Pnew = 1, testOverlap = 0, infiniteLoopPrevention = 500*np, infiniteLoopTest = 0;
  double Dx, Dy, distance;
  while ( Pnew < np && infiniteLoopTest < infiniteLoopPrevention){
    infiniteLoopTest++;
    p[Pnew].Mass = ParticleMass;
    p[Pnew].Rad = Radius;
          
    p[Pnew].vx = 2*drand48()-1;
    p[Pnew].vy = 2*drand48()-1;
          
    p[Pnew].x = Lmin_x + p[Pnew].Rad + (Lmax_x-Lmin_x-2*p[Pnew].Rad)*drand48();
    p[Pnew].y = Lmin_y + p[Pnew].Rad + (Lmax_y-Lmin_y-2*p[Pnew].Rad)*drand48();
      
    Pold = 0;
    testOverlap = 0;
    while (testOverlap == 0 && Pold < Pnew) {
        Dx = getDifference( p[Pnew].x, p[Pold].x);
        Dy = getDifference( p[Pnew].y, p[Pold].y);
        distance = sqrt( Dx*Dx + Dy*Dy );
        if (distance < p[Pnew].Rad + p[Pold].Rad ) {
            testOverlap = 1;
        }
        Pold++;
    }
    if (testOverlap == 0) {
        ++Pnew;
        infiniteLoopTest = 0;
    }
  }
  if(infiniteLoopTest>=infiniteLoopPrevention){
    printf("La densité est trop importante. Il n'est pas possible de trouver une place pour toutes les particules\n");
    free(p);
    exit(0);
  }
}

int FindHorizontalWallCollitions( Event *e, int EventIndex, Particle *p, int Np, double Lmax_y, double Lmin_y ){
    for (int pi = 0; pi < Np; ++pi) {
        if ( p[pi].vy > 0) {//collision with top
            e[EventIndex].type = top;
            e[EventIndex].time = (Lmax_y - p[pi].Rad - p[pi].y) / p[pi].vy;
            e[EventIndex++].ia = pi;
        }
        else{ //collision with bottom
            e[EventIndex].type = bottom;
            e[EventIndex].time = (Lmin_y + p[pi].Rad - p[pi].y) / p[pi].vy;
            e[EventIndex++].ia = pi;
        }
    }
    return EventIndex;
}

int FindVerticalWallCollitions( Event *e, int EventIndex, Particle *p, int Np, double Lmax_x, double Lmin_x ){
    for (int pi = 0; pi < Np; ++pi) {
        if ( p[pi].vx > 0) {//collision with right
            e[EventIndex].type = right;
            e[EventIndex].time = (Lmax_x - p[pi].Rad - p[pi].x) / p[pi].vx;
            e[EventIndex++].ia = pi;
        }
        else{ //collision with left
            e[EventIndex].type = left;
            e[EventIndex].time = (Lmin_x + p[pi].Rad - p[pi].x) / p[pi].vx;
            e[EventIndex++].ia = pi;
        }
    }
    return EventIndex;
}

int FindExternalParticleCollisions( Event *e, int EventIndex, Particle *p, int Np ){
    double dx, dy, dvx, dvy, R1PlusR2, AA, BB, CC, delta;
    for (int p1 = 0; p1 < Np-1; ++p1) {
        for (int p2 = p1+1; p2 < Np; ++p2) {
            dx = getDifference( p[p1].x, p[p2].x);
            dy = getDifference( p[p1].y, p[p2].y);
            
            dvx = p[p1].vx - p[p2].vx;
            dvy = p[p1].vy - p[p2].vy;
            
            BB = 2. * ( dx * dvx + dy * dvy );
            if (BB < 0) {
                R1PlusR2 = p[p1].Rad + p[p2].Rad;
                AA = pow( dvx, 2) + pow( dvy, 2);
                CC = pow( dx, 2 ) + pow( dy, 2 ) - pow( R1PlusR2, 2);
                delta = pow( BB, 2) - 4. * AA * CC;
                if ( delta > 0 && CC > 0) {
                    e[EventIndex].type = particle;
                    e[EventIndex].ia = p1;
                    e[EventIndex].ib = p2;
                    e[EventIndex].time = (1. / (2.*AA) ) * (  -BB - sqrt(delta));
                    
                }
                else e[EventIndex].time = -1;
            }
            else e[EventIndex].time = -1;
            EventIndex++;
        }
    }
    return EventIndex;
}

int FindNextEvent(Event *e, int EventLen){
    int nextEvent = 0; //is animation
    for (int ei = 1; ei < EventLen; ++ei) {
        if ( e[ei].time > 0 && e[ei].time < e[nextEvent].time) nextEvent = ei;
    }
    return nextEvent;
}

void MoveParticles( Particle *p, int Np, double ETime){
    for (int pi = 0; pi < Np; ++pi) {
        p[pi].x += p[pi].vx * ETime;
        p[pi].y += p[pi].vy * ETime;
    }
}

void ChangeVelocity(  Event NextEvent, Particle *p){
    if ( NextEvent.type == top || NextEvent.type == bottom ) {
        p[ NextEvent.ia ].vy *= -1.;
    }
    else if ( NextEvent.type == right || NextEvent.type == left ) {
        p[ NextEvent.ia ].vx *= -1.;
    }
    else if ( NextEvent.type == particle ) {
        int p1 = NextEvent.ia, p2 = NextEvent.ib;
        double dx, dy, dvx, dvy, Norm, scalarProduct, summMass = 0.5*(p[p2].Mass + p[p1].Mass);
        
        dx = getDifference( p[p2].x, p[p1].x);
        dy = getDifference( p[p2].y, p[p1].y);
        Norm = sqrt( pow( dx, 2) + pow( dy, 2) );
        dx /= Norm;
        dy /= Norm;
        
        dvx = p[p1].vx - p[p2].vx;
        dvy = p[p1].vy - p[p2].vy;
        
        scalarProduct = dx * dvx + dy * dvy;
        p[p1].vx -= p[p2].Mass / summMass * scalarProduct * dx;
        p[p1].vy -= p[p2].Mass / summMass * scalarProduct * dy;
        p[p2].vx += p[p1].Mass / summMass * scalarProduct * dx;
        p[p2].vy += p[p1].Mass / summMass * scalarProduct * dy;
        
    }
    
}

int main(){
  double FPS=100., dt = 0.1; //frames per second in animation
  int Np=100; //Number of particles
  double diameter=1, Radius = diameter/2;//particle size
  double ParticleMass = 1;
  int Pix=600; //Number of pixels for window
  double Lmax_x=20, Lmax_y = 10, Lmin_x=0, Lmin_y=0; //Physical dimensions of box
  Graphics gw(Np, Pix, Lmin_x, Lmin_y, Lmax_x, Lmax_y, diameter);// Open a window to plot particles in
  srand48(1);//inititalize random numbers -- to find always the same value // you can replace "1" by time(NULL) 

  Particle *p= (Particle *) malloc( Np *sizeof( Particle)); //an array of particles
  initparticles(p, Np, Radius, ParticleMass, Lmax_x, Lmax_y, Lmin_x, Lmin_y); //place particles in box
  int EventLen = 2*Np + 1 + ( Np*(Np+1)/2 ) ;
  Event *e = (Event *) malloc( EventLen * sizeof(Event) ); // 4 Np possible collisions with walls
  int EventIndex = 0;
  e[0].time = dt;
  e[0].type = animation;

  gw.draw(p,FPS,0); //draw initial position and pause one second
  sleep(1);
  for (int l=0; l<10000;l++){//long loop animation...
      EventIndex = 1;
      EventIndex = FindHorizontalWallCollitions( e,  EventIndex, p, Np, Lmax_y, Lmin_y );
      EventIndex = FindVerticalWallCollitions( e, EventIndex, p, Np, Lmax_x, Lmin_x );
      EventIndex = FindExternalParticleCollisions( e, EventIndex, p, Np );
      
      EventIndex = FindNextEvent( e, EventLen);
      MoveParticles( p, Np, e[EventIndex].time);
      
      if ( e[EventIndex].type == animation ) {
          gw.draw(p, FPS,l);
          e[0].time = dt;
      }
      else{
          ChangeVelocity( e[EventIndex], p);
          e[0].time -= e[EventIndex].time;
      }
  }

  free(p);
  free(e);
  
  return 0;
}




