using System.Collections.Generic;
using Assets.Scripts.Battle.Tools;
using UnityEngine;

namespace Assets.Scripts.Battle.Enemy
{
    /// <summary>
    /// 敌人管理者
    /// </summary>
    public class EnemyManager : BaseManager
    {
        //构造函数
        public EnemyManager()
        {
            _mutipleTypeObjectPool.AddObjectPool<DogFace>(10);
            CreateEnemy();
        }
        //寻找范围内的第一个敌人
        public BaseEnemy SearchEnemyInScope(Vector3 vector3, float dis)
        {
            foreach (KeyValuePair<string, BaseSingleTypeObjectPool> pool in _mutipleTypeObjectPool._objectsDic)
            {
                foreach (BaseEnemy baseEnemy in pool.Value._objectList)
                {
                    if (GameTool.IsObjectInScope(baseEnemy._entityGameObject, vector3, dis))
                    {
                        return baseEnemy;
                    }
                }
            }
            return null;
        }
        //寻找范围内的第一个敌人
        public BaseEnemy SearchRandomEnemyInScope(Vector3 vector3, float dis)
        {
            List<BaseEnemy> tempEnemys = new List<BaseEnemy>();
            foreach (KeyValuePair<string, BaseSingleTypeObjectPool> pool in _mutipleTypeObjectPool._objectsDic)
            {
                foreach (BaseEnemy baseEnemy in pool.Value._objectList)
                {
                    if (GameTool.IsObjectInScope(baseEnemy._entityGameObject, vector3, dis))
                    {
                        tempEnemys.Add(baseEnemy);
                    }
                }
            }
            if (tempEnemys.Count == 0)
                return null;
            int n = Random.Range(0, tempEnemys.Count);
            return tempEnemys[n];
        }
        //设置血量画布的尺寸
        public void SetEnemyHpCanvasScale(float value)
        {
            foreach (KeyValuePair<string, BaseSingleTypeObjectPool> pool in _mutipleTypeObjectPool._objectsDic)
            {
                foreach (BaseEnemy baseEnemy in pool.Value._objectList)
                {
                    baseEnemy.SetHpShowScale(value);
                }
            }
        }

        public override void Update()
        {
            _mutipleTypeObjectPool.Update();
        }
        //创建敌人
        public void CreateEnemy()
        {
            for (int i = 1; i <= 5; i++)
            {
                _mutipleTypeObjectPool.AddObjectToPool<DogFace>(_mutipleTypeObjectPool.GetObjectFromPool
                    <DogFace>().Init(new Vector3(50 + i * 5, 1.5f, i * 5 + 50)));
            }
        }
    }
}
