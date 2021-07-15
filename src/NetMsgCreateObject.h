#pragma once

#include "NetMsg.h"
#ifdef AFTR_CONFIG_USE_BOOST

namespace Aftr
{

	class NetMsgCreateObject : public NetMsg
	{
	public:
		NetMsgMacroDeclaration(NetMsgCreateObject);

		NetMsgCreateObject();
		virtual ~NetMsgCreateObject();
		virtual bool toStream(NetMessengerStreamBuffer& os) const;
		virtual bool fromStream(NetMessengerStreamBuffer& is);
		virtual void onMessageArrived();
		virtual std::string toString() const;



		std::string Positon;


	protected:

	};

} //namespace Aftr

#endif