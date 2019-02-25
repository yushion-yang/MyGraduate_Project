using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.Bullet;
using Assets.Scripts.Battle.CameraControll;
using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.Battle.Map;
using Assets.Scripts.Battle.MediatorSpace;
using Assets.Scripts.Battle.ObjectPool;
using Assets.Scripts.Battle.Turret;
using Assets.Scripts.SingletonSpace;

namespace Assets.Scripts
{
    /// <summary>
    /// 战斗管理者
    /// 继承具体对象池 一般用做静态对象存储
    /// </summary>
    public class BattleManager:ConcreteSingleTypeObjectPool<BaseObject>
    {
        //中介者
        private readonly Mediator _mediator;
        //构造函数 添加各系统
        public BattleManager()
        {
            _mediator = Singleton<Mediator>.Instance;
            AddObject(new BaseObject[]
            {
                Singleton<EnemyManager>.Instance,
                _mediator._turretManager = Singleton<TurretManager>.Instance,
                _mediator._bulletManager = Singleton<BulletManager>.Instance,
                _mediator._mapManager = Singleton<MapManager>.Instance,
                Singleton<InputManager>.Instance,
                Singleton<CameraManager>.Instance
            });
        }
    }
}
