#ifndef XFIX_H
#define XFIX_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "../lib/bstrlib.h"

typedef struct xfix_display{
    Display * display;
    Window root;
    #ifdef XIMERAMA_
    int num_screens;
    #endif
} xfix_display;

typedef struct xfix_window {
    Window window;
    int depth;
} xfix_window;


typedef struct xfix_window_settings{
    int x;
    int y;
    unsigned int width;
    unsigned int height;
} xfix_window_settings;


/* Helper methods ---------------------------------------------------------------- */
XVisualInfo xfix_visualinfo_from_depth(xfix_display * display, int depth);


/* ---------------------------------------------------------------- */
void xfix_flush(xfix_display * display);
void xfix_update(xfix_display * display);

void xfix_fill_display(xfix_display * display);
void xfix_init_display(xfix_display * display);
void xfix_init_display_by_name(xfix_display * display, bstring name);
void xfix_free_display(xfix_display * display);


/* Window methods ---------------------------------------------------------------- */

void xfix_init_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height);
void xfix_init_window_from_settings(xfix_display * display, xfix_window *window, xfix_window_settings settings);
void xfix_free_window(xfix_display * display, xfix_window * window);
void xfix_show(xfix_display * display, xfix_window * window);
void xfix_hide(xfix_display * display, xfix_window * window);


#endif