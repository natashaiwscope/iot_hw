#include <stdio.h>
int main(void)
{
    int i;
    int j;

    statusbar("Vijauad");

    GUI_BANNER("TEST VAL=100");
    GUI_LCD_HEX(1000);
    change_tab(2);
    GUI_LCD_DEC(1000);

    j = 2;
    for (;;)
        for (i = 1; i <= 6; i++)
        {
            GUI_LED(1, j);
            j++;
            osDelay(20);
            GUI_LED(2, j);
            j++;
            osDelay(20);
            GUI_LED(3, j);
            j++;
            osDelay(20);
            GUI_LED(4, j);
            j++;
            osDelay(20);
            GUI_LED(5, j);
            j++;
            osDelay(20);
            GUI_LED(6, j);
            j++;
            osDelay(20);
            if (j > 17) j = 2;
        }


    return 0;
}
