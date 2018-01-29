using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lan8._1_GlobalHooks
{
    public partial class Form2 : Form
    {
        private readonly ConfigFile _config;
        private readonly HookController _hookController;
        private readonly ConfigController _configController = new ConfigController();

        public Form2()
        {
            InitializeComponent();
            _config = _configController.ReadConfigurationFromFile();
            _hookController = new HookController(_config);
            _hookController.ShowWindow += MainWindowShow;
            InitializeGui();
        }

        private void InitializeGui()
        {
            emailTextBox.Text = _config.Email ?? string.Empty;
            sixeOfFile.Value = _config.FileSize;
            hideCheckBox.Checked = _config.HideAfterRun;
        }

        private void ButtonSave_Click(object sender, EventArgs e)
        {
            if (!IsValidEmail(emailTextBox.Text))
            {
                MessageBox.Show(@"Некорректный email!");
                return;
            }
            _config.Email = emailTextBox.Text;
            _config.FileSize = (long)sixeOfFile.Value;
            _config.HideAfterRun = hideCheckBox.Checked;
            _configController.SaveConfigurationToFile(_config);
        }

        private void ButtonHide_Click(object sender, EventArgs e)
        {
            Hide();
        }

        private void MainWindowShow()
        {
            Show();
        }

        private bool IsValidEmail(string email) => new Regex("[.\\-_a-z0-9]+@([a-z0-9][\\-a-z0-9]+\\.)+[a-z]{2,6}").IsMatch(email.ToLower());

        private void Form2_Shown(object sender, EventArgs e)
        {
            if (_config.HideAfterRun)
                Hide();
        }
    }
}
