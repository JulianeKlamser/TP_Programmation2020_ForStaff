#include "file_picker_gui.h"
#include <stdlib.h>
#include <glib/gi18n.h>
#include <assert.h>
#include <unistd.h>

// more simple behavior than strncat
char *my_strncat(char *dest, const char *src, size_t dest_size)
{
    size_t len;

    if (dest == NULL)
    {
        if (src == NULL)
        {
            return NULL;
        }
        else
        {
            return strdup(src);
        }
    }

    if (src == NULL)
    {
        return dest;
    }

    len = strlen(dest);

    if (dest_size > len)
    {
        return strncat(dest, src, dest_size - len);
    }
    else
    {
        return dest;
    }
}



char    *slash_to_backslash(char *path)
{
    int i;

    if (path == NULL)
    {
        return NULL;
    }

    for (i = 0; path[i] != 0; i++)
        if (path[i] == '/')
        {
            path[i] = '\\';
        }

    return path;
}
char    *backslash_to_slash(char *path)
{
    int i;

    if (path == NULL)
    {
        return NULL;
    }

    for (i = 0; path[i] != 0; i++)
        if (path[i] == '\\')
        {
            path[i] = '/';
        }

    return path;
}

char *get_os_specific_path(char *path)
{
#ifdef XV_WIN32
    return slash_to_backslash(path);
#else
    return backslash_to_slash(path);
#endif
}
bool is_path_separator(char c)
{
    return c == '\\' || c == '/';
}
char get_path_separator(void)
{
#ifdef XV_WIN32
    return '\\';
#else
    return '/';
#endif
}
void put_path_separator(char *path, int n_path)
{
    size_t len = 0;

    if (path == NULL) return;
    len = strlen(path);
#ifdef XV_WIN32
    if (len > 0 && path[len-1] == '\\') return;
    my_strncat(path, "\\", n_path);
#else
    if (len > 0 && path[len-1] == '/') return;
    my_strncat(path, "/", n_path);
#endif
}


bool file_exists(const char *filename)
{
    FILE *file = NULL;
    file = fopen(filename, "r");
   if (file != NULL)
   {
       fclose(file);
       return true;
   }
   return false;
}


file_list_t *open_files_internal(const char *dialog_title, const char *current_uri, const char *filter,
                                 bool is_multiple)
{
    char *fulltitle = (char *) malloc(sizeof(char) * (20 + strlen(dialog_title)));
    file_list_t *file_list = NULL;
    GtkWidget *dialog = NULL;

    fulltitle = strcpy(fulltitle, "xvin - ");
    fulltitle = strcat(fulltitle, dialog_title);
    dialog = gtk_file_chooser_dialog_new(fulltitle,
					 NULL,
					 GTK_FILE_CHOOSER_ACTION_OPEN,
					 _("_Cancel"), GTK_RESPONSE_CANCEL,
					 _("_Open"), GTK_RESPONSE_ACCEPT,
					 NULL);

    gtk_file_chooser_select_filename(GTK_FILE_CHOOSER(dialog), current_uri ? current_uri : "");
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), is_multiple);
    add_win_filters_to_gtk_choose_dialog(GTK_FILE_CHOOSER(dialog), filter);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
      {
	GSList *fl;
	fl = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));
	file_list = GSList_to_filelist(fl);
      }
    gtk_widget_destroy(dialog);
    return file_list;
}

char *select_folder(const char *dialog_title, const char *current_uri)
{
    char *fulltitle = (char *) malloc(sizeof(char) * (20 + strlen(dialog_title)));
    char *res = NULL;
    GtkWidget *dialog = NULL;
    
    fulltitle = strcpy(fulltitle, "xvin - ");
    fulltitle = strcat(fulltitle, dialog_title);
    dialog = gtk_file_chooser_dialog_new(fulltitle,
					 NULL,
					 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
					 _("_Cancel"), GTK_RESPONSE_CANCEL,
					 _("_Open"), GTK_RESPONSE_ACCEPT,
					 NULL);
    gtk_file_chooser_select_filename(GTK_FILE_CHOOSER(dialog), current_uri);
    
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
      {
	res = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	
      }
    gtk_widget_destroy(dialog);
    return res;
}


file_list_t *open_files(const char *dialog_title, const char *current_uri, const char *filter)
{
    return open_files_internal(dialog_title, current_uri, filter, true);
}

char *open_one_file(const char *dialog_title, const char *current_uri, const char *filter)
{
    char *res = NULL;
    file_list_t *fl = open_files_internal(dialog_title, current_uri, filter, false);

    if (fl)
    {
        res = strdup(fl->data);
        free_file_list(fl);
    }
    return res;
}
/*
char *open_one_file_with_path(const char *dialog_title, const char *path, const char *file, const char *filter)
{
    char *res = NULL;
    char current_uri[2048] = {0};
    file_list_t *fl = NULL;

    if (build_full_file_name(current_uri, sizeof(current_uri), path, file) == NULL)
      return NULL;
    fl = open_files_internal(dialog_title, (const char*)current_uri, filter, false);
    if (fl)
    {
        res = strdup(fl->data);
        free_file_list(fl);
    }
    return res;
}
*/

