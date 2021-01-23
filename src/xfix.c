#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "../lib/bstrlib.h"

#include "xfix.h"

/* Helpers ---------------------------------------------------------------- */
unsigned long xfix_rgba_to_argb(unsigned long rgba)
{
    unsigned char r = rgba >> 24;
    unsigned char g = (rgba >> 16) & 0xFF;
    unsigned char b = (rgba >> 8) & 0xFF;
    unsigned char a = rgba & 0xFF;
    if (a == 0)
        a = 1;
    return (a << 24) | (r * a / 255) << 16 | (g * a / 255) << 8 | b * a / 255;
}
unsigned long xfix_argb_to_rgba(unsigned long argb)
{
    unsigned char a = argb >> 24;
    unsigned char r = (argb >> 16) & 0xFF;
    unsigned char g = (argb >> 8) & 0xFF;
    unsigned char b = argb & 0xFF;
    if (a == 0)
        a = 1;
    r = (r * 255 / a);
    g = (g * 255 / a);
    b = (b * 255 / a);
    return (r << 24) | (g << 16) | (b << 8) | a;
}
XVisualInfo xfix_visualinfo_from_depth(xfix_display *display, int depth)
{
    XVisualInfo visualinfo;
    XMatchVisualInfo(display->display, DefaultScreen(display), depth, TrueColor, &visualinfo);
    return visualinfo;
}

XWindowAttributes xfix_attributes(xfix_display *display, xfix_window *window)
{
    XWindowAttributes attributes;
    XGetWindowAttributes(display->display, window->window, &attributes);
    return attributes;
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

// Visual ----------------------------------------------------------------
int xfix_show(xfix_display *display, xfix_window *window)
{
    return XMapWindow(display->display, window->window);
}
int xfix_hide(xfix_display *display, xfix_window *window)
{
    return XUnmapWindow(display->display, window->window);
}
int xfix_raise(xfix_display *display, xfix_window *window)
{
    return XRaiseWindow(display, window);
}
int xfix_repaint(xfix_display *display, xfix_window *window)
{
    return XClearWindow(display->display, window->window);
}
int xfix_repaint_area(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height)
{
    return XClearArea(display->display, window->window, x, y, width, height, False);
}

// Setter ----------------------------------------------------------------
int xfix_set_background(xfix_display *display, xfix_window *window, unsigned long rgb)
{
    int res = XSetWindowBackground(display->display, window->window, xfix_rgba_to_argb((rgb << 8) | 0xff));
    xfix_repaint(display, window);
    return res;
}

int xfix_set_rgba_background(xfix_display *display, xfix_window *window, unsigned long rgba)
{
    int res = XSetWindowBackground(display->display, window->window, xfix_rgba_to_argb(rgba));
    xfix_repaint(display, window);
    return res;
}

int xfix_set_undecorated(xfix_display *display, xfix_window *window, Bool undecorated)
{
    XSetWindowAttributes attr;
    attr.override_redirect = undecorated;
    int result = XChangeWindowAttributes(display->display, window->window, CWOverrideRedirect, &attr);
    if (undecorated)
    {
        XWindowAttributes attr = xfix_attributes(display, window);
        if (attr.map_state != IsUnmapped)
        {
            XReparentWindow(display->display, window->window, display->root, attr.x, attr.y);
        }
    }
    else
    {
                //Request reparenting by window manager
        XMapRequestEvent event;
        event.type = MapRequest;
        event.send_event = False;
        event.display = display->display;
        event.window = window->window;
        event.parent = window->window;
        XSendEvent(display->display, window->window, True, StructureNotifyMask, &event);
    }
    return result;
}

int xfix_set_width(xfix_display *display, xfix_window *window, unsigned int width)
{
    return xfix_set_size(display, window, width, xfix_get_height(display, window));
}
int xfix_set_height(xfix_display *display, xfix_window *window, unsigned int height)
{

    return xfix_set_size(display, window, xfix_get_width(display, window), height);
}

int xfix_set_size(xfix_display *display, xfix_window *window, unsigned int width, unsigned int height)
{
    return XResizeWindow(display->display, window->window, width, height);
}

int xfix_set_x(xfix_display *display, xfix_window *window, int x)
{
    return xfix_set_position(display, window, x, xfix_get_y(display, window));
}
int xfix_set_y(xfix_display *display, xfix_window *window, int y)
{

    return xfix_set_position(display, window, xfix_get_x(display, window), y);
}
int xfix_set_position(xfix_display *display, xfix_window *window, int x, int y)
{
    return XMoveWindow(display->display, window->window, x, y);
}

int xfix_set_bounds(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height)
{
    return XMoveResizeWindow(display->display, window->window, x, y, width, height);
}



//Getter ----------------------------------------------------------------
unsigned long xfix_get_background(xfix_display *display, xfix_window *window)
{
    return xfix_argb_to_rgba(xfix_attributes(display, window).backing_pixel) >> 8;
}
unsigned long xfix_get_rgba_background(xfix_display *display, xfix_window *window)
{
    return xfix_argb_to_rgba(xfix_attributes(display, window).backing_pixel);
}

Bool xfix_is_undecorated(xfix_display *display, xfix_window *window)
{
    return xfix_attributes(display, window).override_redirect;
}
unsigned int xfix_get_width(xfix_display *display, xfix_window *window)
{
    return xfix_attributes(display, window).width;
}
unsigned int xfix_get_height(xfix_display *display, xfix_window *window)
{
    return xfix_attributes(display, window).height;
}

xfix_size xfix_get_size(xfix_display *display, xfix_window *window)
{
    XWindowAttributes attributes = xfix_attributes(display, window);
    return (xfix_size){.width = attributes.width, .height = attributes.height};
}
int xfix_get_x(xfix_display *display, xfix_window *window)
{
    return xfix_attributes(display, window).x;
}
int xfix_get_y(xfix_display *display, xfix_window *window)
{
    return xfix_attributes(display, window).y;
}

xfix_position xfix_get_position(xfix_display *display, xfix_window *window)
{
    XWindowAttributes attributes = xfix_attributes(display, window);
    return (xfix_position){.x = attributes.x, .y = attributes.y};
}
xfix_bounds xfix_get_bounds(xfix_display *display, xfix_window *window)
{
    XWindowAttributes attributes = xfix_attributes(display, window);
    return (xfix_bounds){.x = attributes.x, .y = attributes.y, .width = attributes.width, .height = attributes.height};
}

xfix_map_state xfix_get_map_state(xfix_display *display, xfix_window *window)
{
    return xfix_attributes(display, window).map_state;
}
Bool xfix_is_shown(xfix_display *display, xfix_window *window)
{
    return xfix_get_map_state(display, window) != XFIX_MAP_STATE_UNMAPPED;
}