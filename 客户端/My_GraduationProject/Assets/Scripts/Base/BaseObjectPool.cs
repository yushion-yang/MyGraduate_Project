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
    /// </summary>

    public class BaseObjectPool
    {
        //对象链表
        public List<BaseObject> _objectList { get; set; }
        //为使用的对象链表
        protected List<BaseObject> _unuseList;

        //构造函数 初始对象池的数量
        public BaseObjectPool(int objectNum = 0)
        {

        }
        //遍历更新
        public void Update()
        {

        }
        //添加单个对象
        public virtual void AddObject(BaseObject obBaseObject)
        {

        }
        //添加多个对象
        public virtual void AddObject(BaseObject[] obBaseObjects)
        {

        }
        //回收对象 即在链表中转移
        public virtual void RemoveObject(BaseObject obBaseObject)
        {

        }
        //从为使用的对象中获取一个对象  如果没有则创建新的对象
        public virtual BaseObject GetObject()
        {
            return null;
        }
    }
}
