#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include "../lib/bstrlib.h"
#include <Imlib2.h>

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
	XMatchVisualInfo(display->display, display->default_screen, depth, TrueColor, &visualinfo);
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

int xfix_init_display(xfix_display *display, char *name)
{
	display->display = XOpenDisplay(name);
	display->root = XRootWindow(display->display, XDefaultScreen(display->display));

#ifdef XINERAMA
	if (XineramaIsActive(display->display))
	{
		XineramaScreenInfo *xinerama_screens = XineramaQueryScreens(display->display, &display->num_screens);

		display->screens = malloc(sizeof(xfix_screen) * display->num_screens);

		for (int i = 0; i < display->num_screens; i++)
		{
			display->screens[i].width = xinerama_screens[i].width;
			display->screens[i].height = xinerama_screens[i].height;
			display->screens[i].x = xinerama_screens[i].x_org;
			display->screens[i].y = xinerama_screens[i].y_org;
		}
	}
#else
	display->num_screens = 1;
	display->screens = malloc(sizeof(xfix_screen));

	Screen *screen = ScreenOfDisplay(display->display, XDefaultScreen(display->display));
	display->screens[0].width = screen->width;
	display->screens[0].height = screen->height;
	display->screens[0].x = display->screens[0].y = 0;

#endif
	display->default_screen = XDefaultScreen(display->display);
	// ~ Professional
	return 1; // error handling, lol
}
void xfix_free_display(xfix_display *display)
{
	XCloseDisplay(display->display);
}

void xfix_imlib_set_display(xfix_display *display){
	imlib_context_set_display(display->display);
	imlib_context_set_visual(DefaultVisual(display->display,display->default_screen));
	imlib_context_set_colormap(DefaultColormap(display->display, display->default_screen));
}

/* Window ---------------------------------------------------------------- */
void xfix_init_window_with_depth(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height, int depth)
{
	window->depth = depth;
	XSetWindowAttributes attr;
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	XVisualInfo vinfo = xfix_visualinfo_from_depth(display, depth);
	attr.colormap = XCreateColormap(display->display, display->root, vinfo.visual, AllocNone);
	window->visual = vinfo.visual;
	window->colormap = attr.colormap;
	window->window = XCreateWindow(display->display, display->root,
								   x, y, width, height, 0,
								   vinfo.depth,
								   InputOutput,
								   vinfo.visual,
								   CWColormap | CWBorderPixel | CWBackPixel,
								   &attr);
}

void xfix_init_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height)
{
	xfix_init_window_with_depth(display, window, x, y, width, height, 24);
}

/**
 * For further reference:
 * rgba windows need the 3 listed attributes CWColormap, CWBackPixel and CWBorderPixel 
 * 
 **/
void xfix_init_rgba_window(xfix_display *display, xfix_window *window, int x, int y, unsigned int width, unsigned int height)
{
	xfix_init_window_with_depth(display, window, x, y, width, height, 32);
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
	return XRaiseWindow(display->display, window->window);
}
int xfix_focus(xfix_display *display, xfix_window *window)
{
	return XGetInputFocus(display->display, &window->window, RevertToParent);
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

int xfix_set_background_from_imlib()
{
	imlib_render_image_on_drawable(0, 0);
}

int xfix_set_background_from_imlib_at(int x, int y)
{
	imlib_render_image_on_drawable(x, y);
}

int xfix_imlib_set_window(xfix_window *window){
	imlib_context_set_drawable(window->window);
}

int xfix_set_undecorated(xfix_display *display, xfix_window *window, Bool undecorated)
{
	XSetWindowAttributes new_attributes;
	new_attributes.override_redirect = undecorated;
	XWindowAttributes old_attributes = xfix_attributes(display, window);
	XWithdrawWindow(display->display, window->window, display->default_screen);
	int result = XChangeWindowAttributes(display->display, window->window, CWOverrideRedirect, &new_attributes);
	if (old_attributes.map_state != IsUnmapped)
	{
		//Show the window again after applying the changes
		XMapWindow(display->display, window->window);
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

int xfix_set_class(xfix_display *display, xfix_window *window, xfix_window_class *window_class)
{
	return XSetClassHint(display->display, window->window, window_class);
}
void xfix_set_title(xfix_display *display, xfix_window *window, char *title)
{
	//static Atom string_encoding;
	//if (string_encoding == NULL)
	//	string_encoding = XInternAtom(display->display, "STRING", False);
	XTextProperty text_property;
	text_property.value = title;
	text_property.nitems = strlen(title) * sizeof(char);
	text_property.format = 8;
	text_property.encoding = XInternAtom(display->display, "STRING", True);
	XSetWMName(display->display, window->window, &text_property);
}

//Getter ----------------------------------------------------------------

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
xfix_window_class xfix_get_class(xfix_display *display, xfix_window *window)
{
	xfix_window_class window_class;
	XGetClassHint(display->display, window->window, &window_class);
	return window_class;
}
char *xfix_get_title(xfix_display *display, xfix_window *window)
{
	XTextProperty title;
	XGetWMName(display->display, window->window, &title);
	return title.value;
}