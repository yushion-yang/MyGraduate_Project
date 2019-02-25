using Assets.Scripts.Battle.ObjectPool;
using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Net;
using Assets.Scripts.Proto;
using Assets.Scripts.SingletonSpace;
using System;
using UnityEngine;

namespace Assets.Scripts.Battle.Turret
{
    /// <summary>
    /// 炮塔管理者
    /// </summary>
    public class TurretManager:BaseManager
    {
        //测试时间
        private float _testTime;
        //炮塔在地图的占位
        private bool[,] _turretMaps = new bool[20,48];
        //构造函数 网络设置监听事件
        public TurretManager()
        {
            NetManager.AddMsgListener("MsgCreateTurret", OnMsgCreateTurret);
            _mutipleTypeObjectPool.AddObjectPool<TankTurret>(10);
            _mutipleTypeObjectPool.AddObjectPool<LaserTurret>(10);
        }

        public override void Update()
        {
            base.Update();
            if (Input.GetMouseButtonDown(0))
            {
                Vector3 pointPos = GetTurretCreatePos();
                if (pointPos != GameTool.noUsePosition)
                {
                    //_mutipleTypeObjectPool.AddObjectToPool<TankTurret>(_mutipleTypeObjectPool.GetObjectFromPool
                     //   <TankTurret>().Init(pointPos));
                }
                //_mutipleTypeObjectPool.AddObjectToPool<LaserTurret>(_mutipleTypeObjectPool.GetObjectFromPool
                //    <LaserTurret>().Init(pointPos));
                _testTime = Time.time;
                MsgCreateTurret msg = new MsgCreateTurret
                {
                    _x = pointPos.x,
                    _y = 0,
                    _z = pointPos.z,
                    _NO = 0
                };
                NetManager.Send(msg);
            }
        }
        //消息处理
        private void OnMsgCreateTurret(MsgBase msgBase)
        {
            MsgCreateTurret msgCreateTurret = (MsgCreateTurret) msgBase;
            switch (msgCreateTurret._NO)
            {
                case 0:
                    //Vector3 pointPos = GetTurretCreatePos();
                    Vector3 pointPos = new Vector3(msgCreateTurret._x,0,msgCreateTurret._z);
                    if (pointPos != GameTool.noUsePosition)
                    {
                        Debug.Log("Create a new turret");
                        _mutipleTypeObjectPool.AddObjectToPool<TankTurret>(_mutipleTypeObjectPool.GetObjectFromPool
                            <TankTurret>().Init(pointPos));
                    }
                    Debug.Log(Time.time - _testTime);
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
            }
        }
        //获取炮塔产生位置
        public Vector3 GetTurretCreatePos()
        {
            Vector3 pointPos = GameTool.GetMouseWorldPosition();
            if (pointPos != GameTool.noUsePosition)
            {
                int h = (int)(pointPos.z + 2.5) / 5;
                int l = (int)(pointPos.x + 2.5) / 5;
                if (_turretMaps[h, l])
                {
                    //TODO 提示用户此处已经有炮塔
                    return GameTool.noUsePosition;
                }
                int x = l * 5;
                int z = h * 5;
                _turretMaps[h, l] = true;
                return new Vector3(x, 0, z);
            }
            else
            {
                return GameTool.noUsePosition;
            }
        }
    }
}
