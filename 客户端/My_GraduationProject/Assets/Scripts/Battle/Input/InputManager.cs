using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Assets.Scripts
{
    /// <summary>
    /// 输入管理者
    /// </summary>
    public class InputManager : BaseSingleTypeObjectPool
    {
        public InputManager()
        {
            AddObject(new MouseInput());
        }
    }
}
