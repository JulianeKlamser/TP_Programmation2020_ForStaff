# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <gtk/gtk.h>
# include <cairo.h>
# include <cairo-pdf.h>
# include "win_gui.h"
# include "gtkcairo.h"
# include "file_picker_gui.h"

GtkWidget *lwindow = NULL;

void Graphics_frame(double xmin, double xmax, double ymin, double ymax, cairo_t *cr)
{
    cairo_set_source_rgb (cr, 1, 1, 0);//yellow border
    cairo_rectangle (cr,xmin, ymin, xmax-xmin, ymax-ymin);
    cairo_stroke (cr);
}

void Graphics_draw(Simu* simu, double FPS, int l, cairo_t *cr)
{
  //scaling between physical units and pixels
    double gamma = simu->Pix/(simu->Lmax - simu->Lmin + simu->p_diam);
    double alpha = gamma * simu->p_diam/2;
    char a[10];
    cairo_text_extents_t extents;
  
    cairo_push_group(cr); //start drawing
    if (simu->color_mode == SCR)
      cairo_set_source_rgb(cr, simu->s_bg_rgb[0], simu->s_bg_rgb[1], simu->s_bg_rgb[2]);
    else
      cairo_set_source_rgb(cr, simu->p_bg_rgb[0], simu->p_bg_rgb[1], simu->p_bg_rgb[2]);
    cairo_paint (cr); //clear screen with green
    
    if(1)
      {//solid particles
	if (simu->color_mode == SCR)
	  cairo_set_source_rgb(cr, simu->s_fg_rgb[0], simu->s_fg_rgb[1], simu->s_fg_rgb[2]);
	else
	  cairo_set_source_rgb(cr, simu->p_fg_rgb[0], simu->p_fg_rgb[1], simu->p_fg_rgb[2]);
	
	//cairo_set_source_rgb(cr, 0.79, 0.29, 0.29);//dark red for particles  
	for(int i = 0; i < simu->n_prt; i++)
	  {// place the particles in the graphics buffer, without drawing
	    cairo_new_sub_path(cr) ;
	    cairo_arc(cr, (alpha + gamma * (simu->prt[i].x -simu->Lmin)),
		      simu->Pix-(alpha + gamma*(simu->prt[i].y - simu->Lmin))
		      , alpha, 0, 2 * M_PI);
	  }
	cairo_fill(cr);//draw all particles with solid color
      }
    
    
    if(1)
      {//label particles with numbers
	cairo_set_source_rgb(cr, 1, 1, 1);//white
	for(int i = 0; i < simu->n_prt; i++){
	  // number particles
	  snprintf(a, 10,"%d",i);
	  cairo_set_font_size(cr,15);
	  cairo_text_extents(cr, a, &extents);
	  cairo_move_to (cr, (alpha + gamma* (simu->prt[i].x - simu->Lmin ))-(extents.width/2),
			 simu->Pix-(alpha + gamma*(simu->prt[i].y - simu->Lmin))+extents.height/2);
	  
	  cairo_show_text (cr, a);
	}
      }
    cairo_move_to(cr, simu->Pix-120, simu->Pix-35);//print frame number to graphics window
    sprintf(a, "%d",l);
    cairo_set_font_size(cr,20);
    cairo_show_text (cr, a);
    
    if(0)
      {//RING for outter circle
	cairo_set_source_rgb(cr, .2, 0.79, 0.79);//dark blue for particles
	for(int i = 0;i < simu->n_prt; i++)
	  {
	    cairo_new_sub_path(cr) ;
	    cairo_arc(cr,  (alpha + gamma* (simu->prt[i].x -simu->Lmin)),
		      simu->Pix - (alpha + gamma*(simu->prt[i].y - simu->Lmin)),
		      1.4*alpha, 0, 2 * M_PI);
	  }
	cairo_stroke (cr); // hollow particles
      }
    
    Graphics_frame(alpha, simu->Pix - alpha, alpha, simu->Pix - alpha, cr);//draw square border
    cairo_pop_group_to_source(cr); //finished drawing operations for this set of positions
    cairo_paint(cr);//send to screen
    //https://cairographics.org/Xlib/
}


