#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "../lib/bstrlib.h"

#include "xfix.h"

/* Helpers ---------------------------------------------------------------- */
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
void xfix_init_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height)
{
    window->depth = 24;
    XVisualInfo vinfo = xfix_visualinfo_from_depth(display, window->depth);
    XSetWindowAttributes attr;
    attr.background_pixel = WhitePixel(display->display, DefaultScreen(display->display));
    attr.border_pixel = 0;
    window->window = XCreateWindow(display->display, display->root,
                                   x, y, width, height, 0,
                                   window->depth,
                                   InputOutput,
                                   vinfo.visual,
                                   CWBackPixel | CWBorderPixel,
                                   &attr);
}
void xfix_init_window_from_settings(xfix_display * display, xfix_window *window, xfix_window_settings settings){
    if (settings.x == NULL)settings.x = 0;
    if (settings.y == NULL)settings.y = 0;
    if (settings.width == NULL)settings.width = 0;
    if (settings.height == NULL)settings.height = 0;
    window->depth = 24;
    XVisualInfo vinfo = xfix_visualinfo_from_depth(display, window->depth);
    XSetWindowAttributes attr;
    attr.background_pixel = WhitePixel(display->display, DefaultScreen(display->display));
    attr.border_pixel = 0;
    window->window = XCreateWindow(display->display, display->root,
                                   settings.x, settings.y, settings.width, settings.height, 0,
                                   window->depth,
                                   InputOutput,
                                   vinfo.visual,
                                   CWBackPixel | CWBorderPixel,
                                   &attr);
}
void xfix_free_window(xfix_display * display, xfix_window *window){
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