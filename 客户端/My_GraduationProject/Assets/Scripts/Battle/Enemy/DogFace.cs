using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Flyweight;
using Assets.Scripts.SingletonSpace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.ObjectPool;
using UnityEngine;

namespace Assets.Scripts.Battle.Enemy
{
    /// <summary>
    /// 敌人的具体类-士兵类
    /// </summary>
    ///
    /// 在管理类中实例化对象 造成在管理类的构造函数中获取类的单例 死循环
    /// 
    public class DogFace:BaseEnemy
    {
        //构造函数 设置基类构造函数的参数
        public DogFace():base("dogface1")
        {

        }
        //初始化敌人的位置
        public DogFace Init(Vector3 createPos)
        {
            _entityGameObject.transform.position = createPos;
            _isActive = true;
            return this;
        }
        //收到伤害
        public override void TakeDamage(int damage)
        {
            _currentHp -= damage;
            //敌人死亡回收
            if (_currentHp <= 0)
            {
                Singleton<EnemyManager>.Instance.RemoveObject<DogFace>(this);
                _entityGameObject.transform.position = GameTool.noUsePosition;
                _isActive = false;
                return;
            }
            SetHpShowValue();
        }
    }
}
