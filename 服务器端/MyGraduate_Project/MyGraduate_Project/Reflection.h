#ifndef __REFLECTION_H__
#define __REFLECTION_H__

#define REGISTE_FUN(className,funName) Reflection::GetInstance().RegistFun(#funName, std::bind(\
	&className::funName, this, std::placeholders::_1, std::placeholders::_2));
#define INVOKE_FUN(funName,arg1,arg2) Reflection::GetInstance().InvokFun(funName,arg1,arg2)

#include <functional>
#include <map>
#include <cstring>
class ClientState;
class BaseMsg;
class Reflection
{
	using Type1 = ClientState* ;
	using Type2 = BaseMsg* ;
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