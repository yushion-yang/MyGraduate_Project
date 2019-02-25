using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.Enemy;
using UnityEngine;

namespace Assets.Scripts
{
    /// <summary>
    /// 对象池管理者
    /// 注意事项：模板中需要调用的抽象方法在where中执行抽象基类类型然后就可以调用
    /// 限制new则在类中提供无参构造函数进行默认的初始化，然后在通过GetObject获取对象再进行属性设置
    /// </summary>

    public class BaseSingleTypeObjectPool:BaseObject
    {
        //对象链表
        public List<BaseObject> _objectList { get; set; }
        //为使用的对象链表
        protected List<BaseObject> _unuseList;

        //构造函数 初始对象池的数量
        public BaseSingleTypeObjectPool(int objectNum = 0)
        {
            _objectList = new List<BaseObject>();
            _unuseList = new List<BaseObject>(objectNum);
        }
        //遍历更新
        public override void Update()
        {
            for (int i = 0; i < _objectList.Count; i++)
            {
                _objectList[i].Update();
            }
        }
        //添加单个对象
        public void AddObject(BaseObject obBaseObject)
        {
            _objectList.Add(obBaseObject);
            _unuseList.Remove(obBaseObject);
        }
        //添加多个对象
        public void AddObject(BaseObject[] obBaseObjects)
        {
            foreach (BaseObject baseObject in obBaseObjects)
            {
                _objectList.Add(baseObject);
            }
        }
        //回收对象 即在链表中转移
        public void RemoveObject(BaseObject obBaseObject)
        {
            _unuseList.Add(obBaseObject);
            _objectList.Remove(obBaseObject);
        }
        //从为使用的对象中获取一个对象  如果没有则创建新的对象
        public virtual BaseObject GetObject()
        {
            return null;
        }
    }
}
