using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnityConfig.UnityConfiguration
{
    public class SettingProperty<TValue, TControlValue> : ISettingProperty
        where TControlValue : struct
    {


        public SettingProperty()
        {
            this.AsPacketValueFunc = (v) => { return string.Format("{0}", v); };
            this.FormatLabelValue = "{0}";
        }

        public string Name { get; set; }
        public string PacketCode { get; set; }
        public TControlValue DefaultValue { get; set; }
        public TControlValue? MinValue { get; set; }
        public TControlValue? MaxValue { get; set; }
        public TValue Value { get; set; }

        public string FormatLabelValue { get; set; }

        public Func<TValue, string> AsPacketValueFunc { get; set; }
        public Func<TValue, TControlValue> AsControlValueFunc { get; set; }

        public Func<TControlValue, TValue> FromControlValueFunc { get; set; }

        public Func<TControlValue, string> ValidateControlFunc { get; set; }


        public void SetLabel(System.Windows.Forms.Label label)
        {
            label.Text = this.Name;
        }

        public void SetControlProperties(ConfigurationControlSet set, System.Windows.Forms.Control control)
        {
            //Control
            if (control is System.Windows.Forms.NumericUpDown)
            {
                var c = (control as System.Windows.Forms.NumericUpDown);
                if (this.MinValue.HasValue)
                    c.Minimum = (decimal)(object)this.MinValue;
                if (this.MaxValue.HasValue)
                    c.Maximum = (decimal)(object)this.MaxValue;

                if (set.ValueLabel != null)
                {
                    c.ValueChanged += (sender, e) =>
                    {
                        this.ControlValue = (TControlValue)(object)c.Value;
                        set.ValueLabel.Text = string.Format(FormatLabelValue ?? "{0}", ControlValue, Value);
                    };
                }
            }

            if (control is System.Windows.Forms.TrackBar)
            {
                var c = (control as System.Windows.Forms.TrackBar);
                if (this.MinValue.HasValue)
                    c.Minimum = (int)(object)this.MinValue;
                if (this.MaxValue.HasValue)
                    c.Maximum = (int)(object)this.MaxValue;


                if (set.ValueLabel != null)
                {
                    c.ValueChanged += (sender, e) =>
                    {
                        this.ControlValue = (TControlValue)(object)c.Value;
                        set.ValueLabel.Text = string.Format(FormatLabelValue ?? "{0}", ControlValue, Value);
                    };
                }
            }
        }
        public void SetControlValue(ConfigurationControlSet set, System.Windows.Forms.Control control)
        {
            if (control is System.Windows.Forms.TrackBar)
            {
                var c = (control as System.Windows.Forms.TrackBar);
                //c.Value = c.Minimum;
                //c.Value = c.Maximum;
                c.Value = (int)(object)this.ControlValue;

                if (set.ValueLabel != null)
                {
                    set.ValueLabel.Text = string.Format(FormatLabelValue ?? "{0}", ControlValue, Value);
                }
            }

            if (control is System.Windows.Forms.NumericUpDown)
            {
                var c = (control as System.Windows.Forms.NumericUpDown);
                //c.Value = c.Minimum;
                //c.Value = c.Maximum;
                c.Value = (decimal)(object)this.ControlValue;

                if (set.ValueLabel != null)
                {
                    set.ValueLabel.Text = string.Format(FormatLabelValue ?? "{0}", ControlValue, Value);
                }
            }
        }

        public void ReadControlValue(System.Windows.Forms.Control control)
        {
            if (control is System.Windows.Forms.TrackBar)
            {
                this.ControlValue = (TControlValue)(object)(control as System.Windows.Forms.TrackBar).Value;

            }
            if (control is System.Windows.Forms.NumericUpDown)
            {
                this.ControlValue = (TControlValue)(object)(control as System.Windows.Forms.NumericUpDown).Value;
            }
        }

        public TControlValue ControlValue
        {
            get { return this.AsControlValueFunc(this.Value); }
            set { this.Value = FromControlValueFunc(value); }
        }

        public bool Validate(TControlValue valueToValidate, out string message)
        {
            message = ValidateControlFunc(valueToValidate);
            return string.IsNullOrEmpty(message);
        }

        public void SetPropertyAsDefault()
        {
            this.ControlValue = this.DefaultValue;
        }

        public void SetPropertyFromString(string stringValue)
        {
            if (typeof(TValue).Name == typeof(decimal).Name)
            {
                var intValue = (TValue)(object)decimal.Parse(stringValue);
                this.Value = intValue;
            } else
            {
                var intValue = (TValue)(object)Int32.Parse(stringValue);
                this.Value = intValue;
            }
        }

        public string GetPacket()
        {
//printf '$UNLT,5*' > $TTY_DEV
//sleep 1

            return string.Format("printf '${0},{1}*' > $TTY_DEV\r\nsleep 1", this.PacketCode, this.AsPacketValueFunc(this.Value));
        }

        public string Description { get; set; }

        public string Title { get; set; }
    }
}
