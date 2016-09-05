namespace RNDODriverStation
{
    partial class RNDODriverStation
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RNDODriverStation));
            this.nNumber = new System.Windows.Forms.NumericUpDown();
            this.tSend = new System.Windows.Forms.Timer(this.components);
            this.lController = new System.Windows.Forms.Label();
            this.lStatus = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.nNumber)).BeginInit();
            this.SuspendLayout();
            // 
            // nNumber
            // 
            this.nNumber.Location = new System.Drawing.Point(12, 12);
            this.nNumber.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nNumber.Name = "nNumber";
            this.nNumber.Size = new System.Drawing.Size(120, 20);
            this.nNumber.TabIndex = 0;
            // 
            // tSend
            // 
            this.tSend.Interval = 10;
            this.tSend.Tick += new System.EventHandler(this.tSend_Tick);
            // 
            // lController
            // 
            this.lController.Location = new System.Drawing.Point(12, 46);
            this.lController.Name = "lController";
            this.lController.Size = new System.Drawing.Size(260, 207);
            this.lController.TabIndex = 1;
            this.lController.Text = "label1";
            // 
            // lStatus
            // 
            this.lStatus.AutoSize = true;
            this.lStatus.Location = new System.Drawing.Point(147, 14);
            this.lStatus.Name = "lStatus";
            this.lStatus.Size = new System.Drawing.Size(35, 13);
            this.lStatus.TabIndex = 2;
            this.lStatus.Text = "label1";
            // 
            // RNDODriverStation
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.lStatus);
            this.Controls.Add(this.lController);
            this.Controls.Add(this.nNumber);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "RNDODriverStation";
            this.Text = "RNDO Driver Station";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.RNDODriverStation_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.nNumber)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown nNumber;
        private System.Windows.Forms.Timer tSend;
        private System.Windows.Forms.Label lController;
        private System.Windows.Forms.Label lStatus;
    }
}

