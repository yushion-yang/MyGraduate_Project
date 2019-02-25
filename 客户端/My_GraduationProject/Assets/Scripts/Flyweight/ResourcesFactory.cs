using Assets.Scripts.SingletonSpace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.Tools;
using UnityEngine;

namespace Assets.Scripts.Flyweight
{
    /// <summary>
    /// 资源工厂  享元模式  存储加载的预制体
    /// </summary>
    public class ResourcesFactory : Singleton<ResourcesFactory>
    {
        private static Dictionary<string,GameObject> _prefabObjects = new Dictionary<string, GameObject>();
        //构造函数
        public ResourcesFactory()
        {

        }
        //获取预制游戏物体
        public GameObject GetGameObject(string prefabName)
        {
            if (_prefabObjects.ContainsKey(prefabName))
            {
                return _prefabObjects[prefabName];
            }
            else
            {
                GameObject gob = Resources.Load<GameObject>(prefabName);
                if (gob)
                {
                    return _prefabObjects[prefabName] = gob;
                }
                else
                {
                    Debug.LogError("cant find prefab in Resources file, prefab name:"+prefabName);
                    return null;
                }
            }
        }
    }
}
