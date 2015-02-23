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
        public Form_Data Data;
        public SerialPort serial_port;
        public EtherCOM()
        {
            InitializeComponent();
        }

        private void EtherCOM_OnLoad(object sender, EventArgs e)
        {       
            Data = new Form_Data();
            Data.Show();
            Data.Location = new Point(this.Location.X + this.Size.Width, this.Location.Y);
            // Initializace COM Ports
            string[] ports = SerialPort.GetPortNames();
            foreach (string port in ports)
            {
                COM.Items.Add(port);
            }
            //Default parameters
            Module_IP.Text = "192.168.2.102";
            Port.Text = "7";

            COM.Text = "COM8";
            Baudrate.Text = "115200";
            Databits.Text = "8";
            Parity.SelectedIndex = 0;
        }

        private void EtherCOM_Connect(object sender, EventArgs e)
        {
            {
                string portName = COM.Text;
                int baudRate = Convert.ToInt32(Baudrate.Text);
                Parity parity = System.IO.Ports.Parity.None;
                int dataBits = Convert.ToInt32(Databits.Text);
                StopBits stopBits = StopBits.One;
                serial_port = new SerialPort(portName, baudRate, parity, dataBits, stopBits);
                Data.SerialPort_Init(serial_port);
            }
            if (Module_IP.Text.Length > 0 && Port.Text.Length > 0)
            {
                Data.TCP_Init(Module_IP.Text, Port.Text);
            }
        }
    }
}
