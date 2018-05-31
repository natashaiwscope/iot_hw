using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using cTerm;
using PictureBoxLED16x16;
using PictureBoxLEDS;

namespace Termie
{
public partial class Form1 : Form
{
    /// <summary>
    /// Class to keep track of string and color for lines in output window.
    /// </summary>
    private class Line
    {
        public string Str;
        public Color ForeColor;

        public Line(string str, Color color)
        {
            Str = str;
            ForeColor = color;
        }
    };

    ArrayList lines = new ArrayList();
    bool isConnNotInitialized;
    bool isConn = false;
    private const int WM_ACTIVATEAPP = 0x001C;
    private bool appActive = true;

    private const int WM_DESTROY = 0x0002;

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


    Font origFont;
    Font monoFont;
	private Bitmap LEDimages;
	IBitmapLED     connLED;
	IBitmapLED     commLED;
    byte[] rBytes = new byte[1024];

    public Form1()
    {
        InitializeComponent();

        splitContainer1.FixedPanel = FixedPanel.Panel1;
        splitContainer2.FixedPanel = FixedPanel.Panel2;

        AcceptButton = button5; //Send
        CancelButton = button4; //Close
        isConnNotInitialized = true;

        outputList_Initialize();

        Settings.Read();
        TopMost = Settings.Option.StayOnTop;

        // let form use multiple fonts
        origFont = Font;
        FontFamily ff = new FontFamily("Courier New");
        monoFont = new Font(ff, 8, FontStyle.Regular);
        Font = Settings.Option.MonoFont ? monoFont : origFont;


        //FIXME
        //CommPort com = CommPort.Instance;
        //com.StatusChanged += OnStatusChanged;
        //com.DataReceived += OnDataReceived;
        //com.Open();

#if LOADFROMRESOURCE
			LEDimages = new Bitmap(GetType(), "leds.bmp");
#else
			try
			{
				LEDimages = new Bitmap(@"leds.bmp");
			}
			catch
			{
				MessageBox.Show(
				  @"Error: file--""leds.bmp"" not found in current directory",
				  "",
				  MessageBoxButtons.OKCancel,
				  MessageBoxIcon.Error);

				LEDimages = null;
			}

#endif


			connLED = new PictureBoxLED(this, pictureBox1,
				LEDimages,
				LED16x16.LED_COLOR.YELLOW,
				LED16x16.LED_STATE.OFF,
				LED16x16.LED_TYPE.SQUARE);

			commLED = new PictureBoxLED(this, pictureBox2,
				LEDimages,
				LED16x16.LED_COLOR.GREEN,
				LED16x16.LED_STATE.OFF,
				LED16x16.LED_TYPE.SQUARE);

    }

    void m_oWorker_DoWork(object sender, DoWorkEventArgs e)
    {
        //NOTE : Never play with the UI thread here...

        //time consuming operation
        for (;;)
        {
            System.Threading.Thread.Sleep(1);
            cTerm.cTerm.heartbeat_1ms();
        }

        //Report 100% completion on operation completed
        //m_oWorker.ReportProgress(100);
    }


    // shutdown the worker thread when the form closes
    protected override void OnClosed(EventArgs e)
    {
        //FIXME
        //CommPort com = CommPort.Instance;
        //com.Close();

        base.OnClosed(e);
    }
    [System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name = "FullTrust")]
    protected override void WndProc(ref Message m)
    {
        // Listen for operating system messages.
        switch (m.Msg)
        {
        // The WM_ACTIVATEAPP message occurs when the application
        // becomes the active application or becomes inactive.
        case 0x1C:

            // The WParam value identifies what is occurring.
            appActive = (((int)m.WParam != 0));

            // Invalidate to get new text painted.
            this.Invalidate();

            break;
        case WM_CONNECTED:
			connLED.LedOn();
            isConn = true;
            break;
        case WM_CONSOLE3:
            {
            int len;
            len=cTerm.cTerm.wpf_uart_read(3,rBytes);
    
            byte[] tmpBytes = new byte[len];
            Buffer.BlockCopy(rBytes, 0, tmpBytes, 0, len);

            string result = System.Text.Encoding.UTF8.GetString(tmpBytes);

            AddData(result);

            
            }
            break;
        case WM_CONSOLE2:
            break;
        case WM_CONSOLE1:
            break;
        }
        base.WndProc(ref m);
    }

