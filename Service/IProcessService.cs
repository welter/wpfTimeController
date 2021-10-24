using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wpfTimeController.Model;

namespace wpfTimeController.Service
{
    public interface IProcessesService
    {
        List<ProcessInfo> GetProcess(string name);

        List<ProcessInfo> GetProcess(UInt32 id);

        List<ProcessInfo> GetProcessByTitle(string title);

    }

}
