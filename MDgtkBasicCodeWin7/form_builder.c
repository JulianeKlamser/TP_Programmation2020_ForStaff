#include "form_builder.h"
//#include "gui_def.h"
#include <stdlib.h>
#include <glib/gi18n.h>
//# include "gtk_grd.h"

extern GtkWidget *lwindow;
#define FORM_HORIZONTAL_EXPAND FALSE
#define FORM_HORIZONTAL_FILL FALSE
gboolean form_enter_event_handler(GtkWidget *widget, GdkEventKey *event)
{
    if (event->keyval == GDK_KEY_Return || event->keyval == GDK_KEY_ISO_Enter || event->keyval == GDK_KEY_KP_Enter
            || event->keyval == GDK_KEY_3270_Enter)
    {
        gtk_widget_grab_focus(gtk_dialog_get_widget_for_response(GTK_DIALOG(gtk_widget_get_toplevel(widget)),
                              GTK_RESPONSE_ACCEPT));
        //gtk_dialog_response(GTK_DIALOG(gtk_widget_get_toplevel(widget)), GTK_RESPONSE_ACCEPT);
        return TRUE;
    }

    return FALSE;
}

gboolean form_put_above(GtkWidget *widget, GdkEventWindowState *event)
{
    (void)widget;
    if (event->new_window_state & GDK_WINDOW_STATE_WITHDRAWN)
    {
        gtk_window_present(GTK_WINDOW(event->window));
        return TRUE;
    }

    return FALSE;
}

//int CAN_KEY_PRESS = 0;
/*
int	do_mouse_wheel_zoom_in_on_op(void);
int	do_mouse_wheel_zoom_out_on_op(void);
int	do_move_in_x_right(void);
int	do_move_in_x_left(void);


int on_key_press(void *_1, GdkEventKey *event, void *dlg)
{
    if (CAN_KEY_PRESS && event->state != 0)
    {
        switch (event->keyval)
        {
        case 65361//GDK_KEY_leftarrow:
            do_move_in_x_left();
            return 1;

        case 65362//GDK_KEY_uparrow:
            do_mouse_wheel_zoom_in_on_op();
            return 1;

        case 65363//GDK_KEY_rightarrow:
            do_move_in_x_right();
            return 1;

        case 65364//GDK_KEY_downarrow:
            do_mouse_wheel_zoom_out_on_op();
            return 1;

        default:
            break;
        }
    }

    return 0;
}
*/

form_window_t *form_create(const char *title)
{
    GtkDialogFlags flags = GTK_DIALOG_MODAL; //| GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget *dialog, *vertical_box, *horizontal_box;
    form_window_t *res = (form_window_t *) malloc(sizeof(form_window_t));
    form_entry_t *entry_gard = (form_entry_t *) malloc(sizeof(form_entry_t));
    GtkWidget *win = NULL;
    win = lwindow; // to be imprved
    dialog = gtk_dialog_new_with_buttons(title,
                                         GTK_WINDOW(win),
                                         flags,
                                         _("_Yes we can"),
                                         GTK_RESPONSE_ACCEPT,
                                         _("_Cancel"),
                                         GTK_RESPONSE_REJECT,
                                         NULL);
    /*
    if (CAN_KEY_PRESS)
    {
        g_signal_connect(dialog, "key_press_event", G_CALLBACK(on_key_press), dialog);
    }
    */
    gtk_window_set_keep_above(GTK_WINDOW(dialog), true);
    vertical_box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_container_set_border_width(GTK_CONTAINER(vertical_box), 15);
    //vertical_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    //gtk_container_set_border_width(GTK_CONTAINER(horizontal_box), 1);
    //GdkColor color;
    //color.red = 65535;
    //color.green = 0;
    //color.blue = 0;
    //gtk_widget_modify_bg ( GTK_WIDGET(horizontal_box), GTK_STATE_NORMAL, &color);
    //gtk_container_add(GTK_CONTAINER(dialog), vertical_box);
    gtk_box_pack_start(GTK_BOX(vertical_box), horizontal_box, TRUE, TRUE, 0);
    res->dialog = GTK_DIALOG(dialog);
    res->vertical_box = GTK_BOX(vertical_box);
    res->horizontal_box = GTK_BOX(horizontal_box);
    res->last_radio_button = NULL;
    res->last_radio_var = NULL;
    res->current_combo_var = NULL;
    res->current_combo_idx = 0;
    entry_gard->next = NULL;
    res->entry = entry_gard;
    return res;
}

