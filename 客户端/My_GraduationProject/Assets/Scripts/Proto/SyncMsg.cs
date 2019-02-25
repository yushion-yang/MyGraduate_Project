using Assets.Scripts.Net;

namespace Assets.Scripts.Proto
{
    public class MsgCreateTurret : MsgBase
    {
        public MsgCreateTurret()
        {
            _protoName = "MsgCreateTurret";
        }

        //坦克产生位置
        public float _x = 0f;
        public float _y = 0f;
        public float _z = 0f;
        public int _NO = 0;
    }
}