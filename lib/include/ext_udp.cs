using System;
using System.Text;
using System.Windows;
using System.Runtime.InteropServices;


namespace cTerm
{
    public partial class MainWindow : Window
    {
        [StructLayout(LayoutKind.Sequential)]
        struct CS_DataStruct
        {
            public IntPtr data;
            public int len;
        };

        [DllImport("udplib.dll", EntryPoint = "fun_uart_read_struct", CallingConvention = CallingConvention.StdCall)]
        unsafe private static extern int fun_uart_read_struct(short bus_num, CS_DataStruct** datastrct);

        [DllImport("udplib.dll", EntryPoint = "heartbeat_1ms", CallingConvention = CallingConvention.StdCall)]
        unsafe private static extern void heartbeat_1ms();


        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary")]
        static extern int LoadLibrary([MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        [DllImport("kernel32.dll", EntryPoint = "GetProcAddress")]
        static extern IntPtr GetProcAddress(int hModule, [MarshalAs(UnmanagedType.LPStr)] string lpProcName);

        [DllImport("kernel32.dll", EntryPoint = "FreeLibrary")]
        static extern bool FreeLibrary(int hModule);

        [DllImport("udplib.dll", EntryPoint = "GetVersionInfo", CallingConvention = CallingConvention.StdCall)]
        public static extern Int32 GetVersionInfo(char[] versionInfo);

        [DllImport("udplib.dll", EntryPoint = "usSelectDev", CallingConvention = CallingConvention.StdCall)]
        public static extern Int32 usSelectDev(Int32 i);

        [DllImport("udplib.dll")]
        public static extern Int16 PushMessage(UInt32 hMsg, UInt32 wP, UInt32 lP);

        [DllImport("udplib.dll", EntryPoint = "start_lib_interface_task", CallingConvention = CallingConvention.StdCall)]
        private static extern Int16 start_lib_interface_task();

        [DllImport("udplib.dll")]
        public static extern Int16 OnExit();

        [DllImport("udplib.dll")]
        public static extern short usPullStrWinQ(ref short winMsg, ref short wPar, ref short lPar, ref float fVal, char[] strMsg);

        [DllImport("udplib.dll", EntryPoint = "isWinRegistered", CallingConvention = CallingConvention.StdCall)]
        public static extern int isWinRegistered();

        [DllImport("udplib.dll", EntryPoint = "usTestMsg", CallingConvention = CallingConvention.StdCall)]
        public static extern int usTestMsg(short winMsg, short wPar, short lPar);

        [DllImport("udplib.dll", EntryPoint = "RebootCurrentDevice", CallingConvention = CallingConvention.StdCall)]
        public static extern int RebootCurrentDevice();

        [DllImport("user32.dll")]
        public static extern bool PostMessage(IntPtr hWnd, int Msg, int wParam, int lParam);

        [DllImport("udplib.dll")]
        public static extern Int16 PushRawFrame(UInt32 hMsg, UInt32 wP, UInt32 lP);

        [DllImport("udplib.dll")]
        public static extern Int16 set_raw_mode(UInt32 hMode);


        [DllImport("udplib.dll", EntryPoint = "ReadTEST", CallingConvention = CallingConvention.StdCall)]
        public static extern int ReadTEST(short uNum, [In, Out] IntPtr arr);

        [DllImport("udplib.dll", EntryPoint = "usGetConnectedDeviceIP", CallingConvention = CallingConvention.StdCall)]
        public static extern int usGetConnectedDeviceIP(short uNum, [In, Out] IntPtr arr);


        [DllImport("udplib.dll", EntryPoint = "u3PullWinQ", CallingConvention = CallingConvention.StdCall)]
        public static extern int u3PullWinQ(ref short winMsg, ref short wPar, ref short lPar);

        [DllImport("udplib.dll", EntryPoint = "u3PullTest", CallingConvention = CallingConvention.StdCall)]
        public static extern int u3PullTest(out short winMsg, out short wPar, out short lPar);

        [DllImport("udplib.dll", EntryPoint = "RegisterWin", CallingConvention = CallingConvention.StdCall)]
        public static extern int RegisterWin(IntPtr windowHandle);

        [DllImport("udplib.dll")]
        public static extern Int16 set_nrx_ping_count(UInt32 hMode);

        [DllImport("udplib.dll", EntryPoint = "uart_read", CallingConvention = CallingConvention.StdCall)]
        public static extern int uart_read(short uNum, [In, Out] IntPtr arr);
        [DllImport("udplib.dll", EntryPoint = "uart_write", CallingConvention = CallingConvention.StdCall)]
        public static extern int uart_write(short uNum, [In, Out] IntPtr arr,short len);


        unsafe int usDeviceIPAddress(short uNum, out string ipAddress)
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

        public string ReturnCleanASCII(string s)
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

        unsafe int ReadUART(short uNum, [In, Out] byte[] buffInOut)
        {
            byte[] array2 = new byte[1024];
            int size = array2.Length;
            byte kcbyte = 0;

            IntPtr buffer = Marshal.AllocCoTaskMem(Marshal.SizeOf(kcbyte) * array2.Length);
            Marshal.Copy(array2, 0, buffer, array2.Length);

            Int32 result = ReadTEST(uNum, buffer);

            byte[] arrayRes = new byte[size];
            Marshal.Copy(buffer, arrayRes, 0, size);
            Marshal.FreeCoTaskMem(buffer);

            return result;
        }

        unsafe int wpf_uart_read(short uNum, [In, Out] byte[] buffInOut)
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

        unsafe int wpf_uart_write(short uNum, [In, Out] byte[] buffInOut, short len)
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


        public static void OnNewStart()
        {
            start_lib_interface_task();
        }

        public static void OnStop()
        {
            OnExit();
        }

        public static void TestMessage(UInt32 hMsg, UInt32 wp, UInt32 lp)
        {
            PushMessage(hMsg, wp, lp);
        }

        public static void RegisterWPF(IntPtr winHandle)
        {
            //hWnd=winHandle;
            RegisterWin(winHandle);
        }

        public static Int32 myGetVersionInfo(char[] versionInfo)
        {
            return GetVersionInfo(versionInfo);
        }

        public static int uPullWinQ(ref short winMsg, ref short wPar, ref short lPar)
        {
            return u3PullWinQ(ref winMsg, ref wPar, ref lPar);
        }
        public static int iWinRegistered()
        {
            PostMessage(hWnd, 0x403, 0x403, 0x403);
            return isWinRegistered();
        }
        static IntPtr hWnd;
    }
}

