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

void initparticles( Particle *p, int np, int MaxDimerIndex, double Radius, double ParticleMass, double Lmax_x, double Lmax_y, double Lmin_x, double Lmin_y){
  int i;
  double Dist;
  int Pold, Pnew = 0, testOverlap = 0, infiniteLoopPrevention = 500*np, infiniteLoopTest = 0;
  double Dx, Dy, distance, RandomAngle;

  while ( Pnew < MaxDimerIndex && infiniteLoopTest < infiniteLoopPrevention){
    infiniteLoopTest++;
    int Pnew2 = Pnew+1;
    p[Pnew].Mass = ParticleMass*10;
    p[Pnew].Rad = Radius*3;
    p[Pnew].OuterRadius = p[Pnew].Rad*1.3;
    p[Pnew2].Mass = ParticleMass*10;
    p[Pnew2].Rad = Radius*3;
    p[Pnew2].OuterRadius = p[Pnew].Rad*1.3;
    //++++ velocities
    p[Pnew].vx = (2*drand48()-1);
    p[Pnew].vy = (2*drand48()-1);
    p[Pnew2].vx = (2*drand48()-1);
    p[Pnew2].vy = (2*drand48()-1);
    //++++ Positions
    p[Pnew].x = Lmin_x + p[Pnew].Rad + (Lmax_x-Lmin_x-2*p[Pnew].Rad)*drand48();
    p[Pnew].y = Lmin_y + p[Pnew].Rad + (Lmax_y-Lmin_y-2*p[Pnew].Rad)*drand48();
    RandomAngle = M_PI*drand48();
    p[Pnew2].x = p[Pnew].x + (p[Pnew].Rad+p[Pnew2].Rad+0.002)*cos(RandomAngle);
    p[Pnew2].y = p[Pnew].y + (p[Pnew].Rad+p[Pnew2].Rad+0.002)*sin(RandomAngle);
    testOverlap = 0;
    if ( p[Pnew2].x > ( Lmax_x - p[Pnew2].Rad ) || p[Pnew2].x < ( Lmin_x + p[Pnew2].Rad ) || p[Pnew2].y > ( Lmax_y - p[Pnew2].Rad ) || p[Pnew2].y < ( Lmin_y + p[Pnew2].Rad )) {
        testOverlap = 1;
    }
    //++++
    Pold = 0;
    while (testOverlap == 0 && Pold < Pnew) {
        Dx = getDifference( p[Pnew].x, p[Pold].x);
        Dy = getDifference( p[Pnew].y, p[Pold].y);
        distance = sqrt( Dx*Dx + Dy*Dy );
        if (distance < p[Pnew].Rad + p[Pold].Rad ) {
            testOverlap = 1;
        }
        Dx = getDifference( p[Pnew+1].x, p[Pold].x);
        Dy = getDifference( p[Pnew+1].y, p[Pold].y);
        distance = sqrt( Dx*Dx + Dy*Dy );
        if (distance < p[Pnew+1].Rad + p[Pold].Rad ) {
            testOverlap = 1;
        }
        Pold++;
    }
    if (testOverlap == 0) {
        Pnew+=2;
        infiniteLoopTest = 0;
    }
  }

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

double ExternalCollisionTime( Particle P1, Particle P2){
    double dx, dy, dvx, dvy, R1PlusR2, AA, BB, CC, delta;
    dx = getDifference( P1.x, P2.x);
    dy = getDifference( P1.y, P2.y);
    
    dvx = P1.vx - P2.vx;
    dvy = P1.vy - P2.vy;
    
    BB = 2. * ( dx * dvx + dy * dvy );
    if (BB < 0) {
        R1PlusR2 = P1.Rad + P2.Rad;
        AA = pow( dvx, 2) + pow( dvy, 2);
        CC = pow( dx, 2 ) + pow( dy, 2 ) - pow( R1PlusR2, 2);
        delta = pow( BB, 2) - 4. * AA * CC;
        if ( delta > 0 && CC > 0) {
             return (1. / (2.*AA) ) * (  -BB - sqrt(delta));
            
        }
        else return -1;
    }
    else return -1;
}

int FindExternalParticleCollisions( Event *e, int EventIndex, Particle *p, int Np, int MaxDimerIndex ){
    double collTime;
    //INTER dimer collisions
    for (int p1 = 0; p1 < MaxDimerIndex; ++p1) {
        int p2;
        if (p1 % 2 == 0) p2 = p1 + 2;
        else p2 = p1 + 1;
        while (p2 < MaxDimerIndex) {
            collTime = ExternalCollisionTime( p[p1], p[p2]);
            if (collTime > 0) {
                e[EventIndex].type = particle;
                e[EventIndex].ia = p1;
                e[EventIndex].ib = p2;
                e[EventIndex].time = collTime;
            }
            else e[EventIndex].time = collTime;
            EventIndex++;++p2;
        }
    }
    // dimer normal particle collisions
    for (int dimer_i = 0; dimer_i < MaxDimerIndex; ++dimer_i) {
        for (int p_i = MaxDimerIndex; p_i < Np; ++p_i) {
            collTime = ExternalCollisionTime( p[dimer_i], p[p_i]);
            if (collTime > 0) {
                e[EventIndex].type = particle;
                e[EventIndex].ia = dimer_i;
                e[EventIndex].ib = p_i;
                e[EventIndex].time = collTime;
            }
            else e[EventIndex].time = collTime;
            EventIndex++;
        }
    }
    // normal particle collisions
    for (int p1 = MaxDimerIndex; p1 < Np-1; ++p1) {
        for (int p2 = p1+1; p2 < Np; ++p2) {
            collTime = ExternalCollisionTime( p[p1], p[p2]);
            if (collTime > 0) {
                e[EventIndex].type = particle;
                e[EventIndex].ia = p1;
                e[EventIndex].ib = p2;
                e[EventIndex].time = collTime;
            }
            else e[EventIndex].time = collTime;
            EventIndex++;
        }
    }
    return EventIndex;
}

double InternalCollisionTime( Particle P1, Particle P2){
    double dx, dy, dvx, dvy, R1PlusR2, AA, BB, CC, delta;
    dx = getDifference( P1.x, P2.x);
    dy = getDifference( P1.y, P2.y);
    
    dvx = P1.vx - P2.vx;
    dvy = P1.vy - P2.vy;
    
    R1PlusR2 = P1.OuterRadius + P2.OuterRadius;
    AA = pow( dvx, 2) + pow( dvy, 2); // > 0
    BB = 2. * ( dx * dvx + dy * dvy );
    CC = pow( dx, 2 ) + pow( dy, 2 ) - pow( R1PlusR2, 2); // CC < 0! and not == 0
    delta = pow( BB, 2) - 4. * AA * CC; //always > 0
    if (CC < 0) {
        return (1. / (2.*AA) ) * (  -BB + sqrt(delta));
    }
    else{
        return -1;
    }
}

int FindInternalParticleCollisions( Event *e, int EventIndex, Particle *p, int MaxDimerIndex ){
    //dimer External collisions
    double Time;
    for (int p1 = 0; p1 < MaxDimerIndex; p1 += 2) {
        int p2 = p1+1;
        Time = InternalCollisionTime( p[p1], p[p2]);
        if (Time > 0) {
            e[EventIndex].type = particle;
            e[EventIndex].ia = p1;
            e[EventIndex].ib = p2;
            e[EventIndex].time = Time;
        }
        else{
            e[EventIndex].time = -1;
        }
        EventIndex++;
        
        Time = ExternalCollisionTime( p[p1], p[p2]);
        if (Time > 0) {
            e[EventIndex].type = particle;
            e[EventIndex].ia = p1;
            e[EventIndex].ib = p2;
            e[EventIndex].time = Time;
        }
        else{
            e[EventIndex].time = -1;
        }
        EventIndex++;
        
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

int getLenEventArray(int Np, int NDimer, int MaxDimerIndex){
    int test = NDimer*2 + 1 + 2*Np;
    for (int p1 = 0; p1 < MaxDimerIndex; ++p1) {
        int p2;
        if (p1 % 2 == 0) p2 = p1 + 2;
        else p2 = p1 + 1;
        while (p2 < MaxDimerIndex) {
            test++;++p2;
        }
    }
    for (int dimer_i = 0; dimer_i < MaxDimerIndex; ++dimer_i) {
        for (int p_i = MaxDimerIndex; p_i < Np; ++p_i) {
            test++;
        }
    }
    for (int p1 = MaxDimerIndex; p1 < Np-1; ++p1) {
        for (int p2 = p1+1; p2 < Np; ++p2) {
            test++;
        }
    }
    return test;
}

int main(){
  double FPS=100., dt = 0.12; //frames per second in animation
  int Np=20, NDimer = 4, MaxDimerIndex = 2*NDimer, NNormalPs = Np - MaxDimerIndex; //Number of particles
  assert( MaxDimerIndex <= Np);
  double diameter=1., Radius = diameter/2;//particle size
  double ParticleMass = 1;
  int Pix=600; //Number of pixels for window
  double Lmax_x=30, Lmax_y = 15, Lmin_x=0, Lmin_y=0; //Physical dimensions of box
  Graphics gw(Np, MaxDimerIndex, Pix, Lmin_x, Lmin_y, Lmax_x, Lmax_y, diameter);// Open a window to plot particles in
  srand48(1);//inititalize random numbers -- to find always the same value // you can replace "1" by time(NULL) 

  Particle *p= (Particle *) malloc( Np *sizeof( Particle)); //an array of particles
  initparticles(p, Np, MaxDimerIndex, Radius, ParticleMass, Lmax_x, Lmax_y, Lmin_x, Lmin_y); //place particles in box
  int EventLen = getLenEventArray( Np, NDimer, MaxDimerIndex);
  Event *e = (Event *) malloc( EventLen * sizeof(Event) ); // 4 Np possible collisions with walls
  int EventIndex = 0;
  e[0].time = dt;
  e[0].type = animation;

  gw.draw(p,FPS,0); //draw initial position and pause one second
  sleep(2);
  for (int l=0; l<10000;l++){//long loop animation...
      EventIndex = 1;
      EventIndex = FindHorizontalWallCollitions( e,  EventIndex, p, Np, Lmax_y, Lmin_y );
      EventIndex = FindVerticalWallCollitions( e, EventIndex, p, Np, Lmax_x, Lmin_x );
      EventIndex = FindExternalParticleCollisions( e, EventIndex, p, Np, MaxDimerIndex );
      EventIndex = FindInternalParticleCollisions( e, EventIndex, p, MaxDimerIndex );
      //printf("LenEvent = %d\t EventIndex = %d\n", EventLen, EventIndex);
      //exit(0);
      
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




