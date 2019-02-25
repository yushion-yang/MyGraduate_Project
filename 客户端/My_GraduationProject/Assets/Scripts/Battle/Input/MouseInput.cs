using Assets.Scripts.SingletonSpace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.Map;
using Assets.Scripts.Battle.Tools;
using UnityEngine;

namespace Assets.Scripts
{
    /// <summary>
    /// 具体的输入类-鼠标输入
    /// </summary>
    public class MouseInput : BaseObject
    {
        private bool _isTrue;

        public MouseInput()
        {

        }

        public override void Update()
        {
            if (GameTool.IsDoubleHitRMouse())
            {
                //Debug.Log("press key");
                if (_isTrue)
                {
                    Singleton<MapManager>.Instance.OpenShowRedCub();
                    _isTrue = false;
                }
                else
                {
                    Singleton<MapManager>.Instance.CloseShowRedCub();
                    _isTrue = true;
                }
            }
        }
    }
}
