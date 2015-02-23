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
    public partial class TcpForm : Form
    {
        // Zmienne użyte przy połączeniu TCP
        private TcpClient client;
        private string IP;
        private string Port;
        private NetworkStream clientStream;
        private bool Connected;
        // Wątek obsługujący czytanie ze strumienia TCP
        private Thread oThread;      

        public TcpForm()
        {
            InitializeComponent();
        }

        public void Init(string IP_arg, string Port_arg, byte baudRate_arg, byte dataBits_arg, byte parity_arg, byte stopBits_arg)
        {
            try
            {
                // Połączenie TCP
                client = new TcpClient();
                IP = IP_arg;
                Port = Port_arg;
                IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(IP), Convert.ToInt32(Port)); 
                client.Connect(serverEndPoint);
                Connected = true;
                clientStream = client.GetStream();
                // Stworzenie ramki inicjującej
                byte[] RsParams = new byte[4];
                RsParams[0] = baudRate_arg;
                RsParams[1] = dataBits_arg;
                RsParams[2] = parity_arg;
                RsParams[3] = stopBits_arg;
                // Przesłanie ramki
                SendOverIP(RsParams);
                // Ustawienie domyślne na wpisywanie tekstu
                SendType.SelectedIndex = 2;
                // Utworzenie nowego wątku obsługującego odbieranie danych
                oThread = new Thread(new ThreadStart(TcpReading));
                oThread.IsBackground = true;
                oThread.Start();
                
            }
            catch (Exception e)
            {
                TcpReceivedAppendText(e.Message, Color.Red);
            }
        }

        private void TcpReading()
        {
            byte[] buffer_read;
            TcpClient clientReading = new TcpClient();
            IPEndPoint serverEndPointReading = new IPEndPoint(IPAddress.Parse(IP), Convert.ToInt32(Port));
            clientReading.Connect(serverEndPointReading);
            NetworkStream clientStreamReading = clientReading.GetStream();
            while (Connected)
            {
                try
                {
                    string Text = "";
                    buffer_read = new byte[10];
                    clientStreamReading.ReadTimeout = 100;
                    // Czytanie danych ze strumienia
                    clientStreamReading.Read(buffer_read, 0, buffer_read.Length);
                    foreach (byte c in buffer_read)
                        Text += (char)c;
                    // Odwołanie się do wątku głównego
                    TcpReceived.Invoke(new Action(delegate() { TcpReceived.AppendText(Text); }));
                }
                catch (System.IO.IOException) // Przechwycenie TimeOuta
                {
                    try
                    {
                        // Zamknięcie bieżącego połączenia
                        clientReading.Close();
                        clientReading = new TcpClient();
                        // Próba nawiązania ponownego połączenia
                        clientReading.Connect(serverEndPointReading);
                        clientStreamReading = clientReading.GetStream();
                    }
                    catch (Exception e) // Przechwycenie nie udanej próby ponownego połączenia
                    {
                        TcpReceived.Invoke(new Action(delegate() { TcpReceivedAppendText(e.Message, Color.Red); }));
                        Connected = false;
                        break;
                    }
                }
            }
        }   

        private void TcpSend_OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && TcpSend.Text.Length > 0)
            {
                TcpReceivedAppendText(TcpSend.Text, Color.HotPink);
                byte[] bytes_write;
                // Przypadek gdy wybrano Dec
                if( SendType.SelectedIndex == 0)
                {
                    bytes_write = new byte[4];
                    try
                    {
                        bytes_write = BitConverter.GetBytes(Convert.ToInt32(TcpSend.Text));
                        SendOverIP(bytes_write);
                    }
                    catch (Exception ex)
                    {
                        TcpReceivedAppendText(ex.Message, Color.Red);
                    }                               
                }
                // Przypadek gdy wybrano Hex
                else if (SendType.SelectedIndex == 1)
                {
                    bytes_write = new byte[4];
                    try
                    {
                        bytes_write = BitConverter.GetBytes(Convert.ToInt32(TcpSend.Text, 16));
                        SendOverIP(bytes_write);
                    }
                    catch (Exception ex)
                    {
                        TcpReceivedAppendText(ex.Message, Color.Red);
                    }   
                }
                // Przypadek gdy wybrano Text
                else if (SendType.SelectedIndex == 2)
                {
                    // Dzielenie danych na paczki
                    int NumOfPacks = TcpSend.Text.Length / 8;
                    int LastPack = TcpSend.Text.Length % 8;
                    if (NumOfPacks > 0)
                    {
                        // Wysyłanie paczek po 8 bajtów
                        bytes_write = new byte[8];
                        for (int i = 0; i < NumOfPacks; i++)
                        {
                            for (int j = 0; j < 8; j++)
                                bytes_write[j] = (byte)TcpSend.Text[j + i * 8];
                            SendOverIP(bytes_write);
                        }
                    }
                    if (LastPack > 0)
                    {
                        bytes_write = new byte[LastPack];
                        for (int j = 0; j < LastPack; j++)
                            bytes_write[j] = (byte)TcpSend.Text[TcpSend.Text.Length - LastPack + j];
                        SendOverIP(bytes_write);
                    }
                }
            }
        }

        private void SendOverIP(byte[] bytes_write)
        {
            try
            {
                // Próba wysłania danych do strumienia
                clientStream.Write(bytes_write, 0, bytes_write.Length);
            }
            catch (Exception e)
            {
                client.Close();
                TcpReceived.Invoke(new Action(delegate() { TcpReceivedAppendText(e.Message, Color.Red); }));
                Connected = false;
            }
        }

        private void Clean_OnClick(object sender, EventArgs e)
        {
            TcpReceived.Text = String.Empty;
        }

        private void TcpReceivedAppendText(string text, Color color)
        {
            TcpReceived.SelectionStart = TcpReceived.TextLength;
            TcpReceived.SelectionLength = 0;

            TcpReceived.SelectionColor = color;
            TcpReceived.AppendText(text);
            TcpReceived.SelectionColor = TcpReceived.ForeColor;
        }

        private void Disconnect_OnClick(object sender, EventArgs e)
        {
            if (client != null && client.Connected)
            {
                Connected = false;
                client.Close();          
            }
            this.Close();
        }
    }
}


