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
#include "DNSResolver.h"
#include "DNSResolverImpl.h"
#include "BaseIOSystemFactory.h"

#if defined(WIN32)
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif

namespace coconut { 

DNSResolver::DNSResolver() {
	impl_ = BaseIOSystemFactory::instance()->createDNSResolverImpl();
}

DNSResolver::DNSResolver(boost::shared_ptr<IOService> ioService) {
	impl_ = BaseIOSystemFactory::instance()->createDNSResolverImpl();
	impl_->initialize(ioService);
}

DNSResolver::~DNSResolver() {
}

void DNSResolver::initialize(boost::shared_ptr<IOService> ioService) {
	impl_->initialize(ioService);
}

void DNSResolver::cleanUp()	{	
	impl_->cleanUp();
}

bool DNSResolver::resolve(const char *host, struct sockaddr_in *sin, EventHandler* handler, void *ptr) {
	return impl_->resolve(host, sin, handler, ptr);
}

}
