#ifndef __SHARED_STRUCT_H
#define __SHARED_STRUCT_H

#include "options.h"
#include <stdint.h>

typedef unsigned short  IWenum;

typedef enum
{
    user_CSCRIPT = 0,
    user_fsCoAP,
    user_Firmware
} FsUser_t;

typedef enum
{
    TERM_UART2 = 0,
    TERM_UART3 = 1,
    TERM_RS485 = 2,
    TERM_FILE = 3,
    TERM_COAP_SCRATCH = 4,
} DbgPrnOutput_t;

typedef enum
{
    IW_PLAIN_TEXT = 0,
    IW_HEX_DUMP,
    IW_HEX_DUMP_WITH_0x,
    IW_LED,
    IW_STATUS,
    IW_BANNER,
    IW_GRAPH,
} enInfoType;

typedef enum
{
    IW_WHITE = 0,
    IW_BLACK,
    IW_RED,
    IW_DARKRED,
    IW_GREEN,
    IW_DARKGREEN,
    IW_BLUE,
    IW_DARKBLUE,
    IW_CYAN,
    IW_DARKCYAN,
    IW_MAGENTA,
    IW_DARKMAGENTA,
    IW_YELLOW,
    IW_DARKYELLOW,
    IW_GRAY,
    IW_DARKGRAY,
    IW_LIGHTGRAY,
    IW_TRANSPARENT
} enTextColor;

typedef enum
{
    IW_SMALL = 0,
    IW_NORMAL,
    IW_BOLD,
    IW_LARGE
} enTextFont;

typedef enum
{
    IW_CLEAR_SCREEN = 0,
    IW_APPEND_TEXT,
} enWinCmd;

typedef enum
{
    IW_SCRATCH1 = 0,
    IW_SCRATCH2,
    IW_SCRATCH3,
    IW_RS485,
    IW_UART2,
    IW_UART3,
    IW_UART23,
} enTargetWin;

typedef enum
{
    IW_GRAPH1,
    IW_GRAPH2,
    IW_GRAPH3,
    IW_LED1,
    IW_LED2,
    IW_LED3
} enTargetDev;

#define MAX_TFR_SIZE 1024
#define MAX_ADC_DAC_DATA_IN_PKT  250

/* Type of resources */
#define REM_RS485    0x0001
#define REM_UART2    0x0002
#define REM_UART3    0x0003
#define REM_I2C1     0x0004
#define REM_ADC1     0x0005
#define REM_ADC2     0x0006
#define REM_DAC1     0x0007
#define REM_DAC2     0x0008
#define REM_FILE     0x0009
#define REM_LED      0x000A
#define REM_BEEPER   0x000B
#define REM_AUDIO    0x000C
#define REM_C_AUEX   0x000D
#define REM_INC_AUEX 0x000E

/* Type of operation */
#define READ_DATA    0x0001
#define WRITE_DATA   0x0002
#define OPEN_FILE    0x0003
#define READ_FILE    0x0004
#define WRITE_FILE   0x0005

/* Type of operations */


typedef struct
{
    IWenum            DevId;
    IWenum            MsgLen;
} MsgTag;

typedef struct
{
    IWenum            FmtData;
    IWenum            Off_l;
    IWenum            Off_h;
    IWenum            dataLen;
    unsigned char     *cdata;
} FileData;

typedef struct
{
    IWenum            MemType;
    IWenum            Off_l;
    IWenum            Off_h;
    IWenum            range;
} MemData;

typedef struct
{
    const char *version;
    const char *devName;
    const char *date;
    const char *time;
    const char *uart1;
    const char *uart2;
    const char *rs485_1;
    const char *i2c_1;
} DeviceCapability;

#define IW_FILE_SUCCESS 0
#define IW_FILE_ERROR   1

typedef enum
{
    FL_RFILE_NAME = 1,
    FL_WFILE_NAME = 2,
    FL_RCLOSE_FILE = 3,
    FL_WCLOSE_FILE = 4,

    FL_SET_OFFSET = 5,
    FL_GET_OFFSET = 6,

    FL_READ_LINE = 7,
    FL_WRITE_LINE = 8,
    FL_READ_BIN = 9,
    FL_WRITE_BIN = 10,
    FL_APPEND_DATA = 11,

    FL_FILE_EXISTS = 12,
    FL_FILE_SIZE = 13,
    FL_RELOAD_INI = 14,
    FL_RELOAD_CSCRIPT = 15,
    FL_CLOSE_FILE = 16,

    FL_LAST_CMD
} FILE_op;

typedef struct
{
    uint8_t WeekDay;
    uint8_t Month;
    uint8_t Date;
    uint8_t Year;
} CRTC_DateTypeDef;

typedef struct
{
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
    uint8_t TimeFormat;
    uint32_t SubSeconds;
    uint32_t SecondFraction;
    uint32_t DayLightSaving;
    uint32_t StoreOperation;
} CRTC_TimeTypeDef;

#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    CRTC_TimeTypeDef  cTime;
    CRTC_DateTypeDef  cDate;
