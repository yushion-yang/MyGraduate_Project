using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Assets.Scripts.Battle.Tools;
using UnityEngine;

namespace Assets.Scripts.Battle.Map
{
    /// <summary>
    /// 地图管理者 没有使用对象池 无需继承BaseManager
    /// </summary>
    public class MapManager:BaseObject
    {
        //宽度
        private const int WIDTH = 48;
        //高度
        private const int HEIGHT = 20;
        //砖块间隔距离
        private const int DIS_INTERVAL = 5;   
        //砖块预制体
        private readonly GameObject _rsPrefabCub;
        //砖块
        private static GameObject[,] _cubs;
        //地图父物体
        private readonly Transform _parentTrans;
        //当前鼠标指向的砖块
        private GameObject _currentCub;
        //旧的砖块
        private GameObject _oldCub;
        //砖块的默认颜色
        private Color _defaultCubColor;
        //是否显示红砖块
        private bool _isShowRedCub = true;
        //加载场景
        public MapManager()
        {
            _rsPrefabCub = Resources.Load<GameObject>("map_cub");
            _cubs=new GameObject[HEIGHT, WIDTH];
            _parentTrans = new GameObject("map").transform;

            for (int i = 0; i < HEIGHT; i++)
            {
                for (int j = 0; j < WIDTH; j++)
                {
                    _cubs[i, j] = GameObject.Instantiate(_rsPrefabCub);
                    _cubs[i, j].transform.position = new Vector3(j * DIS_INTERVAL, 0, i * DIS_INTERVAL);
                    _cubs[i, j].transform.SetParent(_parentTrans);
                }
            }
            _oldCub = _currentCub = _cubs[0, 0];
            _defaultCubColor = _cubs[0, 0].GetComponent<MeshRenderer>().material.color;
        }
        //获取砖块
        public static GameObject GetCub(Vector3 posVector3)
        {
            int z = (int)(posVector3.x + 2.5) / 5;
            int x = (int)(posVector3.z + 2.5) / 5;
            return _cubs[x, z];
        }
        //设置砖块红色
        public void SetCubRed(GameObject cub)
        {
            cub.GetComponent<MeshRenderer>().material.color = Color.red;
        }
        //清除砖块颜色
        public void ResetCubColor(GameObject cub)
        {
            cub.GetComponent<MeshRenderer>().material.color = _defaultCubColor;
        }
        //开启显示红色砖块
        public void OpenShowRedCub()
        {
            _isShowRedCub = true;
        }
        //关闭显示红色砖块
        public void CloseShowRedCub()
        {
            ResetCubColor(_currentCub);
            ResetCubColor(_oldCub);
            _isShowRedCub = false;
        }

        public override void Update()
        {
            //获取新的指向砖块 并设置颜色
            Vector3 tempVector3 = GameTool.GetMouseWorldPosition();
            if (tempVector3 != GameTool.noUsePosition)
            {
                _oldCub = _currentCub;
                _currentCub = GetCub(tempVector3);
                if (_isShowRedCub)
                {
                    SetCubRed(_currentCub);
                }
            }
            //设置久砖块的颜色
            if (_currentCub != _oldCub)
            {
                ResetCubColor(_oldCub);
            }
        }
    }
}
