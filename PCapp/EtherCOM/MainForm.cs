using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace EtherCOM
{
    public partial class EtherCOM : Form
    {
        // Parametry połączenia
        private byte baudRate;
        private byte dataBits;
        private byte parity;
        private byte stopBits;

        public EtherCOM()
        {
            InitializeComponent();
        }

        private void EtherCOM_OnLoad(object sender, EventArgs e)
        {               
            // Parametry domyślne
            ModuleIP.Text = "192.168.2.102";
            Port.Text = "7";

            Baudrate.SelectedIndex = 10;
            Databits.SelectedIndex = 3;
            Parity.SelectedIndex = 0;
            Stopbits.SelectedIndex = 0;
        }

        private void EtherCOM_Connect(object sender, EventArgs e)
        {
            if (ModuleIP.Text.Length > 0 && Port.Text.Length > 0)
            {
                CalculateRsParameters();
                // Utworzenie i zainicjonowanie okienka zarządzającego połączeniem TCP
                TcpForm TcpFormInstance = new TcpForm();
                TcpFormInstance.Show();
                TcpFormInstance.Location = new Point(this.Location.X + this.Width, this.Location.Y);
                TcpFormInstance.Init(ModuleIP.Text, Port.Text, baudRate, dataBits, parity, stopBits);
            }
            else
            {
                if( ModuleIP.Text.Length == 0 )
                    ModuleIP.Text = "Enter IP";
                if( Port.Text.Length == 0 )
                    Port.Text = "Enter PORT";
            }
        }

        // Zrzutowanie wartości wybranych z ComboBoxów na pojedyncze bajty
        private void CalculateRsParameters()
        {
            baudRate = (byte)Baudrate.SelectedIndex;
            dataBits = (byte)Databits.SelectedIndex;
            parity   = (byte)Parity.SelectedIndex;
            stopBits = (byte)Stopbits.SelectedIndex;
        }
    }
}