int form_run(form_window_t *form)
{
    /*
    int x = 0;
    int y = 0;
    if (mouse_needs_poll())
    {
        poll_mouse();
    }

    x = mouse_x;
    y = mouse_y;
    */
    if (form->last_radio_button && form->last_radio_var)
    {
        form_entry_add(form->entry, FORM_TYPE_OPTION, G_OBJECT(form->last_radio_button), form->last_radio_var);
    }

    g_signal_connect(form->dialog, "window-state-event", G_CALLBACK(form_put_above), form);
    gtk_widget_show_all(GTK_WIDGET(form->dialog));
    //gtk_widget_grab_focus(gtk_dialog_get_widget_for_response(GTK_DIALOG(form->dialog), GTK_RESPONSE_ACCEPT));
    gint result = gtk_dialog_run(form->dialog);
    if (result == GTK_RESPONSE_ACCEPT)
    {
        form_entry_retrieve_data(form->entry);
    }
    gtk_widget_destroy(GTK_WIDGET(form->dialog));
    form->dialog = NULL;
    //while (gtk_events_pending())
    //{    gtk_main_iteration(); }

    // position_mouse(x, y);
    return result == GTK_RESPONSE_ACCEPT ? WIN_OK : WIN_CANCEL;
}

//int form_push();
int form_set_title(form_window_t *form, const char *title)
{
    gtk_window_set_title(GTK_WINDOW(form->dialog), title);
    return 0;
}
int form_newline(form_window_t *form)
{
    //static int colorsw = 1;
    //GdkColor color;
    //color.red = colorsw == 0 ? 65535 : 0;
    //color.green = colorsw == 1 ? 65535 : 0; //- redcolor;
    //color.blue = colorsw == 2 ? 65535 : 0;
    //gtk_container_set_border_width(GTK_CONTAINER(horizontal_box), 1);
    GtkWidget *horizontal_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    //gtk_widget_modify_bg ( GTK_WIDGET(horizontal_box), GTK_STATE_NORMAL, &color);
    //colorsw = (colorsw + 1) % 3;
    gtk_box_pack_start(GTK_BOX(form->vertical_box), horizontal_box, TRUE, TRUE, 0);
    form->horizontal_box = GTK_BOX(horizontal_box);
    return 0;
}

int form_push_label_ap(form_window_t *form, const char *format, va_list ap)
{
    char value[10000] = {0};
    GtkWidget *label = NULL;
    vsnprintf(value, sizeof(value), format, ap);
    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), value);
    gtk_box_pack_start(form->horizontal_box, label, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}
