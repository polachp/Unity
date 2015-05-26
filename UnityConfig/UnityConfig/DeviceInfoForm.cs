using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using UnityConfig.Properties;
using UnityConfig.UnityConfiguration;

namespace UnityConfig
{
    public partial class DeviceInfoForm : Form
    {
        public DeviceInfoForm(UnityDevice device)
        {
            InitializeComponent();

            SetLanguage();

            this.Device = device;
        }

        public UnityDevice _device { get; set; }

        private UnityDevice Device
        {
            get
            {
                return _device;
            }

            set
            {
                _device = value;
                this.LoadDeviceInfo();
            }
        }

        private void SetLanguage()
        {
            groupConnected.Text = Resources.DeviceInfoForm_Group_Connected;
            labelIsConnected.Text = Resources.DeviceInfoForm_Label_IsConnected;

            groupOther.Text = Resources.DeviceInfoForm_Group_Other;
            labelNavigationSW.Text = Resources.DeviceInfoForm_Label_NavigationSW;

            groupDiskInformation.Text = Resources.DeviceInfoForm_Group_DiskInformation;
            labelNavigationSWDirectory.Text = Resources.DeviceInfoForm_Label_Directory_NavigationSW;
            labelKoboDirectory.Text = Resources.DeviceInfoForm_Label_Directory_Kobo;
            labelScriptPath.Text = Resources.DeviceInfoForm_Label_ScriptPath;
        }

        private void LoadDeviceInfo()
        {
            //this.propertyGrid1.SelectedObject = _device;

            if (Device == null)
            {
                MessageBox.Show(Resources.DeviceInfoForm_NoDeviceInfo);
                Close();
            }

            // Connection status
            if (Device.IsConnected)
            {
                if (Device.IsInError)
                {
                    this.labelIsConnectedValue.ForeColor = Color.Red;
                }
                else
                {
                    this.labelIsConnectedValue.ForeColor = Color.Green;
                }
            }
            else
            {
                this.labelIsConnectedValue.ForeColor = Color.Black;
            }

            labelIsConnectedValue.Text = Device.IsConnected ? Resources.DeviceInfoForm_ConnectionStatus_Connected : Resources.DeviceInfoForm_ConnectionStatus_Disconnected;

            var connectionMessage = Device.ConnectionErrorMessage ?? Resources.DeviceInfoForm_NoError;
            toolTip1.SetToolTip(groupConnected, connectionMessage);
            toolTip1.SetToolTip(labelIsConnected, connectionMessage);
            toolTip1.SetToolTip(labelIsConnectedValue, connectionMessage);

            // SW
            labelNavigationSWValue.Text = Device.SWType.ToString();

            // Directories
            labelNavigationSWDirectoryValue.Text = Device.NavigationSWDirectory;
            labelKoboDirectoryValue.Text = Device.KoboDirectory;
            labelScriptPathValue.Text = Device.ConfigSkriptFilePath;
        }

        private void labelNavigationSWDirectoryValue_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            OnDirectoryLinkClick(sender, e);
        }
        private void labelKoboDirectoryValue_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            OnDirectoryLinkClick(sender, e);
        }
                
        private void labelScriptPathValue_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            if(!Device.ConfigFileExists)
            {
                MessageBox.Show(Resources.DeviceInfoForm_ConfigFileDoesntExist);
                return;
            }

            OnNotePadLinkClick(sender, e);
        }

        private void OnDirectoryLinkClick(object sender, LinkLabelLinkClickedEventArgs e)
        {
            var linkLabel = sender as LinkLabel;

            string url;
            if (e.Link.LinkData != null)
                url = e.Link.LinkData.ToString();
            else
                url = linkLabel.Text.Substring(e.Link.Start, e.Link.Length);

            //if (!url.Contains("://"))
            //    url = "http://" + url;

            var si = new ProcessStartInfo(url);
            Process.Start(si);
            linkLabel.LinkVisited = true;
        }

        private void OnNotePadLinkClick(object sender, LinkLabelLinkClickedEventArgs e)
        {
            var linkLabel = sender as LinkLabel;

            string url;
            if (e.Link.LinkData != null)
                url = e.Link.LinkData.ToString();
            else
                url = linkLabel.Text.Substring(e.Link.Start, e.Link.Length);

            //if (!url.Contains("://"))
            //    url = "http://" + url;

            var si = new ProcessStartInfo("notepad.exe", url);
            Process.Start(si);
            linkLabel.LinkVisited = true;
        }
    }
}
