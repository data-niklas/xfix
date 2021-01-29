#include "../../src/xfix.h"
//#include "../../src/xfix_tray.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <Imlib2.h>

void msleep(unsigned int millis)
{
    usleep(millis * 1000);
}
/*
int main(int argc, char *argv[])
{

    xfix_display display;
    xfix_init_display(&display, NULL);

    xfix_window window;
    xfix_init_rgba_window(&display, &window, 1920, 70, 1, 50);
    xfix_window_class class;
    class.res_class = "xfix";
    class.res_name = "XFix window test";
    xfix_set_class(&display, &window, &class);
    xfix_set_title(&display, &window, "XFIX SAMPLE WINDOW");
    puts(xfix_get_title(&display, &window));
    //xfix_init_window_from_settings(&display, &window, (xfix_window_settings){ .width=300, .height=700 });
    xfix_set_rgba_background(&display, &window, 0xff000022);
    xfix_set_undecorated(&display, &window, True);
    xfix_show(&display, &window);
    //xfix_focus(&display, &window);
    xfix_update(&display);
    msleep(1000);
    xfix_set_rgba_background(&display, &window, 0xffff00ff);
    xfix_update(&display);
    for (int i = 0; i < 256; i++)
    {
        xfix_set_rgba_background(&display, &window, (rand() * 2) << 8 | 0x22);
        //printf("%ld\n", xfix_get_background(&display, &window));
        xfix_set_width(&display, &window, xfix_get_width(&display, &window)+15);
        if (i == 100){
            xfix_set_undecorated(&display, &window, False);
        }
        else if (i == 200){
            xfix_set_undecorated(&display, &window, True);
            xfix_set_bounds(&display, &window, 1920, 0, 500,1000);
        }
        xfix_update(&display);
        msleep(18);
    }
    XSelectInput(display.display, window.window, ExposureMask);
    xfix_set_size(&display, &window, 3840, 2160);
    XEvent events;
     while (1)
    {
        XNextEvent(display.display, &events);
        switch (events.type)
        {
        case Expose:
            xfix_imlib_set_display(&display);
            xfix_imlib_set_window(&window);
            Imlib_Image image = imlib_load_image("/home/niklas/Pictures/memes/rangfolge.JPG");
            imlib_context_set_image(image);
            Pixmap pix = XCreatePixmap(display.display, window.window, imlib_image_get_width(), imlib_image_get_height(), 32);
            imlib_context_set_drawable(pix);
            xfix_set_background_from_imlib();
            XSetWindowBackgroundPixmap(display.display, window.window, pix);
            imlib_free_image();
            goto end;
        break;
        }
    }
    end:
    msleep(2000);
    xfix_free_window(&display, &window);
    xfix_free_display(&display);
    return 0;
}*/

int main(int argc, char *argv[])
{
    xfix_display display;
    xfix_init_display(&display, NULL);

    xfix_window window;
    xfix_init_rgba_window(&display, &window, 1920, 0, 3840, 2160);
    xfix_set_undecorated(&display, &window, False);
    xfix_set_rgba_background(&display, &window, 0xFF000066);
    xfix_imlib_set_display(&display);
    xfix_imlib_set_window(&window);


    xfix_show(&display, &window);
    xfix_update(&display);
            Imlib_Image icon = imlib_load_image("/home/niklas/Downloads/pixil-frame-0.png");
    imlib_context_set_image(icon);
    xfix_add_icon_from_imlib(&display, &window);
    imlib_free_image();
    xfix_update(&display);

    Imlib_Image image = imlib_load_image("/home/niklas/Pictures/memes/rangfolge.JPG");
    imlib_context_set_image(image);
    //Needed for rgba windows
    imlib_image_set_has_alpha(1);
    imlib_render_image_on_drawable_at_size(1500, 1000, 600, 450);
    imlib_free_image();
    msleep(5000);
    xfix_free_window(&display, &window);
    xfix_free_display(&display);
    return 0;
}