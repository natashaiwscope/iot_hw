#include <stdio.h>
int main(void)
{
    int i;
    change_tab(0);
    statusbar("Vijauad");

    i = 2000;

    for (i = 0; i < 4095; i += 100)
    {
        signalA(0, i, 1, 2);
        signalB(0, 4095 - i, 1, 1);
        start_signal(110, 110);
        osDelay(40);
        if (i > 3900) i = 0;
    }


    return 0;
}
