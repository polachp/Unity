using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using UnityConfig.UnityConfiguration;
using UnityConfig.Properties;

namespace UnityConfig
{
    public partial class ConfigurationControl : UserControl
    {
        public ConfigurationControl()
        {
            InitializeComponent();

        }

        private void PreparePropertyList(UnitySetting setting)
        {
            this.PropertyList = new List<ConfigurationControlSet>();
            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.LiftTreshold, ValueControl = valLiftTreshold, PropertyLabel = labelLiftTreshold, ValueLabel = vallabelLiftTreshold });
            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.SinkTreshold, ValueControl = valSinkTreshold, PropertyLabel = labelSinkTreshold, ValueLabel = vallabelSinkTreshold });
            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.Sensitivity, ValueControl = valSensitivity, PropertyLabel = labelSensitivity, ValueLabel = vallabelSensitivity });
            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.BaseFrequency, ValueControl = valBaseFreq, PropertyLabel = labelBaseFreq, ValueLabel = vallabelBaseFreq });
            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.BeepRateMultiplier, ValueControl = valBeepRateMul, PropertyLabel = labelBeepRateMul, ValueLabel = vallabelBeepRateMul });

            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.LowSoundVolume, ValueControl = valLowSoundVolume, PropertyLabel = labelLowSoundVolume, ValueLabel = vallabelLowSoundVolume });
            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.LowSoundBaseFrequency, ValueControl = valLowSoundBasefreq, PropertyLabel = labelLowSoundBasefreq, ValueLabel = vallabelLowSoundBasefreq });
            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.AirspeedCompensationAParam, ValueControl = valAirspeedCompensationAParam, PropertyLabel = labelAirspeedCompensationAParam, ValueLabel = vallabelAirspeedCompensationAParam });
            this.PropertyList.Add(new ConfigurationControlSet { Property = setting.AirspeedCompensationBParam, ValueControl = valAirspeedCompensationBParam, PropertyLabel = labelAirspeedCompensationBParam, ValueLabel = vallabelAirspeedCompensationBParam });

            SetControls();
        }

        private void SetControls()
        {
            foreach (var item in this.PropertyList)
            {
                if (item.PropertyLabel != null)
                {
                    item.PropertyLabel.Text = item.Property.Title;
                }
                item.Property.SetControlProperties(item, item.ValueControl);

                if(!string.IsNullOrEmpty(item.Property.Description))
                {
                    toolTip.SetToolTip(item.ValueControl, item.Property.Description);
                }
            }
        }

        private void WriteControlValues()
        {
            foreach (var item in this.PropertyList)
            {
                item.Property.SetControlValue(item, item.ValueControl);
            }
        }

        private void ReadControlValues()
        {
            foreach (var item in this.PropertyList)
            {
                item.Property.ReadControlValue(item.ValueControl);
            }
        }


        private UnitySetting setting;

        private List<ConfigurationControlSet> PropertyList { get; set; }
        private void SetSetting(UnitySetting setting)
        {
            this.setting = setting;

            if (this.setting == null) return;

            PreparePropertyList(this.setting);

            WriteControlValues();
        }

        private UnitySetting GetSetting()
        {
            if (this.setting == null) return null;

            ReadControlValues();

            return this.setting;
        }

        public UnitySetting Setting { get { return GetSetting(); } set { this.SetSetting(value); } }

        private void ConfigurationControl_Load(object sender, EventArgs e)
        {
            this.groupVario.Text = Resources.PROPERTY_GROUP_Vario;
            this.groupSounds.Text = Resources.PROPERTY_GROUP_Sounds;
            this.groupSoundBasics.Text = Resources.PROPERTY_GROUP_Sounds_Basic;
            this.groupLowSound.Text = Resources.PROPERTY_GROUP_Sounds_LowSound;
            this.groupAirspeedCompensation.Text = Resources.PROPERTY_GROUP_AirspeedCompensation;

            labelLow1.Text = Resources.Control_Low;
            labelLow2.Text = Resources.Control_Low;
            labelHigh1.Text = Resources.Control_High;
            labelHigh2.Text = Resources.Control_High;
        }
    }
}
