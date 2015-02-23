namespace EtherCOM
{
    partial class TcpForm
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
            this.TcpSend = new System.Windows.Forms.TextBox();
            this.TcpReceivedLabel = new System.Windows.Forms.Label();
            this.TcpSendLabel = new System.Windows.Forms.Label();
            this.Clean = new System.Windows.Forms.Button();
            this.TcpReceived = new System.Windows.Forms.RichTextBox();
            this.SendType = new System.Windows.Forms.ComboBox();
            this.Disconnect = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // TcpSend
            // 
            this.TcpSend.Location = new System.Drawing.Point(48, 12);
            this.TcpSend.Name = "TcpSend";
            this.TcpSend.Size = new System.Drawing.Size(100, 20);
            this.TcpSend.TabIndex = 10;
            this.TcpSend.KeyDown += new System.Windows.Forms.KeyEventHandler(this.TcpSend_OnKeyDown);
            // 
            // TcpReceivedLabel
            // 
            this.TcpReceivedLabel.AutoSize = true;
            this.TcpReceivedLabel.Location = new System.Drawing.Point(7, 52);
            this.TcpReceivedLabel.Name = "TcpReceivedLabel";
            this.TcpReceivedLabel.Size = new System.Drawing.Size(107, 13);
            this.TcpReceivedLabel.TabIndex = 9;
            this.TcpReceivedLabel.Text = "Received/Send data";
            // 
            // TcpSendLabel
            // 
            this.TcpSendLabel.AutoSize = true;
            this.TcpSendLabel.Location = new System.Drawing.Point(7, 15);
            this.TcpSendLabel.Name = "TcpSendLabel";
            this.TcpSendLabel.Size = new System.Drawing.Size(32, 13);
            this.TcpSendLabel.TabIndex = 8;
            this.TcpSendLabel.Text = "Send";
            // 
            // Clean
            // 
            this.Clean.Location = new System.Drawing.Point(217, 39);
            this.Clean.Name = "Clean";
            this.Clean.Size = new System.Drawing.Size(75, 23);
            this.Clean.TabIndex = 12;
            this.Clean.Text = "Clean";
            this.Clean.UseVisualStyleBackColor = true;
            this.Clean.Click += new System.EventHandler(this.Clean_OnClick);
            // 
            // TcpReceived
            // 
            this.TcpReceived.Location = new System.Drawing.Point(12, 68);
            this.TcpReceived.Name = "TcpReceived";
            this.TcpReceived.Size = new System.Drawing.Size(280, 191);
            this.TcpReceived.TabIndex = 13;
            this.TcpReceived.Text = "";
            // 
            // SendType
            // 
            this.SendType.FormattingEnabled = true;
            this.SendType.Items.AddRange(new object[] {
            "Dec",
            "Hex",
            "Text"});
            this.SendType.Location = new System.Drawing.Point(154, 12);
            this.SendType.Name = "SendType";
            this.SendType.Size = new System.Drawing.Size(44, 21);
            this.SendType.TabIndex = 14;
            // 
            // Disconnect
            // 
            this.Disconnect.Location = new System.Drawing.Point(217, 10);
            this.Disconnect.Name = "Disconnect";
            this.Disconnect.Size = new System.Drawing.Size(75, 23);
            this.Disconnect.TabIndex = 15;
            this.Disconnect.Text = "Disconnect";
            this.Disconnect.UseVisualStyleBackColor = true;
            this.Disconnect.Click += new System.EventHandler(this.Disconnect_OnClick);
            // 
            // TcpForm
            // 
            this.ClientSize = new System.Drawing.Size(304, 271);
            this.Controls.Add(this.Disconnect);
            this.Controls.Add(this.SendType);
            this.Controls.Add(this.TcpReceived);
            this.Controls.Add(this.Clean);
            this.Controls.Add(this.TcpSend);
            this.Controls.Add(this.TcpReceivedLabel);
            this.Controls.Add(this.TcpSendLabel);
            this.Name = "TcpForm";
            this.Text = "Communication over TCP";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label Send_label;
        private System.Windows.Forms.TextBox Send;
        private System.Windows.Forms.Label All_data;
        private System.Windows.Forms.TextBox Data;
        private System.Windows.Forms.TextBox TcpSend;
        private System.Windows.Forms.Label TcpReceivedLabel;
        private System.Windows.Forms.Label TcpSendLabel;
        private System.Windows.Forms.Button Clean;
        private System.Windows.Forms.RichTextBox TcpReceived;
        private System.Windows.Forms.ComboBox SendType;
        private System.Windows.Forms.Button Disconnect;
    }
}