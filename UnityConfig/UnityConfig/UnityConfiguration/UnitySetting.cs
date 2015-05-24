using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityConfig.Properties;

namespace UnityConfig.UnityConfiguration
{
    public class UnitySetting
    {
        public UnitySetting()
        {
            //vario
            this.LiftTreshold = SettingPropertyManager.CreateDouble100(title: Resources.PROPERTY_LiftTreshold_Title, name: "LiftTreshold", packetCode: "UNLT", formatLabelValue: "{0} m/s", defaultValue: 0.1m, minValue: 0m, maxValue: 15m, description: Resources.PROPERTY_LiftTreshold_Description);
            this.SinkTreshold = SettingPropertyManager.CreateDouble100(title: Resources.PROPERTY_SinkTreshold_Title, name: "SinkTreshold", packetCode: "UNST", formatLabelValue: "{0} m/s", defaultValue: 2m, minValue: 0m, maxValue: 15m, description: Resources.PROPERTY_SinkTreshold_Description);
            this.Sensitivity = SettingPropertyManager.CreateInt(title: Resources.PROPERTY_Sensitivity_Title, name: "Sensitivity", formatLabelValue: "{0} %", packetCode: "UNSEN", defaultValue: 85, minValue: 0, maxValue: 100, description: Resources.PROPERTY_Sensitivity_Description);
            this.Sensitivity.FromControlValueFunc = SettingPropertyManager.IntSensitivityToIntFromControlValue;
            this.Sensitivity.AsControlValueFunc = SettingPropertyManager.IntToIntSensitivityToControlValue;

            //sounds
            this.BaseFrequency = SettingPropertyManager.CreateDouble(title: Resources.PROPERTY_BaseFrequency_Title, name: "BaseFrequency", packetCode: "UNBF", formatLabelValue: "{0} Hz", defaultValue: 2750, minValue: 500, maxValue: 3000, description: Resources.PROPERTY_BaseFrequency_Description);
            this.BeepRateMultiplier = SettingPropertyManager.CreateInt(title: Resources.PROPERTY_BeepRateMultiplier_Title, name: "BeepRateMultiplier", packetCode: "UNBR", formatLabelValue: "{0}", defaultValue: 100, minValue: 70, maxValue: 130, description: Resources.PROPERTY_BeepRateMultiplier_Description);

            //Low Sound
            this.LowSoundVolume = SettingPropertyManager.CreateInt(title: Resources.PROPERTY_LowSoundVolume_Title, name: "LowSoundVolume", packetCode: "UNLV", formatLabelValue: "{0}", defaultValue: 9, minValue: 1, maxValue: 9, description: Resources.PROPERTY_LowSoundVolume_Description);
            this.LowSoundBaseFrequency = SettingPropertyManager.CreateDouble(title: Resources.PROPERTY_LowSoundBaseFrequency_Title, name: "LowSoundBaseFrequency", packetCode: "UNLBF", formatLabelValue: "{0} Hz", defaultValue: 550m, minValue: 550m, maxValue: 3000m, description: Resources.PROPERTY_LowSoundBaseFrequency_Description);

            //Airspeed compensation
            this.AirspeedCompensationAParam = SettingPropertyManager.CreateDouble100(title: Resources.PROPERTY_AirspeedCompensationAParam_Title, name: "AirspeedCompensationAParam", packetCode: "UNSPA", formatLabelValue: "{0}", defaultValue: 1m, minValue: null, maxValue: null, description: Resources.PROPERTY_AirspeedCompensationAParam_Description);
            this.AirspeedCompensationBParam = SettingPropertyManager.CreateDouble100(title: Resources.PROPERTY_AirspeedCompensationBParam_Title, name: "AirspeedCompensationBParam", packetCode: "UNSPB", formatLabelValue: "{0}", defaultValue: 0m, minValue: null, maxValue: null, description: Resources.PROPERTY_AirspeedCompensationBParam_Description);
        }

        public SettingProperty<int, decimal> LiftTreshold { get; private set; }
        public SettingProperty<int, decimal> SinkTreshold { get; private set; }
        public SettingProperty<int, int> Sensitivity { get; private set; }
        
        public SettingProperty<decimal, decimal> BaseFrequency { get; private set; }
        public SettingProperty<int, int> BeepRateMultiplier { get; private set; }
        
        public SettingProperty<int, int> LowSoundVolume { get; private set; }
        public SettingProperty<decimal, decimal> LowSoundBaseFrequency { get; private set; }
        
        public SettingProperty<int, decimal> AirspeedCompensationAParam { get; private set; }
        public SettingProperty<int, decimal> AirspeedCompensationBParam { get; private set; }
        public List<ISettingProperty> AllProperties
        {
            get
            {
                List<ISettingProperty> list = new List<ISettingProperty>();

                var properties = this.GetType().GetProperties().Where(p => typeof(ISettingProperty).IsAssignableFrom(p.PropertyType)).ToList();

                properties.ForEach(p =>
                {
                    var val = p.GetValue(this);
                    list.Add((ISettingProperty)val);
                });

                return list;
            }
        }

        public static UnitySetting CreateDefault()
        {
            var setting = new UnityConfiguration.UnitySetting();
            setting.AllProperties.ForEach(p => p.SetPropertyAsDefault());
            return setting;
        }
    }
}
