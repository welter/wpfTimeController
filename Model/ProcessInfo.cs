using Microsoft.Practices.Prism.ViewModel;
using System;

namespace wpfTimeController.Model
{
    public class ProcessInfo : NotificationObject
    {

        public string ProcessName { get; set; }
        public UInt32 PID { get; set; }
        public string Category { get; set; }
        public string ProcessTitle { get; set; }
        public double ProcessCpusage { get; set; }
    }
}
