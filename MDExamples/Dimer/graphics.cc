//https:www.cairographics.org/manual/cairo-Paths.html fix
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include "Graphics.h"

static const double dashed1[] = {4.0, 21.0, 2.0};

void
Graphics::draw(Particle *p, double FPS,int l){
  assert(FPS > 1);
  //  const int FPS=50;//frames per second
  struct timespec tm={0,(long int)(1000000000/FPS)}; //Sleep in nanoseconds between frames
  XEvent event;//check if window closed and finish
  if( XCheckWindowEvent(dsp,da, DestroyNotify , &event)){ XCloseDisplay(dsp); exit(1);}

  double gamma = DimY/(lmax_y-lmin_y + diam);//scaling between physical units and pixels
  double alpha= gamma*diam/2.;//DO NOT Touch ME!
  char a[20];
      
  cairo_push_group(cr); //start drawing
  cairo_set_source_rgb(cr, 0.0, 0.19, .19);//dark green background
  cairo_paint (cr); //clear screen with green
  cairo_set_line_width(cr, 4.);
  if(1){//solid particles
    cairo_set_dash(cr, dashed1, 1, 0);
    cairo_set_source_rgb(cr, 0.79, 0.29, 0.29);//dark red for particles
    int i=0;
    for(i=0;i<NDimer;i += 2){// place the particles in the graphics buffer, without drawing
      cairo_new_sub_path(cr) ;
      cairo_arc(cr,   (alpha + gamma* (p[i].x -lmin_x)) ,  DimY-(alpha + gamma*(p[i].y - lmin_y)), gamma*p[i].OuterRadius, 0, 2 * M_PI);
      cairo_new_sub_path(cr) ;
      cairo_arc(cr,   (alpha + gamma* (p[i+1].x -lmin_x)) ,  DimY-(alpha + gamma*(p[i+1].y - lmin_y)), gamma*p[i+1].OuterRadius, 0, 2 * M_PI);
    }
    cairo_stroke (cr);
    //cairo_fill(cr);//draw all particles with solid color
    cairo_set_dash(cr, NULL, 0, 0);
    for(i=0;i<NDimer;i += 2){// place the particles in the graphics buffer, without drawing
      cairo_new_sub_path(cr) ;
      cairo_arc(cr,   (alpha + gamma* (p[i].x -lmin_x)) ,  DimY-(alpha + gamma*(p[i].y - lmin_y)), gamma*p[i].Rad, 0, 2 * M_PI);
      cairo_new_sub_path(cr) ;
      cairo_arc(cr,   (alpha + gamma* (p[i+1].x -lmin_x)) ,  DimY-(alpha + gamma*(p[i+1].y - lmin_y)), gamma*p[i+1].Rad, 0, 2 * M_PI);
    }
    cairo_fill(cr);
    //cairo_fill(cr);//draw all particles with solid color
    
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    for(i=0;i<NDimer; i += 2){
      
      cairo_move_to ( cr, (alpha + gamma* (p[i].x -lmin_x)) , DimY-(alpha + gamma*(p[i].y - lmin_y)));
      cairo_line_to ( cr, (alpha + gamma* (p[i+1].x -lmin_x)) , DimY-(alpha + gamma*(p[i+1].y - lmin_y)));
      cairo_stroke (cr);
      
    }
    
      
    cairo_set_source_rgb(cr, 0.79, 0.49, 0.29);
    for( ;i<Np;i++){// place the particles in the graphics buffer, without drawing
        cairo_new_sub_path(cr) ;
        cairo_arc(cr,   (alpha + gamma* (p[i].x -lmin_x)) ,  DimY-(alpha + gamma*(p[i].y - lmin_y)), gamma*p[i].Rad, 0, 2 * M_PI);
    }
    cairo_fill(cr);//draw all particles with solid color
  }
  

  if(1){//label particles with numbers
    cairo_set_source_rgb(cr, 1, 1, 1);//white
    for(int i=0;i<Np;i++){// number particles
  
      cairo_move_to (cr, (alpha + gamma* (p[i].x -lmin_x)) ,  DimY-(alpha + gamma*(p[i].y - lmin_y)));
      sprintf(a, "%d",i);
      cairo_set_font_size(cr,15);

      cairo_show_text (cr, a);
    }
  }
  cairo_move_to(cr, DimX-120, DimY-35);//print frame number to graphics window
  sprintf(a, "%d",l);
  cairo_set_font_size(cr,20);
  cairo_show_text (cr, a);

  if(0){//RING for outter circle
    cairo_set_source_rgb(cr, .2, 0.79, 0.79);//dark blue for particles
    for(int i=0;i<Np;i++){
      cairo_new_sub_path(cr) ;
      cairo_arc(cr,  (alpha + gamma* (p[i].x -lmin_x)) ,  DimY - (alpha + gamma*(p[i].y - lmin_y)), 1.4*gamma*p[i].Rad, 0, 2 * M_PI);
    }
    cairo_stroke (cr); // hollow particles
  }
  frame(alpha, (alpha + gamma* (lmax_x -lmin_x)), alpha, DimY-alpha);//draw square border
  cairo_pop_group_to_source(cr); //finished drawing operations for this set of positions
  cairo_paint(cr);//send to screen
  cairo_surface_flush(sfc); //send to x11
  nanosleep( &tm , NULL); //this sets the animations speed
  XFlush(dsp);//sync X11 to cairo


  //https://cairographics.org/Xlib/

  
}

void Graphics::frame(double xmin, double xmax, double ymin, double ymax){
  cairo_set_source_rgb (cr, 1, 1, 0);//yellow border
  cairo_rectangle (cr,xmin, ymin, xmax-xmin, ymax-ymin);
  cairo_stroke (cr);
}
