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

        public const  Int32 WM_LED = (0x400 + 1);
        public const  Int32 WM_MSG = (0x400 + 2);
        public const  Int32 WM_GRAPH = (0x400 + 3);
        public const  Int32 WM_WIN_DIS_FRAME1 = (0x400 + 4);
        public const  Int32 WM_WIN_DIS_FRAME2 = (0x400 + 5);
        public const  Int32 WM_WIN_DIS_FRAME3 = (0x400 + 6);
        public const  Int32 WM_WIN_DIS_FRAME4 = (0x400 + 7);
        public const  Int32 WM_WIN_DIS_FRAME5 = (0x400 + 8);
        public const  Int32 WM_FEED_R_X_Y = (0x400 + 9);
        public const  Int32 WM_STATUS_MSG = (0x400 + 10);
        public const  Int32 WM_SCROLL_MSG = (0x400 + 11);
        public const  Int32 WM_TIMER_MSG = (0x400 + 12);
        public const  Int32 WM_BOXT = (0x400 + 13);
        public const  Int32 WM_BOXR = (0x400 + 14);
        public const  Int32 WM_BOXF = (0x400 + 15);
        public const  Int32 WM_BANNER = (0x400 + 16);
        public const  Int32 WM_CLEAR_MSG = (0x400 + 17);
        public const  Int32 WM_APPEND_MSG = (0x400 + 18);
        public const  Int32 WM_CAL_MSG = (0x400 + 19);
        public const  Int32 WM_CAL_LED = (0x400 + 20);
        public const  Int32 WM_INFO_MSG = (0x400 + 21);
        public const  Int32 WM_WIN_DIS_IMG1 = (0x400 + 22);
        public const  Int32 WM_WIN_DIS_IMG2 = (0x400 + 23);
        public const  Int32 WM_WIN_DIS_IMG3 = (0x400 + 24);
        public const  Int32 WM_WIN_DIS_IMG4 = (0x400 + 25);
        public const  Int32 WM_WIN_DIS_IMG5 = (0x400 + 26);
        public const  Int32 WM_WIN_DIS_IMG6 = (0x400 + 27);
        public const  Int32 WM_CLEAR_LOG = (0x400 + 28);
        public const  Int32 WM_LOG = (0x400 + 29);
        public const  Int32 WM_WIN_PLOT_C1 = (0x400 + 30);
        public const  Int32 WM_WIN_PLOT_C2 = (0x400 + 31);
        public const  Int32 WM_WIN_PLOT_C3 = (0x400 + 32);
        public const  Int32 WM_WIN_APC_BOX = (0x400 + 33);
        public const  Int32 WM_TIMER_STOP_MSG = (0x400 + 34);
        public const  Int32 WM_WIN_SAVE_IMG = (0x400 + 35);
        public const  Int32 WM_HW_CONNECTED = (0x400 + 36);
        public const  Int32 WM_HW_NOTCONNECTED = (0x400 + 37);
        public const  Int32 WM_RECP_SCO = (0x400 + 38);
        public const  Int32 WM_RECP_SIG = (0x400 + 39);
        public const  Int32 WM_APP_LOADER = (0x400 + 42);
        public const  Int32 WM_APP_COMOK = (0x400 + 43);
        public const  Int32 WM_APP_FILEOK = (0x400 + 44);
        public const  Int32 WM_APP_PROGRESS = (0x400 + 45);
        public const  Int32 WM_APP_DOWNOK = (0x400 + 46);
        public const  Int32 WM_APP_PYTHON = (0x400 + 47);
        public const  Int32 WM_CRV_T1 = (0x400 + 48);
        public const  Int32 WM_CRV_T2 = (0x400 + 49);
        public const  Int32 WM_CRV_T3 = (0x400 + 50);
        public const  Int32 WM_CRV_T4 = (0x400 + 51);
        public const  Int32 WM_CRV_T5 = (0x400 + 52);
        public const  Int32 WM_CRV_T6 = (0x400 + 53);
        public const  Int32 WM_SET_IMG_TAB = (0x400 + 55);
        public const  Int32 WM_CIMG_DISPLAY = (0x400 + 56);
        public const  Int32 WM_GIMG_DISPLAY = (0x400 + 57);
        public const  Int32 WMTM_LOG_MSG = (0x400 + 58);
        public const  Int32 WMTM_STATUS_MSG = (0x400 + 59);
        public const  Int32 WMTM_STATUS_GRAPH = (0x400 + 60);
        public const  Int32 WMTM_BOX_MSG = (0x400 + 61);
        public const  Int32 WMTM_LED_MSG = (0x400 + 62);
        public const  Int32 WMTM_TAB_MSG = (0x400 + 63);
        public const  Int32 WMTM_ARRIVED_MSG = (0x400 + 64);
        public const  Int32 WM_SIGNAL_TAB = (0x400 + 65);
        public const  Int32 WM_SCOPE_TAB = (0x400 + 66);
        public const  Int32 WM_PLUGIN_TAB = (0x400 + 67);
        public const  Int32 WM_SCRIPT_TAB = (0x400 + 68);
        public const  Int32 WM_SCOPE_PROGR = (0x400 + 69);
        public const  Int32 WM_SCOPE_READY = (0x400 + 70);
        public const  Int32 WM_SCOPE_PARTREADY = (0x400 + 71);
        public const  Int32 WM_SCOPE_START = (0x400 + 72);
        public const  Int32 WM_BOOT_START = (0x400 + 73);
        public const  Int32 WM_CSCRIPT_SETTINGS = (0x400 + 74);
        public const  Int32 WM_CONNECTED = (0x400 + 75);
        public const  Int32 WM_CHANGE_TAB = (0x400 + 76);
        public const  Int32 WM_RESP_GOT = (0x400 + 77);
        public const  Int32 WM_WIEGAND = (0x400 + 78);
        public const  Int32 WM_WIEGAND_RESULT= (0x400 + 79);
        public const  Int32 WM_DISCONNECT= (0x400 + 80);
        public const  Int32 WM_CONSOLE = (0x400 + 81);
        public const  Int32 WM_CONSOLE1 = (0x400 + 82);
        public const  Int32 WM_CONSOLE2 = (0x400 + 83);
        public const  Int32 WM_CONSOLE3 = (0x400 + 84);
        public const  Int32 WM_USART = (0x400 + 85);
        public const  Int32 WM_REBOOT = (0x400 + 86);
        public const  Int32 WM_REGISTERED = (0x400 + 90);

        [DllImport("udp.dll", EntryPoint = "heartbeat_1ms", CallingConvention = CallingConvention.StdCall)]
        unsafe public static extern void heartbeat_1ms();

        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary")]
        static extern int LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        [DllImport("kernel32.dll", EntryPoint = "GetProcAddress")]
        static extern IntPtr GetProcAddress(int hModule, [MarshalAs(UnmanagedType.LPStr)] string lpProcName);

        [DllImport("kernel32.dll", EntryPoint = "FreeLibrary")]
        static extern bool FreeLibrary(int hModule);

        [DllImport("udp.dll", EntryPoint = "RegisterWin", CallingConvention = CallingConvention.StdCall)]
        public static extern int RegisterWin(IntPtr windowHandle);

        [DllImport("udp.dll", EntryPoint = "uart_read", CallingConvention = CallingConvention.StdCall)]
        public static extern int uart_read(short uNum, [In, Out] IntPtr arr);

        [DllImport("udp.dll", EntryPoint = "uart_write", CallingConvention = CallingConvention.StdCall)]
        public static extern int uart_write(short uNum, [In, Out] IntPtr arr,short len);

        [DllImport("udp.dll", EntryPoint = "GetVersionInfo", CallingConvention = CallingConvention.StdCall)]
        public static extern Int32 GetVersionInfo(char[] versionInfo);

        [DllImport("udp.dll", EntryPoint = "usSelectDevIndex", CallingConvention = CallingConvention.StdCall)]
        public static extern Int32 usSelectDev(Int32 i);

        [DllImport("udp.dll", EntryPoint = "weigResult", CallingConvention = CallingConvention.StdCall)]
        public static extern Int32 weigResult(Int32 i);

        [DllImport("udp.dll", EntryPoint = "fn_scan_raw", CallingConvention = CallingConvention.StdCall)]
        public static extern Int32 fn_scan_raw(Int32 i);

        [DllImport("udp.dll", EntryPoint = "start_lib_interface_task", CallingConvention = CallingConvention.StdCall)]
        public static extern Int16 start_lib_interface_task();

        [DllImport("udp.dll", EntryPoint = "isWinRegistered", CallingConvention = CallingConvention.StdCall)]
        public static extern int isWinRegistered();

        [DllImport("udp.dll", EntryPoint = "RebootCurrentDevice", CallingConvention = CallingConvention.StdCall)]
        public static extern int RebootCurrentDevice();

        [DllImport("udp.dll", EntryPoint = "usGetConnectedDeviceIP", CallingConvention = CallingConvention.StdCall)]
        public static extern int usGetConnectedDeviceIP(short uNum, [In, Out] IntPtr arr);

        public static void OnNewStart()
        {
            start_lib_interface_task();
        }

        static public unsafe int usDeviceIPAddress(short uNum, out string ipAddress)
        {
            byte[] array2 = new byte[1024];
            int size = array2.Length;
            byte kcbyte = 0;

            IntPtr buffer = Marshal.AllocCoTaskMem(Marshal.SizeOf(kcbyte) * array2.Length);
            //Marshal.Copy( array2, 0, buffer, array2.Length );
            Int32 result = usGetConnectedDeviceIP(uNum, buffer);

            int isize = 20;
            byte[] arrayRes = new byte[isize];
            Marshal.Copy(buffer, arrayRes, 0, isize);
            Marshal.FreeCoTaskMem(buffer);
            ipAddress = System.Text.Encoding.ASCII.GetString(arrayRes);
            //ipAddress = Encoding.ASCII.GetString(Encoding.ASCII.GetBytes(ipAddress));
            ipAddress = ReturnCleanASCII(ipAddress);

            //System.Text.Encoding.ASCII.GetString(System.Text.Encoding.ASCII.GetBytes(arrayRes));
            //arrayRes.ToString();// buffer.ToString();

            return result;
        }

        static public string ReturnCleanASCII(string s)
        {
            StringBuilder sb = new StringBuilder(0);
            foreach (char c in s)
            {
                if ((int)c > 127) // you probably don't want 127 either
                    continue;
                if ((int)c < 32)  // I bet you don't want control characters 
                    continue;
                if (c == ',')
                    continue;
                if (c == '"')
                    continue;
                sb.Append(c);
            }
            return sb.ToString();
        }


        static public unsafe int wpf_uart_read(int uNum, [In, Out] byte[] buffInOut)
        {
            byte[] array2 = new byte[1024];
            int size = array2.Length;
            byte kcbyte = 0;
            IntPtr buffer = Marshal.AllocCoTaskMem(Marshal.SizeOf(kcbyte) * array2.Length);
            //Marshal.Copy(array2, 0, buffer, array2.Length);
            Int32 result = uart_read((short)uNum, buffer);
            Marshal.Copy(buffer, buffInOut, 0, result);
            Marshal.FreeCoTaskMem(buffer);
            return result;
        }

        static public unsafe int wpf_uart_write(int uNum, [In, Out] byte[] buffInOut, int len)
        {
            byte[] array2 = new byte[len];
            int size = array2.Length;
            byte kcbyte = 0;
            IntPtr buffer = Marshal.AllocCoTaskMem(Marshal.SizeOf(kcbyte) * array2.Length);
            Marshal.Copy(buffInOut, 0, buffer, array2.Length);
            Int32 result = uart_write((short)uNum, buffer,(short) len);
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

