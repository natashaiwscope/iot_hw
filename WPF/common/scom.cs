using System;
using System.Text;
using System.Windows;
using System.Runtime.InteropServices;
using System.Threading;
using System.ComponentModel;
using System.Windows;


namespace slib
{
    public static class scom
    {
        [StructLayout(LayoutKind.Sequential)]
        struct CS_DataStruct
        {
            public IntPtr data;
            public int len;
        };

        const Int32 WM_LED = (0x400 + 1);
        const Int32 WM_MSG = (0x400 + 2);
        const Int32 WM_GRAPH = (0x400 + 3);
        const Int32 WM_WIN_DIS_FRAME1 = (0x400 + 4);
        const Int32 WM_WIN_DIS_FRAME2 = (0x400 + 5);
        const Int32 WM_WIN_DIS_FRAME3 = (0x400 + 6);
        const Int32 WM_WIN_DIS_FRAME4 = (0x400 + 7);
        const Int32 WM_WIN_DIS_FRAME5 = (0x400 + 8);
        const Int32 WM_FEED_R_X_Y = (0x400 + 9);
        const Int32 WM_STATUS_MSG = (0x400 + 10);
        const Int32 WM_SCROLL_MSG = (0x400 + 11);
        const Int32 WM_TIMER_MSG = (0x400 + 12);
        const Int32 WM_BOXT = (0x400 + 13);
        const Int32 WM_BOXR = (0x400 + 14);
        const Int32 WM_BOXF = (0x400 + 15);
        const Int32 WM_BANNER = (0x400 + 16);
        const Int32 WM_CLEAR_MSG = (0x400 + 17);
        const Int32 WM_APPEND_MSG = (0x400 + 18);
        const Int32 WM_CAL_MSG = (0x400 + 19);
        const Int32 WM_CAL_LED = (0x400 + 20);
        const Int32 WM_INFO_MSG = (0x400 + 21);
        const Int32 WM_WIN_DIS_IMG1 = (0x400 + 22);
        const Int32 WM_WIN_DIS_IMG2 = (0x400 + 23);
        const Int32 WM_WIN_DIS_IMG3 = (0x400 + 24);
        const Int32 WM_WIN_DIS_IMG4 = (0x400 + 25);
        const Int32 WM_WIN_DIS_IMG5 = (0x400 + 26);
        const Int32 WM_WIN_DIS_IMG6 = (0x400 + 27);
        const Int32 WM_CLEAR_LOG = (0x400 + 28);
        const Int32 WM_LOG = (0x400 + 29);
        const Int32 WM_WIN_PLOT_C1 = (0x400 + 30);
        const Int32 WM_WIN_PLOT_C2 = (0x400 + 31);
        const Int32 WM_WIN_PLOT_C3 = (0x400 + 32);
        const Int32 WM_WIN_APC_BOX = (0x400 + 33);
        const Int32 WM_TIMER_STOP_MSG = (0x400 + 34);
        const Int32 WM_WIN_SAVE_IMG = (0x400 + 35);
        const Int32 WM_HW_CONNECTED = (0x400 + 36);
        const Int32 WM_HW_NOTCONNECTED = (0x400 + 37);
        const Int32 WM_RECP_SCO = (0x400 + 38);
        const Int32 WM_RECP_SIG = (0x400 + 39);
        const Int32 WM_APP_LOADER = (0x400 + 42);
        const Int32 WM_APP_COMOK = (0x400 + 43);
        const Int32 WM_APP_FILEOK = (0x400 + 44);
        const Int32 WM_APP_PROGRESS = (0x400 + 45);
        const Int32 WM_APP_DOWNOK = (0x400 + 46);
        const Int32 WM_APP_PYTHON = (0x400 + 47);
        const Int32 WM_CRV_T1 = (0x400 + 48);
        const Int32 WM_CRV_T2 = (0x400 + 49);
        const Int32 WM_CRV_T3 = (0x400 + 50);
        const Int32 WM_CRV_T4 = (0x400 + 51);
        const Int32 WM_CRV_T5 = (0x400 + 52);
        const Int32 WM_CRV_T6 = (0x400 + 53);
        const Int32 WM_SET_IMG_TAB = (0x400 + 55);
        const Int32 WM_CIMG_DISPLAY = (0x400 + 56);
        const Int32 WM_GIMG_DISPLAY = (0x400 + 57);
        const Int32 WMTM_LOG_MSG = (0x400 + 58);
        const Int32 WMTM_STATUS_MSG = (0x400 + 59);
        const Int32 WMTM_STATUS_GRAPH = (0x400 + 60);
        const Int32 WMTM_BOX_MSG = (0x400 + 61);
        const Int32 WMTM_LED_MSG = (0x400 + 62);
        const Int32 WMTM_TAB_MSG = (0x400 + 63);
        const Int32 WMTM_ARRIVED_MSG = (0x400 + 64);
        const Int32 WM_SIGNAL_TAB = (0x400 + 65);
        const Int32 WM_SCOPE_TAB = (0x400 + 66);
        const Int32 WM_PLUGIN_TAB = (0x400 + 67);
        const Int32 WM_SCRIPT_TAB = (0x400 + 68);
        const Int32 WM_SCOPE_PROGR = (0x400 + 69);
        const Int32 WM_SCOPE_READY = (0x400 + 70);
        const Int32 WM_SCOPE_PARTREADY = (0x400 + 71);
        const Int32 WM_SCOPE_START = (0x400 + 72);
        const Int32 WM_BOOT_START = (0x400 + 73);
        const Int32 WM_CSCRIPT_SETTINGS = (0x400 + 74);
        const Int32 WM_CONNECTED = (0x400 + 75);
        const Int32 WM_CHANGE_TAB = (0x400 + 76);
        const Int32 WM_RESP_GOT = (0x400 + 77);
        const Int32 WM_WIEGAND = (0x400 + 78);
        const Int32 WM_DISCONNECT = (0x400 + 79);
        const Int32 WM_CONSOLE = (0x400 + 80);
        const Int32 WM_CONSOLE1 = (0x400 + 81);
        const Int32 WM_CONSOLE2 = (0x400 + 82);
        const Int32 WM_CONSOLE3 = (0x400 + 83);
        const Int32 WM_USART = (0x400 + 84);
        const Int32 WM_REGISTERED = (0x400 + 90);


