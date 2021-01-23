#include "../../src/xfix.h"
//#include "../../src/xfix_tray.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void msleep(unsigned int millis)
{
    usleep(millis * 1000);
}

int main(int argc, char *argv[])
{

    xfix_display display;
    xfix_init_display(&display);

    xfix_window window;
    xfix_init_rgba_window(&display, &window, 1920, 70, 1, 50);
    //xfix_init_window_from_settings(&display, &window, (xfix_window_settings){ .width=300, .height=700 });
    xfix_set_rgba_background(&display, &window, 0xff000022);
    xfix_set_undecorated(&display, &window, True);
    xfix_show(&display, &window);
    xfix_update(&display);
    msleep(1000);
    xfix_set_rgba_background(&display, &window, 0xffff00ff);
    xfix_update(&display);
    for (int i = 0; i < 256; i++)
    {
        xfix_set_rgba_background(&display, &window, (rand() * 2) << 8 | 0xaa);
        //printf("%ld\n", xfix_get_background(&display, &window));
        xfix_set_width(&display, &window, xfix_get_width(&display, &window)+15);
        if (i == 100){
            xfix_set_undecorated(&display, &window, False);
        }
        else if (i == 200){
            xfix_set_undecorated(&display, &window, True);

        }
        xfix_update(&display);
        msleep(18);
    }

    //xfix_set_window_size(&display, &window, );

    msleep(1000);
    xfix_free_window(&display, &window);
    xfix_free_display(&display);
    return 0;
}