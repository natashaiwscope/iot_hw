using System;
using System.IO;
using System.IO.Ports;
using System.Collections;
using System.Windows.Forms; // for Application.StartupPath

namespace Termie
{
/// <summary>
/// Persistent settings
/// </summary>
public class Settings
{
    /// <summary> Port settings. </summary>
    public class Port
    {
        public static string PortName = "COM1";
        public static int BaudRate = 115200;
        public static int DataBits = 8;
        public static System.IO.Ports.Parity Parity = System.IO.Ports.Parity.None;
        public static System.IO.Ports.StopBits StopBits = System.IO.Ports.StopBits.One;
        public static System.IO.Ports.Handshake Handshake = System.IO.Ports.Handshake.None;
    }

    /// <summary> Option settings. </summary>
    public class Option
    {
        public enum AppendType
        {
            AppendNothing,
            AppendCR,
            AppendLF,
            AppendCRLF
        }

        public static AppendType AppendToSend = AppendType.AppendCR;
        public static bool HexOutput = false;
        public static bool MonoFont = true;
        public static bool LocalEcho = true;
        public static bool StayOnTop = false;
        public static bool FilterUseCase = false;
        public static string LogFileName = "";
    }

    /// <summary>
    ///   Read the settings from disk. </summary>
    public static void Read()
    {
        IniFile ini = new IniFile(Application.StartupPath + "\\Termie.ini");
        Port.PortName = ini.ReadValue("Port", "PortName", Port.PortName);
        Port.BaudRate = ini.ReadValue("Port", "BaudRate", Port.BaudRate);
        Port.DataBits = ini.ReadValue("Port", "DataBits", Port.DataBits);
        Port.Parity = (Parity)Enum.Parse(typeof(Parity), ini.ReadValue("Port", "Parity", Port.Parity.ToString()));
        Port.StopBits = (StopBits)Enum.Parse(typeof(StopBits), ini.ReadValue("Port", "StopBits", Port.StopBits.ToString()));
        Port.Handshake = (Handshake)Enum.Parse(typeof(Handshake), ini.ReadValue("Port", "Handshake", Port.Handshake.ToString()));

        Option.AppendToSend = (Option.AppendType)Enum.Parse(typeof(Option.AppendType), ini.ReadValue("Option", "AppendToSend", Option.AppendToSend.ToString()));
        Option.HexOutput = bool.Parse(ini.ReadValue("Option", "HexOutput", Option.HexOutput.ToString()));
        Option.MonoFont = bool.Parse(ini.ReadValue("Option", "MonoFont", Option.MonoFont.ToString()));
        Option.LocalEcho = bool.Parse(ini.ReadValue("Option", "LocalEcho", Option.LocalEcho.ToString()));
        Option.StayOnTop = bool.Parse(ini.ReadValue("Option", "StayOnTop", Option.StayOnTop.ToString()));
        Option.FilterUseCase = bool.Parse(ini.ReadValue("Option", "FilterUseCase", Option.FilterUseCase.ToString()));
    }

    /// <summary>
    ///   Write the settings to disk. </summary>
    public static void Write()
    {
        IniFile ini = new IniFile(Application.StartupPath + "\\Termie.ini");
        ini.WriteValue("Port", "PortName", Port.PortName);
        ini.WriteValue("Port", "BaudRate", Port.BaudRate);
        ini.WriteValue("Port", "DataBits", Port.DataBits);
        ini.WriteValue("Port", "Parity", Port.Parity.ToString());
        ini.WriteValue("Port", "StopBits", Port.StopBits.ToString());
        ini.WriteValue("Port", "Handshake", Port.Handshake.ToString());

        ini.WriteValue("Option", "AppendToSend", Option.AppendToSend.ToString());
        ini.WriteValue("Option", "HexOutput", Option.HexOutput.ToString());
        ini.WriteValue("Option", "MonoFont", Option.MonoFont.ToString());
        ini.WriteValue("Option", "LocalEcho", Option.LocalEcho.ToString());
        ini.WriteValue("Option", "StayOnTop", Option.StayOnTop.ToString());
        ini.WriteValue("Option", "FilterUseCase", Option.FilterUseCase.ToString());
    }
}
}
