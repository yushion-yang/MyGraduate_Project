using System;
using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Battle.MediatorSpace;
using Assets.Scripts.SingletonSpace;
using UnityEngine;


namespace Assets.Scripts.Battle.Turret
{
    public class TankTurret : BaseTurret
    {
        //如果是特定类型的对象所具备的组件则在该类中获取  否则在子类中获取
        public TankTurret() : base("tank_turret", 0)
        {
            _rotateObject = GameTool.GetComFromObjectsChild<Transform>(_entityGameObject, "Rotate");
            _attackTransform = GameTool.GetComFromObjectsChild<Transform>(_entityGameObject, "AttackDirection");
        }
        //初始化 设置位置
        public override BaseTurret Init(Vector3 createPos)
        {
            _manager = Singleton<TurretManager>.Instance;
            _entityGameObject.transform.position = createPos;
            return this;
        }

        public override void Update()
        {
            Attack();
        }
        //攻击
        protected override void Attack(Action<Transform> action = null)
        {
            base.Attack((targeTransform) =>
            {
                Singleton<Mediator>.Instance._bulletManager.CreateTankBullet(
                    _attackTransform, targeTransform, _targetEnemy, _initDamage);
            });
        }
    }
}
