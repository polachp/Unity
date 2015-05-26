using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnityConfig.UnityConfiguration
{
    public class UnityDevice
    {
        public UnityDevice()
        {
            this.IsConnected = false;
            this.SWType = UnityDeviceSWType.None;
            this.ConnectionErrorMessage = null;
        }

        public bool IsConnected { get; set; }

        public bool IsInError
        {
            get
            {
                return this.ConnectionErrorMessage != null;
            }
        }

        public bool IsReady
        {
            get
            {
                return IsConnected && !IsInError;
            }
        }

        public bool ConfigFileExists
        {
            get;
            set;
        }

        public string ConnectionErrorMessage { get; set; }

        public UnityDeviceSWType SWType { get; set; }

        public System.IO.DriveInfo Disk { get; set; }

        public string NavigationSWDirectory { get; set; }
        public string KoboDirectory { get; set; }

        public string ConfigSkriptFilePath { get; set; }
    }
}
