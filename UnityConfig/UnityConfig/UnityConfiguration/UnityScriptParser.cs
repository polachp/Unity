using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NLog;
using NLog.Fluent;
using System.IO;

namespace UnityConfig.UnityConfiguration
{
//printf '$UNLT,10*' > $TTY_DEV
//sleep 1
//printf '$UNST,200*' > $TTY_DEV
//sleep 1
//printf '$UNSEN,124*' > $TTY_DEV
//sleep 1
//printf '$UNBF,2750*' > $TTY_DEV
//sleep 1
//printf '$UNBR,100*' > $TTY_DEV
//sleep 1
//printf '$UNLV,9*' > $TTY_DEV
//sleep 1
//printf '$UNLBF,550*' > $TTY_DEV
//sleep 1
//printf '$UNSPA,100*' > $TTY_DEV
//sleep 1
//printf '$UNSPB,0*' > $TTY_DEV
//sleep 1
//printf '$UNSAVE*' > $TTY_DEV


    public class UnityScriptParser
    {
        public UnityScriptParser()
        {

        }

        //public UnitySetting ParseScriptFromFile(string fileName)
        //{

        //}

        
        public static string xxx = @"printf '$UNLT,10*' > $TTY_DEV
sleep 1
printf '$UNST,211*' > $TTY_DEV
sleep 1
printf '$UNSEN,155*' > $TTY_DEV
sleep 1
printf '$UNBF,2999*' > $TTY_DEV
sleep 1
printf '$UNBR,111*' > $TTY_DEV
sleep 1
printf '$UNLV,7*' > $TTY_DEV
sleep 1
printf '$UNLBF,999*' > $TTY_DEV
sleep 1
printf '$UNSPA,75*' > $TTY_DEV
sleep 1
printf '$UNSPB,3*' > $TTY_DEV
sleep 1
printf '$UNSAVE*' > $TTY_DEV";

        public static UnitySetting ParseScriptFromFile(string scriptFileName, out bool error)
        {
            error = false;
            if(!File.Exists(scriptFileName))
            {
                error = true;
                return null;
            }

            var script = File.ReadAllText(scriptFileName);

            return ParseScript(script, out error);
        }
        public static UnitySetting ParseScript(string script, out bool error)
        {
            error = false;
            var logger = NLog.LogManager.GetCurrentClassLogger();
            logger.Trace("ParseScript starting...");
            logger.Trace("script: \r\n{0}", script);

            var result = UnityConfiguration.UnitySetting.CreateDefault();
            if(string.IsNullOrEmpty(script))
            {
                logger.Trace("ParseScript - script is empty.");
                return null;
            }

            logger.Trace("Parsing lines...");
            
            var scriptLines = script.Split(new[] { "\r\n", "\r", "\n" }, StringSplitOptions.None);
            logger.Trace("script lines count: {0}", scriptLines.Length);
            
            var lines = scriptLines.Where(l => l.Contains("$UN")).ToList();
            logger.Trace("script lines with $UN count: {0}", lines.Count);

            //Remove alll before $UN and $
            lines = lines.Select(l =>
                {
                    var ind = l.IndexOf("$UN");
                    var res = l.Substring(ind + 1, l.Length - ind - 1);
                    return res;
                }).ToList();
            logger.Trace("script lines cleaning before $UN: \r\n{0}", string.Join(Environment.NewLine, lines.ToArray()));

            //Remove all after * and *
            lines = lines.Select(l =>
            {
                var ind = l.IndexOf("*");
                var res = l.Substring(0, ind);
                return res;
            }).ToList();
            logger.Trace("script lines cleaning after *: \r\n{0}", string.Join(Environment.NewLine, lines.ToArray()));

            //Prepare dictionary
            var dic = new Dictionary<string, string>();
            foreach (var line in lines)
            {
                logger.Trace("script line to parse: {0}", line ?? "NULL");
                var vals = line.Split(',');
                logger.Trace("script line parsing length: {0}", vals.Length);
                if(vals.Length != 2)
                {
                    if(vals.Length == 1 && vals[0] == "UNSAVE")
                    {
                        continue;
                    }
                    error = true;
                    logger.Warn("Line {0} could not be parsed.", line);
                    continue;
                }

                var key = vals[0];
                var value = vals[1];
                logger.Trace("line key={0} and value={1}", key ?? "NULL", value ?? "NULL");

                if(dic.ContainsKey(key))
                {
                    logger.Warn("Value with key {0} exists.", key ?? "NULL");
                    dic.Remove(key);
                }

                dic.Add(key, value);
                logger.Trace("Key and value added.");
            }

            logger.Trace("script lines parsed into key count: {0}", dic.Count);

            //SET Setting
            logger.Trace("Setting settings from scripts...");

            foreach (var keyValue in dic)
            {
                var baseMessage = string.Format("Property with key {0}|{1}", keyValue.Key, keyValue.Value);
                logger.Trace("{0} - {1}", baseMessage, "Finding property...");
                var property = result.AllProperties.FirstOrDefault(p => p.PacketCode == keyValue.Key);
                logger.Trace("{0} - Property found = {1}", baseMessage, property != null);
                if (property != null)
                {
                    try
                    {
                        property.SetPropertyFromString(keyValue.Value);
                        logger.Trace("{0} - {1}", baseMessage, "Property setted.");
                    }
                    catch (Exception ex)
                    {
                        logger.Error("{0} - Error by parsing value.", baseMessage);
                        error = true;
                    }
                }
            }

            return result;
        }
    }
}
