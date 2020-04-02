#ifndef _WIN_GUI_C_
#define _WIN_GUI_C_


#include "locale.h"
#include <stdarg.h>
#include "win_gui.h"
# include <stdlib.h>
# include <string.h>

#include "form_builder.h"

/* we need to load fonts.dat to access the big font */


/*    int win_print(char *fmt, ...)
 *    DESCRIPTION    Print message in a window in a way similar to printf.
 *            The format "fmt" may contain messages with the
 *            %s, %d, %f etc formatting info of printf and the
 *            associated parameters.
 *            "\n"    produces a new line.
 *            "\t"    produces a tab spacing.
 *             The size of the window adapts to the number and size
 *            of the messages.
 *
 *    EXEMPLE        win_printf("\t\tHello world\n good morning M %s",name)
 *
 *    RETURNS        OK on success, WIN_CANCEL otherwise
 */


char *win_printf_message = NULL;

int debug_message(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));


static int verror_message_log(const char *log_file, const char *fmt, va_list ap) __attribute__ ((format (printf, 2, 0)));
static int vmessage_log(const char *log_file, const char *message_type, const char *fmt, va_list ap) __attribute__((format(printf, 3, 0)));
static int vwarning_message_log(const char *log_file, const char *fmt, va_list ap) __attribute__((format(printf, 2, 0)));
static int vdebug_message_log(const char *log_file, const char *fmt, va_list ap) __attribute__ ((format (printf, 2, 0)));


  
#ifdef XV_MAC
static int d_print_slash_proc(int msg, DIALOG *d, int c)
{
    int ret;
    //   ASSERT(d); // debuging
    /* call the parent object */
    ret = d_button_proc(msg, d, c);

    if (msg == MSG_CLICK)
    {
        d->dp2 = my_sprintf(d->dp2, "\\");
    }

    /* trap the close return value and change the font */
    if (ret == D_CLOSE)
    {
        return D_REDRAW;
    }

    /* otherwise just return */
    return ret;
}
#endif


/*
static int d_picoTeX_proc(int msg, void *DIALOG_d, int c)
{
    struct box *b = NULL;

    (void)c;
    return 0;
}
*/
# ifdef KEEP
    if (msg == MSG_DRAW)
    {
        if (d->dp2 == NULL)
        {
            b = (struct box *)calloc(1, sizeof(struct box));

            if (b == NULL)
            {
                return D_O_K;
            }

            ttf_enable = 0;
            init_box(b);
            b->color = White;
            string_to_box((char *) d->dp, b);  // UNSAFE CAST
            set_box_origin(b, d->x, d->y + d->h - 20, screen_to_box);
            d->dp2 = (void *)b;
        }

        b = (struct box *)d->dp2;
        //    set_clip(screen, d->x, d->y, d->x+d->w-1, d->y+d->h-1);
        set_box_origin(b, d->x, SCREEN_H - d->y, text_scale);

# ifdef XV_WIN32

        if (win_get_window() == GetForegroundWindow())
# endif
        {
            //  if ( screen_used)
            //  {
            //screen_used = 1;
            //scare_mouse();
            //acquire_screen();
            display_box(b, text_scale, White, TRUE_COLOR);
            /*    wr_box(b); */
            //release_screen();
            //unscare_mouse();
            //screen_used = 0;
            // }
        }

        set_clip_rect(screen, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
    }

    ttf_enable = tf;
    return D_O_K;
}
# endif

# ifdef KEEP
/*    add_text_to_box (struct box* ba, int x0, int y0, char *text);
 *    DESCRIPTION    Add a text to be display in x0, y0, y0 goes
 *            in viewport unit
 *
 *    RETURNS        0 on success, 1
 */
static struct box *add_simple_text_to_box(struct box *ba, int x0, int y0, char *text)
{
    struct box *bo = NULL;

    if (ba->n_box == 0)
    {
        init_box(ba);
    }

    bo = mknewbox(ba);

    if (bo == NULL)
    {
        return NULL;
    }

    bo->color = White;
    simple_string_to_box(text, bo);
    set_box_origin(bo, x0, -y0, text_scale);
    ba->w = (ba->w < bo->xc + bo->w) ? bo->xc + bo->w : ba->w;
    ba->d = (ba->d < - bo->yc + bo->d) ? - bo->yc + bo->d : ba->d;
    ba->h = (ba->h < bo->yc + bo->h) ? bo->yc + bo->h : ba->h;
    return bo;
}

