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
        public List<ProcessInfo> GetProcess(string name)
        {
            return Array.FindAll(_P1.ToArray(), x => x.ProcessName == name).ToList();
        }
        public List<ProcessInfo> GetProcess(UInt32 id)
        {
            return Array.FindAll(_P1.ToArray(), x => x.PID == id).ToList();
        }
        public List<ProcessInfo> GetProcessByTitle(string title)
        {
            return Array.FindAll(_P1.ToArray(), x => x.ProcessTitle == title).ToList();
        }
        public ProcessesService()
        {
            var p = new ProcessInfo();
            p.ProcessName = "test";
            _P1.Add(p);
        }
    }

}
