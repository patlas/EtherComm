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
            this.Baudrate = new System.Windows.Forms.TextBox();
            this.Baudrate_label = new System.Windows.Forms.Label();
            this.Databits_label = new System.Windows.Forms.Label();
            this.Databits = new System.Windows.Forms.TextBox();
            this.COM_label = new System.Windows.Forms.Label();
            this.Parity_label = new System.Windows.Forms.Label();
            this.Parity = new System.Windows.Forms.ComboBox();
            this.Module_IP_label = new System.Windows.Forms.Label();
            this.Module_IP = new System.Windows.Forms.TextBox();
            this.Port_label = new System.Windows.Forms.Label();
            this.Port = new System.Windows.Forms.TextBox();
            this.Connect_button = new System.Windows.Forms.Button();
            this.COM = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // Baudrate
            // 
            this.Baudrate.Location = new System.Drawing.Point(15, 64);
            this.Baudrate.Name = "Baudrate";
            this.Baudrate.Size = new System.Drawing.Size(100, 20);
            this.Baudrate.TabIndex = 0;
            // 
            // Baudrate_label
            // 
            this.Baudrate_label.AutoSize = true;
            this.Baudrate_label.Location = new System.Drawing.Point(12, 48);
            this.Baudrate_label.Name = "Baudrate_label";
            this.Baudrate_label.Size = new System.Drawing.Size(50, 13);
            this.Baudrate_label.TabIndex = 1;
            this.Baudrate_label.Text = "Baudrate";
            // 
            // Databits_label
            // 
            this.Databits_label.AutoSize = true;
            this.Databits_label.Location = new System.Drawing.Point(12, 87);
            this.Databits_label.Name = "Databits_label";
            this.Databits_label.Size = new System.Drawing.Size(46, 13);
            this.Databits_label.TabIndex = 3;
            this.Databits_label.Text = "Databits";
            // 
            // Databits
            // 
            this.Databits.Location = new System.Drawing.Point(15, 103);
            this.Databits.Name = "Databits";
            this.Databits.Size = new System.Drawing.Size(100, 20);
            this.Databits.TabIndex = 2;
            // 
            // COM_label
            // 
            this.COM_label.AutoSize = true;
            this.COM_label.Location = new System.Drawing.Point(12, 9);
            this.COM_label.Name = "COM_label";
            this.COM_label.Size = new System.Drawing.Size(31, 13);
            this.COM_label.TabIndex = 5;
            this.COM_label.Text = "COM";
            // 
            // Parity_label
            // 
            this.Parity_label.AutoSize = true;
            this.Parity_label.Location = new System.Drawing.Point(12, 126);
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
            "Mark"});
            this.Parity.Location = new System.Drawing.Point(15, 141);
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
            // Module_IP
            // 
            this.Module_IP.Location = new System.Drawing.Point(197, 25);
            this.Module_IP.Name = "Module_IP";
            this.Module_IP.Size = new System.Drawing.Size(100, 20);
            this.Module_IP.TabIndex = 15;
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
            // Connect_button
            // 
            this.Connect_button.Location = new System.Drawing.Point(197, 91);
            this.Connect_button.Name = "Connect_button";
            this.Connect_button.Size = new System.Drawing.Size(75, 23);
            this.Connect_button.TabIndex = 19;
            this.Connect_button.Text = "Connect";
            this.Connect_button.UseVisualStyleBackColor = true;
            this.Connect_button.Click += new System.EventHandler(this.EtherCOM_Connect);
            // 
            // COM
            // 
            this.COM.FormattingEnabled = true;
            this.COM.Location = new System.Drawing.Point(15, 25);
            this.COM.Name = "COM";
            this.COM.Size = new System.Drawing.Size(121, 21);
            this.COM.TabIndex = 20;
            // 
            // EtherCOM
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(329, 174);
            this.Controls.Add(this.COM);
            this.Controls.Add(this.Connect_button);
            this.Controls.Add(this.Port_label);
            this.Controls.Add(this.Port);
            this.Controls.Add(this.Module_IP_label);
            this.Controls.Add(this.Module_IP);
            this.Controls.Add(this.Parity);
            this.Controls.Add(this.Parity_label);
            this.Controls.Add(this.COM_label);
            this.Controls.Add(this.Databits_label);
            this.Controls.Add(this.Databits);
            this.Controls.Add(this.Baudrate_label);
            this.Controls.Add(this.Baudrate);
            this.Name = "EtherCOM";
            this.Text = "EtherCOM";
            this.Load += new System.EventHandler(this.EtherCOM_OnLoad);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox Baudrate;
        private System.Windows.Forms.Label Baudrate_label;
        private System.Windows.Forms.Label Databits_label;
        private System.Windows.Forms.TextBox Databits;
        private System.Windows.Forms.Label COM_label;
        private System.Windows.Forms.Label Parity_label;
        private System.Windows.Forms.ComboBox Parity;
        private System.Windows.Forms.Label Module_IP_label;
        private System.Windows.Forms.TextBox Module_IP;
        private System.Windows.Forms.Label Port_label;
        private System.Windows.Forms.TextBox Port;
        private System.Windows.Forms.Button Connect_button;
        private System.Windows.Forms.ComboBox COM;
    }
}

