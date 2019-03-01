#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

class ClientState;
class EventHandler
{
public:
	EventHandler();
	~EventHandler();
	void OnDisconnect(ClientState* state);
private:

};
#endif // !__EVENT_HANDLER_H__
