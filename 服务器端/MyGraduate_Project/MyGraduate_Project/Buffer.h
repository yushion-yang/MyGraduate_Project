#ifndef __BUFFER_H__
#define __BUFFER_H__
const int DATA_SIZE = 1024;
//TODO Buffer的结构需要修改一下  参考C#服务器的ByteArray的设计
class Buffer
{
public:
	Buffer();
	~Buffer();
	bool EnoughOnePackageLen();
	bool EnoughHeadLen();
	void SetSolveDataLen();
	int GetPackageLen();
	int GetDataLen();
	void SolveLen();
	int ReadData(int socket);
	char* GetDatas();
	void AppendData(char *datas, int len);
	bool HasData();
	void MoveCurrentIndex(int index);
private:
	char datas_[DATA_SIZE];
	int current_index_ = 0;		//
	int solve_data_len_ = 0;	//	
	int all_data_len_ = 0;		//
};
#endif // !__BUFFER_H__