    /// <summary>
    /// output string to log file
    /// </summary>
    /// <param name="stringOut">string to output</param>
    public void logFile_writeLine(string stringOut)
    {
        if (Settings.Option.LogFileName != "")
        {
            Stream myStream = File.Open(Settings.Option.LogFileName,
                                        FileMode.Append, FileAccess.Write, FileShare.Read);
            if (myStream != null)
            {
                StreamWriter myWriter = new StreamWriter(myStream, Encoding.UTF8);
                myWriter.WriteLine(stringOut);
                myWriter.Close();
            }
        }
    }

    #region Output window

    string filterString = "";
    bool scrolling = true;
    Color receivedColor = Color.Green;
    Color sentColor = Color.Blue;

    /// <summary>
    /// context menu for the output window
    /// </summary>
    ContextMenu popUpMenu;

    /// <summary>
    /// check to see if filter matches string
    /// </summary>
    /// <param name="s">string to check</param>
    /// <returns>true if matches filter</returns>
    bool outputList_ApplyFilter(String s)
    {
        if (filterString == "")
        {
            return true;
        }
        else if (s == "")
        {
            return false;
        }
        else if (Settings.Option.FilterUseCase)
        {
            return (s.IndexOf(filterString) != -1);
        }
        else
        {
            string upperString = s.ToUpper();
            string upperFilter = filterString.ToUpper();
            return (upperString.IndexOf(upperFilter) != -1);
        }
    }

    /// <summary>
    /// clear the output window
    /// </summary>
    void outputList_ClearAll()
    {
        lines.Clear();
        partialLine = null;

        outputList.Items.Clear();
    }

    /// <summary>
    /// refresh the output window
    /// </summary>
    void outputList_Refresh()
    {
        outputList.BeginUpdate();
        outputList.Items.Clear();
        foreach (Line line in lines)
        {
            if (outputList_ApplyFilter(line.Str))
            {
                outputList.Items.Add(line);
            }
        }
        outputList.EndUpdate();
        outputList_Scroll();
    }

    /// <summary>
    /// add a new line to output window
    /// </summary>
    Line outputList_Add(string str, Color color)
    {
        Line newLine = new Line(str, color);
        lines.Add(newLine);

        if (outputList_ApplyFilter(newLine.Str))
        {
            outputList.Items.Add(newLine);
            outputList_Scroll();
        }

        return newLine;
    }

