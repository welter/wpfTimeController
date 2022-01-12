using System;
using System.Collections.Generic;
using wpfTimeController.Model;

namespace wpfTimeController.Service
{
    public interface IProcessesService
    {
        ProcessInfo GetProcess(string name);

        ProcessInfo GetProcess(UInt32 id);

        ProcessInfo GetProcessByTitle(string title);
        List<ProcessInfo> GetAllProcess();

        List<ProcessInfo> Update();
    }

}
