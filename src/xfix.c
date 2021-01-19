#include "Xlib.h"
#include "../lib/bstrlib.h"

#include "xfix.h"


void xfix_flush(xfix_display * display){
    XFlush(display->display);
}

void xfix_fill_display(xfix_display * display){
    display->root = XRootWindow(display->display, XDefaultScreen(display->display));
}
void xfix_init_display(xfix_display * display){
    display->display = XOpenDisplay(NULL);
    xfix_fill_display(display);
}
void xfix_init_display_by_name(xfix_display * display, bstring name){
    display->display = XOpenDisplay(name->data);
    xfix_fill_display(display);
}
void xfix_free_display(xfix_display * display){
    XCloseDisplay(display->display);
}