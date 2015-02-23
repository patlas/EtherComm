namespace EtherCOM
{
    partial class EtherCOM
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Baudrate_label = new System.Windows.Forms.Label();
            this.Databits_label = new System.Windows.Forms.Label();
            this.Parity_label = new System.Windows.Forms.Label();
            this.Parity = new System.Windows.Forms.ComboBox();
            this.Module_IP_label = new System.Windows.Forms.Label();
            this.ModuleIP = new System.Windows.Forms.TextBox();
            this.Port_label = new System.Windows.Forms.Label();
            this.Port = new System.Windows.Forms.TextBox();
            this.ConnectButton = new System.Windows.Forms.Button();
            this.Baudrate = new System.Windows.Forms.ComboBox();
            this.Stopbits = new System.Windows.Forms.ComboBox();
            this.Stopbits_label = new System.Windows.Forms.Label();
            this.Databits = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // Baudrate_label
            // 
            this.Baudrate_label.AutoSize = true;
            this.Baudrate_label.Location = new System.Drawing.Point(12, 9);
            this.Baudrate_label.Name = "Baudrate_label";
            this.Baudrate_label.Size = new System.Drawing.Size(50, 13);
            this.Baudrate_label.TabIndex = 1;
            this.Baudrate_label.Text = "Baudrate";
            // 
            // Databits_label
            // 
            this.Databits_label.AutoSize = true;
            this.Databits_label.Location = new System.Drawing.Point(12, 48);
            this.Databits_label.Name = "Databits_label";
            this.Databits_label.Size = new System.Drawing.Size(50, 13);
            this.Databits_label.TabIndex = 3;
            this.Databits_label.Text = "Data Bits";
            // 
            // Parity_label
            // 
            this.Parity_label.AutoSize = true;
            this.Parity_label.Location = new System.Drawing.Point(12, 87);
            this.Parity_label.Name = "Parity_label";
            this.Parity_label.Size = new System.Drawing.Size(33, 13);
            this.Parity_label.TabIndex = 7;
            this.Parity_label.Text = "Parity";
            // 
            // Parity
            // 
            this.Parity.FormattingEnabled = true;
            this.Parity.Items.AddRange(new object[] {
            "None",
            "Even",
            "Odd",
            "Mark",
            "Space"});
            this.Parity.Location = new System.Drawing.Point(15, 103);
            this.Parity.Name = "Parity";
            this.Parity.Size = new System.Drawing.Size(121, 21);
            this.Parity.TabIndex = 12;
            // 
            // Module_IP_label
            // 
            this.Module_IP_label.AutoSize = true;
            this.Module_IP_label.Location = new System.Drawing.Point(194, 9);
            this.Module_IP_label.Name = "Module_IP_label";
            this.Module_IP_label.Size = new System.Drawing.Size(55, 13);
            this.Module_IP_label.TabIndex = 16;
            this.Module_IP_label.Text = "Module IP";
            // 
            // ModuleIP
            // 
            this.ModuleIP.Location = new System.Drawing.Point(197, 25);
            this.ModuleIP.Name = "ModuleIP";
            this.ModuleIP.Size = new System.Drawing.Size(100, 20);
            this.ModuleIP.TabIndex = 15;
            // 
            // Port_label
            // 
            this.Port_label.AutoSize = true;
            this.Port_label.Location = new System.Drawing.Point(194, 48);
            this.Port_label.Name = "Port_label";
            this.Port_label.Size = new System.Drawing.Size(26, 13);
            this.Port_label.TabIndex = 18;
            this.Port_label.Text = "Port";
            // 
            // Port
            // 
            this.Port.Location = new System.Drawing.Point(197, 64);
            this.Port.Name = "Port";
            this.Port.Size = new System.Drawing.Size(100, 20);
            this.Port.TabIndex = 17;
            // 
            // ConnectButton
            // 
            this.ConnectButton.Location = new System.Drawing.Point(197, 91);
            this.ConnectButton.Name = "ConnectButton";
            this.ConnectButton.Size = new System.Drawing.Size(75, 23);
            this.ConnectButton.TabIndex = 19;
            this.ConnectButton.Text = "Connect";
            this.ConnectButton.UseVisualStyleBackColor = true;
            this.ConnectButton.Click += new System.EventHandler(this.EtherCOM_Connect);
            // 
            // Baudrate
            // 
            this.Baudrate.FormattingEnabled = true;
            this.Baudrate.Items.AddRange(new object[] {
            "110   ",
            "150   ",
            "300   ",
            "1200  ",
            "2400  ",
            "4800  ",
            "9600  ",
            "19200 ",
            "38400 ",
            "57600 ",
            "115200",
            "230400",
            "460800",
            "921600"});
            this.Baudrate.Location = new System.Drawing.Point(15, 23);
            this.Baudrate.Name = "Baudrate";
            this.Baudrate.Size = new System.Drawing.Size(121, 21);
            this.Baudrate.TabIndex = 21;
            // 
            // Stopbits
            // 
            this.Stopbits.FormattingEnabled = true;
            this.Stopbits.Items.AddRange(new object[] {
            "1 bit ",
            "2 bits"});
            this.Stopbits.Location = new System.Drawing.Point(15, 143);
            this.Stopbits.Name = "Stopbits";
            this.Stopbits.Size = new System.Drawing.Size(121, 21);
            this.Stopbits.TabIndex = 22;
            // 
            // Stopbits_label
            // 
            this.Stopbits_label.AutoSize = true;
            this.Stopbits_label.Location = new System.Drawing.Point(12, 127);
            this.Stopbits_label.Name = "Stopbits_label";
            this.Stopbits_label.Size = new System.Drawing.Size(49, 13);
            this.Stopbits_label.TabIndex = 23;
            this.Stopbits_label.Text = "Stop Bits";
            // 
            // Databits
            // 
            this.Databits.FormattingEnabled = true;
            this.Databits.Items.AddRange(new object[] {
            "5 bits",
            "6 bits",
            "7 bits",
            "8 bits"});
            this.Databits.Location = new System.Drawing.Point(15, 65);
            this.Databits.Name = "Databits";
            this.Databits.Size = new System.Drawing.Size(121, 21);
            this.Databits.TabIndex = 24;
            // 
            // EtherCOM
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(329, 174);
            this.Controls.Add(this.Databits);
            this.Controls.Add(this.Stopbits_label);
            this.Controls.Add(this.Stopbits);
            this.Controls.Add(this.Baudrate);
            this.Controls.Add(this.ConnectButton);
            this.Controls.Add(this.Port_label);
            this.Controls.Add(this.Port);
            this.Controls.Add(this.Module_IP_label);
            this.Controls.Add(this.ModuleIP);
            this.Controls.Add(this.Parity);
            this.Controls.Add(this.Parity_label);
            this.Controls.Add(this.Databits_label);
            this.Controls.Add(this.Baudrate_label);
            this.Name = "EtherCOM";
            this.Text = "EtherCOM";
            this.Load += new System.EventHandler(this.EtherCOM_OnLoad);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label Baudrate_label;
        private System.Windows.Forms.Label Databits_label;
        private System.Windows.Forms.Label Parity_label;
        private System.Windows.Forms.ComboBox Parity;
        private System.Windows.Forms.Label Module_IP_label;
        private System.Windows.Forms.TextBox ModuleIP;
        private System.Windows.Forms.Label Port_label;
        private System.Windows.Forms.TextBox Port;
        private System.Windows.Forms.Button ConnectButton;
        private System.Windows.Forms.ComboBox Baudrate;
        private System.Windows.Forms.ComboBox Stopbits;
        private System.Windows.Forms.Label Stopbits_label;
        private System.Windows.Forms.ComboBox Databits;
    }
}

