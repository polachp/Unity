using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityConfig.Properties;
using NLog;
using NLog.Fluent;

namespace UnityConfig.UnityConfiguration
{
    public static class UnityDeviceManager
    {
        static readonly Logger logger;
        static UnityDeviceManager()
        {
            logger = LogManager.GetCurrentClassLogger();
        }
                
        private static DriveInfo GetKoboDriveInfo()
        {
            var drives = DriveInfo.GetDrives();
            var nameToCheck = Settings.Default.Unity_Device_Default_VolumeLabel.ToLower();
            foreach (var drive in drives)
            {
                var driveName = "uknown";
                try
                {
                    driveName = drive.Name;
                    logger.Trace("Drive with name {0}", driveName);
                    var driveVolumeName = drive.VolumeLabel.ToLower();
                    if(driveVolumeName == nameToCheck)
                    {
                        return drive;
                    }
                }
                catch(Exception ex)
                {   
                    logger.Error("Error by comparing drive Volume Label.", ex);
                }
            }

            return null;
        }

        public static UnityDevice TryGetDeviceInformation()
        {
            var result = new UnityDevice();
            //var diskInfo = DriveInfo.GetDrives().FirstOrDefault(di => di.VolumeLabel.ToLower() == Settings.Default.Unity_Device_Default_VolumeLabel.ToLower());
            //var diskInfo = DriveInfo.GetDrives().FirstOrDefault(di => di.VolumeLabel == "NEW");
            var diskInfo = GetKoboDriveInfo();

            result.IsConnected = diskInfo != null;

            if (result.IsConnected)
            {
                result.Disk = diskInfo;
                var allKoboDirectories = result.Disk.RootDirectory.GetDirectories();
                var lkDirectory = allKoboDirectories.FirstOrDefault(d => d.Name.ToLower() == "LK8000".ToLower());
                var xcSoarDirectory = allKoboDirectories.FirstOrDefault(d => d.Name.ToLower() == "XCsoar".ToLower());

                if (lkDirectory == null && xcSoarDirectory == null)
                {
                    result.ConnectionErrorMessage = "No SW directory was found (LK8000, XCSoar).";
                    return result;
                }

                if (lkDirectory != null && xcSoarDirectory != null)
                {
                    result.ConnectionErrorMessage = "More than one SW directory was found (LK8000, XCSoar...).";
                    return result;
                }

                DirectoryInfo swDirectory = null;
                DirectoryInfo koboDirectory = null;

                if (lkDirectory != null)
                {
                    result.SWType = UnityDeviceSWType.LK8000;
                    swDirectory = lkDirectory;
                }

                if (xcSoarDirectory != null)
                {
                    result.SWType = UnityDeviceSWType.XCSoar;
                    swDirectory = xcSoarDirectory;
                }

                if (result.SWType != UnityDeviceSWType.None)
                {
                    koboDirectory = lkDirectory.GetDirectories().FirstOrDefault(d => d.Name.ToLower() == Settings.Default.Unity_Device_Default_KoboDirectory.ToLower());

                    if (koboDirectory == null)
                    {
                        result.ConnectionErrorMessage = "No Kobo directory in SW directory was found.";
                        return result;
                    }
                    else
                    {
                        result.KoboDirectory = koboDirectory.FullName;

                        var configScriptFile = koboDirectory.GetFiles().FirstOrDefault(f => f.Name.ToLower() == Settings.Default.Unity_Device_Default_ConfigFileName.ToLower());
                        if(configScriptFile != null)
                        {
                            result.ConfigSkriptFilePath = configScriptFile.FullName;
                            result.ConfigFileExists = true;
                        }
                        else
                        {
                            result.ConfigSkriptFilePath = Path.Combine(result.KoboDirectory, Settings.Default.Unity_Device_Default_ConfigFileName);
                            result.ConfigFileExists = false;
                        }
                    }

                    result.NavigationSWDirectory = swDirectory.FullName;
                }
            }

            return result;
        }
    }
}
