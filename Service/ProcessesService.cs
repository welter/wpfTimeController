using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wpfTimeController.Model;
namespace wpfTimeController.Service
{
    class ProcessesService : IProcessesService
    {
        private List <ProcessInfo> _P1=new List<ProcessInfo>();
        public ProcessInfo GetProcess(string name)
        {
            return Array.Find(_P1.ToArray(), x => x.ProcessName == name);
        }
        public ProcessInfo GetProcess(UInt32 id)
        {
            return Array.Find(_P1.ToArray(), x => x.PID == id);
        }
        public ProcessInfo GetProcessByTitle(string title)
        {
            return Array.Find(_P1.ToArray(), x => x.ProcessTitle == title);
        }
        public List<ProcessInfo> GetAllProcess()
        {
            return _P1;
        }
        public ProcessesService()
        {
            var p = new ProcessInfo();
            p.ProcessName = "test";
            _P1.Add(p);
        }
    }

}