#ifndef __GNUC__
} IW_CDATE_CTIME;
#else
} IW_CDATE_CTIME __attribute__ ((aligned(4), packed));
#endif

#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    unsigned short humid;
    unsigned short temp;
#ifndef __GNUC__
} IW_TEMP_HUMIDITY;
#else
} IW_TEMP_HUMIDITY __attribute__ ((aligned(4), packed));
#endif

typedef enum
{
    TEXT_SCRATCH = 0,
    BIN_SCRATCH
} enumScratchMode;

typedef struct
{
    unsigned char   ringTx;
    unsigned char buff[SCRATCH_BUFFER_LEN];
    enumScratchMode mode;
} ScratchBuffer;


#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    unsigned char  uart_num;
    unsigned char  uart_stop;
    unsigned char  uart_databits;
    unsigned char  uart_parity;
    unsigned long  uart_baud;
    unsigned long  uartDataLen;
    char           line_buffer[MAX_TFR_SIZE + 4];
#ifndef __GNUC__
} iUARTStruct ;
#else
} iUARTStruct __attribute__ ((aligned(4), packed));
#endif


#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    char           filename[12];
    uint16_t       fileTrfSize;
    uint16_t       tokenID;
    uint32_t       fileLength;
    uint32_t       fileOffset;
    char           line_buffer[MAX_TFR_SIZE + 4];
#ifndef __GNUC__
} iNFileExchangeStruct ;
#else
} iNFileExchangeStruct __attribute__ ((aligned(4), packed));
#endif

#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    unsigned short index;
    unsigned short offset;
    unsigned short ncount;
    unsigned short crc;
    unsigned short ch1[MAX_ADC_DAC_DATA_IN_PKT];
    unsigned short ch2[MAX_ADC_DAC_DATA_IN_PKT];
#ifndef __GNUC__
} io_ADC_DAC_Exchng;
#else
} io_ADC_DAC_Exchng __attribute__ ((aligned(4), packed));
#endif

#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    uint16_t         uart1offset;
    uint16_t         uart1len;
    uint16_t         uart2offset;
    uint16_t         uart2len;
    char             uart_data[MAX_TFR_SIZE];
#ifndef __GNUC__
} ioDualUARTLogStruct ;
#else
} ioDualUARTLogStruct __attribute__ ((aligned(4), packed));
#endif

#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    uint16_t         uartoffset;
    uint16_t         uartlen;
    uint16_t         _uart0offset;
    uint16_t         _uart0len;
    char             uart_data[MAX_TFR_SIZE];
#ifndef __GNUC__
} ioMonoUARTLogStruct ;
#else
} ioMonoUARTLogStruct __attribute__ ((aligned(4), packed));
#endif



#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    uint16_t         len;
    uint16_t         Transferred;
    uint8_t          prev_state;
    uint8_t          new_state;
    uint8_t          error;
    uint8_t          file_error;
    unsigned char    a[16];
    char             line_buffer[MAX_TFR_SIZE];
#ifndef __GNUC__
} ioFileExchangeStruct ;
#else
} ioFileExchangeStruct __attribute__ ((aligned(4), packed));
#endif

typedef struct
{
#if 1
    uint8_t tWin;
    uint8_t pInfo;
    uint8_t colTxt;
    uint8_t uartNum;
    uint16_t cmdTxt;
    uint16_t lenTxt;
#else
    enTargetWin tWin;
    enInfoType  pInfo;
    enTextColor colTxt;
    enTextFont  fontTxt;
    enWinCmd    cmdTxt;
    uint32_t    lenTxt;
#endif
} GUI_Dest;

#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef struct
{
    GUI_Dest guiDest;
    union
    {
        unsigned char  hex_data_buffer[MAX_TFR_SIZE];
        char           char_data_buffer[MAX_TFR_SIZE];
    };

#ifndef __GNUC__
} ioTxtGrpthExchStrct;
#else
} ioTxtGrpthExchStrct __attribute__ ((aligned(4), packed));
#endif

typedef union
{
    struct
    {
        unsigned short mid;
        unsigned short len;
        unsigned short csum;
        unsigned short dest;
        unsigned char  pkt_buff[1024];
    };
    struct
    {
        unsigned char buff[1032];
    };
} tcpPkt;


#ifndef __GNUC__
#pragma data_alignment = 4
#endif
typedef union
{
    iNFileExchangeStruct in_glue;
    ioFileExchangeStruct io_glue;
    iUARTStruct          io_uart;
    ioTxtGrpthExchStrct  iGuiUpdate;
    io_ADC_DAC_Exchng    iADC_DAC;
    IW_CDATE_CTIME       iDateTime;
    IW_TEMP_HUMIDITY     iTmpHum;
#ifndef __GNUC__
} ioExchangeUnion;
#else
} ioExchangeUnion __attribute__ ((aligned(4), packed));
#endif

typedef enum
{
    fs_SERVER_READY = 0,
    fsREAD_BIN___File = 1,
    fsWRITE_BIN__File = 2,
    fsCLOSE______File = 3
} FsMode_t;

#endif
