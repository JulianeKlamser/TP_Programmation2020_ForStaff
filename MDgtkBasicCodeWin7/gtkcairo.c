# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <gtk/gtk.h>
# include <cairo.h>
# include <cairo-pdf.h>
# include "win_gui.h"
# include "gtkcairo.h"


// This function initialize the Simu structure
Simu* init_simulation(int Np,         // This is the number of particle
		      int Ne,         // This is the number of events
		      int Pix,        // This is the size on screen
		      double Lmin,    // This is the minimum real position
		      double Lmax,    // This is the maximum real position
		      double diameter)// This is the particle diameter
{
    Simu* sim = NULL;

    if (Lmin > Lmax || Np <= 0 || Ne <= 0) return NULL;
    sim = (Simu*)calloc(1,sizeof(Simu));
    if (sim == NULL) return NULL;
    sim->prt = (Particle*)calloc(Np,sizeof(Particle));
    if (sim->prt == NULL) return NULL;
    sim->n_prt = Np;
    sim->evt = (Event*)calloc(Ne,sizeof(Event));
    if (sim->evt == NULL) return NULL;
    sim->n_evt = sim->m_evt = Ne;
    sim->p_diam = diameter;
    sim->Pix = Pix;
    sim->Lmin = Lmin;
    sim->Lmax = Lmax;
    sim->step = 0;
    return sim;
}

void initparticles(Simu *simu) 
{
    int i;

    if (simu == NULL) return;// 
    for(i = 0; i < simu->n_prt; i++)
      { // drand48() -- random double numbers uniform on (0 1)
	simu->prt[i].x = simu->Lmin + simu->p_diam/2 + (simu->Lmax - simu->Lmin -simu->p_diam)*rand()/(double)RAND_MAX; //random positions for intial condition
	simu->prt[i].y = simu->Lmin + simu->p_diam/2 + (simu->Lmax - simu->Lmin - simu->p_diam)*rand()/(double)RAND_MAX;
	
	
	simu->prt[i].vx =   (2*(rand()/(double)RAND_MAX) -1);// choose random speeds too using drand48();
	simu->prt[i].vy =  (2*(rand()/(double)RAND_MAX) -1);
      }
	 
}


void TestMoveParticles (Simu *simu, double dt)
{
    if (simu == NULL) return;//
    for(int i = 0; i < simu->n_prt; i++)
    {
        simu->prt[i].x += dt * simu->prt[i].vx;
        simu->prt[i].y += dt * simu->prt[i].vy;
        
    }
}


// This is the function which actually perform the simulation that you should
// modify. This function is called by the GtK main function with a periodicity
// equal to 1000/FPS in ms.
// Since GtK does not know what your data corresponds to it provives a pointer
// to void (gpointer) that allows passing whatever data (user_data)

gboolean simu_idle(gpointer user_data)          // a pointer to the user_data
{
    Simu* simu = NULL;
    static int old_run = -1;

    if (user_data == NULL) return FALSE;
    // the first thing that we do is to cast user_data to the Simu* type
    simu = (Simu*)user_data;

    // do simulation
    if (simu->run == 0)  // simulation is stoped
      {
	if (old_run != 0)
	  {  // this piece of code keeps the menu buttons up todate do not modify
	    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(simu->cont_sim),FALSE);
	    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(simu->one_sim),FALSE);
	    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(simu->st_sim),TRUE);
	    old_run = 0;
	  }
	// we are in stop mode thus after menus have been updated we are finish
	return TRUE;
      }
    else
      {
	//This is where you need to insert your code
	//find future collision using exact calculations of collision times

	TestMoveParticles(simu, simu->dT);

	// gtk_widget_queue_draw is called when ever you need to refresh the screen
	// send_to_status allows you to display stuff in the status bar
	// for instance the step number
	send_to_status(simu,"general","Step %d\n",simu->step++);
	gtk_widget_queue_draw (simu->darea);
	if (simu->run == 1)
	  { // il you are in the one step simulation mode, you have done your step
	    // otherwise in continuous modeyou skip this part 
	    // thus you switch back to stop mode and update menus state
	    simu->run = 0;
	    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(simu->cont_sim),FALSE);
	    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(simu->one_sim),FALSE);
	    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(simu->st_sim),TRUE);
	  }
      }
    

    return TRUE;
}

