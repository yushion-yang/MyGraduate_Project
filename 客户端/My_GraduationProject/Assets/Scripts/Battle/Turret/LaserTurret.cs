using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Flyweight;
using Assets.Scripts.SingletonSpace;
using UnityEngine;
using UnityEngine.TestTools;

namespace Assets.Scripts.Battle.Turret
{
    public class LaserTurret : BaseTurret
    {
        //激光物体
        protected GameObject _laserObject;
        //激光组件
        protected LineRenderer _laserLineRenderer;
        //四个发射口位置
        protected Transform[] _attackTransforms;
        //激光显示时间
        protected float _laserShowTime;

        public LaserTurret() : base("laser_turret", 1)
        {
            _laserObject = GameObject.Instantiate(Singleton<ResourcesFactory>.Instance.GetGameObject("laser"),
                GameTool.noUsePosition, Quaternion.identity);
            _laserLineRenderer = _laserObject.GetComponent<LineRenderer>();
            _laserLineRenderer.SetPositions(new Vector3[] { Vector3.zero, Vector3.zero });
            _rotateObject = GameTool.GetComFromObjectsChild<Transform>(_entityGameObject, "Rotate");
            _attackTransforms = new Transform[4];
            for (int i = 1; i <= 4; i++)
            {
                _attackTransforms[i-1] = GameTool.GetComFromObjectsChild<Transform>(
                    _entityGameObject, "AttackDirection" + i);
            }
            _attackTransform = GameTool.GetComFromObjectsChild<Transform>(_entityGameObject, "AttackDirection1");
        }

        protected override void Attack(Action<Transform> action = null)
        {
            SolveLaserShow();
            base.Attack((targeTransform) =>
            {
                _laserLineRenderer.SetPositions(new Vector3[] { _attackTransform.position, targeTransform.position });
                _laserShowTime = 0;
                _targetEnemy.TakeDamage(_initDamage);
            });
        }
        //处理激光的显示
        private void SolveLaserShow()
        {
            if (_laserShowTime > 0.2f)
            {
                return;
            }
            _laserShowTime += Time.deltaTime;
            if (_laserShowTime > 0.2f)
            {
                _laserLineRenderer.SetPositions(new Vector3[] { Vector3.zero, Vector3.zero });
            }
        }
    }
}
