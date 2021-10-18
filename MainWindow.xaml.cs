using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace wpfTimeController
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            InitializeComponent();

            System.Windows.Forms.NotifyIcon ni = new System.Windows.Forms.NotifyIcon();
            ni.Icon = new System.Drawing.Icon("Main.ico");
            ni.Visible = true;
            ni.DoubleClick +=
             delegate (object sender, EventArgs args)
             {
                 var win = new UnlockForm();
                 win.ShowDialog(this);
                 this.Show();
                 this.WindowState = WindowState.Normal;
             };
        }
        protected override void OnStateChanged(EventArgs e)
        {
            if (WindowState == WindowState.Minimized)
                this.Hide();

            base.OnStateChanged(e);
        }


        private void OnClick(object sender, RoutedEventArgs e)
        {

        }

        private void MenuAbout_Click(object sender, RoutedEventArgs e)
        {
            var win = new About();
            win.ShowDialog(this);
        }

        private void MenuHelp_Click(object sender, RoutedEventArgs e)
        {
            var win = new HelpForm();
            win.ShowDialog(this);
        }

        private void MenuRestart_Click(object sender, RoutedEventArgs e)
        {

        }

        private void MenuExit_Click(object sender, RoutedEventArgs e)
        {
            App.Current.Shutdown();
        }

        private void ButtonMin_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
        }
    }
}

