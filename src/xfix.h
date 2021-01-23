#ifndef XFIX_H
#define XFIX_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif

#include "../lib/bstrlib.h"



typedef struct xfix_screen{
    int x;
    int y;
    int width;
    int height;
} xfix_screen;
typedef struct xfix_display
{
    Display *display;
    Window root;
    xfix_screen *screens;
    size_t num_screens;
} xfix_display;

typedef struct xfix_window
{
    Window window;
    int depth;
} xfix_window;

typedef enum xfix_map_state
{
    XFIX_MAP_STATE_UNMAPPED,
    XFIX_MAP_STATE_UNVIEWABLE,
    XFIX_MAP_STATE_VISIBLE
} xfix_map_state;

typedef struct xfix_size{
    unsigned int width;
    unsigned int height;
} xfix_size;

typedef struct xfix_position{
    int x;
    int y;
} xfix_position;

typedef struct xfix_bounds{
    unsigned int width;
    unsigned int height;
    int x;
    int y;
} xfix_bounds;

/* Helper methods ---------------------------------------------------------------- */
unsigned long xfix_rgba_to_argb(unsigned long color);
unsigned long xfix_argb_to_rgba(unsigned long color);
XVisualInfo xfix_visualinfo_from_depth(xfix_display *display, int depth);
XWindowAttributes xfix_attributes(xfix_display *display, xfix_window *window);

/* ---------------------------------------------------------------- */
void xfix_flush(xfix_display *display);
void xfix_sync(xfix_display *display);
void xfix_update(xfix_display *display);

void xfix_fill_display(xfix_display *display);
void xfix_init_display(xfix_display *display);
void xfix_init_display_by_name(xfix_display *display, bstring name);
void xfix_free_display(xfix_display *display);

/* Window methods ---------------------------------------------------------------- */

Window xfix_create_x_window(xfix_display *display, int x, int y, unsigned int width, unsigned int height, int depth, XSetWindowAttributes attributes, unsigned long attribute_mask);
void xfix_init_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height);
void xfix_init_rgba_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height);
void xfix_free_window(xfix_display *display, xfix_window *window);

int xfix_show(xfix_display *display, xfix_window *window);
int xfix_hide(xfix_display *display, xfix_window *window);
int xfix_raise(xfix_display *display, xfix_window *window);
int xfix_repaint(xfix_display *display, xfix_window *window);
int xfix_repaint_area(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height);

//Window setter
int xfix_set_background(xfix_display *display, xfix_window *window, unsigned long rgb);
int xfix_set_rgba_background(xfix_display *display, xfix_window *window, unsigned long rgba);
int xfix_set_undecorated(xfix_display *display, xfix_window *window, Bool undecorated);
int xfix_set_width(xfix_display *display, xfix_window *window, unsigned int width);
int xfix_set_height(xfix_display *display, xfix_window *window, unsigned int height);
int xfix_set_size(xfix_display *display, xfix_window *window, unsigned int width, unsigned int height);
int xfix_set_x(xfix_display *display, xfix_window *window, int x);
int xfix_set_y(xfix_display *display, xfix_window *window, int y);
int xfix_set_position(xfix_display *display, xfix_window *window, int x, int y);
int xfix_set_bounds(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height);
int xfix_set_map_state(xfix_display *display, xfix_window *window, xfix_map_state map_state);

//Window getter
unsigned long xfix_get_background(xfix_display *display, xfix_window *window);
unsigned long xfix_get_rgba_background(xfix_display *display, xfix_window *window);
Bool xfix_is_undecorated(xfix_display *display, xfix_window *window);
unsigned int xfix_get_width(xfix_display *display, xfix_window *window);
unsigned int xfix_get_height(xfix_display *display, xfix_window *window);
xfix_size xfix_get_size(xfix_display *display, xfix_window * window);
int xfix_get_x(xfix_display *display, xfix_window *window);
int xfix_get_y(xfix_display *display, xfix_window *window);
xfix_position xfix_get_position(xfix_display *display, xfix_window *window);
xfix_bounds xfix_get_bounds(xfix_display *display, xfix_window * window);
xfix_map_state xfix_get_map_state(xfix_display *display, xfix_window *window);
Bool xfix_is_shown(xfix_display *display, xfix_window *window);

#endif