static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr,  gpointer user_data)
{
    Simu* simu = NULL;

    (void)widget;
    if (user_data == NULL) return FALSE;
    simu = (Simu*)user_data;
    //do_drawing(widget, cr, user_data);
    Graphics_draw(simu, 5, 1, cr);
  
    //Graphics_frame(50, 750, 50, 750, cr);
  return FALSE;
}
void do_PNG(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;
    cairo_surface_t *surface;
    cairo_t *crpng;
    char *fileout = NULL;
   
   
    (void)widget;
    if (user_data == NULL) return;
    simu = (Simu*)user_data;

    surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, simu->Pix, simu->Pix);
    crpng = cairo_create(surface);

    fileout = save_one_file("Save PNG file", ".", "data.png", "PNG files\0*.png\0all\0*.*\0");
    if (fileout == NULL) return;

    Graphics_draw(simu, simu->FPS, 1, crpng);
    cairo_surface_write_to_png(surface, fileout);
    cairo_surface_destroy(surface);
    cairo_destroy(crpng);
}
void do_PDF(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;
    cairo_surface_t *surface;
    cairo_t *crpdf;
    char *fileout = NULL;

    (void)widget;
    if (user_data == NULL) return;
    simu = (Simu*)user_data;

   // stuff to draw pdf image
    fileout = save_one_file("Save PDF file", ".", "pdffile.pdf", "PDF files\0*.pdf\0all\0*.*\0");
    if (fileout == NULL) return;
    simu->color_mode = PRINT;
    surface = cairo_pdf_surface_create(fileout, 8*72, 8*72);
    crpdf = cairo_create(surface);
cairo_scale (crpdf, (double)(8*72)/simu->Pix, (double)(8*72)/simu->Pix);
    Graphics_draw(simu, simu->FPS, 1, crpdf);
    cairo_show_page(crpdf);
    
    cairo_surface_destroy(surface);
    cairo_destroy(crpdf);
    simu->color_mode = SCR;
}
void do_file(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;
    char *fileout = NULL;

    (void)widget;
    if (user_data == NULL) return;
    simu = (Simu*)user_data;

   // stuff to draw pdf image
    fileout = save_one_file("Open file to Save data", ".", "simulation-data.txt", "TXT files\0*.txt\0all\0*.*\0");
    if (fileout == NULL) return;
    if (simu->filetosavedata != NULL)
      free(simu->filetosavedata);
    simu->filetosavedata = fileout;
    simu->fp = fopen(simu->filetosavedata,"w");
    if (simu->fp == NULL)
      {
	win_printf("Could not open file :\n%s\nto save data!\n"
		   ,backslash_to_slash(simu->filetosavedata));
	if (simu->filetosavedata != NULL)
	  free(simu->filetosavedata);
	simu->filetosavedata = NULL;
	return;
      }
    fclose(simu->fp);
}
// This function is called whenever you press the "One step" menu button
// GtK imposes the two parameters: widget and userdata
void action_one_bis(GtkWidget *widget, gpointer user_data)
{
    Simu* simu = NULL;

    (void)widget;
    if (user_data == NULL) return;
    // the first thing that we do is to cast user_data to the Simu* type
    simu = (Simu*)user_data;
    // now we switch the parameter run to 1    
    simu->run = 1;
}


GtkWidget* build_menubar(Simu* simu)
{
    GtkWidget* item;
    simu->menubar = gtk_menu_bar_new();

    simu->fileMenu = gtk_menu_item_new_with_label("File");
    gtk_widget_show(simu->fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menubar), simu->fileMenu);

    simu->menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(simu->fileMenu), simu->menu);
    
    simu->fileMi = gtk_menu_item_new_with_label("Select TXT saving File");
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), simu->fileMi);
    gtk_widget_show(simu->fileMi);
    g_signal_connect(G_OBJECT(simu->fileMi), "activate", G_CALLBACK(do_file), simu);

    simu->fileMi = gtk_menu_item_new_with_label("Convert to PNG");
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), simu->fileMi);
    gtk_widget_show(simu->fileMi);
    g_signal_connect(G_OBJECT(simu->fileMi), "activate", G_CALLBACK(do_PNG), simu);    

    simu->fileMi = gtk_menu_item_new_with_label("convert to PDF");
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), simu->fileMi);
    gtk_widget_show(simu->fileMi);
    g_signal_connect(G_OBJECT(simu->fileMi), "activate", G_CALLBACK(do_PDF), simu);    

    simu->quitMi = gtk_menu_item_new_with_label("Quit");
    gtk_widget_show(simu->fileMi);
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menu), simu->quitMi);

    build_simulation_menu(simu);

    item = gtk_menu_item_new_with_label("One step");
    gtk_widget_show(item);
    gtk_menu_shell_append(GTK_MENU_SHELL(simu->menubar), item);
    g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(action_one_bis), simu);   


    return simu->menubar;
}    