// This function is called whenever you press the "Stop" menu button
// GtK imposes the two parameters: widget and userdata
void action_stop(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;

    (void)widget;
    if (user_data == NULL) return;
    // the first thing that we do is to cast user_data to the Simu* type        
    simu = (Simu*)user_data;
    // now we switch the parameter run to 0
    simu->run = 0;
}

// This function is called whenever you press the "One step" menu button
// GtK imposes the two parameters: widget and userdata
void action_one(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;

    (void)widget;
    if (user_data == NULL) return;
    // the first thing that we do is to cast user_data to the Simu* type
    simu = (Simu*)user_data;
    // now we switch the parameter run to 1    
    simu->run = 1;
}
// This function is called whenever you press the "Continuous" menu button
// GtK imposes the two parameters: widget and userdata
void action_cont(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;

    (void)widget;
    if (user_data == NULL) return;
    // the first thing that we do is to cast user_data to the Simu* type
    simu = (Simu*)user_data;
    // we update menu
     if (simu->st_sim)
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(simu->st_sim),FALSE);
    // now we switch the parameter run to 2
    simu->run = 2;
    
}

// This function is called whenever you press the "change parameters" menu button
// GtK imposes the two parameters: widget and userdata
void do_chg_simu(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;
    double p_diam;         //particle size
    int Pix, i;            //Number of pixels for window
    double Lmax, Lmin;     //Physical dimensions of box
    double FPS;            // frame per sec
    double dT;             // time interval
  
    (void)widget;
    if (user_data == NULL) return;
    // the first thing that we do is to cast user_data to the Simu* type    
    simu = (Simu*)user_data;

    // we caopy the values of the structure to local values
    p_diam = simu->p_diam;
    Pix = simu->Pix;               //Number of pixels for window
    Lmax = simu->Lmax;
    Lmin = simu->Lmin;     //Physical dimensions of box
    FPS = simu->FPS;
    dT = simu->dT;

    // win_scanf is a function that we provide which allos easily to modify
    // parameters, it works exactly as the scanf function except that it opens
    // a GtK window with a form to fill.
    // Any character is written directly except % which indicates that a value
    // will be asked to the user. Depending on the parameter type that you use
    // you need to specify the format: %d for an integer, %f for a float
    // %lf for a double, ... look at scanf documentation
    // last but not least, you need to specify the address of the variable you
    // want to modify this explain the & sign preceeding each variable
    // if you forget this your program will crash !
    i = win_scanf("Changing simulation parameters\n"
		  "Particle diameter %lf\n"
		  "Size on display %d\n"
		  "Lmin  %lf\n"
		  "Lmax  %lf\n"		  
		  "Frame per sec. %lf (if > 500 full speed)\n"
		  "Time interval dt %lf\n"
		  ,&p_diam,&Pix,&Lmin,&Lmax,&FPS,&dT);
    if (i == WIN_CANCEL) return;
    // If the user did not cancel then we modify the structure
    simu->p_diam = p_diam;
    simu->Pix = Pix;               //Number of pixels for window
    if (FPS != simu->FPS)
      {
	g_source_remove(simu->simu_id);
	simu->FPS = FPS;
	if (simu->FPS > 500)
	    simu->simu_id = g_idle_add_full(G_PRIORITY_LOW, simu_idle, (gpointer)simu, NULL);
	else
	  simu->simu_id = g_timeout_add (1000/simu->FPS, simu_idle,(gpointer)simu);

      }
    simu->Lmax = Lmax;
    simu->Lmin = Lmin;
    simu->dT = dT;
    // we reinitialise particle positions
    initparticles(simu);
    return;
}

