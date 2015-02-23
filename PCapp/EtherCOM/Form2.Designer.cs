namespace EtherCOM
{
    partial class Form_Data
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
            this.DataSend_label = new System.Windows.Forms.Label();
            this.DataReceived_label = new System.Windows.Forms.Label();
            this.DataSend = new System.Windows.Forms.TextBox();
            this.DataReceived = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // DataSend_label
            // 
            this.DataSend_label.AutoSize = true;
            this.DataSend_label.Location = new System.Drawing.Point(12, 9);
            this.DataSend_label.Name = "DataSend_label";
            this.DataSend_label.Size = new System.Drawing.Size(32, 13);
            this.DataSend_label.TabIndex = 0;
            this.DataSend_label.Text = "Send";
            // 
            // DataReceived_label
            // 
            this.DataReceived_label.AutoSize = true;
            this.DataReceived_label.Location = new System.Drawing.Point(12, 37);
            this.DataReceived_label.Name = "DataReceived_label";
            this.DataReceived_label.Size = new System.Drawing.Size(107, 13);
            this.DataReceived_label.TabIndex = 1;
            this.DataReceived_label.Text = "Received/Send data";
            // 
            // DataSend
            // 
            this.DataSend.Location = new System.Drawing.Point(53, 6);
            this.DataSend.Name = "DataSend";
            this.DataSend.Size = new System.Drawing.Size(100, 20);
            this.DataSend.TabIndex = 2;
            this.DataSend.KeyDown += new System.Windows.Forms.KeyEventHandler(this.DataSend_OnKeyDown);
            // 
            // DataReceived
            // 
            this.DataReceived.Location = new System.Drawing.Point(13, 54);
            this.DataReceived.Multiline = true;
            this.DataReceived.Name = "DataReceived";
            this.DataReceived.Size = new System.Drawing.Size(246, 159);
            this.DataReceived.TabIndex = 3;
            // 
            // Form_Data
            // 
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.DataReceived);
            this.Controls.Add(this.DataSend);
            this.Controls.Add(this.DataReceived_label);
            this.Controls.Add(this.DataSend_label);
            this.Name = "Form_Data";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label Send_label;
        private System.Windows.Forms.TextBox Send;
        private System.Windows.Forms.Label All_data;
        private System.Windows.Forms.TextBox Data;
        private System.Windows.Forms.Label DataSend_label;
        private System.Windows.Forms.Label DataReceived_label;
        private System.Windows.Forms.TextBox DataSend;
        public System.Windows.Forms.TextBox DataReceived;
    }
}