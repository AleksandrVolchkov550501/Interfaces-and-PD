namespace Lan8._1_GlobalHooks
{
    internal class ConfigFile
    {
        public string Email { get; set; }

        private long _fileSize;

        public long FileSize
        {
            get => _fileSize;
            set => _fileSize = value >= 1000 ? value : 1000;
        }

        public bool HideAfterRun { get; set; }
    }
}
