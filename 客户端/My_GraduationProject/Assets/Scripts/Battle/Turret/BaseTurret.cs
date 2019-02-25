using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using Assets.Scripts.Base;
using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Flyweight;
using Assets.Scripts.SingletonSpace;
using UnityEngine;

namespace Assets.Scripts.Battle.Turret
{
    /// <summary>
    /// 炮塔基类
    /// </summary>
    public class BaseTurret:BasePoolObject
    {
        //初始伤害值
        protected  int _initDamage;
        //攻击距离
        protected  int _attackDistance;
        //攻击CD
        protected  float _attackCDInterval;
        //有攻击目标
        protected  bool _hasTarget;
        //上次攻击时间
        protected float _attackTime;
        //炮塔旋转部分
        protected Transform _rotateObject;
        //发射口位置
        protected Transform _attackTransform;
        //攻击目标
        protected BaseEnemy _targetEnemy;

        //TODO 从配置文件中读取数据
        //炮塔属性预设值   攻击伤害 距离 攻击时间间隔
        private readonly float[,] _attribute = new float[,]
        {
            {13,20,0.8f },
            {10,20,1f },
            {10,20,1.5f },
            {10,20,0.2f },
            {10,20,1.1f }
        };
        //初始化
        public virtual BaseTurret Init(Vector3 createPos)
        {
            _manager = Singleton<TurretManager>.Instance;
            _entityGameObject.transform.position = createPos;
            return this;
        }

        //构造函数 实例化对象 初始化属性
        public BaseTurret(string turretName,int turretNO):base(turretName)
        {
            InitAttribute(turretNO);
        }
        //初始化属性
        private void InitAttribute(int turretNO)
        {
            _initDamage = (int)_attribute[turretNO, 0];
            _attackDistance = (int) _attribute[turretNO, 1];
            _attackCDInterval = _attribute[turretNO, 2];
            _hasTarget = false;
            _attackTime = 0;
        }

        public override void Update()
        {
            Attack();
        }

        protected virtual void Attack(Action<Transform> attackAction = null)
        {
            //判断攻击CD
            _attackTime += Time.deltaTime;
            if (_attackTime < _attackCDInterval)
            {
                return;
            }
            //还没有攻击目标
            if (_targetEnemy == null)
            {
                if (!SearchEnemy())
                {
                    return;
                }
            }
            else
            {
                //敌人超出攻击范围 马上寻找  
                if (!GameTool.IsObjectInScope(_targetEnemy._entityGameObject,
                    _entityGameObject.transform.position, _attackDistance))
                {
                    _targetEnemy = null;
                    return;
                    //如果需要延长可以去掉寻找敌人部分代码
                    //if (!SearchEnemy())
                    //{
                    //    return;
                    //}
                }
            }
            //获取攻击坐标
            Transform targeTransform = _targetEnemy._entityGameObject.transform;
            //炮口转向目标
            float ty = _rotateObject.position.y;
            Vector3 tv3 = targeTransform.position;
            _rotateObject.LookAt(new Vector3(tv3.x, ty, tv3.z));
            //产生攻击特效
            attackAction(targeTransform);
            //刷新攻击时间
            _attackTime = 0;
        }
        //查找敌人  策略为 范围内的随机敌人
        protected bool SearchEnemy()
        {
            _targetEnemy = Singleton<EnemyManager>.Instance.SearchRandomEnemyInScope
                (_entityGameObject.transform.position, _attackDistance);
            if (_targetEnemy == null)
            {
                return false;
            }
            return true;
        }
    }
}
