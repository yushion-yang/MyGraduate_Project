using Assets.Scripts.Base;
using Assets.Scripts.Battle.Enemy;
using Assets.Scripts.Battle.Tools;
using Assets.Scripts.Flyweight;
using Assets.Scripts.SingletonSpace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace Assets.Scripts.Battle.Bullet
{
    /// <summary>
    /// 子弹基类
    /// </summary>
    /// 
    /// 遇到的一个bug 在获取管理者的单例时造成了实例化对象的无限循环 原因 类的单例还没有构造完成
    /// 遇到问题的解决过程记录：首先表现是unity开启运行后直接关闭了  代码肯定有严重bug
    /// 然后尝试VS调试运行  打断点找出大致位置  然后查找到了是 栈溢出导致的
    /// 是由于TankBullet类实例化太多了  然后导致栈的溢出  然后去查找为什么会实例化太多的原因
    /// 在管理类中打断点 TankBullet类构造函数中打断点 发现程序在两个断点中一直跳转 所以是在这里的
    /// 死循环导致的栈溢出  然后查找导致死循环的原因  对比相似系统部分的代码  发现差异点在于
    /// _manager = Singleton<BulletManager>.Instance; 即尝试在管理者的构造函数中获取类的单例
    /// 由于类的构造还没有完成 所以无法获取类的单例（这样子会继续调用类的构造） 造成死循环
    /// 解决方法：在类的构造完成后再获取类的单例，或者通过this进行传递。
    /// 
    public class BaseBullet:BasePoolObject
    {
        public BaseBullet(string bulletName):base(bulletName)
        {

        }

        public override void Update()
        {

        }

        public virtual BaseBullet Init(Transform createTransform, Transform targeTransform, BaseEnemy targetEnemy,int damage)
        {
            return null;
        }
    }
}
