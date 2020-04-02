#pragma once


#include <stdbool.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

typedef struct file_list_s {
char *data;
struct file_list_s *next;
} file_list_t;

file_list_t* open_files(const char *dialog_title, const char *current_uri, const char *filter);

char* open_one_file(const char *dialog_title, const char *current_uri, const char *filter);
char* save_one_file(const char *dialog_title, const char *current_dir, const char *filename, const char *filter);
char* open_one_file_with_path(const char *dialog_title, const char *path, const char *file, const char *filter);
void add_win_filters_to_gtk_choose_dialog(GtkFileChooser *dialog, const char *filters);
char* select_folder(const char *dialog_title, const char *current_uri);

file_list_t* GSList_to_filelist (GSList *gsl);
void free_file_list(file_list_t *file_list);
char* get_default_extension(const char *filters);