/*    add_text_to_box (struct box* ba, int x0, int y0, char *text);
 *    DESCRIPTION    Add a text to be display in x0, y0, y0 goes
 *            in viewport unit
 *
 *    RETURNS        0 on success, 1
 */
static struct box *add_text_to_box(struct box *ba, int x0, int y0, char *text)
{
    struct box *bo = NULL;

    if (ba->n_box == 0)
    {
        init_box(ba);
    }

    bo = mknewbox(ba);

    if (bo == NULL)
    {
        return NULL;
    }

    bo->color = White;
    string_to_box(text, bo);
    set_box_origin(bo, x0, -y0, text_scale);
    ba->w = (ba->w < bo->xc + bo->w) ? bo->xc + bo->w : ba->w;
    ba->d = (ba->d < - bo->yc + bo->d) ? - bo->yc + bo->d : ba->d;
    ba->h = (ba->h < bo->yc + bo->h) ? bo->yc + bo->h : ba->h;
    return bo;
}
# endif
static int vmessage_log(const char *log_file, const char *message_type, const char *fmt, va_list ap)
{
    FILE *file = NULL;

    if (log_file)
    {
        file = fopen(log_file, "a");

        if (file != NULL)
        {
            fprintf(file, "%s", message_type);
            vfprintf(file, fmt, ap);
            fprintf(file, "%s", "\n");
            fclose(file);
        }
    }

    return file != NULL;
}


static int vwarning_message_log(const char *log_file, const char *fmt, va_list ap)
{
    va_list ap_log;

    va_copy(ap_log, ap);

#ifndef NO_WARNING_MESSAGES
#ifndef NO_WARNING_WINDOWS
    win_ti_vprintf("Warning", fmt, ap);
#else
    vprintf(fmt, ap);
    printf("\n");
#endif
    va_end(ap);
#endif

    vmessage_log(log_file, "Warning: ", fmt, ap_log);
    va_end(ap_log);

    return D_O_K;
}


int warning_message(const char *fmt, ...)
{
    va_list ap;
#ifndef NO_WARNING_MESSAGES
    va_start(ap, fmt);
    vwarning_message_log(NULL, fmt, ap);
    va_end(ap);
#endif
    return D_O_K;
}
int warning_message_log(const char *log_file, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vwarning_message_log(log_file, fmt, ap);
    va_end(ap);
    return D_O_K;

}

static int vdebug_message_log(const char *log_file, const char *fmt, va_list ap)
{
    va_list ap_log;

    va_copy(ap_log, ap);

#ifdef DEBUG_MESSAGES
#ifdef DEBUG_WINDOWS
    win_ti_vprintf("Debug", fmt, ap);
#else
    vprintf(fmt, ap);
    printf("\n");
#endif
    va_end(ap);
#endif

    vmessage_log(log_file, "Debug: ", fmt, ap_log);
    va_end(ap_log);

    return D_O_K;
}


#ifdef DEBUG_MESSAGES
int debug_message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vdebug_message_log(NULL, fmt, ap);
    va_end(ap);
    return D_O_K;
}
#else
int debug_message(const char *fmt, ...)
{
  (void)*fmt;
  return D_O_K;
}
#endif


int debug_message_log(const char *log_file, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vdebug_message_log(log_file, fmt, ap);
    va_end(ap);
    return D_O_K;

}


static int verror_message_log(const char *log_file, const char *fmt, va_list ap)
{
    va_list ap_log;

    va_copy(ap_log, ap);

#ifndef NO_ERROR_WINDOWS
    win_ti_vprintf("Error", fmt, ap);
#else
    vprintf(fmt, ap);
#endif

    vmessage_log(log_file, "Error: ", fmt, ap_log);
    va_end(ap_log);

    return D_O_K;
}


int error_message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verror_message_log(NULL, fmt, ap);
    va_end(ap);
    return D_O_K;
}

int error_message_log(const char *log_file, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verror_message_log(log_file, fmt, ap);
    va_end(ap);
    return D_O_K;

}

int win_printf_OK(const char *fmt, ...)
{
    //int i;
    va_list ap;
    va_start(ap, fmt);
    win_ti_vprintf("Message", fmt, ap);
    va_end(ap);
    return D_O_K;
}

