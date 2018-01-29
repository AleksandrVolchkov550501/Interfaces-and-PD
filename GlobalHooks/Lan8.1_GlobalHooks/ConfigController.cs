using System;
using System.IO;
using System.Linq;
using Newtonsoft.Json;

namespace Lan8._1_GlobalHooks
{
    internal class ConfigController
    {
        private const char Key = 'I';

        public ConfigFile ReadConfigurationFromFile()
        {
            try
            {
                using (var streamReader = new StreamReader(@".\gh.config"))
                {
                    return JsonConvert.DeserializeObject<ConfigFile>(Cipher(streamReader.ReadToEnd())) ??
                           new ConfigFile {FileSize = 100};
                }
            }
            catch (Exception)
            {
                return new ConfigFile
                {
                    FileSize = 100
                };
            }
        }

        public void SaveConfigurationToFile(ConfigFile config)
        {
            using (var streamWriter = new StreamWriter(@".\gh.config", false))
            {
                streamWriter.Write(Cipher(JsonConvert.SerializeObject(config)));
            }
        }

        private string Cipher(string input)
        {
            return input.Aggregate(string.Empty, (current, c) => current + Convert.ToChar(c ^ Key));
        }
    }
}