int form_push_label(form_window_t *form, const char *format, ...)
{
    int i = 0;
    va_list ap;
    va_start(ap, format);
    i = form_push_label_ap(form, format, ap);
    va_end(ap);
    return i;
}
int form_push_text(form_window_t *form, char *var)
{
    GtkWidget *en = gtk_entry_new();
    g_signal_connect(en, "key-press-event", G_CALLBACK(form_enter_event_handler), form);

    if (var != NULL)
    {
        gtk_entry_set_text(GTK_ENTRY(en), var);
        form_entry_add(form->entry, FORM_TYPE_TEXT, G_OBJECT(en), var);
    }

    gtk_box_pack_start(form->horizontal_box, en, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_bool(form_window_t *form, bool *var)
{
    GtkWidget *sw = gtk_switch_new();

    if (var != NULL)
    {
        gtk_switch_set_active(GTK_SWITCH(sw), *var);
        form_entry_add(form->entry, FORM_TYPE_BOOL, G_OBJECT(sw), var);
    }

    gtk_box_pack_start(form->horizontal_box, sw, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_char_as_bool(form_window_t *form, char *var)
{
    GtkWidget *sw = gtk_switch_new();

    if (var != NULL)
    {
        gtk_switch_set_active(GTK_SWITCH(sw), *var);
        form_entry_add(form->entry, FORM_TYPE_CHAR_AS_BOOL, G_OBJECT(sw), var);
    }

    gtk_box_pack_start(form->horizontal_box, sw, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_int_as_bool(form_window_t *form, int *var)
{
    GtkWidget *sw = gtk_switch_new();

    if (var != NULL)
    {
        gtk_switch_set_active(GTK_SWITCH(sw), *var);
        form_entry_add(form->entry, FORM_TYPE_INT_AS_BOOL, G_OBJECT(sw), var);
    }

    gtk_box_pack_start(form->horizontal_box, sw, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_checkbox_bool(form_window_t *form, bool *var)
{
    GtkWidget *sw = gtk_check_button_new();

    if (var != NULL)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sw), *var);
        form_entry_add(form->entry, FORM_TYPE_TOGGLE_BOOL, G_OBJECT(sw), var);
    }

    gtk_box_pack_start(form->horizontal_box, sw, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_checkbox_char_as_bool(form_window_t *form, char *var)
{
    GtkWidget *sw = gtk_check_button_new();

    if (var != NULL)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sw), *var);
        form_entry_add(form->entry, FORM_TYPE_TOGGLE_CHAR_AS_BOOL, G_OBJECT(sw), var);
    }

    gtk_box_pack_start(form->horizontal_box, sw, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_checkbox_int_as_bool(form_window_t *form, int *var)
{
    GtkWidget *sw = gtk_check_button_new();

    if (var != NULL)
    {
        gtk_switch_set_active(GTK_SWITCH(sw), *var);
        form_entry_add(form->entry, FORM_TYPE_TOGGLE_INT_AS_BOOL, G_OBJECT(sw), var);
    }

    gtk_box_pack_start(form->horizontal_box, sw, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_int(form_window_t *form, int *var)
{
    GtkAdjustment *adjustment = gtk_adjustment_new(*var, INT_MIN, INT_MAX, 1, 10, 100);
    GtkWidget *sp = gtk_spin_button_new(adjustment, 1, 0);
    g_signal_connect(sp, "key-press-event", G_CALLBACK(form_enter_event_handler), form);
    //gtk_entry_set_max_width_chars(GTK_ENTRY(sp), 7);
    gtk_entry_set_width_chars(GTK_ENTRY(sp), 7);
    form_entry_add(form->entry, FORM_TYPE_INT, G_OBJECT(adjustment), var);
    gtk_box_pack_start(form->horizontal_box, sp, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_uint(form_window_t *form, uint *var)
{
    GtkAdjustment *adjustment = gtk_adjustment_new(*var, 0, UINT_MAX, 1, 10, 100);
    GtkWidget *sp = gtk_spin_button_new(adjustment, 1, 0);
    g_signal_connect(sp, "key-press-event", G_CALLBACK(form_enter_event_handler), form);
    //gtk_entry_set_max_width_chars(GTK_ENTRY(sp), 7);
    gtk_entry_set_width_chars(GTK_ENTRY(sp), 7);
    form_entry_add(form->entry, FORM_TYPE_UINT, G_OBJECT(adjustment), var);
    gtk_box_pack_start(form->horizontal_box, sp, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}
int form_push_float(form_window_t *form, float step, float *var)
{
    GtkAdjustment *adjustment = gtk_adjustment_new(*var, -FLT_MAX, FLT_MAX, step, step * 10, step * 100);
    GtkWidget *sp = gtk_spin_button_new(adjustment, step, 5);
    g_signal_connect(sp, "key-press-event", G_CALLBACK(form_enter_event_handler), form);
    //gtk_entry_set_max_width_chars(GTK_ENTRY(sp), 10);
    gtk_entry_set_width_chars(GTK_ENTRY(sp), 10);
    form_entry_add(form->entry, FORM_TYPE_FLOAT, G_OBJECT(adjustment), var);
    gtk_box_pack_start(form->horizontal_box, sp, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}
int form_push_double(form_window_t *form, float step, double *var)
{
    GtkAdjustment *adjustment = gtk_adjustment_new(*var, -DBL_MAX, DBL_MAX, step, step * 10, step * 100);
    GtkWidget *sp = gtk_spin_button_new(adjustment, 1, 5);
    g_signal_connect(sp, "key-press-event", G_CALLBACK(form_enter_event_handler), form);
    //gtk_entry_set_max_width_chars(GTK_ENTRY(sp), 10);
    gtk_entry_set_width_chars(GTK_ENTRY(sp), 10);
    form_entry_add(form->entry, FORM_TYPE_DOUBLE, G_OBJECT(adjustment), var);
    gtk_box_pack_start(form->horizontal_box, sp, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_option(form_window_t *form, const char *label, int *var)
{
    GtkWidget *rb = NULL;

    if (var == form->last_radio_var)
    {
        GSList *group = form->last_radio_button ? gtk_radio_button_get_group(form->last_radio_button) : NULL;
        rb = gtk_radio_button_new_with_label(group, label);
    }
    else
    {
        if (form->last_radio_var && form->last_radio_button)
        {
            form_entry_add(form->entry, FORM_TYPE_OPTION, G_OBJECT(form->last_radio_button), form->last_radio_var);
        }

        rb = gtk_radio_button_new_with_label(NULL, label);
    }

    if (var  && *var == (int) g_slist_length(gtk_radio_button_get_group(GTK_RADIO_BUTTON(rb))) - 1)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb), TRUE);
    }

    form->last_radio_button = GTK_RADIO_BUTTON(rb);
    g_signal_connect(rb, "key-press-event", G_CALLBACK(form_enter_event_handler), form);
    form->last_radio_var = var;
    //form_entry_add(form->entry, FORM_TYPE_DOUBLE, G_OBJECT(sp), var);
    gtk_box_pack_start(form->horizontal_box, rb, FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
    return 0;
}

int form_push_combo_option_with_value(form_window_t *form, const char *label, int *var, char *value)
{
    if (var == form->current_combo_var)
    {
        gtk_combo_box_text_append_text(form->current_combo_box, label);
        form->current_combo_idx++;
    }
    else
    {
        form->current_combo_box = GTK_COMBO_BOX_TEXT(gtk_combo_box_text_new());
        gtk_combo_box_text_append_text(form->current_combo_box, label);
        form->current_combo_var = var;
        form->current_combo_idx = 0;
        gtk_box_pack_start(form->horizontal_box, GTK_WIDGET(form->current_combo_box),
                           FORM_HORIZONTAL_EXPAND, FORM_HORIZONTAL_FILL, 0);
        form_combo_entry_t *combo_entry = (form_combo_entry_t *) malloc(sizeof(form_combo_entry_t));
        combo_entry->index = var;
        combo_entry->value = value;
        form_entry_add(form->entry, FORM_TYPE_COMBO_OPTION, G_OBJECT(form->current_combo_box), combo_entry);
    }

    if (var  && *var == form->current_combo_idx)
    {
        gtk_combo_box_set_active(GTK_COMBO_BOX(form->current_combo_box), *var);
    }

    return 0;
}
int form_push_combo_option(form_window_t *form, const char *label, int *var)
{
    return form_push_combo_option_with_value(form, label, var, NULL);
}

int form_entry_add(form_entry_t *entry_list, form_data_type_t data_type, GObject *widget, void *out_data)
{
    form_entry_t *new_entry = (form_entry_t *) malloc(sizeof(form_entry_t));
    new_entry->data_type = data_type;
    new_entry->widget = widget;
    new_entry->out_data = out_data;
    new_entry->next = entry_list->next;
    entry_list->next = new_entry;
    return 0;
}

int form_entry_retrieve_data(form_entry_t *entry_list)
{
    form_entry_t *cur_entry = entry_list->next;
    form_entry_t *tmp = NULL;

    while (cur_entry != NULL)
    {
        switch (cur_entry->data_type)
        {
        case FORM_TYPE_BOOL:
            *((bool *) cur_entry->out_data) =  gtk_switch_get_active(GTK_SWITCH(cur_entry->widget));
            break;

        case FORM_TYPE_CHAR_AS_BOOL:
            *((char *) cur_entry->out_data) =  gtk_switch_get_active(GTK_SWITCH(cur_entry->widget));
            break;

        case FORM_TYPE_INT_AS_BOOL:
            *((int *) cur_entry->out_data) =  gtk_switch_get_active(GTK_SWITCH(cur_entry->widget));
            break;

        case FORM_TYPE_TOGGLE_BOOL:
            *((bool *) cur_entry->out_data) =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cur_entry->widget));
            break;

        case FORM_TYPE_TOGGLE_CHAR_AS_BOOL:
            *((char *) cur_entry->out_data) =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cur_entry->widget));
            break;

        case FORM_TYPE_TOGGLE_INT_AS_BOOL:
            *((int *) cur_entry->out_data) =  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cur_entry->widget));
            break;

        case FORM_TYPE_INT:
            *((int *) cur_entry->out_data) =  gtk_adjustment_get_value(GTK_ADJUSTMENT(cur_entry->widget));
            break;

        case FORM_TYPE_UINT:
            *((uint *) cur_entry->out_data) =  gtk_adjustment_get_value(GTK_ADJUSTMENT(cur_entry->widget));
            break;

        case FORM_TYPE_FLOAT:
            *((float *) cur_entry->out_data) =  gtk_adjustment_get_value(GTK_ADJUSTMENT(cur_entry->widget));
            break;

        case FORM_TYPE_DOUBLE:
            *((double *) cur_entry->out_data) =  gtk_adjustment_get_value(GTK_ADJUSTMENT(cur_entry->widget));
            break;

        case FORM_TYPE_TEXT:
        {
            const char *text = gtk_entry_get_text(GTK_ENTRY(cur_entry->widget));
            strcpy((char *) cur_entry->out_data, text); // FIXME : Unsafe copy !!!!
            break;
        }

        case FORM_TYPE_COMBO_OPTION:
        {
            *((form_combo_entry_t *) cur_entry->out_data)->index = gtk_combo_box_get_active(GTK_COMBO_BOX(cur_entry->widget));

            if (((form_combo_entry_t *) cur_entry->out_data)->value)
            {
                const char *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(cur_entry->widget));
                strcpy(((form_combo_entry_t *) cur_entry->out_data)->value, text);
            }

            //combo entry is allocated by the library
            free(cur_entry->out_data);
            break;
        }

        case FORM_TYPE_OPTION:
        {
            GSList *cur_radio = gtk_radio_button_get_group(GTK_RADIO_BUTTON(cur_entry->widget));
            int size = g_slist_length(cur_radio);
            int i = 0;

            while (cur_radio && gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(cur_radio->data)) == FALSE)
            {
                i++;
                cur_radio = cur_radio->next;
            }

            if (!cur_radio)
            {
                i = -1;
            }

            *((int *) cur_entry->out_data) =    size - i - 1;
            break;
        }
        }

        tmp = cur_entry;
        cur_entry = cur_entry->next;
        free(tmp);
    }

    entry_list->next = NULL;
    return 0;
}

int form_free(form_window_t *form)
{
    form_entry_t *cur_entry = form->entry;
    form_entry_t *tmp = NULL;

    while (cur_entry != NULL)
    {
        tmp = cur_entry;
        cur_entry = tmp->next;
        free(tmp);
    }

    free(form);
    return 0;
}
