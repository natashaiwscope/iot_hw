using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using System.Threading;
 
using System.ComponentModel;
using System.Windows.Interop;



using System.Windows.Interop;


namespace WSim
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        bool isConnNotInitialized=true;
        BackgroundWorker m_oWorker;
        byte[] rBytes = new byte[1024];

        private IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            if (msg == slib.scom.WM_CONNECTED)
            {
                //L0.IsActive = true;
                //connLED.IsActive = true;
                this.connLED.Background = Brushes.LimeGreen;
                slot_ReInitDevList();
            }
            else if (msg == slib.scom.WM_CONSOLE)
            {
                //i++;
            }
            else if (msg == slib.scom.WM_CONSOLE1)
            {
                int len;
                len=slib.scom.wpf_uart_read(1,rBytes);
                byte[] tmpBytes = new byte[len];
                Buffer.BlockCopy(rBytes, 0, tmpBytes, 0, len);
                string result = System.Text.Encoding.UTF8.GetString(tmpBytes);
                rxRS485.Text += result;
            }
            else if (msg == slib.scom.WM_CONSOLE2)
            {
                int len;
                len=slib.scom.wpf_uart_read(2,rBytes);
                byte[] tmpBytes = new byte[len];
                Buffer.BlockCopy(rBytes, 0, tmpBytes, 0, len);
                rxUART2.Text += System.Text.Encoding.UTF8.GetString(tmpBytes);
            }
            else if (msg == slib.scom.WM_CONSOLE3)
            {
                int len;
                len=slib.scom.wpf_uart_read(3,rBytes);
                byte[] tmpBytes = new byte[len];
                Buffer.BlockCopy(rBytes, 0, tmpBytes, 0, len);
                rxUART3.Text += System.Text.Encoding.UTF8.GetString(tmpBytes);
            }
            return IntPtr.Zero;
        }

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Start_Click(object sender, RoutedEventArgs e)
        {
           if(isConnNotInitialized)
           {
            IntPtr windowHandle = (new WindowInteropHelper(this)).Handle;
            //RegisterWPF(windowHandle);

            HwndSource src = HwndSource.FromHwnd(windowHandle);
            src.AddHook(new HwndSourceHook(WndProc));

                slib.scom.RegisterWPF(windowHandle);
                slib.scom.OnNewStart();
                isConnNotInitialized = false;
           }

        }

        private void connLED_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            this.connLED.Background = Brushes.LightBlue;
            slib.scom.RebootCurrentDevice();
        }

        private void slot_ReInitDevList()
        {

            string s_ip;
            int iCount;
            short j;

            iCount=slib.scom.usDeviceIPAddress(0, out s_ip);

            ipAddrBox.Items.Clear();
            ipAddrBox.Text = String.Empty;
            ipAddrBox.SelectedItem = -1;

            for(j=0;j<iCount;j++)
            {
                iCount=slib.scom.usDeviceIPAddress(j, out s_ip);
                ipAddrBox.Items.Add(s_ip);

            }
            //ipAddrBox.Items.RemoveAt(ipAddrBox.Items.IndexOf(ipAddrBox.SelectedItem));
            ipAddrBox.SelectedIndex = 0;
        }

        private void ipAddrBox_DropDownClosed(object sender, EventArgs e)
        {
            this.connLED.Background = Brushes.Yellow;
            this.statusLine.Text = "->" + ipAddrBox.SelectedIndex;
            slib.scom.usSelectDev(ipAddrBox.SelectedIndex);
        }

        private void ipAddrBox_DropDownOpened(object sender, EventArgs e)
        {
            //this.connLED.Background = Brushes.Green;
        }

        private void scanSingle1_Click(object sender, RoutedEventArgs e)
        {
            //this.statusLine.Text = "FacCode" + int.Parse(this.facCode.Text) + "CardNum" + int.Parse(this.cardNum.Text);
             
        }

        private void txSerialPort2_Click(object sender, RoutedEventArgs e)
        {
            string lineEnding = "";
            string s=txUART2.Text+lineEnding;
            byte[] toBytes = Encoding.ASCII.GetBytes(s);
            slib.scom.wpf_uart_write(2,toBytes,(short) s.Length);
        }

        private void txSerialPort3_Click(object sender, RoutedEventArgs e)
        {
            string lineEnding = "";
            string s=txUART3.Text+lineEnding;
            byte[] toBytes = Encoding.ASCII.GetBytes(s);
            slib.scom.wpf_uart_write(3,toBytes,(short) s.Length);

        }

        private void txRS1_Click(object sender, RoutedEventArgs e)
        {
            string lineEnding = "";
            string s=txRS485.Text+lineEnding;
            byte[] toBytes = Encoding.ASCII.GetBytes(s);
            slib.scom.wpf_uart_write(3,toBytes,(short) s.Length);
        }
    }
}
