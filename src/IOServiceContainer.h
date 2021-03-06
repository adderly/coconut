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

#pragma once

#include <vector>
#include <signal.h>
#include "BaseIOServiceContainer.h"
#include "IOService.h"

namespace coconut {

class COCONUT_API IOServiceContainer : public BaseIOServiceContainer {
public:
	IOServiceContainer(int threadCount = 0) : threadCount_(threadCount) 
#if defined(WIN32)
		, iocpEnabled_(false)
		, cpuCnt_(0)
#endif
	{
	}

	~IOServiceContainer();

public:
	size_t ioServiceCount() {
		return ioservices_.size();
	}
	boost::shared_ptr<IOService> ioServiceByRoundRobin();
	boost::shared_ptr<IOService> ioServiceByIndex(size_t index);
	boost::shared_ptr<IOService> ioServiceOfCurrentThread();
	void initialize();
	void run();
	void stop();
#if defined(WIN32)
	void turnOnIOCP(size_t cpuCount);	// must call before initialize()
#endif

private:
	int threadCount_;
#if defined(WIN32)
	bool iocpEnabled_;
	size_t cpuCnt_;
#endif
	std::vector< boost::shared_ptr<IOService> > ioservices_;
};

}
