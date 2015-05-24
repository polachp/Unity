using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityConfig.UnityConfiguration;

namespace UnityConfig
{
    public class ConfigurationControlSet
    {
        public ISettingProperty Property {get; set;}
        public System.Windows.Forms.Control ValueControl { get; set; }
        public System.Windows.Forms.Label ValueLabel { get; set; }
        public System.Windows.Forms.Label PropertyLabel { get; set; }
    }
}
