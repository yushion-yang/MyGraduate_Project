using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.ObjectPool;
using Assets.Scripts.SingletonSpace;

namespace Assets.Scripts
{
    public class BaseManager:BaseObject
    {
        //多类型对象池
        protected MutipleTypeObjectPool _mutipleTypeObjectPool;

        public BaseManager()
        {
            _mutipleTypeObjectPool = new MutipleTypeObjectPool();
        }

        public override void Update()
        {
            _mutipleTypeObjectPool.Update();
        }
        //移除对象
        public void RemoveObject<T>(BaseObject baseBullet)
        {
            _mutipleTypeObjectPool.RecycleObject<T>(baseBullet);
        }

    }
}
