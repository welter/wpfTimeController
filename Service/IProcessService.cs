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
        ProcessInfo GetProcess(string name);

        ProcessInfo GetProcess(UInt32 id);

        ProcessInfo GetProcessByTitle(string title);

    }

}
