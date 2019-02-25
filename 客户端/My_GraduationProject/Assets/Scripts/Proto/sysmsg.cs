using Assets.Scripts.Net;

namespace Assets.Scripts.Proto
{
    public class MsgPing : MsgBase
    {
        public MsgPing() { _protoName = "MsgPing"; }
    }

    public class MsgPong : MsgBase
    {
        public MsgPong()
        {
            _protoName = "MsgPong";
        }
    }
}