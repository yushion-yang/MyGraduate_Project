using Assets.Scripts.Flyweight;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.Tools;
using Assets.Scripts.SingletonSpace;
using UnityEngine;
using UnityEngine.UI;
using Assets.Scripts.Base;

namespace Assets.Scripts.Battle.Enemy
{
    /// <summary>
    /// 敌人基类
    /// </summary>
    public class BaseEnemy:BasePoolObject
    {
        //是否存活
        public bool _isActive { get; set; }
        //最大生命值      
        protected int _maxHp;
        //当前生命值
        protected int _currentHp;
        //血条
        protected Slider _hpSlider;
        //血量值文本
        protected GameObject _hpText;
        //血条画布
        protected GameObject _hpCanvas;
        //画布初始尺寸
        protected Vector3 _canvasInitScale;
        //血量值Text组件
        protected Text _hpTextCom;
        //移动速度
        protected float _moveSpeed;
        //初始移动速度
        protected float _initMoveSpeed;
        //构造函数
        public BaseEnemy(string enemyName):base(enemyName)
        {
            InitAttribute();
        }
        //初始化属性 获取物体 组件
        protected void InitAttribute()
        {
            _hpSlider = GameTool.GetComFromObjectsChild<Slider>(_entityGameObject, "Slider");
            _hpTextCom = GameTool.GetComFromObjectsChild<Text>(_entityGameObject, "Text");
            _hpCanvas = GameTool.FindGameObjectByParent(_entityGameObject, "Canvas");

            _canvasInitScale = _hpCanvas.transform.localScale;
            _maxHp = _currentHp = 1000;
            _moveSpeed = 10;
            _isActive = false;
        }
        //设置血条显示的数值
        protected void SetHpShowValue()
        {
            _hpTextCom.text = "" + _currentHp;
            _hpSlider.value = _currentHp * 1.0f / _maxHp;
        }
        //设置血条显示的尺寸
        public void SetHpShowScale(float value)
        {
            _hpCanvas.transform.localScale = _canvasInitScale * value;
        }
        //受到伤害
        public virtual void TakeDamage(int damage)
        {
            //_currentHp -= damage;
            ////敌人死亡回收
            //if (_currentHp <= 0)
            //{
            //    Singleton<EnemyManager>.Instance.RemoveObject(this);
            //    _enemyGameObject.transform.position = GameTool.noUsePosition;
            //    _isActive = false;
            //    return;
            //}
            //SetHpShowValue();
        }
    }
}