        [DllImport("libscom.so", EntryPoint = "iwInitSerial", CallingConvention = CallingConvention.StdCall)]
        unsafe public static extern void iwInitSerial();

        [DllImport("kernel32.so", EntryPoint = "LoadLibrary")]
        static extern int LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        [DllImport("kernel32.so", EntryPoint = "GetProcAddress")]
        static extern IntPtr GetProcAddress(int hModule, [MarshalAs(UnmanagedType.LPStr)] string lpProcName);

        [DllImport("kernel32.so", EntryPoint = "FreeLibrary")]
        static extern bool FreeLibrary(int hModule);

        [DllImport("libscom.so", EntryPoint = "RegisterWin", CallingConvention = CallingConvention.StdCall)]
        public static extern int RegisterWin(IntPtr windowHandle);

        [DllImport("libscom.so", EntryPoint = "uart_read", CallingConvention = CallingConvention.StdCall)]
        public static extern int uart_read(short uNum, [In, Out] IntPtr arr);

        [DllImport("libscom.so", EntryPoint = "uart_write", CallingConvention = CallingConvention.StdCall)]
        public static extern int uart_write(short uNum, [In, Out] IntPtr arr,short len);

        public static int wpf_uart_read(short uNum, [In, Out] byte[] buffInOut)
        {
            byte[] array2 = new byte[1024];
            int size = array2.Length;
            byte kcbyte = 0;
            IntPtr buffer = Marshal.AllocCoTaskMem(Marshal.SizeOf(kcbyte) * array2.Length);
            //Marshal.Copy(array2, 0, buffer, array2.Length);
            Int32 result = uart_read(uNum, buffer);
            byte[] arrayRes = new byte[result];
            Marshal.Copy(buffer, arrayRes, 0, result);
            Marshal.FreeCoTaskMem(buffer);
            return result;
        }

        public static int wpf_uart_write(short uNum, [In, Out] byte[] buffInOut, short len)
        {
            byte[] array2 = new byte[len];
            int size = array2.Length;
            byte kcbyte = 0;
            IntPtr buffer = Marshal.AllocCoTaskMem(Marshal.SizeOf(kcbyte) * array2.Length);
            Marshal.Copy(buffInOut, 0, buffer, array2.Length);
            Int32 result = uart_write(uNum, buffer, len);
            Marshal.FreeCoTaskMem(buffer);
            return result;
        }

        public static void RegisterWPF(IntPtr winHandle)
        {
            //hWnd=winHandle;
            RegisterWin(winHandle);
        }

         
        static IntPtr hWnd;
    }
}

