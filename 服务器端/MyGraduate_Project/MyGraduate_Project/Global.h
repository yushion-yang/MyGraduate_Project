#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include <functional>
namespace global
{
	void setnonblocking(int sock);

	template<class F, class...Args>
	std::function<void()> BindFun(F&& f, Args...args)
	{
		return std::bind(f, std::forward<Args>(args)...);
	}

	void PrintData(char *data, int len);
}
#endif // !__GLOBAL_H__