// This function is called whenever you press the "change particule number" menu button
// GtK imposes the two parameters: widget and userdata
void do_chg_part_nb(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;
    int n_prt;            // the numner of particle
    int m_evt;             // The number of events
    int i, run_save = 0;
    static int ne_auto = 1;
    
    (void)widget;
    if (user_data == NULL) return;
    // the first thing that we do is to cast user_data to the Simu* type    
    simu = (Simu*)user_data;

    n_prt = simu->n_prt;
    m_evt = simu->m_evt; 
    run_save = simu->run;
    simu->run = 0;  // xe stop the simulation 
    // win_scanf is a function that we provide which allos easily to modify
    // parameters, it works exactly as the scanf function except that it opens
    // a GtK window with a form to fill.
    // Any character is written directly except % which indicates that a value
    // will be asked to the user. Depending on the parameter type that you use
    // you need to specify the format: %d for an integer, %f for a float
    // %lf for a double, ... look at scanf documentation
    // last but not least, you need to specify the address of the variable you
    // want to modify this explain the & sign preceeding each variable
    // if you forget this your program will crash !
    // win_scanf accept a non classical parameter %b (for button) this allos
    // to have an on/off button which value will be 0(off) or 1(on) 
    i = win_scanf("Changing simulation Nb. of particles\n"
		  "Particle number Np=%d\n"
		  "You need to change the number of events\n"
		  "Click here %b to set it to 4 * Np + Np * ( Np - 1 ) + 1\n"
		  "otherwise select your Number of event %d\n"
		  ,&n_prt,&ne_auto,&m_evt);
    if (i == WIN_CANCEL) return;

    // we reallocate the size of the particle array
    simu->prt = (Particle*)realloc(simu->prt,n_prt*sizeof(Particle));
    if (simu->prt == NULL)
      { // reallocation fails, we display a window with an error message
	// win_printf works like printf excepted that it opens a window
	win_printf("Coul not realloc particle array !");
	return;
      }
    simu->n_prt = n_prt;
    // if the buuton has been clicked on we compute the number of events
    // using the specified formula
    if (ne_auto) m_evt = 4 * n_prt + n_prt * ( n_prt - 1 ) + 1;
    // otherwise we use the value specified by the user 
    simu->evt = (Event*)realloc(simu->evt,m_evt*sizeof(Event));
    if (simu->evt == NULL)
      {
	win_printf("Coul not realloc event array !");
	return;
      }
    simu->m_evt = m_evt;
    initparticles(simu);
    simu->run = run_save; // we rstore stat 
    return;
}

// this function build the simulation menu attached to the menubar    
GtkWidget* build_simulation_menu(Simu* simu)
{
    // this is the submenu calling button 
    simu->simuMenu = gtk_menu_item_new_with_label("Simulation");
    gtk_widget_show(simu->simuMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menubar), simu->simuMenu);

    // we create a new submen
    simu->menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(simu->simuMenu), simu->menu);

    GtkWidget *item = NULL;

    // we attach the stop button
    simu->st_sim = item = gtk_check_menu_item_new_with_label("Stop");
    gtk_widget_show(item);
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), item);
    g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(action_stop), simu);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),TRUE);
    gtk_check_menu_item_set_draw_as_radio(GTK_CHECK_MENU_ITEM(item),TRUE);

    // we attach the one step button
    simu->one_sim = item = gtk_check_menu_item_new_with_label("One step");
    gtk_widget_show(item);
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), item);
    g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(action_one), simu);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),FALSE);
    gtk_check_menu_item_set_draw_as_radio(GTK_CHECK_MENU_ITEM(item),TRUE);

    // we attach the continuous button
    simu->cont_sim = item = gtk_check_menu_item_new_with_label("Continuous");
    gtk_widget_show(item);
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), item);
    g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(action_cont), simu);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),FALSE);
    gtk_check_menu_item_set_draw_as_radio(GTK_CHECK_MENU_ITEM(item),TRUE);

    // we draw a line separator
    item = gtk_separator_menu_item_new ();
    gtk_widget_show(item);
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), item);

    // we attach the Change simulation parameters button
    item = gtk_menu_item_new_with_label("Change simulation parameters");
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), item);
    gtk_widget_show(item);
    g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(do_chg_simu), simu);

    // we attach the Change simulation Nb. of particle button
    item = gtk_menu_item_new_with_label("Change simulation Nb. of particle");
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), item);
    gtk_widget_show(item);
    g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(do_chg_part_nb), simu);        
    
    return simu->menubar;
}
