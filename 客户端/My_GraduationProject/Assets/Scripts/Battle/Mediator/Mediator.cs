using Assets.Scripts.Battle.Bullet;
using Assets.Scripts.Battle.Map;
using Assets.Scripts.Battle.Turret;
using Assets.Scripts.SingletonSpace;
using UnityEngine;

namespace Assets.Scripts.Battle.MediatorSpace
{
    /// <summary>
    /// 中介者
    /// </summary>
    public class Mediator:Singleton<Mediator>
    {
        public TurretManager _turretManager { get; set; }
        public MapManager _mapManager { get; set; }
        public BulletManager _bulletManager { get; set; }

        public Mediator()
        {

        }
    }
}
