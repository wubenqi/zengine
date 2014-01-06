// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef NET_HAMMER_HAMMER_IO_HANDLER_FACTORY_H_
#define NET_HAMMER_HAMMER_IO_HANDLER_FACTORY_H_
#pragma once

#include "net/engine/io_handler_factory.h"
#include "net/hammer/hammer_io_handler.h"

namespace net {

class HammerIOHandlerFactory :
	public IOHandlerFactory {
public:
	static HammerIOHandlerFactory* GetInstance() {
		return Singleton<HammerIOHandlerFactory>::get();
	}

	virtual IOHandler* CreateIOHandler(SOCKET socket, Reactor* reactor) {
		return new HammerIOHandler(socket, reactor/*, soucheng_ih_delegate*/);
		return NULL;
	}

private:
	friend class Singleton<HammerIOHandlerFactory>;
	friend struct DefaultSingletonTraits<HammerIOHandlerFactory>;

	HammerIOHandlerFactory() {}
	virtual ~HammerIOHandlerFactory() {}

	DISALLOW_COPY_AND_ASSIGN(HammerIOHandlerFactory);
};

}

#endif	//
