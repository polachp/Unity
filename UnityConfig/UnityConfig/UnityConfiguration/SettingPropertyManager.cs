using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnityConfig.UnityConfiguration
{
    public class SettingPropertyManager
    {
        static Func<decimal, int> DecimalToInt = (v) => Convert.ToInt32(v);
        static Func<int, decimal> IntToDecimal = (v) => Convert.ToDecimal(v);
        static Func<decimal, int> DecimalToInt100 = (v) => Convert.ToInt32(v * 100);
        static Func<int, decimal> IntToDecimal100 = (v) => Convert.ToDecimal((double)v / 100);
        static Func<int, int> IntToInt100 = (v) => v / 100;
        static Func<int, int> Int100ToInt = (v) => v * 100;
        static Func<decimal, decimal> DecimalToDecimal = (v) => v;
        static Func<int, int> IntToInt = (v) => v;

        public static Func<int, int> IntToIntSensitivityToControlValue = (v) => Convert.ToInt32(Convert.ToDouble((v - 30)) / 1.1);
        public static Func<int, int> IntSensitivityToIntFromControlValue = (v) => Convert.ToInt32((Convert.ToDouble(v) * (1.1)) + 30);

        public static SettingProperty<int, int> CreateInt(string title, string name, string packetCode, string formatLabelValue, int defaultValue, int? minValue, int? maxValue, string description)
        {
            var val = Create<int, int>(title, name, packetCode, formatLabelValue, defaultValue, minValue, maxValue, description);
            val.FromControlValueFunc = IntToInt;
            val.AsControlValueFunc = IntToInt;
            val.ValidateControlFunc = (v) => v == 0 ? "0la" : null;
            return val;
        }

        public static SettingProperty<int, int> CreateInt100(string title, string name, string packetCode, string formatLabelValue, int defaultValue, int? minValue, int? maxValue, string description)
        {
            var val = Create<int, int>(title, name, packetCode, formatLabelValue, defaultValue, minValue, maxValue, description);
            val.FromControlValueFunc = Int100ToInt;
            val.AsControlValueFunc = IntToInt100;
            val.ValidateControlFunc = (v) => v == 0 ? "0la" : null;
            return val;
        }

        public static SettingProperty<decimal, decimal> CreateDouble(string title, string name, string packetCode, string formatLabelValue, decimal defaultValue, decimal? minValue, decimal? maxValue, string description)
        {
            var val = Create<decimal, decimal>(title, name, packetCode, formatLabelValue, defaultValue, minValue, maxValue, description);
            val.FromControlValueFunc = DecimalToDecimal;
            val.AsControlValueFunc = DecimalToDecimal;
            val.ValidateControlFunc = (v) => v == 0 ? "0la" : null;
            return val;
        }

        public static SettingProperty<int, decimal> CreateDouble100(string title, string name, string packetCode, string formatLabelValue, decimal defaultValue, decimal? minValue, decimal? maxValue, string description)
        {
            var val = Create<int, decimal>(title, name, packetCode, formatLabelValue, defaultValue, minValue, maxValue, description);
            val.FromControlValueFunc = DecimalToInt100;
            val.AsControlValueFunc = IntToDecimal100;
            val.ValidateControlFunc = (v) => v == 0 ? "0la" : null;
            return val;
        }

        private static SettingProperty<TValue, TControlValue> Create<TValue, TControlValue>(string title, string name, string packetCode, string formatLabelValue, TControlValue defaultValue, TControlValue? minValue, TControlValue? maxValue, string description)
            where TValue : struct
            where TControlValue : struct
        {
            var val = new SettingProperty<TValue, TControlValue>();
            val.Name = name;
            val.Title = title;
            val.PacketCode = packetCode;
            val.DefaultValue = defaultValue;
            val.MinValue = minValue;
            val.MaxValue = maxValue;
#if DEBUG
            val.FormatLabelValue = formatLabelValue + " | {1}" ?? "{0} | {1}";
#else
            val.FormatLabelValue = formatLabelValue ?? "{0}";
#endif
            val.Description = description;
            return val;
        }
    }
}
