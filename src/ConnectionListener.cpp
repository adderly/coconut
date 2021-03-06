/*
* Copyright (c) 2012, Eunhyuk Kim(gunoodaddy) 
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*   * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of Redis nor the names of its contributors may be used
*     to endorse or promote products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "CoconutLib.h"
#include "IOService.h"
#include "ConnectionListener.h"
#include "Exception.h"
#include "InternalLogger.h"
#include "ConnectionListenerImpl.h"
#include "BaseIOSystemFactory.h"

namespace coconut {

ConnectionListener::ConnectionListener()
	: ioService_(), handler_(NULL) {
	impl_ = BaseIOSystemFactory::instance()->createConnectionListenerImpl();
}

ConnectionListener::ConnectionListener(boost::shared_ptr<IOService> ioService, int port)
	: ioService_(ioService), handler_(NULL) {

	impl_ = BaseIOSystemFactory::instance()->createConnectionListenerImpl();
	impl_->initialize(this, port);
}

ConnectionListener::ConnectionListener(boost::shared_ptr<IOService> ioService, const char* path)
	: ioService_(ioService), handler_(NULL) {

	impl_ = BaseIOSystemFactory::instance()->createConnectionListenerImpl();
	impl_->initialize(this, path);
}

ConnectionListener::~ConnectionListener(void) {
}


void ConnectionListener::initialize(boost::shared_ptr<IOService> ioService, int port) {
	ioService_ = ioService;
	impl_->initialize(this, port);
}

void ConnectionListener::initialize(boost::shared_ptr<IOService> ioService, const char* path) {
	ioService_ = ioService;
	impl_->initialize(this, path);
}

void ConnectionListener::listen() {
	impl_->listen();
}

const char* ConnectionListener::listeningPath() {
	return impl_->listeningPath().c_str();
}

}