int win_printf(const char *fmt, ...)
{
    int i;
    va_list ap;
    va_start(ap, fmt);
    i = win_ti_vprintf("Message", fmt, ap);
    va_end(ap);
    return i;
}
void win_printf_void(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    win_ti_vprintf("Message", fmt, ap);
    va_end(ap);
    return;
}

int win_ti_printf(const char *title, const char *fmt, ...)
{
    int i;
    va_list ap;
    va_start(ap, fmt);
    i = win_ti_vprintf(title, fmt, ap);
    va_end(ap);
    return i;
}

char *tex_to_pango_markup(const char *src)
{
    GError *err = NULL;
    gchar *res = NULL;
    gchar *tmp = g_markup_escape_text(src, -1);
    size_t i = 0;
    int    done = 0;
    char *reg[] = { "{\\\\raw (.*)}",                  "\\1",
                    "_\\{(.*)\\}|_(.)",              "<sub>\\1\\2</sub>",
                    "\\^\\{(.*)\\}|\\^(.)",          "<sup>\\1\\2</sup>",
                    "\\\\frac\\{(.*)\\}\\{(.*)\\}",  "\\1 / \\2",
                    "\\{\\\\color\\{(.*)\\}(.*)\\}", "<b>\\2</b>",
                    "\\\\sigma",                     "&#963;",
                    "\\\\Sigma",                     "&#931;",
                    "\\\\delta",                     "&#916;",
                    "\\\\[Gg]amma",                  "&#947;",
                    "\\\\Mu",                     "&#924;",
                    "\\\\mu",                     "&#956;",
                    "\\\\[Oo]c",                     "&#8226;",
                  };

    for(; i < sizeof(reg)/sizeof(char*) && done == 0; i += 2)
    {
        GRegex *gex = g_regex_new(reg[i], G_REGEX_UNGREEDY, G_REGEX_MATCH_NEWLINE_ANY, &err);
        res = g_regex_replace(gex, tmp, -1, 0, reg[i+1],    G_REGEX_MATCH_NEWLINE_ANY, &err);
        if(i == 0 && strcmp(tmp, res) != 0)
            done = 1;

        free(tmp);
        g_regex_unref(gex);
        tmp = res;

    }
    return res;
}

static int win_ti_form_vprintf(const char *title, const char *fmt, va_list ap)
{
    int i;
    char *pango_fmt = NULL;
    setlocale(LC_ALL, "C");
    form_window_t *form = NULL;

    form = form_create(title);
    pango_fmt = tex_to_pango_markup(fmt);
//    printf("%s\n",fmt);
//    printf("%s\n",pango_fmt);
    form_push_label_ap(form, pango_fmt, ap);
    i = form_run(form);
    form_free(form);

    return i;
}

int win_ti_vprintf(const char *title, const char *fmt, va_list ap)
{
     setlocale(LC_ALL, "C");

    return win_ti_form_vprintf(title, fmt, ap);

}
void *win_printf_ptr(const char *fmt, ...)
{
    //int i;
    va_list ap;
    setlocale(LC_ALL, "C");
    va_start(ap, fmt);
    win_ti_vprintf("Message", fmt, ap);
    va_end(ap);
    return NULL;
}

