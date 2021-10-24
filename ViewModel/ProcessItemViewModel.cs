using Microsoft.Practices.Prism.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wpfTimeController.Model;

namespace wpfTimeController.ViewModel
{
    class ProcessItemViewModel : NotificationObject
    {
        public ProcessInfo ProcessInfo { get; set; }
        //将IsSelected属性和ProcessInfo中的属性一起作为ProcessViewItemViewModel里的属性
        private bool isSelected;

        public bool IsSelected
        {
            get { return isSelected; }
            set
            {
                //RaisePorpertyChanged方法，源于引入的Microsoft.Practices.Prism.ViewModel中的dll属性
                isSelected = value;
                RaisePropertyChanged("IsSelected");//"IsSelected"属性值变化之后，自动通知使用该属性的方法，有点观察者模式的意思
            }
        }
    }
}
