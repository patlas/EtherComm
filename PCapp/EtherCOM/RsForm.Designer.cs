namespace EtherCOM
{
    partial class RsForm
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
            this.RsReceived = new System.Windows.Forms.TextBox();
            this.RsSend = new System.Windows.Forms.TextBox();
            this.RsReceivedLabel = new System.Windows.Forms.Label();
            this.RsSendLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // RsReceived
            // 
            this.RsReceived.Location = new System.Drawing.Point(7, 60);
            this.RsReceived.Multiline = true;
            this.RsReceived.Name = "RsReceived";
            this.RsReceived.Size = new System.Drawing.Size(270, 190);
            this.RsReceived.TabIndex = 7;
            // 
            // RsSend
            // 
            this.RsSend.Location = new System.Drawing.Point(47, 12);
            this.RsSend.Name = "RsSend";
            this.RsSend.Size = new System.Drawing.Size(100, 20);
            this.RsSend.TabIndex = 6;
            this.RsSend.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Send_OnKeyDown);
            // 
            // RsReceivedLabel
            // 
            this.RsReceivedLabel.AutoSize = true;
            this.RsReceivedLabel.Location = new System.Drawing.Point(6, 43);
            this.RsReceivedLabel.Name = "RsReceivedLabel";
            this.RsReceivedLabel.Size = new System.Drawing.Size(107, 13);
            this.RsReceivedLabel.TabIndex = 5;
            this.RsReceivedLabel.Text = "Received/Send data";
            // 
            // RsSendLabel
            // 
            this.RsSendLabel.AutoSize = true;
            this.RsSendLabel.Location = new System.Drawing.Point(6, 15);
            this.RsSendLabel.Name = "RsSendLabel";
            this.RsSendLabel.Size = new System.Drawing.Size(32, 13);
            this.RsSendLabel.TabIndex = 4;
            this.RsSendLabel.Text = "Send";
            // 
            // RsForm
            // 
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.RsReceived);
            this.Controls.Add(this.RsSend);
            this.Controls.Add(this.RsReceivedLabel);
            this.Controls.Add(this.RsSendLabel);
            this.Name = "RsForm";
            this.Text = "Communication over RS";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.TextBox RsReceived;
        private System.Windows.Forms.Label RsReceivedLabel;
        private System.Windows.Forms.Label RsSendLabel;
        public System.Windows.Forms.TextBox RsSend;
    }
}