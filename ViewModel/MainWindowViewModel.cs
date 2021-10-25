﻿using Microsoft.Practices.Prism.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wpfTimeController.Model;
using wpfTimeController.Service;
namespace wpfTimeController.ViewModel
{
    class MainWindowViewModel: NotificationObject
    {
        private List<ProcessItemViewModel> activeProcessList;
        public List<ProcessItemViewModel> ActiveProcessList
        {
            get { return activeProcessList; }
            set
            {
                activeProcessList = value;
                this.RaisePropertyChanged("ActiveProcessList");
            }
        }

        public MainWindowViewModel()
        {
            LoadActiveProcesses();
        }
        private void LoadActiveProcesses()
        {
            IProcessesService i = new ProcessesService();
            var PiL=i.GetAllProcess();
            ActiveProcessList = new List<ProcessItemViewModel>();
            foreach (var Pi in PiL)
            {
                var processItem = new ProcessItemViewModel();
                processItem.ProcessInfo = Pi;
                ActiveProcessList.Add(processItem);
            }

        }
    }
}
