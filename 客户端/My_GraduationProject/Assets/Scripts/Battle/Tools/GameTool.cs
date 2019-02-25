using System;
using UnityEngine;

namespace Assets.Scripts.Battle.Tools
{
    /// <summary>
    /// 游戏工具类
    /// </summary>
    public class GameTool
    {
        //一个vector3类型的数据 让暂时不使用的物体移动到该位置
        public static Vector3 noUsePosition = new Vector3(10000, 10000, 10000);
        //点击右键计时器
        private static float _getMouseRDownTime = 0f;
        //右键点击计数器
        private static int _getMouseRDownCount = 0;
        //获取鼠标点击的世界坐标
        public static Vector3 GetMouseWorldPosition()
        {
            float validTouchDistance = 200;
            string layerName = "Plane";
            //随鼠标点发射
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            //屏幕中心点发射
            //Ray ray = Camera.main.ScreenPointToRay(new Vector3(Screen.width / 2, Screen.height / 2, 0));
            RaycastHit hitInfo;
            //Debug.Log("Mask"+LayerMask.GetMask(layerName));
            if (Physics.Raycast(ray, out hitInfo, validTouchDistance, LayerMask.GetMask(layerName)))
            {
                return hitInfo.point;
            }
            return noUsePosition;
        }
       //判断是否右键双击
        public static bool IsDoubleHitRMouse()
        {
            if (Input.GetMouseButtonUp(1))
            {
                _getMouseRDownCount++;
                //当第一次点击鼠标，启动计时器
                if (_getMouseRDownCount == 1)
                {
                    _getMouseRDownTime = Time.time;
                }
                //当第二次点击鼠标，且时间间隔满足要求时双击鼠标
                if (2 == _getMouseRDownCount && Time.time - _getMouseRDownTime <= 0.5f)
                {
                    return true;
                }
            }
            if (Time.time - _getMouseRDownTime > 0.5f)
            {
                _getMouseRDownCount = 0;
            }
            return false;
        }
        //查找子物体
        public static GameObject FindChild(GameObject parent, string childName)
        {
            Transform[] children = parent.GetComponentsInChildren<Transform>();
            bool isFinded = false;
            Transform child = null;
            foreach (Transform t in children)
            {
                if (t.name == childName)
                {
                    if (isFinded)
                    {
                        Debug.LogWarning("在游戏物体" + parent + "下存在不止一个子物体:" + childName);
                    }
                    isFinded = true;
                    child = t;
                }
            }
            if (isFinded)
                return child.gameObject;
            return null;
        }
        //通过路径深度优先查找子物体（对于静态布置在场景的物体适合）
        public static GameObject FindGameObjectByPath(string childName)
        {
            string[] childNames = childName.Split('/');
            if (childNames.Length <= 0)
                return null;
            GameObject patentObject = GameObject.Find(childNames[0]);

            for (int i = 1; i < childNames.Length ; i++)
            {
                patentObject = FindChild(patentObject, childNames[i]);
                if (patentObject == null)
                {
                    break;
                }
            }
            return patentObject;
        }
        //通过父物体深度优先查找子物体（对于动态加载创建的物体适合）
        public static GameObject FindGameObjectByParent(GameObject patentObject, string childName)
        {
            if (patentObject == null)
            {
                return null;
            }
            string[] childNames = childName.Split('/');

            for (int i = 0; i < childNames.Length ; i++)
            {
                patentObject = FindChild(patentObject, childNames[i]);
                if (patentObject == null)
                {
                    break;
                }
            }
            CheckVarNull(patentObject, "Scene objects");
            return patentObject;
        }
        //通过父物体深度优先查找到子物体 然后泛型获取物体上的组件
        public static T GetComFromObjectsChild<T>(GameObject patentObject, string childName)
        {
            GameObject tempObject = FindGameObjectByParent(patentObject, childName);
            if (!CheckVarNull(tempObject, "Scene objects"))
                return default(T);
            return tempObject.GetComponent<T>();
        }
        //判断一个物体和一点坐标点的距离，如果在范围内则返回Transform
        public static bool IsObjectInScope(GameObject gameObject, Vector3 pos,float dis)
        {
            Vector3 v3 = gameObject.transform.position;
            if (Vector2.Distance(new Vector2(v3.x, v3.z), new Vector2(pos.x, pos.z)) <= dis)
            {
                return true;
            }
            return false;
        }
        //检测加载或者查找获取的物体是否为空  为空则报错
        public static bool CheckVarNull(object obj,string pathStr = "Resouces")
        {
            if (obj == null)
            {
                //TODO
                Debug.LogError("Cant find " + "xxx" + " in " + pathStr);
                return false;
            }
            return true;
        }

        #region //动态实例化对象 根据类名实例化
        /// <summary>
        /// 创建对象实例
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="fullName">命名空间.类型名</param>
        /// <param name="assemblyName">程序集</param>
        /// <returns></returns>
        public static T CreateInstance<T>(string fullName)
        {
            string path = fullName;//命名空间.类型名
            Type o = Type.GetType(path);//加载类型
            object obj = Activator.CreateInstance(o);//根据类型创建实例
            return (T)obj;//类型转换并返回
        }
        #endregion

    }
}
