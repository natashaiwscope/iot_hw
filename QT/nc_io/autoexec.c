#include <stdio.h>
int main(void)
{
    int i, j;
    char buffer[100];

    change_tab(1);

    statusbar("Starting Execution");

    GUI_BANNER("TEST VAL=100");

    GUI_LCD_DEC(1000);
    GUI_LCD_DEC(1000);


    GUI_CLEAR_BOX();
    GUI_BEEP();

    for (i = 0; i < 50; i++)
    {
        j = i % 10;
        osDelay(20);
        ledOff(j);
        osDelay(20);
        ledOn(j);
        printf("balle balle test");
    }


    for (i = 0; i < 100; i++)
    {
        GUI_LCD_DEC(i);
        GUI_LCD_HEX(i + 1);
        GUI_LCD_BIN(i + 2);

        printf("balle balle %d", i);
        GUI_APPEND_BOX();


        osDelay(120);
        // if lines more then 20 then clear text box
        if (!(i % 20)) GUI_CLEAR_BOX();
    }

    change_tab(0);

    return 0;
}
