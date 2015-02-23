using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EtherCOM
{
    public partial class RsForm : Form
    {
        private SerialPort serial_port;

        public RsForm()
        {
            InitializeComponent();
        }

        public void Init(SerialPort serial_port_arg)
        {
            serial_port = serial_port_arg;
            serial_port.DataReceived += new SerialDataReceivedEventHandler(SerialPort_OnDataReceived);
            serial_port.Open();
        }

        private void SerialPort_OnDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string Text = Environment.NewLine + "Received:";
            char[] char_buffer = new char[serial_port.BytesToRead];
            serial_port.Read(char_buffer, 0, serial_port.BytesToRead);
            foreach (char c in char_buffer)
                Text += c;
            RsReceived.Invoke(new Action(delegate() { RsReceived.AppendText(Text); }));
        }

        private void Send_OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && RsSend.Text.Length > 0)
            {
                string ToSend = RsSend.Text;
                RsReceived.Text += Environment.NewLine + "Sending:" + ToSend;
                char[] chars_write;
                int NumOfPacks = ToSend.Length / 8;
                int LastPack = ToSend.Length % 8;
                if (NumOfPacks > 0)
                {
                    chars_write = new char[8];
                    for (int i = 0; i < NumOfPacks; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            chars_write[j] = ToSend[j + i * 8];
                        }
                        serial_port.Write(chars_write, 0, chars_write.Length);
                    }
                    chars_write = new char[8];
                }
                if(LastPack > 0)
                {
                    chars_write = new char[LastPack];
                    for (int j = 0; j < LastPack; j++)
                    {
                        chars_write[j] = ToSend[ToSend.Length - LastPack + j];
                    }
                    serial_port.Write(chars_write, 0, chars_write.Length);
                }
            }
        }
    }
}
 