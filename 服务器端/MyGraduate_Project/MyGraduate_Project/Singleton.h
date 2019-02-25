#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template<typename T>
class Singleton
{
public:
	static T& Instance()
	{
		static T value_;
		return value_;
	}
private:
	Singleton() {}
	~Singleton() {}
};

#endif // !__SINGLETON_H__
