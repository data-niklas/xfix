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
unsigned long xfix_rgba_to_argb(unsigned long color);
XVisualInfo xfix_visualinfo_from_depth(xfix_display * display, int depth);


/* ---------------------------------------------------------------- */
void xfix_flush(xfix_display * display);
void xfix_sync(xfix_display * display);
void xfix_update(xfix_display * display);

void xfix_fill_display(xfix_display * display);
void xfix_init_display(xfix_display * display);
void xfix_init_display_by_name(xfix_display * display, bstring name);
void xfix_free_display(xfix_display * display);


/* Window methods ---------------------------------------------------------------- */

Window xfix_create_x_window(xfix_display *display, int x, int y, unsigned int width, unsigned int height, int depth, XSetWindowAttributes attributes, unsigned long attribute_mask);
void xfix_init_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height);
void xfix_init_rgba_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height);

//void xfix_init_window_from_settings(xfix_display * display, xfix_window *window, xfix_window_settings settings);
void xfix_free_window(xfix_display * display, xfix_window * window);
void xfix_show(xfix_display * display, xfix_window * window);
void xfix_hide(xfix_display * display, xfix_window * window);


int xfix_set_background(xfix_display * display, xfix_window * window, unsigned long rgb);
int xfix_set_rgba_background(xfix_display * display, xfix_window * window, unsigned long rgba);
int xfix_repaint(xfix_display * display, xfix_window * window);
int xfix_repaint_area(xfix_display * display, xfix_window * window, int x, int y, unsigned int width, unsigned int height);
//

#endif