using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.SingletonSpace;
using UnityEngine;

namespace Assets.Scripts.Battle.CameraControll
{
    /// <summary>
    /// 相机操作管理类 没有使用对象池 无需继承BaseManager
    /// </summary>
    public class CameraManager:BaseObject
    {
        private readonly Camera _mainCamera;
        private Vector3 _cameraPosition;
        //相机的边界移动位置
        private static readonly int MAX_X = 235;
        private static readonly int MIN_X = 0;
        private static readonly int MAX_Z = 85;
        private static readonly int MIN_Z = -10;
        //相机的视野大小限制
        private static readonly int MAX_FIELDOFVIEW = 100;
        private static readonly int MIN_FIELDOFVIEW = 25;
        //初始相机的视野度
        protected static readonly int INIT_FIELDOFVIEW = 50;
        //收缩放速度
        private static readonly int PER_ADD = 5;
        //相机移动速度
        private static readonly int PER_DX = 3;
        //构造函数 获取主相机以及位置
        public CameraManager()
        {
            _mainCamera = Camera.main;
            _cameraPosition = _mainCamera.transform.position;
        }

        public override void Update()
        {
            ControllCameraFieldOfView();
            ControllCameraMove();
        }
        //控制相机视野
        private void ControllCameraFieldOfView()
        {
            if (Input.GetAxis("Mouse ScrollWheel") > 0)
            {
                if (_mainCamera.fieldOfView > MIN_FIELDOFVIEW)
                    _mainCamera.fieldOfView -= PER_ADD;
            }
            if (Input.GetAxis("Mouse ScrollWheel") < 0)
            {
                if (_mainCamera.fieldOfView < MAX_FIELDOFVIEW)
                    _mainCamera.fieldOfView += PER_ADD;
            }
            Singleton<EnemyManager>.Instance.SetEnemyHpCanvasScale(
                1.5f*_mainCamera.fieldOfView / INIT_FIELDOFVIEW);
        }
        //键盘鼠标控制相机的位置
        private void ControllCameraMove()
        {
            if (Input.GetKey(KeyCode.W))
                _cameraPosition += new Vector3(0, 0, PER_DX);
            if (Input.GetKey(KeyCode.S))
                _cameraPosition += new Vector3(0, 0, -PER_DX);
            if (Input.GetKey(KeyCode.D))
                _cameraPosition += new Vector3(PER_DX, 0, 0);
            if (Input.GetKey(KeyCode.A))
                _cameraPosition += new Vector3(-PER_DX, 0, 0);

            if (Input.GetMouseButton(2) || Input.GetMouseButton(1))
            {
                float norx = Input.GetAxis("Mouse X");
                float nory = Input.GetAxis("Mouse Y");
                float value = 1.0f * _mainCamera.fieldOfView / MAX_FIELDOFVIEW;
                _cameraPosition += new Vector3(-norx * 5 * value, 0, -nory * 5 * value);
            }
            LimitCameraPosition();
            _mainCamera.transform.position = _cameraPosition;
        }
        //限制相机的位置
        private void LimitCameraPosition()
        {
            if (_cameraPosition.z > MAX_Z)
                _cameraPosition = new Vector3(_cameraPosition.x, _cameraPosition.y, MAX_Z);
            if (_cameraPosition.z < MIN_Z)
                _cameraPosition = new Vector3(_cameraPosition.x, _cameraPosition.y, MIN_Z);
            if (_cameraPosition.x > MAX_X)
                _cameraPosition = new Vector3(MAX_X, _cameraPosition.y, _cameraPosition.z);
            if (_cameraPosition.x < MIN_X)
                _cameraPosition = new Vector3(MIN_X, _cameraPosition.y, _cameraPosition.z);
        }
    }
}
