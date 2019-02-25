using Assets.Scripts.Battle.Enemy;
using UnityEngine;

namespace Assets.Scripts.Battle.Bullet
{
    /// <summary>
    /// 子弹的管理者
    /// </summary>
    public class BulletManager:BaseManager
    {
        //构造函数  初始十发子弹
        public BulletManager()
        {
            _mutipleTypeObjectPool.AddObjectPool<TankBullet>(10);
        }
        //
        public override void Update()
        {
            _mutipleTypeObjectPool.Update();
        }
        //产生子弹
        public void CreateTankBullet(Transform createTransform,Transform targeTransform,BaseEnemy targetEnemy,int damage)
        {
            _mutipleTypeObjectPool.AddObjectToPool<TankBullet>(_mutipleTypeObjectPool.GetObjectFromPool
                <TankBullet>().Init(createTransform, targeTransform, targetEnemy,damage));
        }
    }
}
