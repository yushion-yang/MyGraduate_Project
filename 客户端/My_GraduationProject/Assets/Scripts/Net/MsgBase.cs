using System;
using UnityEngine;
using System.Linq;
using Assets.Scripts.Proto;

namespace Assets.Scripts.Net
{
    /// <summary>
    /// 数据编码解码类
    /// </summary>
    /// +----------+----------+--------+
    /// |          |          |        |
    /// |数据总长度|协议名长度|数据内容|
    /// |          |          |        |
    /// +----------+----------+--------+
    public class MsgBase
    {
        public string _protoName = "null";
        
        //编码
        public static byte[] Encode(MsgBase msgBase)
        {
            string s = JsonUtility.ToJson(msgBase);
            return System.Text.Encoding.UTF8.GetBytes(s);
        }

        //解码
        public static MsgBase Decode(string protoName, byte[] bytes, int offset, int count)
        {
            string s = System.Text.Encoding.UTF8.GetString(bytes, offset, count);
            Debug.Log("Debug decode:" + s);
            Type type = Type.GetType("Assets.Scripts.Proto." + protoName);
            if (type == null)
            {
                Debug.LogError("can not get protoName type");
            }
            MsgBase msgBase = (MsgBase) JsonUtility.FromJson(s, type);
            return msgBase;
        }

        //编码协议名（2字节长度+字符串）
        public static byte[] EncodeName(MsgBase msgBase)
        {
            //名字bytes和长度
            byte[] nameBytes = System.Text.Encoding.UTF8.GetBytes(msgBase._protoName);
            Int16 len = (Int16) nameBytes.Length;
            //申请bytes数值
            byte[] bytes = new byte[2 + len];
            //组装2字节的长度信息
            bytes[0] = (byte) (len % 256);
            bytes[1] = (byte) (len / 256);
            //组装名字bytes
            Array.Copy(nameBytes, 0, bytes, 2, len);

            return bytes;
        }

        //解码协议名（2字节长度+字符串）
        public static string DecodeName(byte[] bytes, int offset, out int count)
        {
            count = 0;
            //必须大于2字节
            if (offset + 2 > bytes.Length)
            {
                return "";
            }

            //读取长度
            Int16 len = (Int16) ((bytes[offset + 1] << 8) | bytes[offset]);
            //长度必须足够
            if (offset + 2 + len > bytes.Length)
            {
                return "";
            }

            //解析
            count = 2 + len;
            string name = System.Text.Encoding.UTF8.GetString(bytes, offset + 2, len);
            return name;
        }
    }
}