    /// <summary>
    /// Update a line in the output window.
    /// </summary>
    /// <param name="line">line to update</param>
    void outputList_Update(Line line)
    {
        // should we add to output?
        if (outputList_ApplyFilter(line.Str))
        {
            // is the line already displayed?
            bool found = false;
            for (int i = 0; i < outputList.Items.Count; ++i)
            {
                int index = (outputList.Items.Count - 1) - i;
                if (line == outputList.Items[index])
                {
                    // is item visible?
                    int itemsPerPage = (int)(outputList.Height / outputList.ItemHeight);
                    if (index >= outputList.TopIndex &&
                            index < (outputList.TopIndex + itemsPerPage))
                    {
                        // is there a way to refresh just one line
                        // without redrawing the entire listbox?
                        // changing the item value has no effect
                        outputList.Refresh();
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                // not found, so add it
                outputList.Items.Add(line);
            }
        }
    }

    /// <summary>
    /// Initialize the output window
    /// </summary>
    private void outputList_Initialize()
    {
        // owner draw for listbox so we can add color
        outputList.DrawMode = DrawMode.OwnerDrawFixed;
        outputList.DrawItem += new DrawItemEventHandler(outputList_DrawItem);
        outputList.ClearSelected();

        // build the outputList context menu
        popUpMenu = new ContextMenu();
        popUpMenu.MenuItems.Add("&Copy", new EventHandler(outputList_Copy));
        popUpMenu.MenuItems[0].Visible = true;
        popUpMenu.MenuItems[0].Enabled = false;
        popUpMenu.MenuItems[0].Shortcut = Shortcut.CtrlC;
        popUpMenu.MenuItems[0].ShowShortcut = true;
        popUpMenu.MenuItems.Add("Copy All", new EventHandler(outputList_CopyAll));
        popUpMenu.MenuItems[1].Visible = true;
        popUpMenu.MenuItems.Add("Select &All", new EventHandler(outputList_SelectAll));
        popUpMenu.MenuItems[2].Visible = true;
        popUpMenu.MenuItems[2].Shortcut = Shortcut.CtrlA;
        popUpMenu.MenuItems[2].ShowShortcut = true;
        popUpMenu.MenuItems.Add("Clear Selected", new EventHandler(outputList_ClearSelected));
        popUpMenu.MenuItems[3].Visible = true;
        outputList.ContextMenu = popUpMenu;
    }

    /// <summary>
    /// draw item with color in output window
    /// </summary>
    void outputList_DrawItem(object sender, DrawItemEventArgs e)
    {
        e.DrawBackground();
        if (e.Index >= 0 && e.Index < outputList.Items.Count)
        {
            Line line = (Line)outputList.Items[e.Index];

            // if selected, make the text color readable
            Color color = line.ForeColor;
            if ((e.State & DrawItemState.Selected) == DrawItemState.Selected)
            {
                color = Color.Black;    // make it readable
            }

            e.Graphics.DrawString(line.Str, e.Font, new SolidBrush(color),
                                  e.Bounds, StringFormat.GenericDefault);
        }
        e.DrawFocusRectangle();
    }

    /// <summary>
    /// Scroll to bottom of output window
    /// </summary>
    void outputList_Scroll()
    {
        if (scrolling)
        {
            int itemsPerPage = (int)(outputList.Height / outputList.ItemHeight);
            outputList.TopIndex = outputList.Items.Count - itemsPerPage;
        }
    }

    /// <summary>
    /// Enable/Disable copy selection in output window
    /// </summary>
    private void outputList_SelectedIndexChanged(object sender, EventArgs e)
    {
        popUpMenu.MenuItems[0].Enabled = (outputList.SelectedItems.Count > 0);
    }

    /// <summary>
    /// copy selection in output window to clipboard
    /// </summary>
    private void outputList_Copy(object sender, EventArgs e)
    {
        int iCount = outputList.SelectedItems.Count;
        if (iCount > 0)
        {
            String[] source = new String[iCount];
            for (int i = 0; i < iCount; ++i)
            {
                source[i] = ((Line)outputList.SelectedItems[i]).Str;
            }

            String dest = String.Join("\r\n", source);
            Clipboard.SetText(dest);
        }
    }

    /// <summary>
    /// copy all lines in output window
    /// </summary>
    private void outputList_CopyAll(object sender, EventArgs e)
    {
        int iCount = outputList.Items.Count;
        if (iCount > 0)
        {
            String[] source = new String[iCount];
            for (int i = 0; i < iCount; ++i)
            {
                source[i] = ((Line)outputList.Items[i]).Str;
            }

            String dest = String.Join("\r\n", source);
            Clipboard.SetText(dest);
        }
    }

    /// <summary>
    /// select all lines in output window
    /// </summary>
    private void outputList_SelectAll(object sender, EventArgs e)
    {
        outputList.BeginUpdate();
        for (int i = 0; i < outputList.Items.Count; ++i)
        {
            outputList.SetSelected(i, true);
        }
        outputList.EndUpdate();
    }

    /// <summary>
    /// clear selected in output window
    /// </summary>
    private void outputList_ClearSelected(object sender, EventArgs e)
    {
        outputList.ClearSelected();
        outputList.SelectedItem = -1;
    }

    #endregion

    #region Event handling - data received and status changed

    /// <summary>
    /// Prepare a string for output by converting non-printable characters.
    /// </summary>
    /// <param name="StringIn">input string to prepare.</param>
    /// <returns>output string.</returns>
    private String PrepareData(String StringIn)
    {
        // The names of the first 32 characters
        string[] charNames = { "NUL", "SOH", "STX", "ETX", "EOT",
                               "ENQ", "ACK", "BEL", "BS", "TAB", "LF", "VT", "FF", "CR", "SO", "SI",
                               "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB",
                               "ESC", "FS", "GS", "RS", "US", "Space"
                             };

        string StringOut = "";

        foreach (char c in StringIn)
        {
            if (Settings.Option.HexOutput)
            {
                StringOut = StringOut + String.Format("{0:X2} ", (int)c);
            }
            else if (c < 32 && c != 9)
            {
                StringOut = StringOut + "<" + charNames[c] + ">";

                //Uglier "Termite" style
                //StringOut = StringOut + String.Format("[{0:X2}]", (int)c);
            }
            else
            {
                StringOut = StringOut + c;
            }
        }
        return StringOut;
    }

    /// <summary>
    /// Partial line for AddData().
    /// </summary>
    private Line partialLine = null;

    /// <summary>
    /// Add data to the output.
    /// </summary>
    /// <param name="StringIn"></param>
    /// <returns></returns>
    private Line AddData(String StringIn)
    {
        String StringOut = PrepareData(StringIn);

        // if we have a partial line, add to it.
        if (partialLine != null)
        {
            // tack it on
            partialLine.Str = partialLine.Str + StringOut;
            outputList_Update(partialLine);
            return partialLine;
        }

        return outputList_Add(StringOut, receivedColor);
    }

    // delegate used for Invoke
    internal delegate void StringDelegate(string data);

    /// <summary>
    /// Handle data received event from serial port.
    /// </summary>
    /// <param name="data">incoming data</param>
    public void OnDataReceived(string dataIn)
    {
        //Handle multi-threading
        if (InvokeRequired)
        {
            Invoke(new StringDelegate(OnDataReceived), new object[] { dataIn });
            return;
        }

        // pause scrolling to speed up output of multiple lines
        bool saveScrolling = scrolling;
        scrolling = false;

        // if we detect a line terminator, add line to output
        int index;
        while (dataIn.Length > 0 &&
                ((index = dataIn.IndexOf("\r")) != -1 ||
                 (index = dataIn.IndexOf("\n")) != -1))
        {
            String StringIn = dataIn.Substring(0, index);
            dataIn = dataIn.Remove(0, index + 1);

            logFile_writeLine(AddData(StringIn).Str);
            partialLine = null; // terminate partial line
        }

        // if we have data remaining, add a partial line
        if (dataIn.Length > 0)
        {
            partialLine = AddData(dataIn);
        }

        // restore scrolling
        scrolling = saveScrolling;
        outputList_Scroll();
    }

    /// <summary>
    /// Update the connection status
    /// </summary>
    public void OnStatusChanged(string status)
    {
        //Handle multi-threading
        if (InvokeRequired)
        {
            Invoke(new StringDelegate(OnStatusChanged), new object[] { status });
            return;
        }

        textBox1.Text = status;
    }

    #endregion

    #region User interaction

    /// <summary>
    /// toggle connection status
    /// </summary>
    private void textBox1_Click(object sender, MouseEventArgs e)
    {
        //FIXME
       // CommPort com = CommPort.Instance;
       // if (com.IsOpen)
       // {
       //     com.Close();
       // }
       // else
       // {
       //     com.Open();
       // }
       // outputList.Focus();
    }

    /// <summary>
    /// Change filter
    /// </summary>
    private void textBox2_TextChanged(object sender, EventArgs e)
    {
        filterString = textBox2.Text;
        outputList_Refresh();
    }

    /// <summary>
    /// Show settings dialog
    /// </summary>
    private void button1_Click(object sender, EventArgs e)
    {
        TopMost = false;

        Form2 form2 = new Form2();
        form2.ShowDialog();

        TopMost = Settings.Option.StayOnTop;
        Font = Settings.Option.MonoFont ? monoFont : origFont;
    }

    /// <summary>
    /// Clear the output window
    /// </summary>
    private void button2_Click(object sender, EventArgs e)
    {
        outputList_ClearAll();
    }

    /// <summary>
    /// Show about dialog
    /// </summary>
    private void button3_Click(object sender, EventArgs e)
    {
        //Register Win
        if (isConnNotInitialized)
        {
            m_oWorker = new BackgroundWorker();
            m_oWorker.DoWork += new DoWorkEventHandler(m_oWorker_DoWork);
            //m_oWorker.ProgressChanged += new ProgressChangedEventHandler(m_oWorker_ProgressChanged);
            //m_oWorker.RunWorkerCompleted += new RunWorkerCompletedEventHandler(m_oWorker_RunWorkerCompleted);
            m_oWorker.WorkerReportsProgress = false;
            m_oWorker.WorkerSupportsCancellation = true;
            m_oWorker.RunWorkerAsync();

            IntPtr hWnd = this.Handle;
            cTerm.cTerm.RegisterWin(hWnd);
            cTerm.cTerm.OnNewStart();

            isConnNotInitialized = false;
			commLED.LedOn();
            AddData("Connected");
        }
    }

    /// <summary>
    /// Close the application
    /// </summary>
    private void button4_Click(object sender, EventArgs e)
    {
        Close();
    }

    /// <summary>
    /// If character 0-9 a-f A-F, then return hex digit value ?
    /// </summary>
    private static int GetHexDigit(char c)
    {
        if ('0' <= c && c <= '9') return (c - '0');
        if ('a' <= c && c <= 'f') return (c - 'a') + 10;
        if ('A' <= c && c <= 'F') return (c - 'A') + 10;
        return 0;
    }

    /// <summary>
    /// Parse states for ConvertEscapeSequences()
    /// </summary>
    public enum Expecting : byte
    {
        ANY = 1,
        ESCAPED_CHAR,
        HEX_1ST_DIGIT,
        HEX_2ND_DIGIT
    };

    /// <summary>
    /// Convert escape sequences
    /// </summary>
    private string ConvertEscapeSequences(string s)
    {
        Expecting expecting = Expecting.ANY;

        int hexNum = 0;
        string outs = "";
        foreach (char c in s)
        {
            switch (expecting)
            {
            case Expecting.ANY:
                if (c == '\\')
                    expecting = Expecting.ESCAPED_CHAR;
                else
                    outs += c;
                break;
            case Expecting.ESCAPED_CHAR:
                if (c == 'x')
                {
                    expecting = Expecting.HEX_1ST_DIGIT;
                }
                else
                {
                    char c2 = c;
                    switch (c)
                    {
                    case 'n': c2 = '\n'; break;
                    case 'r': c2 = '\r'; break;
                    case 't': c2 = '\t'; break;
                    }
                    outs += c2;
                    expecting = Expecting.ANY;
                }
                break;
            case Expecting.HEX_1ST_DIGIT:
                hexNum = GetHexDigit(c) * 16;
                expecting = Expecting.HEX_2ND_DIGIT;
                break;
            case Expecting.HEX_2ND_DIGIT:
                hexNum += GetHexDigit(c);
                outs += (char)hexNum;
                expecting = Expecting.ANY;
                break;
            }
        }
        return outs;
    }

    public void Write_Serial(string data)
    {
            string lineEnding = "";
            switch (Settings.Option.AppendToSend)
            {
            case Settings.Option.AppendType.AppendCR:
                lineEnding = "\r"; break;
            case Settings.Option.AppendType.AppendLF:
                lineEnding = "\n"; break;
            case Settings.Option.AppendType.AppendCRLF:
                lineEnding = "\r\n"; break;
            }
            string s=data+lineEnding;
            byte[] toBytes = Encoding.ASCII.GetBytes(s);
            cTerm.cTerm.wpf_uart_write(3,toBytes, s.Length);
    }

    /// <summary>
    /// Send command
    /// </summary>
    private void button5_Click(object sender, EventArgs e)
    {
        string command = comboBox1.Text;
        comboBox1.Items.Add(comboBox1.Text);
        comboBox1.Text = "";
        comboBox1.Focus();

        if (command.Length > 0)
        {
            command = ConvertEscapeSequences(command);

            //FIXME
            //CommPort com = CommPort.Instance;
            //com.Send(command);
            Write_Serial(command);

            if (Settings.Option.LocalEcho)
            {
                outputList_Add(command + "\n", sentColor);
            }
        }
    }

    /// <summary>
    /// send file to serial port
    /// </summary>
    private void button6_Click(object sender, EventArgs e)
    {
        OpenFileDialog dialog = new OpenFileDialog();
        dialog.RestoreDirectory = false;
        dialog.Title = "Select a file";
        if (dialog.ShowDialog() == DialogResult.OK)
        {
            String text = System.IO.File.ReadAllText(dialog.FileName);

            //FIXME
            //CommPort com = CommPort.Instance;
            //com.Send(text);

            if (Settings.Option.LocalEcho)
            {
                outputList_Add("SendFile " + dialog.FileName + "," +
                               text.Length.ToString() + " byte(s)\n", sentColor);
            }
        }
    }

    /// <summary>
    /// toggle scrolling
    /// </summary>
    private void button7_Click(object sender, EventArgs e)
    {
        scrolling = !scrolling;
        outputList_Scroll();
    }

    #endregion
}
}
