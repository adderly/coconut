#include "Coconut.h"
#include "JSONProtocol.h"
#include "VirtualTransportHelper.h"
#define NDEBUG
#include <libjson/libjson.h>

namespace coconut { namespace protocol {

class JSONProtocolImpl {
public:
	JSONProtocolImpl(JSONProtocol *owner) : owner_(owner), readComplete_(false), stream_(NULL), remainStreamPos_(0) {

	}

	bool isReadComplete() {
		return readComplete_;
	}

	bool processSerialize(size_t bufferSize) {
		owner_->resetWritingBuffer();

		VirtualTransportHelper::writeBinary(owner_->writingBuffer(), internalStreamBuffer_.c_str(), internalStreamBuffer_.size());

		if(owner_->callParentProcessSerialize(bufferSize + owner_->writingBuffer()->totalSize()) == false)
			return false;

		return true;
	}

	bool processRead(boost::shared_ptr<BaseVirtualTransport> transport) {
		if(owner_->callParentProcessRead(transport) == false)
			return false;
		
		_makeStream();
		try {
			do {
#define JSON_STREAM_SIZE	1024
				size_t nread = JSON_STREAM_SIZE;
				const void *buffer = transport->peek(nread);
				if(nread <= 0)
					break;
				stream_->push(buffer, nread);
				transport->ackReadSize(nread - remainStreamPos_);

				if(isReadComplete()) {
					return true;
				}
			} while(1); 
		} catch (SocketException &e) {
			(void)e;
		} catch (ProtocolException &e) {
			(void)e;
		}
		return false;
	}

	const void * remainingBufferPtr() {
		return (const char*)owner_->payloadBuffer()->currentPtr() - remainStreamPos_;
	}

	size_t remainingBufferSize() {
		return remainStreamPos_;
	}

private:
	static void errorCallback(void *) {
		// TODO, JSON STREAM PARSING ERROR..
	}

	static bool Callback(::JSONNode & test, size_t remainpos, void * ide) {
		JSONProtocolImpl *SELF = (JSONProtocolImpl *)ide;
		SELF->_parsingSucc(test, remainpos);
		return false;	// stop parsing..
	}

	void _makeStream() {
		if(NULL == stream_) {
			stream_ = new ::JSONStream(Callback, errorCallback, this);
		}
	}

	void _parsingSucc(::JSONNode &node, size_t remainSize) {
		remainStreamPos_ = remainSize;

		// serialize!
		internalStreamBuffer_ = node.write();
		readComplete_ = true;
	}

public:
	void setJSON(const std::string &buffer) {
		internalStreamBuffer_ = buffer;
	}

	std::string &jsonString() {
		return internalStreamBuffer_;
	}

private:
	JSONProtocol *owner_;
	bool readComplete_;
	std::string internalStreamBuffer_;
	::JSONStream *stream_;
	size_t remainStreamPos_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

JSONProtocol::JSONProtocol() {
	LOG_TRACE("JSONProtocol : %p", this);

	impl_ = new JSONProtocolImpl(this);
}

JSONProtocol::JSONProtocol(BaseProtocol *protocol) {
	LOG_TRACE("JSONProtocol with parent_protocol : %p", this);
	parent_protocol_ = protocol;
	
	impl_ = new JSONProtocolImpl(this);
}

JSONProtocol::JSONProtocol(boost::shared_ptr<BaseProtocol> protocol) {
	LOG_TRACE("JSONProtocol with parent_protocol_shared_ptr : %p", this);
	parent_protocol_shared_ptr_ = protocol;
	
	impl_ = new JSONProtocolImpl(this);
}

JSONProtocol::~JSONProtocol() {
	LOG_TRACE("~JSONProtocol : %p", this);

	delete impl_;
}

bool JSONProtocol::isReadComplete() {
	return impl_->isReadComplete();
}

bool JSONProtocol::processSerialize(size_t bufferSize) {
	return impl_->processSerialize(bufferSize);
}

bool JSONProtocol::processRead(boost::shared_ptr<BaseVirtualTransport> transport) {
	return impl_->processRead(transport);
}

void JSONProtocol::setJSON(const std::string &jsonString) {
	impl_->setJSON(jsonString);
}

const char* JSONProtocol::jsonPtr() {
	return impl_->jsonString().c_str();
}

const void * JSONProtocol::remainingBufferPtr() { 
	if(isReadComplete())
	{
		return impl_->remainingBufferPtr();
	}
	return BaseProtocol::remainingBufferPtr();
}

size_t JSONProtocol::remainingBufferSize() {
	if(isReadComplete()) {
		return impl_->remainingBufferSize();
	}
	return BaseProtocol::remainingBufferSize();
}

}}