using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.Battle.ObjectPool;
using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Flyweight;
using Assets.Scripts.SingletonSpace;
using UnityEngine;

namespace Assets.Scripts.Battle.Bullet
{
    /// <summary>
    /// 子弹中的具体类-坦克子弹
    /// </summary>
    public class TankBullet:BaseBullet
    {
        //子弹移动速度
        protected readonly float _moveSpeed = 20f;
        //子弹目标位置
        protected Transform _targetTransform;         
        //子弹的目标敌人
        protected BaseEnemy _targetEnemy;
        //伤害值
        protected int _damage;

        //构造函数 实例化子弹 仅实例化 不设置参数  在使用时设置
        public TankBullet():base("tank_bullet")
        {

        }
        //初始化函数 对象池技术中复用重设参数
        public override BaseBullet Init(Transform createTransform, Transform targeTransform,BaseEnemy targetEnemy,int damage)
        {
            _manager = Singleton<BulletManager>.Instance;
            _entityGameObject.transform.position = createTransform.position;
            _entityGameObject.transform.rotation = createTransform.rotation;
            _targetTransform = targeTransform;
            _targetEnemy = targetEnemy;
            _damage = damage;
            return this;
        }

        public override void Update()
        {
            Move();
        }
        //子弹移动
        private void Move()
        {
            //判断是否有目标
            if (!_targetTransform)
            {
                return;
            }

            if (!_targetEnemy._isActive)
            {
                Recycle();
                return;
            }
            //朝着目标移动
            _entityGameObject.transform.position = Vector3.MoveTowards(_entityGameObject.transform.position,
                _targetTransform.position, _moveSpeed * Time.deltaTime);
            //当子弹到达目标的时候“销毁”子弹
            Vector3 selfVector3 = _entityGameObject.transform.position;
            Vector3 targetVector3 = _targetTransform.position;
            if (Vector3.Distance(selfVector3, targetVector3) < 0.1f)
            {
                Recycle();
                _targetEnemy.TakeDamage(_damage);
            }
        }
        //回收子弹
        private void Recycle()
        {
            _manager.RemoveObject<TankBullet>(this);
            _entityGameObject.transform.position = GameTool.noUsePosition;
        }
    }
}
