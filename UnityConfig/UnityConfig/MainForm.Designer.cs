﻿namespace UnityConfig
{
    partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importScriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportScriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateScriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.quitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.refreshDeviceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.languagesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.englishToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.czechToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unityToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deviceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.readConfigToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.writeConfigToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveScriptFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.openScriptFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.deviceToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.quitToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.configurationControl = new UnityConfig.ConfigurationControl();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.unityToolStripMenuItem,
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.languagesToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(453, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.importScriptToolStripMenuItem,
            this.exportScriptToolStripMenuItem,
            this.generateScriptToolStripMenuItem,
            this.quitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            this.fileToolStripMenuItem.Visible = false;
            // 
            // importScriptToolStripMenuItem
            // 
            this.importScriptToolStripMenuItem.Name = "importScriptToolStripMenuItem";
            this.importScriptToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.importScriptToolStripMenuItem.Text = "Import script";
            this.importScriptToolStripMenuItem.Click += new System.EventHandler(this.importScriptToolStripMenuItem_Click);
            // 
            // exportScriptToolStripMenuItem
            // 
            this.exportScriptToolStripMenuItem.Name = "exportScriptToolStripMenuItem";
            this.exportScriptToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.exportScriptToolStripMenuItem.Text = "Export script";
            this.exportScriptToolStripMenuItem.Click += new System.EventHandler(this.exportScriptToolStripMenuItem_Click);
            // 
            // generateScriptToolStripMenuItem
            // 
            this.generateScriptToolStripMenuItem.Name = "generateScriptToolStripMenuItem";
            this.generateScriptToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.generateScriptToolStripMenuItem.Text = "Generate script";
            this.generateScriptToolStripMenuItem.Click += new System.EventHandler(this.generateScriptToolStripMenuItem_Click);
            // 
            // quitToolStripMenuItem
            // 
            this.quitToolStripMenuItem.Name = "quitToolStripMenuItem";
            this.quitToolStripMenuItem.Size = new System.Drawing.Size(153, 22);
            this.quitToolStripMenuItem.Text = "&Quit";
            this.quitToolStripMenuItem.Click += new System.EventHandler(this.quitToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.settingToolStripMenuItem,
            this.refreshDeviceToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
            this.toolsToolStripMenuItem.Text = "Tools";
            // 
            // settingToolStripMenuItem
            // 
            this.settingToolStripMenuItem.Name = "settingToolStripMenuItem";
            this.settingToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.settingToolStripMenuItem.Text = "Setting";
            this.settingToolStripMenuItem.Click += new System.EventHandler(this.settingToolStripMenuItem_Click);
            // 
            // refreshDeviceToolStripMenuItem
            // 
            this.refreshDeviceToolStripMenuItem.Name = "refreshDeviceToolStripMenuItem";
            this.refreshDeviceToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.refreshDeviceToolStripMenuItem.Text = "Refresh device";
            this.refreshDeviceToolStripMenuItem.Click += new System.EventHandler(this.refreshDeviceToolStripMenuItem_Click);
            // 
            // languagesToolStripMenuItem
            // 
            this.languagesToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.englishToolStripMenuItem,
            this.czechToolStripMenuItem,
            this.deviceToolStripMenuItem1});
            this.languagesToolStripMenuItem.Name = "languagesToolStripMenuItem";
            this.languagesToolStripMenuItem.Size = new System.Drawing.Size(76, 20);
            this.languagesToolStripMenuItem.Text = "Languages";
            this.languagesToolStripMenuItem.Visible = false;
            // 
            // englishToolStripMenuItem
            // 
            this.englishToolStripMenuItem.Name = "englishToolStripMenuItem";
            this.englishToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.englishToolStripMenuItem.Text = "English";
            this.englishToolStripMenuItem.Click += new System.EventHandler(this.englishToolStripMenuItem_Click);
            // 
            // czechToolStripMenuItem
            // 
            this.czechToolStripMenuItem.Name = "czechToolStripMenuItem";
            this.czechToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.czechToolStripMenuItem.Text = "Czech";
            this.czechToolStripMenuItem.Click += new System.EventHandler(this.czechToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // unityToolStripMenuItem
            // 
            this.unityToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.deviceToolStripMenuItem,
            this.readConfigToolStripMenuItem,
            this.writeConfigToolStripMenuItem,
            this.quitToolStripMenuItem1});
            this.unityToolStripMenuItem.Name = "unityToolStripMenuItem";
            this.unityToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.unityToolStripMenuItem.Text = "Unity";
            // 
            // deviceToolStripMenuItem
            // 
            this.deviceToolStripMenuItem.Name = "deviceToolStripMenuItem";
            this.deviceToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.deviceToolStripMenuItem.Text = "Device";
            this.deviceToolStripMenuItem.Click += new System.EventHandler(this.deviceToolStripMenuItem_Click);
            // 
            // readConfigToolStripMenuItem
            // 
            this.readConfigToolStripMenuItem.Name = "readConfigToolStripMenuItem";
            this.readConfigToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.readConfigToolStripMenuItem.Text = "Read Config";
            this.readConfigToolStripMenuItem.Click += new System.EventHandler(this.readConfigToolStripMenuItem_Click);
            // 
            // writeConfigToolStripMenuItem
            // 
            this.writeConfigToolStripMenuItem.Name = "writeConfigToolStripMenuItem";
            this.writeConfigToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.writeConfigToolStripMenuItem.Text = "Write Config";
            this.writeConfigToolStripMenuItem.Click += new System.EventHandler(this.writeConfigToolStripMenuItem_Click);
            // 
            // saveScriptFileDialog
            // 
            this.saveScriptFileDialog.DefaultExt = "sh";
            this.saveScriptFileDialog.FileName = "unityConfig";
            this.saveScriptFileDialog.Filter = "Unity Config Files|*.sh";
            this.saveScriptFileDialog.RestoreDirectory = true;
            // 
            // openScriptFileDialog
            // 
            this.openScriptFileDialog.DefaultExt = "sh";
            this.openScriptFileDialog.Filter = "Unity Config Files|*.sh";
            // 
            // deviceToolStripMenuItem1
            // 
            this.deviceToolStripMenuItem1.Name = "deviceToolStripMenuItem1";
            this.deviceToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.deviceToolStripMenuItem1.Text = "Device";
            // 
            // quitToolStripMenuItem1
            // 
            this.quitToolStripMenuItem1.Name = "quitToolStripMenuItem1";
            this.quitToolStripMenuItem1.Size = new System.Drawing.Size(152, 22);
            this.quitToolStripMenuItem1.Text = "Quit";
            // 
            // configurationControl
            // 
            this.configurationControl.Location = new System.Drawing.Point(0, 27);
            this.configurationControl.Name = "configurationControl";
            this.configurationControl.Setting = null;
            this.configurationControl.Size = new System.Drawing.Size(451, 485);
            this.configurationControl.TabIndex = 2;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(453, 512);
            this.Controls.Add(this.configurationControl);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Unity Config";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importScriptToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportScriptToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem quitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private ConfigurationControl configurationControl;
        private System.Windows.Forms.SaveFileDialog saveScriptFileDialog;
        private System.Windows.Forms.OpenFileDialog openScriptFileDialog;
        private System.Windows.Forms.ToolStripMenuItem generateScriptToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem languagesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem englishToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem czechToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unityToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem refreshDeviceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem readConfigToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem writeConfigToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deviceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deviceToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem quitToolStripMenuItem1;
    }
}

