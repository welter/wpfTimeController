using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wpfTimeController.Model;
namespace wpfTimeController.Service
{
    class ProcessesService:IProcessesService
    {
        private ProcessInfo[] _P1;
        public ProcessInfo GetProcess(string name)
        {

            return Array.Find(this._P1, x => x.ProcessName == name);
        }
        public ProcessInfo GetProcess(UInt32 id)
        {
            return Array.Find(this._P1, x => x.PID == id);
        }
        public ProcessInfo GetProcessByTitle(string title)
        {
            return Array.Find(this._P1, x => x.ProcessTitle == title);
        }
    }

}
