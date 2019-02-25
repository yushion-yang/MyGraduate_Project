using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Flyweight;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Assets.Scripts.Base
{
    public class BasePoolObject:BaseObject
    {
        //游戏对象的管理者
        protected BaseManager _manager;
        //游戏的实体
        public GameObject _entityGameObject { get; set; }

        //对象池对象的构造函数 实例化游戏场景实体
        public BasePoolObject(string objectName)
        {
            _entityGameObject = GameObject.Instantiate(ResourcesFactory.Instance.
                GetGameObject(objectName), GameTool.noUsePosition, Quaternion.identity);
        }
    }
}