char *save_one_file(const char *dialog_title, const char *current_dir, const char *filename, const char *filter)
{
    char *fulltitle = (char *) malloc(sizeof(char) * (20 + strlen(dialog_title)));
    char *choosen_file = NULL;
    char file[1024] = {0};
    char path[1024] = {0};
    char fullfilebuf[16384] = {0};
    char current_uri[16384] = {0};
    GtkWidget *dialog = NULL;
	
    fulltitle = strcpy(fulltitle, "xvin - ");
    fulltitle = strcat(fulltitle, dialog_title);

    if (filename == NULL || filename[0] == '\0')
    {
        char *ext = get_default_extension(filter);
        snprintf(file, sizeof(file), "Untitled.%s", ext);
        filename = file;
        free(ext);
    }
    // unsure correct url.
    strncpy(path, current_dir, sizeof(path));
    get_os_specific_path(path);
    put_path_separator(path, sizeof(path));
    strncpy(current_uri, path, sizeof(current_uri));
    my_strncat(current_uri, filename, sizeof(current_uri));
    strncpy(fullfilebuf, current_uri, sizeof(fullfilebuf)); // was filename

    dialog = gtk_file_chooser_dialog_new(fulltitle,
					 NULL,
					 GTK_FILE_CHOOSER_ACTION_SAVE,
					 _("_Cancel"), GTK_RESPONSE_CANCEL,
					 _("_Save"), GTK_RESPONSE_ACCEPT,
					 NULL);
    
    
    if (file_exists(current_uri))//, 0, NULL))
      {
	gtk_file_chooser_select_filename(GTK_FILE_CHOOSER(dialog), current_uri);
      }
    else
      {
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), filename);
      }
    add_win_filters_to_gtk_choose_dialog(GTK_FILE_CHOOSER(dialog), filter);
    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), 1);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
      {
	choosen_file = strdup(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog)));
      }
    gtk_widget_destroy(dialog);
    return choosen_file;
}

char *get_default_extension(const char *filters)
{
    const char *str_cursor = filters;
    char tmp[1024];
    const char *pattern = NULL;
    char *res = NULL;

    str_cursor = str_cursor + strlen(str_cursor) + 1;
    assert(str_cursor[0 != '\0']);
    strncpy(tmp, str_cursor, sizeof(tmp));

    pattern = strtok(tmp, ";");

    res = (char *) malloc(strlen(pattern) + 1);
    int j = 0;

    for (int i = 0; pattern[i] != '\0' && pattern[i] != ';'; i++)
    {
        if (pattern[i] != '*' && pattern[i] != '.' && pattern[i] != ' ') // FIXME : it is not very robust
        {
            res[j] = pattern[i];
            j++;
        }
    }
    res[j] = '\0';
    return res;
}

void add_win_filters_to_gtk_choose_dialog(GtkFileChooser *dialog, const char *filters)
{

    const char *str_cursor = filters;
    char tmp[1024];
    const char *pattern = NULL;

    while (str_cursor[0] != '\0')
    {
        GtkFileFilter *gfilter = gtk_file_filter_new();
        gtk_file_filter_set_name(gfilter, str_cursor);
        str_cursor = str_cursor + strlen(str_cursor) + 1;
        assert(str_cursor[0 != '\0']);
        strncpy(tmp, str_cursor, sizeof(tmp));

        pattern = strtok(tmp, ";");

        do
        {
            gtk_file_filter_add_pattern(gfilter, pattern);
        }
        while ((pattern = strtok(NULL, ";")));

        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), gfilter);
        str_cursor = str_cursor + strlen(str_cursor) + 1;
    }
}


file_list_t *GSList_to_filelist(GSList *gsl)
{
    if (gsl)
    {
        file_list_t *fl = (file_list_t *) malloc(sizeof(file_list_t));
        file_list_t *curs = fl;
        fl->data = strdup((const char *)gsl->data);
        fl->next = NULL;
        gsl = gsl->next;

        while (gsl != NULL)
        {
            file_list_t *tmp = (file_list_t *) malloc(sizeof(file_list_t));
            tmp->data = strdup((const char *)gsl->data);
            tmp->next = NULL;
            curs->next = tmp;
            curs = tmp;
            gsl = gsl->next;
        }

        return fl;
    }

    return NULL;
}
void free_file_list(file_list_t *file_list)
{
    while (file_list != NULL)
    {
        file_list_t *tmp = file_list;
        file_list = file_list->next;
        free(tmp->data);
        free(tmp);
    }
}
