using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using UnityConfig.Properties;
using UnityConfig.UnityConfiguration;

namespace UnityConfig
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void SetLanguage()
        {
            

            //Menus
            fileToolStripMenuItem.Text = Resources.Menu_File_Title;
            importScriptToolStripMenuItem.Text = Resources.Menu_File_Import_Title;
            exportScriptToolStripMenuItem.Text = Resources.Menu_File_Export_Title;
            generateScriptToolStripMenuItem.Text = Resources.Menu_File_Generate_Title;
            quitToolStripMenuItem.Text = Resources.Menu_File_Quit_Title;

            languagesToolStripMenuItem.Text = Resources.Menu_Languages_Title;
            englishToolStripMenuItem.Text = Resources.Menu_Languages_English_Title;
            czechToolStripMenuItem.Text = Resources.Menu_Languages_Czech_Title;

            helpToolStripMenuItem.Text = Resources.Menu_Help_Title;
            aboutToolStripMenuItem.Text = Resources.Menu_Help_About_Title;

            toolsToolStripMenuItem.Text = Resources.Menu_Tool_Title;
            settingToolStripMenuItem.Text = Resources.Menu_Tool_Setting_Title;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            SetLanguage();

            var setting = UnityConfiguration.UnitySetting.CreateDefault();
            this.configurationControl.Setting = setting;

            //using (var form = new AboutForm())
            //{
            //    form.ShowDialog();
            //}
        }

        private string GenerateScript(UnitySetting setting)
        {
            var z = setting.AllProperties;
            StringBuilder sb = new StringBuilder();
            z.ForEach(p => sb.AppendLine(p.GetPacket()));
            sb.AppendLine("printf '$UNSAVE*' > $TTY_DEV");

            return sb.ToString();
        }

        #region Menu handlers

        private void exportScriptToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (saveScriptFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                var fileName = saveScriptFileDialog.FileName;
                var script = this.GenerateScript(configurationControl.Setting);
                File.WriteAllText(fileName, script);
            }
        }

        private void importScriptToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openScriptFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                var fileName = openScriptFileDialog.FileName;
                var error = false;
                var setting = UnityScriptParser.ParseScriptFromFile(fileName, out error);
                if (error)
                {
                    MessageBox.Show(Resources.Import_Error);
                }
                else
                {
                    configurationControl.Setting = setting;
                }
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var form = new AboutForm())
            {
                form.ShowDialog();
            }
        }

        private void quitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void generateScriptToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var setting = this.configurationControl.Setting;
            var script = this.GenerateScript(setting);

            MessageBox.Show(script + Resources.Generate_ScriptGenerated);
            Clipboard.SetText(script);
        }

        #endregion

        #region Languages

        private void englishToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void czechToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        #endregion

        private void MainForm_Shown(object sender, EventArgs e)
        {
            if (!Settings.Default.SettingInitialized)
            {
                using (var form = new SettingForm())
                {
                    form.ShowDialog();
                }
            }
        }

        private void settingToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (var form = new SettingForm())
            {
                form.ShowDialog();
            }
        }
    }
}
