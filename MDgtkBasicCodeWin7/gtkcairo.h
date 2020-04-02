# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <gtk/gtk.h>
# include <cairo.h>
# include <cairo-pdf.h>
# include "win_gui.h"

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
    int ia;
    int ib;
    double time;
} Event;

typedef struct {
    double x,y,vx,vy;
} Particle; //this is our definition of  a particle, position plus speed

# define SCR 0
# define PRINT 1
// below is the structure containing all the simulation elements
typedef struct
{
  Particle* prt;       // the array of particles
  int n_prt;            // the numner of particle
  Event* evt;            // the colision event array 
  int n_evt;             // The number of events
  int m_evt;             // The number of events  allocated
  double p_diam;         //particle size
  int Pix;               //Number of pixels for window
  double Lmax, Lmin;     //Physical dimensions of box
  guint step;            // The # of steps done  
  int run;               // define if simulation is active (0->stop, 1->One step, 2->continuous)
  int NextEv;
  double FPS;
  double dT;
  char *filetosavedata;
  FILE *fp;
  float s_bg_rgb[3];   // screen background color
  float s_fg_rgb[3];   // screen particle color
  float p_bg_rgb[3];   // print background color
  float p_fg_rgb[3];   // print particle color
  int color_mode;
  // all variable below are related to the GTK interface
  GtkWidget *window;     // This is the general window
  GtkWidget *vbox;       // this is a vertical box containing the menubar and statusbar on top of darea
  GtkWidget *hbox_menu_status;  // hbox containing menu and status bar
  GtkWidget *statusbar;  // the status bar
  GtkWidget *darea_scrolled_window;   // this is the scrollable region to display darea
  GtkWidget *darea;      // This is the Cairo graphic region
  GtkWidget *fileMenu;   // this is the Filemenu
  GtkWidget *simuMenu;   // this is the Filemenu
  GtkWidget *menubar;    // this is the menu bar
  GtkWidget *fileMi;     // this is the button for file 
  GtkWidget *quitMi;     // this is the quit button
  GtkWidget *menu;       // this is a menu
  GtkWidget *st_sim;     // stop simulation radiobutton
  GtkWidget *one_sim;    // one step simulation radiobutton
  GtkWidget *cont_sim;   // continuous simulation radiobutton  
  guint movie_id;        // this is the animation number ID for the simulation
  guint simu_id;
} Simu;


void initparticles(Simu *simu);
Simu* init_simulation(int Np, int Ne, int Pix, double Lmin , double Lmax, double diameter);
gboolean simu_idle(gpointer user_data);
GtkWidget* build_simulation_menu(Simu* simu);
Simu* init_simulation(int Np, int Ne, int Pix, double Lmin , double Lmax, double diameter);
int send_to_status(Simu* sim, const char* context, const char* format, ...);
void action_one(GtkWidget *widget, gpointer user_data);
void action_stop(GtkWidget *widget, gpointer user_data);
void action_cont(GtkWidget *widget, gpointer user_data);
void do_chg_simu(GtkWidget *widget, gpointer user_data);
void do_chg_part_nb(GtkWidget *widget, gpointer user_data);

