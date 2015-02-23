using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EtherCOM
{
    public partial class Form_Data : Form
    {
        private TcpClient client;
        private string IP;
        private string Port;
        private NetworkStream clientStream;
        private ASCIIEncoding encoder;
        private byte[] buffer_write;

        private SerialPort serial_port;
        private Thread oThread;
        public Form_Data()
        {
            InitializeComponent();
        }

        public void TCP_Init(string IP, string Port)
        {
            client = new TcpClient();
            this.IP = IP;
            this.Port = Port;
            try
            {
                IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(IP), Convert.ToInt32(Port));
                client.Connect(serverEndPoint);
                clientStream = client.GetStream();
                encoder = new ASCIIEncoding();
                buffer_write = new byte[8];
                string rs232_parameters;

                    rs232_parameters = "12345678";
                    buffer_write = encoder.GetBytes(rs232_parameters);
                    clientStream.Write(buffer_write, 0, buffer_write.Length);
                    //Thread.Sleep(1000);              
            }
            catch (Exception){ }
            finally
            {
                if(client.Connected)
                    client.Close();
            }
        }

        public void SerialPort_Init(SerialPort sp)
        {
            serial_port = sp;
           // serial_port.DataReceived += new SerialDataReceivedEventHandler(SerialPort_OnDataReceived);             
            try
            {
                serial_port.Open();               
                oThread = new Thread(new ThreadStart(Reading));
                oThread.Start();
            }
            catch (Exception e) 
            {
                string error = GetTextBoxText(DataReceived);
                error += Environment.NewLine + e.Message;
                SetControlPropertyThreadSafe(DataReceived, "Text", error);
                serial_port.Close();             
            }               
        }
        
        private void Reading()
        {
            while (true)
            {
                Thread.Sleep(100);
                if (serial_port.BytesToRead > 0)
                {
                    string History = GetTextBoxText(DataReceived);
                    History += "Received:";
                    char[] char_buffer = new char[serial_port.BytesToRead];
                    serial_port.Read(char_buffer, 0, serial_port.BytesToRead);
                    foreach (char c in char_buffer)
                        History += c;
                    History += Environment.NewLine;
                    SetControlPropertyThreadSafe(DataReceived, "Text", History);
                }
            }
        }
        /*
        private void SerialPort_OnDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string History = GetTextBoxText(DataReceived);
            char []char_buffer = new char[serial_port.BytesToRead];
            serial_port.Read(char_buffer, 0, serial_port.BytesToRead);
            foreach (char c in char_buffer)
                History += c;
            History += Environment.NewLine;
            SetControlPropertyThreadSafe(DataReceived, "Text", History);

        }*/
        
        private void DataSend_OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && DataSend.Text.Length > 0)
            {
                client = new TcpClient();
                DataReceived.Text += "Sending:" + DataSend.Text + Environment.NewLine;
                char[] chars_write;
                int NumOfPacks = DataSend.Text.Length/8;
                int LastPack = DataSend.Text.Length%8;
                if (NumOfPacks > 0)
                {
                    chars_write = new char[8];
                    for (int i = 0; i < NumOfPacks; i++)
                    {
                        for (int j = 0; j < 8; j++)
                        {
                            chars_write[j] = DataSend.Text[j + i * 8];
                        }
                        SendOverIP(chars_write);
                    }
                    chars_write = new char[8];
                }
                else
                {
                    chars_write = new char[LastPack];
                    for (int j = 0; j < LastPack; j++)
                    {
                        chars_write[j] = DataSend.Text[j];
                    }
                    SendOverIP(chars_write);
                }
            }
        }

        private void SendOverIP(char[] chars_write)
        {
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(IP), Convert.ToInt32(Port));
            client.Connect(serverEndPoint);
            buffer_write = encoder.GetBytes(chars_write);
            clientStream = client.GetStream();
            clientStream.Write(buffer_write, 0, buffer_write.Length);
        }

        private delegate void SetControlPropertyThreadSafeDelegate(Control control, string propertyName, object propertyValue);

        public static void SetControlPropertyThreadSafe(Control control, string propertyName, object propertyValue)
        {
            try
            {
                if (control.InvokeRequired)
                {
                    control.Invoke(new SetControlPropertyThreadSafeDelegate(SetControlPropertyThreadSafe), new object[] { control, propertyName, propertyValue });
                }
                else
                {
                    control.GetType().InvokeMember(propertyName, BindingFlags.SetProperty, null, control, new object[] { propertyValue });
                }
            }
            catch (Exception) { }
        }
        private string GetTextBoxText(TextBox box)
        {
            if (box.InvokeRequired)
            {
                Func<TextBox, string> deleg = new Func<TextBox, string>(GetTextBoxText);
                return box.Invoke(deleg, new object[] { box }).ToString();
            }
            else
            {
                return box.Text;
            }
        }
    }
}
