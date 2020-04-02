# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <gtk/gtk.h>
# include <cairo.h>
# include <cairo-pdf.h>
# include "win_gui.h"
# include "gtkcairo.h"

extern GtkWidget *lwindow = NULL;

void Graphics_draw(Simu* simu, double FPS, int l, cairo_t *cr);
void Graphics_frame(double xmin, double xmax, double ymin, double ymax, cairo_t *cr);
int send_to_status(Simu* sim, const char* context, const char* format, ...);
int append_data_to_saving_file(Simu* sim, const char* format, ...);
