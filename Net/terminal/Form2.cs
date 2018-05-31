using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;

namespace Termie
{
public partial class Form2 : Form
{
    public Form2()
    {
        InitializeComponent();

        CommPort com = CommPort.Instance;

        int found = 0;
        string[] portList = com.GetAvailablePorts();
        for (int i = 0; i < portList.Length; ++i)
        {
            string name = portList[i];
            comboBox1.Items.Add(name);
            if (name == Settings.Port.PortName)
                found = i;
        }
        if (portList.Length > 0)
            comboBox1.SelectedIndex = found;

        Int32[] baudRates =
        {
            100, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200,
            38400, 56000, 57600, 115200, 128000, 256000, 0
        };
        found = 0;
        for (int i = 0; baudRates[i] != 0; ++i)
        {
            comboBox2.Items.Add(baudRates[i].ToString());
            if (baudRates[i] == Settings.Port.BaudRate)
                found = i;
        }
        comboBox2.SelectedIndex = found;

        comboBox3.Items.Add("5");
        comboBox3.Items.Add("6");
        comboBox3.Items.Add("7");
        comboBox3.Items.Add("8");
        comboBox3.SelectedIndex = Settings.Port.DataBits - 5;

        foreach (string s in Enum.GetNames(typeof(Parity)))
        {
            comboBox4.Items.Add(s);
        }
        comboBox4.SelectedIndex = (int)Settings.Port.Parity;

        foreach (string s in Enum.GetNames(typeof(StopBits)))
        {
            comboBox5.Items.Add(s);
        }
        comboBox5.SelectedIndex = (int)Settings.Port.StopBits;

        foreach (string s in Enum.GetNames(typeof(Handshake)))
        {
            comboBox6.Items.Add(s);
        }
        comboBox6.SelectedIndex = (int)Settings.Port.Handshake;

        switch (Settings.Option.AppendToSend)
        {
        case Settings.Option.AppendType.AppendNothing:
            radioButton1.Checked = true;
            break;
        case Settings.Option.AppendType.AppendCR:
            radioButton2.Checked = true;
            break;
        case Settings.Option.AppendType.AppendLF:
            radioButton3.Checked = true;
            break;
        case Settings.Option.AppendType.AppendCRLF:
            radioButton4.Checked = true;
            break;
        }

        checkBox1.Checked = Settings.Option.HexOutput;
        checkBox2.Checked = Settings.Option.MonoFont;
        checkBox3.Checked = Settings.Option.LocalEcho;
        checkBox4.Checked = Settings.Option.StayOnTop;
        checkBox5.Checked = Settings.Option.FilterUseCase;

        textBox1.Text = Settings.Option.LogFileName;
    }

    // OK
    private void button1_Click(object sender, EventArgs e)
    {
        Settings.Port.PortName = comboBox1.Text;
        Settings.Port.BaudRate = Int32.Parse(comboBox2.Text);
        Settings.Port.DataBits = comboBox3.SelectedIndex + 5;
        Settings.Port.Parity = (Parity)comboBox4.SelectedIndex;
        Settings.Port.StopBits = (StopBits)comboBox5.SelectedIndex;
        Settings.Port.Handshake = (Handshake)comboBox6.SelectedIndex;

        if (radioButton2.Checked)
            Settings.Option.AppendToSend = Settings.Option.AppendType.AppendCR;
        else if (radioButton3.Checked)
            Settings.Option.AppendToSend = Settings.Option.AppendType.AppendLF;
        else if (radioButton4.Checked)
            Settings.Option.AppendToSend = Settings.Option.AppendType.AppendCRLF;
        else
            Settings.Option.AppendToSend = Settings.Option.AppendType.AppendNothing;

        Settings.Option.HexOutput = checkBox1.Checked;
        Settings.Option.MonoFont = checkBox2.Checked;
        Settings.Option.LocalEcho = checkBox3.Checked;
        Settings.Option.StayOnTop = checkBox4.Checked;
        Settings.Option.FilterUseCase = checkBox5.Checked;

        Settings.Option.LogFileName = textBox1.Text;

        CommPort com = CommPort.Instance;
        com.Open();

        Settings.Write();

        Close();
    }

    // Cancel
    private void button2_Click(object sender, EventArgs e)
    {
        Close();
    }

    private void button3_Click(object sender, EventArgs e)
    {
        Settings.Option.LogFileName = "";

        SaveFileDialog fileDialog1 = new SaveFileDialog();

        fileDialog1.Title = "Save Log As";
        fileDialog1.Filter = "Log files (*.log)|*.log|All files (*.*)|*.*";
        fileDialog1.FilterIndex = 2;
        fileDialog1.RestoreDirectory = true;
        fileDialog1.FileName = Settings.Option.LogFileName;

        if (fileDialog1.ShowDialog() == DialogResult.OK)
        {
            textBox1.Text = fileDialog1.FileName;
            if (File.Exists(textBox1.Text))
                File.Delete(textBox1.Text);
        }
        else
        {
            textBox1.Text = "";
        }
    }
}
}