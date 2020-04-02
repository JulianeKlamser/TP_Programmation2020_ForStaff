# pragma once


# define	WIN_OK		128
# define	WIN_CANCEL		129

# define D_O_K 0
# define OFF D_O_K
char *tex_to_pango_markup(const char *src);
int color_chooser(void);
//int simple_string_to_box(char *st,struct box *b0);
//int string_to_box,(char *st,struct box *b0);
char* backslash_to_slash(char *path);

int warning_message(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int warning_message_log(const char *log_file, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int error_message(const char *fmt, ...) __attribute__((format(printf, 1, 0)));
int error_message_log(const char *log_file, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int debug_message(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int debug_message_log(const char *log_file, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

int win_scanf(const char *fmt, ...);
int win_vscanf(const char *fmt, va_list ap);
int win_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void win_printf_void(const char *fmt, ...)__attribute__((format(printf, 1, 2)));
int win_printf_OK(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int win_ti_vprintf(const char *title, const char *fmt, va_list ap) __attribute__((format(printf, 2, 0)));
int win_ti_printf(const char *title, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
void* win_printf_ptr(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int win_simple_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int win_simple_printf_OK(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int win_ti_simple_vprintf(const char *title, const char *fmt, va_list ap) __attribute__((format(printf, 2, 0)));
int win_ti_simple_printf(const char *title, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
void* win_printf_simple_ptr(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

int win_simple_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int win_simple_printf_OK(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
int win_ti_simple_vprintf(const char *title, const char *fmt, va_list ap) __attribute__((format(printf, 2, 0)));
int win_ti_simple_printf(const char *title, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
void* win_simple_printf_ptr(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

int set_XVin_resolution(int size, int height, int is_full_screen);
int TeX_modify(const char *stuff);

int	display_title_message(const char *format, ...) __attribute__((format(printf, 1, 2)));

//# define UNCHANGED	8192	/* leave it as it was */

