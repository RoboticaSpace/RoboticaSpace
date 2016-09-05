namespace RNDOGraph
{
    partial class RNDOGraph
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(RNDOGraph));
            this.lData = new System.Windows.Forms.Label();
            this.tDisplay = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // lData
            // 
            this.lData.AutoSize = true;
            this.lData.Location = new System.Drawing.Point(617, 3);
            this.lData.Name = "lData";
            this.lData.Size = new System.Drawing.Size(35, 13);
            this.lData.TabIndex = 0;
            this.lData.Text = "label1";
            // 
            // tDisplay
            // 
            this.tDisplay.Interval = 10;
            this.tDisplay.Tick += new System.EventHandler(this.tDisplay_Tick);
            // 
            // RNDOGraph
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(867, 439);
            this.Controls.Add(this.lData);
            this.DoubleBuffered = true;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "RNDOGraph";
            this.Text = "Graph";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.RNDOGraph_FormClosing);
            this.Load += new System.EventHandler(this.RNDOGraph_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.RNDOGraph_Paint);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lData;
        private System.Windows.Forms.Timer tDisplay;
    }
}

