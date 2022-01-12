using Microsoft.Practices.Prism.ViewModel;
using System;

namespace wpfTimeController.Model
{
    enum RunningRule { };
    enum LimitRule { };
    class ControlTask : NotificationObject
    {
        public UInt16 Id { get; set; }
        public string TaskName { get; set; }
        public string ProgramTitle { get; set; }
        public string ProgramDirectory { get; set; }
        public string RunPath { get; set; }
        public RunningRule Rule { get; set; }
        public DateTime StartTime { get; set; }
        public DateTime EndTime { get; set; }
        public DateTime PerPeriodTime { get; set; }
        public int Times { get; set; }
        public DateTime TotalTime { get; set; }
        public LimitRule Limit { get; set; }

    }
}
