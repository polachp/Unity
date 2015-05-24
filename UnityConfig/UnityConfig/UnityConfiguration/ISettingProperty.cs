using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnityConfig.UnityConfiguration
{
    public interface ISettingProperty
    {
        string Name { get; }
        string Title { get; }
        string PacketCode { get; }
        string FormatLabelValue { get; }

        void SetControlProperties(ConfigurationControlSet set, System.Windows.Forms.Control control);
        void SetControlValue(ConfigurationControlSet set, System.Windows.Forms.Control control);
        void ReadControlValue(System.Windows.Forms.Control control);
        void SetPropertyAsDefault();

        void SetPropertyFromString(string stringValue);

        string GetPacket();

        string Description { get; set; }
    }
}