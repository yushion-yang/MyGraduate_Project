using System;
using System.Collections;
using System.Collections.Generic;
using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Flyweight;
using Assets.Scripts.Net;
using Assets.Scripts.SingletonSpace;
using UnityEngine;

namespace Assets.Scripts
{
    /// <summary>
    /// 游戏的入口 相当于C++的main函数
    /// </summary>
    /// 记录两个关于原代码中的问题 一个是NetManager中 SendCallback 函数的 writeQueue.First();
    /// 在使用.net4.6框架后会发生错误（调试的时候） 使用.net4.6是为了使用nameof
    /// 另外一个是Type.GetType函数 需要包括命名空间才能获取到类（对函数不熟悉就是难搞）
    ///
    public class GameMain : MonoBehaviour
    {
        private BaseObject[] _baseObjects;
        //初始化游戏系统
        void Start()
        {
            _baseObjects = new BaseObject[] {
                new BattleManager(),
                new NetManager()
            };
        }
        //游戏主循环
        void Update()
        {
            foreach (BaseObject myObject in (_baseObjects))
            {
                myObject.Update();
            }
        }
    }
}
