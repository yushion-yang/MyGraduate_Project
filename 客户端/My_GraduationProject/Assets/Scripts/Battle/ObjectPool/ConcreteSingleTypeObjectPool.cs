using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Assets.Scripts.Battle.ObjectPool
{
    public class ConcreteSingleTypeObjectPool<T> : BaseSingleTypeObjectPool where T : BaseObject, new()
    {
        //构造函数 初始对象池的数量
        public ConcreteSingleTypeObjectPool(int objectNum = 0):base(objectNum)
        {
            for (int i = 0; i < objectNum; i++)
            {
                _unuseList.Add(new T());
            }
        }
        //从为使用的对象中获取一个对象  如果没有则创建新的对象
        public override BaseObject GetObject()
        {
            if (_unuseList.Count > 0)
            {
                return _unuseList[0];
            }
            else
            {
                _unuseList.Add(new T());
                return _unuseList[0];
            }
        }
    }
}
