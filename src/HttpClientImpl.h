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

namespace coconut {

class HttpClientImpl {
public:
	virtual ~HttpClientImpl() { }

public:
	virtual void initialize(
				HttpClient *owner, 
				boost::shared_ptr<IOService> ioService, 
				HttpMethodType method = HTTP_GET, 
				const char *uri = "", 
				const HttpParameter *param = NULL, 
				int timeout = 0) = 0;
	
	virtual boost::shared_ptr<IOService> ioService() = 0;

	virtual void cleanUp(bool deleteReqFlag) = 0;
	virtual void cancelRequest() = 0;

	virtual const void* responseBody() = 0;
	virtual int responseBodySize() = 0;
	virtual int responseCode() = 0;

	virtual const std::string& findHeader(const char *key) = 0;

	virtual void request(HttpMethodType method, const char *uri, const HttpParameter * param, int timeout) = 0;
	virtual void request() = 0;
};

}
