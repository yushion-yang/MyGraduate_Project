#ifndef __REFLECTION_H__
#define __REFLECTION_H__

#define REGIST_FUN(fun) Reflection::GetInstance().RegistFun(#fun,fun)
#define INVOKE_FUN(fun,arg1,arg2) Reflection::GetInstance().InvokFun(fun,arg1,arg2)

#include <functional>
#include <map>
#include <cstring>

class Reflection
{
	using Type1 = int;
	using Type2 = int;
	using FunCall = std::function<void(Type1, Type2)>;
	using FunReflections = std::map<std::string, FunCall>;
public:
	static Reflection& GetInstance();
	void RegistFun(std::string funName, FunCall fun);
	void InvokFun(std::string funName, Type1 arg1, Type2 arg2);
private:
	Reflection();
	~Reflection();
	FunReflections _funReflections;
};

#endif __REFLECTION_H__