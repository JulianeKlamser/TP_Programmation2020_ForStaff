#pragma once

#include <gtk/gtk.h>
//#include "platform.h"
#include <stdbool.h>
typedef unsigned int uint;

# define	WIN_OK		128
# define	WIN_CANCEL		129

typedef enum
{
    FORM_TYPE_BOOL,
    FORM_TYPE_CHAR_AS_BOOL,
    FORM_TYPE_INT_AS_BOOL,
    FORM_TYPE_TOGGLE_BOOL,
    FORM_TYPE_TOGGLE_CHAR_AS_BOOL,
    FORM_TYPE_TOGGLE_INT_AS_BOOL,
    FORM_TYPE_INT,
    FORM_TYPE_UINT,
    FORM_TYPE_FLOAT,
    FORM_TYPE_DOUBLE,
    FORM_TYPE_TEXT,
    FORM_TYPE_OPTION,
    FORM_TYPE_COMBO_OPTION

} form_data_type_t;
typedef struct form_combo_entry_s
{
    int *index;
    char *value;
} form_combo_entry_t;

typedef struct form_entry_s
{
    form_data_type_t data_type;
    GObject *widget;
    void *out_data;
    struct form_entry_s *next;
} form_entry_t;

typedef struct
{
    GtkDialog *dialog;
    GtkBox *vertical_box;
    GtkBox *horizontal_box;

    GtkRadioButton *last_radio_button;
    int *last_radio_var;

    GtkComboBoxText *current_combo_box;
    int *current_combo_var;
    int current_combo_idx;

    form_entry_t *entry;
} form_window_t;

form_window_t *form_create(const char *title);
int form_run(form_window_t *form);
int form_free(form_window_t *form);

int form_newline(form_window_t *form);
int form_set_title(form_window_t *form, const char *title);
int form_push_label(form_window_t *form, const char *format, ...);
int form_push_label_ap(form_window_t *form, const char *format, va_list ap);
int form_push_bool(form_window_t *form, bool *var);
int form_push_char_as_bool(form_window_t *form, char *var);
int form_push_int_as_bool(form_window_t *form, int *var);
int form_push_checkbox_bool(form_window_t *form, bool *var);
int form_push_checkbox_char_as_bool(form_window_t *form, char *var);
int form_push_checkbox_int_as_bool(form_window_t *form, int *var);
int form_push_int(form_window_t *form, int *var);
int form_push_uint(form_window_t *form, uint *var);
int form_push_float(form_window_t *form, float step, float *var);
int form_push_double(form_window_t *form, float step, double *var);
int form_push_option(form_window_t *form, const char *label, int *var);
int form_push_combo_option(form_window_t *form, const char *label, int *var);
int form_push_combo_option_with_value(form_window_t *form, const char *label, int *var, char *value);
int form_push_text(form_window_t *form, char *var);
int form_entry_add(form_entry_t *entry_list, form_data_type_t data_type, GObject *widget, void *out_data);
int form_entry_retrieve_data(form_entry_t *entry_list);
