using Assets.Scripts.SingletonSpace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Base;
using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.Battle.Bullet;
using UnityEngine;

namespace Assets.Scripts.Battle.ObjectPool
{
    /// <summary>
    /// 所有同一类型基类对象的对象池
    /// </summary>
    /// 
    /// 尝试实现一个统一管理对象的对象池 功能包括 添加对象类型以及初始数量
    /// 获取某种类型的对象
    /// 回收某种类型的对象
    /// 
    public class MutipleTypeObjectPool
    {
        public Dictionary<string, BaseSingleTypeObjectPool> _objectsDic { get; set; }

        public MutipleTypeObjectPool()
        {
            _objectsDic = new Dictionary<string, BaseSingleTypeObjectPool>();
        }
        //获取指定类型的对象
        public T GetObjectFromPool<T>() where T:BaseObject
        {
            if (!_objectsDic.ContainsKey(typeof(T).ToString()))
            {
                Debug.LogError("there is no key:"+ typeof(T).ToString() + " in dictionary");
                return default(T);
            }
            return _objectsDic[typeof(T).ToString()].GetObject() as T;
        }

        public void Update()
        {
            foreach (KeyValuePair<string,BaseSingleTypeObjectPool> baseManager in _objectsDic)
            {
                baseManager.Value.Update();
            }
        }
        //移除对象
        public void RecycleObject<T>(BaseObject obBaseObject)
        {
            if (!_objectsDic.ContainsKey(typeof(T).ToString()))
            {
                Debug.LogError("there is no key:" + typeof(T).ToString() + " in dictionary");
            }
            _objectsDic[typeof(T).ToString()].RemoveObject(obBaseObject);
        }
        //添加单个对象
        public void AddObjectToPool<T>(BaseObject obBaseObject)
        {
            if (!_objectsDic.ContainsKey(typeof(T).ToString()))
            {
                Debug.LogError("there is no key:" + typeof(T).ToString() + " in dictionary");
            }
            _objectsDic[typeof(T).ToString()].AddObject(obBaseObject);
        }
        //添加某种类型的对象池
        public void AddObjectPool<T>(int objectNum = 10) where T : BaseObject, new()
        {
            _objectsDic[typeof(T).ToString()] = new ConcreteSingleTypeObjectPool<T>(objectNum);
        }
    }
}
