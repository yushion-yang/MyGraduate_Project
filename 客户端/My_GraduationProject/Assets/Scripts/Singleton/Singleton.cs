using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Assets.Scripts.SingletonSpace
{
    /// <summary>
    /// 单例抽象模板类
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public abstract class Singleton<T> where T : new()
    {
        private static T _instance;
        static readonly object _lock = new object();
        //获取单例
        public static T Instance
        {
            get
            {
                if (_instance == null)
                {
                    lock (_lock)
                    {
                        if (_instance == null)
                            _instance = new T();
                    }
                }
                return _instance;
            }
        }
    }
}