int send_to_status(Simu* sim, const char* context, const char* format, ...)
{
    va_list ap;
    static char *c = NULL;
    guint id;

    if (sim == NULL) return 1;
    if (c == NULL)
      c = (char *)calloc(2048, sizeof(char));

    if (c == NULL)              return 2;

    va_start(ap, format);
    vsnprintf(c, 2048, format, ap);
    va_end(ap);

    id = gtk_statusbar_get_context_id (GTK_STATUSBAR(sim->statusbar),context);
    gtk_statusbar_remove_all(GTK_STATUSBAR(sim->statusbar),id);
    gtk_statusbar_push (GTK_STATUSBAR(sim->statusbar), id, c);
    return 0;
}

int append_data_to_saving_file(Simu* sim, const char* format, ...)
{
    va_list ap;
    static char *c = NULL;

    if (sim == NULL) return 1;
    if (sim->filetosavedata == NULL)
      {
	send_to_status(sim, "simulation", "You need to select a saving file first!");
	return 2;
      }
    sim->fp = fopen(sim->filetosavedata,"a");
    if (sim->fp == NULL)
      {
	send_to_status(sim, "simulation", "cannot open file:\n %s\n"
		       ,backslash_to_slash(sim->filetosavedata));
	return 3;
      }
    
    if (c == NULL)
      c = (char *)calloc(2048, sizeof(char));
    if (c == NULL)              return 3;

    va_start(ap, format);
    vsnprintf(c, 2048, format, ap);
    va_end(ap);
    fprintf(sim->fp, "%s", c);
    fclose(sim->fp);
    return 0;
}

int main (int    argc,	  char **argv)
{
    Simu* simu = NULL;
    int Np = 100, LenEvArr = 4 * Np + Np * ( Np - 1 ) + 1;
    
    srand(time(NULL)); //inititalize random numbers -- to find always the same value // you can replace "1" by time(NULL)
    //srand48(1);


    simu = init_simulation(Np, LenEvArr, 600, 0 , 20, 1);
    initparticles(simu);
    simu->FPS = 500; //frames per second in animation
    simu->dT = 0.125;
    simu->evt[0].time = simu->dT;
    simu->evt[0].type = animation;
    //dark green background
    simu->s_bg_rgb[0] = 0.0; simu->s_bg_rgb[1] = 0.19; simu->s_bg_rgb[2] = 0.19;;
    //dark red for particles  
    simu->s_fg_rgb[0] = 0.79; simu->s_fg_rgb[1] = 0.29; simu->s_fg_rgb[2] = 0.29;
    //print white background
    simu->p_bg_rgb[0] = 1.0; simu->p_bg_rgb[1] = 1.0; simu->p_bg_rgb[2] = 1.0;;
    //print red for particles  
    simu->p_fg_rgb[0] = 0.79; simu->p_fg_rgb[1] = 0.29; simu->p_fg_rgb[2] = 0.29;
    gtk_init(&argc, &argv);
    
    lwindow = simu->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(simu->window), 512, 512);
    
    simu->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,2);
    gtk_container_add(GTK_CONTAINER(simu->window), simu->vbox);

    simu->hbox_menu_status = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,2);
    gtk_box_pack_start(GTK_BOX(simu->vbox), simu->hbox_menu_status, FALSE, FALSE, 0);

    simu->menubar = build_menubar(simu);
    gtk_box_pack_start(GTK_BOX(simu->hbox_menu_status), simu->menubar, FALSE, FALSE, 0);

    simu->statusbar = gtk_statusbar_new();
    gtk_box_pack_end(GTK_BOX(simu->hbox_menu_status), simu->statusbar, TRUE, TRUE, 0);
    

    send_to_status(simu,"genaral","Hello %d",1); 
      
    simu->darea_scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (simu->darea_scrolled_window),
				    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_placement (GTK_SCROLLED_WINDOW (simu->darea_scrolled_window)
                                       ,GTK_CORNER_BOTTOM_LEFT);


    gtk_box_pack_start(GTK_BOX(simu->vbox), simu->darea_scrolled_window, TRUE, TRUE, 1);
    
    
    simu->darea = gtk_drawing_area_new();
    gtk_widget_set_size_request(simu->darea, simu->Pix, simu->Pix);
    gtk_container_add(GTK_CONTAINER(simu->darea_scrolled_window), simu->darea);
    
    
    g_signal_connect(G_OBJECT(simu->darea), "draw", G_CALLBACK(on_draw_event), simu);

    g_signal_connect(G_OBJECT(simu->quitMi), "activate",G_CALLBACK(gtk_main_quit), NULL);


    g_signal_connect(simu->window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_window_set_position(GTK_WINDOW(simu->window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(simu->window), "MD Simulation");
    gtk_widget_show_all(simu->window);


    simu->simu_id = g_timeout_add (1000/simu->FPS, simu_idle,(gpointer)simu);

    gtk_main();
    
    return 0;
}
  
