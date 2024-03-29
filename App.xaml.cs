﻿using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace wpfTimeController
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
    }
    public static class DialogHelper
    {
        public static bool? ShowDialog(this Window win, Window owner)
        {
            win.Owner = owner;
            win.ShowInTaskbar = false;
            return win.ShowDialog();
        }
    }
}
