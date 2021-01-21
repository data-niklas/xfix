#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "../lib/bstrlib.h"

#include "xfix.h"

/* Helpers ---------------------------------------------------------------- */
unsigned long xfix_rgba_to_argb(unsigned long rgba){
    unsigned char r = rgba >> 24;
    unsigned char g = (rgba >> 16)&0xFF;
    unsigned char b = (rgba >> 8)&0xFF;
    unsigned char a = rgba & 0xFF;
    if (a == 0)a = 1;
    return (a<<24) | (r*a/255)<<16 | (g*a/255)<<8 | b*a/255;
}
XVisualInfo xfix_visualinfo_from_depth(xfix_display *display, int depth)
{
    XVisualInfo visualinfo;
    XMatchVisualInfo(display->display, DefaultScreen(display), depth, TrueColor, &visualinfo);
    return visualinfo;
}

/* Display ---------------------------------------------------------------- */

void xfix_flush(xfix_display *display)
{
    XFlush(display->display);
}
void xfix_update(xfix_display *display)
{
    XFlush(display->display);
}
void xfix_sync(xfix_display *display)
{
    XSync(display->display, 0);
}


void xfix_fill_display(xfix_display *display)
{
    display->root = XRootWindow(display->display, XDefaultScreen(display->display));
}
void xfix_init_display(xfix_display *display)
{
    display->display = XOpenDisplay(NULL);
    xfix_fill_display(display);
}
void xfix_init_display_by_name(xfix_display *display, bstring name)
{
    display->display = XOpenDisplay(name->data);
    xfix_fill_display(display);
}
void xfix_free_display(xfix_display *display)
{
    XCloseDisplay(display->display);
}

/* Window ---------------------------------------------------------------- */
Window xfix_create_x_window(xfix_display *display, int x, int y, unsigned int width, unsigned int height, int depth, XSetWindowAttributes attributes, unsigned long attribute_mask)
{
    XVisualInfo vinfo = xfix_visualinfo_from_depth(display, depth);
    attributes.colormap = XCreateColormap(display->display, display->root, vinfo.visual, AllocNone);
    return XCreateWindow(display->display, display->root,
                         x, y, width, height, 0,
                         vinfo.depth,
                         InputOutput,
                         vinfo.visual,
                         attribute_mask,
                         &attributes);
}

void xfix_init_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height)
{
    window->depth = 24;
    XSetWindowAttributes attr;
    attr.background_pixel = 0;
    attr.border_pixel = 0;
    window->window = xfix_create_x_window(display, x, y, width, height, window->depth, attr, CWColormap | CWBackPixel | CWBorderPixel);
}

/**
 * For further reference:
 * rgba windows need the 3 listed attributes CWColormap, CWBackPixel and CWBorderPixel 
 * 
 **/
void xfix_init_rgba_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height)
{
    window->depth = 32;
    XSetWindowAttributes attr;
    attr.background_pixel = 0;
    attr.border_pixel = 0;
    window->window = xfix_create_x_window(display, x, y, width, height, window->depth, attr, CWColormap | CWBackPixel | CWBorderPixel);
}

void xfix_free_window(xfix_display *display, xfix_window *window)
{
    XDestroyWindow(display->display, window->window);
}
void xfix_show(xfix_display *display, xfix_window *window)
{
    XMapWindow(display->display, window->window);
}
void xfix_hide(xfix_display *display, xfix_window *window)
{
    XUnmapWindow(display->display, window->window);
}

int xfix_set_background(xfix_display *display, xfix_window *window, unsigned long rgb)
{
    int res = XSetWindowBackground(display->display, window->window, xfix_rgba_to_argb((rgb<<8) | 0xff));
    xfix_repaint(display, window);
    return res;
}

int xfix_set_rgba_background(xfix_display *display, xfix_window *window, unsigned long rgba)
{
    int res = XSetWindowBackground(display->display, window->window, xfix_rgba_to_argb(rgba));
    xfix_repaint(display, window);
    return res;
}

int xfix_repaint(xfix_display * display, xfix_window * window){
    return XClearWindow(display->display, window->window);
}

int xfix_repaint_area(xfix_display * display, xfix_window * window, int x, int y, unsigned int width, unsigned int height){
    return XClearArea(display->display, window->window, x, y, width, height, False);
}