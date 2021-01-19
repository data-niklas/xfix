#ifndef XFIX_H
#define XFIX_H

#include "Xlib.h"

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
} xfix_window;



/* ---------------------------------------------------------------- */
void xfix_flush(xfix_display * display);

void xfix_fill_display(xfix_display * display);
void xfix_init_display(xfix_display * display);
void xfix_init_display_by_name(xfix_display * display, bstring name);
void xfix_free_display(xfix_display * display);


#endif