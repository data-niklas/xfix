#include "../../src/xfix.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]){
    xfix_display display;
    xfix_init_display(&display);

    xfix_window window;
    //xfix_init_window(&display, &window, 0, 0, 1000, 800);
    xfix_init_window_from_settings(&display, &window, (xfix_window_settings){ .width=300, .height=700 });
    xfix_show(&display, &window);
    xfix_update(&display);

    xfix_window_settings set = (xfix_window_settings){ .height=700 };
    printf("%d %d %d %d\n", set.x, set.y, set.width, set.height);

    //xfix_set_window_size(&display, &window, );

    usleep(10000000);
    xfix_free_window(&display, &window);
    xfix_free_display(&display);
    return 0;
}