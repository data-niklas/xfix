#include "../../src/xfix.h"
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
    xfix_init_rgba_window(&display, &window, 30, 30, 1000, 800);
    //xfix_init_window_from_settings(&display, &window, (xfix_window_settings){ .width=300, .height=700 });
    xfix_set_rgba_background(&display, &window, 0xff000022);
    xfix_show(&display, &window);
    xfix_update(&display);
    msleep(1000);
    xfix_set_rgba_background(&display, &window, 0xffff00ff);
    xfix_update(&display);
    for (int i = 0; i < 256; i++)
    {
        xfix_set_rgba_background(&display, &window, (rand() * 2) << 8 | 0x33);
        xfix_update(&display);
        msleep(18);
    }

    //xfix_set_window_size(&display, &window, );

    msleep(5000);
    xfix_free_window(&display, &window);
    xfix_free_display(&display);
    return 0;
}