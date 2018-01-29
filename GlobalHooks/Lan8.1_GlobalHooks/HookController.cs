using System.Diagnostics;
using System.Windows.Forms;
using Gma.System.MouseKeyHook;

namespace Lan8._1_GlobalHooks
{
    internal class HookController
    {
        public delegate void ShowMainWindowHandler();

        public event ShowMainWindowHandler ShowWindow;

        private readonly IKeyboardMouseEvents _globalHook = Hook.GlobalEvents();

        private readonly System.Timers.Timer _timer = new System.Timers.Timer(2000);

        private readonly Stopwatch _tick = new Stopwatch();

        private readonly Logger _logController;

        public HookController(ConfigFile config)
        {
            _logController = new Logger(config);
            _globalHook.KeyDown += KeyDown;
            _globalHook.MouseClick += MouseClick;
        }

        private void MouseClick(object sender, MouseEventArgs e)
        {
            _logController.LogMouse(e.Button.ToString(), e.Location.ToString());
        }

        private void KeyDown(object sender, KeyEventArgs e)
        {

            switch (e.KeyData)
            {
                case Keys.A:
                    _tick.Restart();
                    e.Handled = true;
                    break;
                case Keys.RControlKey:
                    ShowWindow?.Invoke();
                    e.Handled = true;
                    break;
                default:
                    if (_tick.IsRunning && _tick.Elapsed.Seconds < 2)
                    {
                        e.SuppressKeyPress = true;
                    }

                    if (_tick.IsRunning && _tick.Elapsed.Seconds > 2)
                    {
                        _tick.Stop();
                    }
                    break;
            }
            _logController.LogKeyboard(e.KeyData.ToString());
        }
    }
}
