#include "IMServer.h"
#include "Logger.h"
///掌管所有的RoomSession

bool IMServer::Init(EventLoop * loop)
{
	//设置服务器智能指针指向
	m_server.reset(new TcpServer(loop));

	//设置接收连接回调
	m_server->setConnectionCallback(std::bind(&IMServer::OnConnection, this, std::placeholders::_1));
	//启动侦听
	m_server->Start();

	return true;
}

void IMServer::GetSessions(std::list<std::shared_ptr<RoomSession>>& sessions)
{
	std::lock_guard<std::mutex> guard(m_sessionMutex);
	sessions = m_sessions;
}

bool IMServer::GetSessionByUserIdAndClientType(std::shared_ptr<RoomSession>& session, int32_t userid, int32_t clientType)
{
	return false;
}

bool IMServer::GetSessionsByUserId(std::list<std::shared_ptr<RoomSession>>& sessions, int32_t userid)
{
	std::lock_guard<std::mutex> guard(m_sessionMutex);
	std::shared_ptr<RoomSession> tmpSession;
	for (const auto& iter : m_sessions)
	{
		tmpSession = iter;
		if (iter->GetUserId() == userid)
		{
			sessions.push_back(tmpSession);
			return true;
		}
	}

	return false;
}

int32_t IMServer::GetUserStatusByUserId(int32_t userid)
{
	return 0;
}

int32_t IMServer::GetUserClientTypeByUserId(int32_t userid)
{
	return int32_t();
}
//让connection连接完成或者链接断开时调用此函数进行处理
void IMServer::OnConnection(std::shared_ptr<TcpConnection> conn)
{
	//连接成功则创建一个会话	并且将conn的业务数据处理回调指定为RoomSession的OnRread函数
	if (conn->connected())
	{
		//LOG_INFO << "client connected:" << conn->peerAddress().toIpPort();
		LogInfo("create a new connection session");
		//TODO 修改为不保存Session集合 增设一个单例NetManager类 里面包括了ClientState集合 以及心跳事件检测
		//此类作为数据处理类 将TcpConnection的信息处理回调设置为NetManager类的对应函数
		++m_sessionId;
		std::shared_ptr<RoomSession> spSession(new RoomSession(conn, m_sessionId)); 
		conn->setMessageCallback(std::bind(&RoomSession::OnRead, spSession.get(), std::placeholders::_1, std::placeholders::_2));

		std::lock_guard<std::mutex> guard(m_sessionMutex);
		m_sessions.push_back(spSession);
	}
	else
	{
		OnClose(conn);
	}
}

void IMServer::OnClose(const std::shared_ptr<TcpConnection>& conn)
{
	//TODO 为空 可能是当连接断开是不马上进行Session的释放 等待重新连接时再使用
	//这里如果不实现断线重连 则直接删除关联的RoomSession

	//思考断线重连的做法：Session会保存玩家在服务器的状态，进行游戏后即使断开连接，这个状态也会保持一段时间
	//只是Seesion无法获取玩家相关操作输入，但是其他玩家所产生的数据还是会传送到此，当玩家重现连接后，玩家将
	//拉取此Session的数据恢复客户端的状态从而重新开始游戏。
}
