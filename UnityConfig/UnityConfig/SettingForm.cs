using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using UnityConfig.Properties;
using UnityConfig.UnityConfiguration;

namespace UnityConfig
{
    public partial class SettingForm : Form
    {
        public SettingForm()
        {
            InitializeComponent();
        }

        private void SettingForm_Load(object sender, EventArgs e)
        {
            // Resources
            btnCancel.Text = Resources.Button_Cancel;
            btnOK.Text = Resources.Button_OK;
            
            labelLanguage.Text = Resources.Setting_Property_Language;

            List<ComboboxItem> languages = new List<ComboboxItem>();
            languages.Add(new ComboboxItem { Value = "cs-CZ", Text = "Čeština" });
            languages.Add(new ComboboxItem { Value = "en-GB", Text = "English" });

            // Languages
            comboBoxLanguages.Items.Clear();
            languages.ForEach(l => comboBoxLanguages.Items.Add(l));
            var selected = languages.FirstOrDefault(l => l.Value.ToString() == Settings.Default.Language);

            comboBoxLanguages.SelectedItem = selected;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            Settings.Default.Language = (comboBoxLanguages.SelectedItem as ComboboxItem).Value.ToString();
            Settings.Default.SettingInitialized = true;
            Settings.Default.Save();
            System.Threading.Thread.CurrentThread.CurrentUICulture = new System.Globalization.CultureInfo(Settings.Default.Language);

            MessageBox.Show(Resources.MessageBox_PleaseRestartApplication);

            Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void comboBoxLanguages_SelectedIndexChanged(object sender, EventArgs e)
        {
            EnsureValidation();
        }

        private void EnsureValidation()
        {
            var enabled = true;

            enabled = enabled && comboBoxLanguages.SelectedItem != null;

            btnOK.Enabled = enabled;
        }
    }
}
