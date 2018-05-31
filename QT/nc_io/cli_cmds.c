#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cli_cmds.h"
#include "cli.h"

static unsigned char echoToggle = 0x00;

unsigned char EchoFlag()
{
    return echoToggle;
}

static void prvSaveTraceFile( void );

static BaseType_t prvEchoToggle( char *pcWriteBuffer, size_t xWriteBufferLen, const char *extCmdString );
static BaseType_t prvResetCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *extCmdString );
static BaseType_t prvATCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *extCmdString );

static const CLI_Command_Definition_t xToggleEcho =
{
    "et", /* The command string to type. */
    "et: Toggle Echo\r\n",
    prvATCommand, /* The function to run. */
    0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xATCmd =
{
    "at", /* The command string to type. */
    "at: AT Commands\r\n",
    prvATCommand, /* The function to run. */
    0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xReset =
{
    "reset",
    "trace [start | stop]:Starts or stops a trace \r\n",
    prvResetCommand, /* The function to run. */
    1 /* One parameter is expected.  Valid values are "start" and "stop". */
};

void vRegisterCLICommands( void )
{
    CLIRegCmd( &xToggleEcho);
    CLIRegCmd( &xReset );
    CLIRegCmd( &xATCmd );
}

static BaseType_t prvATCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *extCmdString )
{
    char *ch;
    BaseType_t len;
    BaseType_t num;

    printf(extCmdString);
    if (prvGetNumberOfParameters(extCmdString))
    {
        ch = CLIGetPar(extCmdString, 1, &len);
    }
    //num = atoi(ch);

    sprintf(pcWriteBuffer, "Number of par=%d\r\n", prvGetNumberOfParameters(extCmdString));
    //printf("at command %d\r\n",prvGetNumberOfParameters(extCmdString));

}

static BaseType_t prvResetCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *extCmdString )
{
    char *ch;
    BaseType_t len;
    BaseType_t num;
    BaseType_t num_par;

    num_par = prvGetNumberOfParameters(extCmdString);
    if (num_par > 0)
    {
        ch = CLIGetPar(extCmdString, 1, &len);
        num = atoi(ch);
        printf("num=%d\r\n", num);
        if (num == 0)
        {
        }
    }
    else
    {
    }
    sprintf(pcWriteBuffer, "Number of par=%d\r\n", prvGetNumberOfParameters(extCmdString));
}

static BaseType_t prvStartStopTraceCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *extCmdString )
{

    return pdFALSE;
}

static void prvSaveTraceFile( void )
{
}

static BaseType_t prvEchoToggle( char *pcWriteBuffer, size_t xWriteBufferLen, const char *extCmdString )
{
    BaseType_t xReturn;

    ( void ) extCmdString;
    echoToggle ^= 0x01;

    return xReturn;
}