/*
int win_simple_printf_OK(const char *fmt, ...)
{
    //int i;
    va_list ap;
    va_start(ap, fmt);
    win_ti_vprintf("Message", fmt, ap);
    va_end(ap);
    return D_O_K;
}

int win_simple_printf(const char *fmt, ...)
{
    int i;
    va_list ap;
    va_start(ap, fmt);
    i = win_ti_simple_vprintf("Message", fmt, ap);
    va_end(ap);
    return i;
}
void *win_simple_printf_ptr(const char *fmt, ...)
{
    //int i;
    va_list ap;
    setlocale(LC_ALL, "C");
    va_start(ap, fmt);
    win_ti_simple_vprintf("Message", fmt, ap);
    va_end(ap);
    return NULL;
}
int win_ti_simple_printf(const char *title, const char *fmt, ...)
{
    int i;
    va_list ap;
    va_start(ap, fmt);
    i = win_ti_simple_vprintf(title, fmt, ap);
    va_end(ap);
    return i;
}
*/
# ifdef KEEP
int win_ti_simple_vprintf(const char *title, const char *fmt, va_list ap)
{
    int i;
    char *p = NULL, *c = NULL, *s = NULL;
    DIALOG *win_dialog = NULL;
    int  n_dia = 0;
    struct box *bo = NULL, *ba = NULL;
    int wth, hg, Lw, bg, fg, x0 = 150, y0 = 350; //l
    int col = 1, n_char = 0, hl = 0, x, y, w, h;
    int tf = ttf_enable;
    DIALOG_PLAYER *player;
    BITMAP *bmp = NULL, *bmps = NULL;
    int finish, m_x, m_y;
    setlocale(LC_ALL, "C");

    if ((win_dialog = (DIALOG *)calloc(64, sizeof(DIALOG))) == NULL)
    {
        return 1;
    }

    wth = 3 * menu_x0 / 8;
    hg = max_y / 3;
    Lw = Lift_W;
    //l = Lift_W/2;
    ttf_enable = 1;
    pc_fen = text_scale;

    if (win_printf_message == NULL)
    {
        if ((win_printf_message = (char *)calloc(8192, sizeof(char))) == NULL)
        {
            return 1;
        }
    }

    c = win_printf_message;
    vsnprintf(c, 8192, fmt, ap);
    ba = (struct box *)calloc(1, sizeof(struct box));

    if (ba == NULL)
    {
        return 1;
    }

    ba->color = White;

    for (p = c, s = c, hg = 5 * Lw / 2; *p ; p++)
    {
        switch (*p)
        {
        case '\n':
            *p = 0;
            bo = add_simple_text_to_box(ba , Lw * col, hg + Lw / 2, s);
            i = (bo->h + bo->d) / screen_to_box;
            hl = (i > hl) ? i : hl;
            hg += Lw / 2 + hl;
            hl = 0;
            i = Lw * col + (bo->w) / screen_to_box + Lw;
            wth = (i > wth) ? i : wth;
            col = 1;
            n_char = 0;
            s = p + 1;
            break;

        case '\t':
            *p = 0;
            s = (*s == 0) ? s + 1 : s;

            if (n_char)
            {
                bo = add_simple_text_to_box(ba , Lw * col, hg + Lw / 2, s);
                n_char = 0;
                i = (bo->h + bo->d) / screen_to_box;
                hl = (i > hl) ? i : hl;
                i = Lw * col + (bo->w) / screen_to_box + Lw;
                wth = (i > wth) ? i : wth;
                s = p + 1;
            }

            col++;
            break;

        default :        /* hoops    */
            n_char = (n_char == 0) ? 1 : n_char;
            break;
        };
    }

    if (n_char)
    {
        bo = add_simple_text_to_box(ba , Lw * col, hg + Lw / 2, s);
        i = (bo->h + bo->d) / screen_to_box;
        hl = (i > hl) ? i : hl;
        hg += Lw / 2 + hl;
        i = Lw * col + (bo->w) / screen_to_box + Lw;
        wth = (i > wth) ? i : wth;
    }

    hg += 3 * Lw;
    h = (hg > SCREEN_H) ? SCREEN_H : hg;
    w = (wth > SCREEN_W) ? SCREEN_W : wth;
    x0 = x = (SCREEN_W - w) / 2;
    y0 = y = (SCREEN_H - h) / 2;
    set_box_origin(ba, x0, SCREEN_H - y0, text_scale);
    /*
          if ( hg >  ac->sc.max_y - Lw)
          {
          wth += Lw;
          hg = ac->sc.max_y - Lw;
          li = build_vert_lift(RIGHT_SIDE-Lw, 2*l, BOTTOM_LINE -4*l, 0);
          li->frame.action = move_text;
          hook_it_up(ac, IS_LIFT, (void *)li, TEXT_LIFT);
          }
          */
    bg = makecol(200, 200, 200);
    fg = makecol(32, 32, 32);
    win_dialog[n_dia++] = (DIALOG)
    {
        d_myclear_proc, 0, 0, w, h, fg, bg, 0, 0, 0, 0, NULL, NULL, NULL
    };
    win_dialog[n_dia++] = (DIALOG)
    {
        d_title_proc, 0, 0, w, 2 * Lw, fg, bg, 0, 0, 0, 0, title, NULL, NULL
    };
    h -= (11 * Lw) / 2;
    win_dialog[n_dia++] = (DIALOG)
    {
        d_picoTeX_proc, 0, 0, w, h, fg, bg, 0, D_DISABLED, 0, 0, c, ba, NULL
    };
    y = h + (3 * Lw);
    x = w / 4 - 50;
    win_dialog[n_dia++] = (DIALOG)
    {
        d_yield_proc, 0, 0, 0, 0, 255, 0, 't', 0, 0, 0, NULL, NULL, NULL
    };
    win_dialog[n_dia++] = (DIALOG)
    {
        d_button_proc, x, y, 100, 2 * Lw, fg, bg, '\r', D_EXIT, 0, 0, (void *)"Yes We Can", NULL, NULL
    };
    x = 3 * w / 4 - 50;
    win_dialog[n_dia++] = (DIALOG)
    {
        d_button_proc, x, y, 100, 2 * Lw, fg, bg, 27, D_EXIT, 0, 0, (void *) "Cancel", NULL, NULL
    };
    //win_dialog[n_dia++] = (DIALOG){general_idle,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL};
    win_dialog[n_dia++] = (DIALOG)
    {
        NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL
    };
    position_dialog(win_dialog, x0, y0);
    clear_keybuf();

    do
    {
    }
    while (gui_mouse_b());

    for (i = -1; i == -1;)
    {
        //does not work with create_video_bitmap
        bmp = create_bitmap(SCREEN_W, SCREEN_H);

        if (bmp == NULL)
        {
            bmp = create_bitmap(SCREEN_W, SCREEN_H);
        }

        bmps = create_bitmap(win_dialog->w, win_dialog->h);

        if (bmps == NULL)
        {
            bmps = create_bitmap(win_dialog->w, win_dialog->h);
        }

        if (bmp == NULL || bmps == NULL)
        {
            return win_printf("out of mem");
        }

        //if(!screen_used)
        //{
        //  screen_used = 1
        //
# ifdef XV_WIN32
        scare_mouse();
#endif
	for(;screen_acquired;); // we wait for screen_acquired back to 0; 
	screen_acquired = 1;	
        acquire_screen(); // 2013-10-06 vc;
        set_clip_rect(screen, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
        blit(screen, bmp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        release_screen(); // 2013-10-06 vc
	screen_acquired = 0;	
# ifdef XV_WIN32
        unscare_mouse();
#endif
        //  screen_used = 0;
        //  }
        player = init_dialog(win_dialog, 2);

        for (finish = 0 ; finish == 0;)
        {
            while (update_dialog(player));

            if (player->obj == 1)
            {
                m_x = player->mouse_ox;
                m_y = player->mouse_oy;

                while (mouse_b)
                {
                    if ((m_x != mouse_x) || (m_y != mouse_y))
                    {
                        vsync();
                        //if (!screen_used)
                        //{
                        //  screen_used = 1;
# ifdef XV_WIN32
                        scare_mouse_area(win_dialog->x, win_dialog->y, win_dialog->w, win_dialog->h);
#endif
			for(;screen_acquired;); // we wait for screen_acquired back to 0; 
			screen_acquired = 1;
			
                        acquire_screen(); // 2013-10-06 vc
                        blit(screen, bmps, win_dialog->x, win_dialog->y, 0, 0, win_dialog->w, win_dialog->h);
                        blit(bmp, screen, win_dialog->x, win_dialog->y, win_dialog->x, win_dialog->y, win_dialog->w, win_dialog->h);
                        release_screen(); // 2013-10-06 vc
			screen_acquired = 0;


# ifdef XV_WIN32
                        unscare_mouse();
#endif
                        x0 += mouse_x - m_x;
                        y0 += mouse_y - m_y;
                        position_dialog(win_dialog, x0, y0);
                        m_x = mouse_x;
                        m_y = mouse_y;
# ifdef XV_WIN32
                        scare_mouse_area(win_dialog->x, win_dialog->y, win_dialog->w, win_dialog->h);
#endif
			for(;screen_acquired;); // we wait for screen_acquired back to 0; 
			screen_acquired = 1;        
                        acquire_screen(); // 2013-10-06 vc
                        blit(bmps, screen, 0, 0, win_dialog->x, win_dialog->y, win_dialog->w, win_dialog->h);
                        release_screen(); // 2013-10-06 vc
			screen_acquired = 0;        			
# ifdef XV_WIN32
                        unscare_mouse();
#endif
                        //  screen_used = 0;
                        //}
                    }
                }
            }
            else
            {
                finish = 1;
                i = player->obj;
            }
        }
    }

    //if(!screen_used)
    // {
    //screen_used = 1;
# ifdef XV_WIN32
    scare_mouse_area(win_dialog->x, win_dialog->y, win_dialog->w, win_dialog->h);
#endif
    for(;screen_acquired;); // we wait for screen_acquired back to 0;     
    screen_acquired = 1;
    acquire_screen(); // 2013-10-06 vc
    blit(bmp, screen, win_dialog->x, win_dialog->y, win_dialog->x, win_dialog->y, win_dialog->w, win_dialog->h);
    release_screen(); // 2013-10-06 vc
    screen_acquired = 0;    
# ifdef XV_WIN32
    unscare_mouse();
#endif
    //screen_used = 1;
    //}
    destroy_bitmap(bmp);
    destroy_bitmap(bmps);
    shutdown_dialog(player);

    //position_dialog(win_dialog,x0,y0);
    //i = popup_dialog(win_dialog,3);
    if (win_dialog)
    {
        free(win_dialog);
    }

    free_box(ba);

    if (ba)
    {
        free(ba);
    }

    ttf_enable = tf;
    return (i == 4) ? WIN_OK : WIN_CANCEL;
}

# endif
/*
static int no_ac(void)
{
    return D_O_K;
}
*/

int win_scanf(const char *fmt, ...)
{
    int ret = 0;
    va_list ap;
    va_start(ap, fmt);
    ret = win_vscanf(fmt, ap);
    va_end(ap);
    return ret;
}
//TODO : add field size, implement last functionalities
static int win_form_vscanf(const char *fmt, va_list ap)
{
    const char *p = NULL;
    char str[1024] = {0};
    char *pango_str = NULL;
    int i = 0;
    int n_format = 0;
    char *bp = NULL;
    int *radio_ptr = NULL;
    char *serial_b_ptr = NULL;
    int serial_b_count = 0;
    //MENU *menu = NULL;
    char *title = NULL;

    setlocale(LC_ALL, "C");
    form_window_t *form = NULL;

    form = form_create("Enter Information");


    for (p = fmt; *p ; p++)
    {
        if (*p == '\n')
        {
            str[i] = 0;
            pango_str = tex_to_pango_markup(str);
            form_push_label(form, pango_str);
            free(pango_str);
            form_newline(form);
            i = 0;

        }
        else if (*p != '%')
        {
            str[i] = *p;
            i = (i < 1024) ? i + 1 : i;
            continue;
        }
        else
        {
            n_format = (int)strtol(p + 1, &bp, 10);

            if (n_format != 0 && bp != NULL)
            {
                p = bp - 1;
            }
            else
            {
                n_format = 0;
            }

            switch (*++p)
            {
            case 'd':
            {
                int *var_int = va_arg(ap, int *);
                str[i] = 0;
                pango_str = tex_to_pango_markup(str);
                form_push_label(form, pango_str);
                free(pango_str);
                form_push_int(form, var_int);
                i = 0;
                break;
            }

            case 's':
            {
                char *var_str = va_arg(ap, char *);
                str[i] = 0;

                pango_str = tex_to_pango_markup(str);
                form_push_label(form, pango_str);
                free(pango_str);
                form_push_text(form, var_str);

                i = 0;
                break;
            }

            case 'f':
            {
                float *var_float = va_arg(ap, float *);
                str[i] = 0;
                pango_str = tex_to_pango_markup(str);
                form_push_label(form, pango_str);
                free(pango_str);
                form_push_float(form, 0.000001, var_float); //TODO change precision
                i = 0;
                break;
            }

            case 'b':
            {
                int *var_bool = (int *) va_arg(ap, int *);
                str[i] = 0;
                pango_str = tex_to_pango_markup(str);
                form_push_label(form, pango_str);
                free(pango_str);
                form_push_int_as_bool(form, var_bool);
                i = 0;
                break;
            }

            case 'Q':
            {
                serial_b_ptr = va_arg(ap, char *);
                str[i] = 0;
                pango_str = tex_to_pango_markup(str);
                form_push_label(form, pango_str);
                free(pango_str);
                form_push_checkbox_char_as_bool(form, &(serial_b_ptr[serial_b_count]));
                serial_b_count++;
                i = 0;
                break;
            }

            case 'q':
            {
                if (!serial_b_ptr)
                {
                    serial_b_ptr = va_arg(ap, char *);
                }

                str[i] = 0;
                pango_str = tex_to_pango_markup(str);
                form_push_label(form, pango_str);
                free(pango_str);
                form_push_checkbox_char_as_bool(form, &(serial_b_ptr[serial_b_count]));
                serial_b_count++;
                i = 0;
                break;
            }

            case 'R':
            {
                radio_ptr = va_arg(ap, int *);
                str[i] = 0;
                pango_str = tex_to_pango_markup(str);
                form_push_label(form, pango_str);
                free(pango_str);
                form_push_option(form, "", radio_ptr);

                i = 0;
                break;
            }

            case 'r':
            {
                if (radio_ptr == NULL)
                {
                    radio_ptr = va_arg(ap, int *);
                }

                str[i] = 0;
                pango_str = tex_to_pango_markup(str);
                form_push_label(form, pango_str);
                free(pango_str);
                form_push_option(form, "", radio_ptr);

                i = 0;
                break;
            }

            case 'l':
            {
                if (p[1] == 'f')
                {
                    double *var_double = va_arg(ap, double *);
                    str[i] = 0;
                    pango_str = tex_to_pango_markup(str);
                    form_push_label(form, pango_str);
                    free(pango_str);
                    form_push_double(form, 0.000001, var_double); // TODO : change precision
                    i = 0;

                    p++;
                }
                else if (p[1] == 's')
                {
                    char *var_char = va_arg(ap, char *);
                    str[i] = 0;
                    pango_str = tex_to_pango_markup(str);
                    form_push_label(form, pango_str);
                    free(pango_str);
                    form_push_text(form, var_char);
                    i = 0;
                    p++;
                }

                break;
            }

            case 'm':
            {
                //menu = va_arg(ap, MENU *); // TODO NOT IMPLEMENTED
                break;
            }

            case 't':
            {
                title = va_arg(ap, char *);
                form_set_title(form, title);
                break;
            }

            default :        /* hoops    */
            {
                str[i] = *p;
                i = (i < 255) ? i + 1 : i;
                break;
            }
            };
        }
    }

    if (i > 0)
    {
        str[i] = 0;
        pango_str = tex_to_pango_markup(str);
        form_push_label(form, pango_str);
        free(pango_str);
    }

    i = form_run(form);
    form_free(form);
    return i;

}
/*    int win_scanf(char *fmt, ...)
 *    DESCRIPTION    Get input from a window in a way similar to scanf.
 *            The format "fmt" may contain messages separated by
 *            %s, %d, %f; a line is print with the message, an
 *            input-line follows providing input for this parameter.
 *            The address of the parameter must be supply, the
 *            initial value is displayed. A final %m in the format
 *             allow to attach a menu to the window. %t customizes
 *            the title of the alert window. The size of the
 *            window adapts to the number and size of the messages.
 *
 *    EXEMPLE        win_scanf("Enter your name %s and your age %d%m",name,
 *                &age,quidam_menu);
 *
 *    RETURNS        OK on success, WIN_CANCEL otherwise
 *
 */
int win_vscanf(const char *fmt, va_list ap)
{
    setlocale(LC_ALL, "C");

    return win_form_vscanf(fmt, ap);

}
int    display_title_message(const char *format, ...)
{
  (void)format;
    return 0;
}

/*


int color_chooser(void)
{
    static int color = 0;
    int ret = 0;
    ret = win_scanf("%R Yellow\n %r Green\n%r Blue\n %r Red\n %r Cyan \n %r Magenta \n %r White \n %r Brown", &color);

    if (ret != 128)
    {
        return -1;
    }

    if (color == 0)
    {
        color = Yellow;
    }
    else if (color == 1)
    {
        color = Lightgreen;
    }
    else if (color == 2)
    {
        color = Lightblue;
    }
    else if (color == 3)
    {
        color = Lightred;
    }
    else if (color == 4)
    {
        color = Lightcyan;
    }
    else if (color == 5)
    {
        color = Lightmagenta;
    }
    else if (color == 6)
    {
        color = White;
    }
    else
    {
        color = Brown;
    }

    return color;
}


*/

# endif /* _WIN_GUI_C_ */











