#include "Reflection.h"

Reflection & Reflection::GetInstance()
{
	static Reflection _instance;
	return _instance;
}

Reflection::Reflection()
{

}

Reflection::~Reflection()
{

}

void Reflection::RegistFun(std::string funName, FunCall fun)
{
	_funReflections[funName] = fun;
}

void Reflection::InvokFun(std::string funName, Type1 arg1, Type2 arg2)
{
	_funReflections[funName](arg1, arg2);
}
