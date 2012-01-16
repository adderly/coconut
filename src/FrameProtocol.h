#pragma once

#include <string>
#include <vector>
#if ! defined(COCONUT_USE_PRECOMPILE)
#include <boost/shared_ptr.hpp>
#endif
#include "BaseVirtualTransport.h"
#include "VirtualTransportHelper.h"
#include "BufferedTransport.h"
#include "BaseProtocol.h"

namespace coconut { namespace protocol {

class COCONUT_API FrameHeader {
public:
	FrameHeader(boost::int32_t command, boost::int32_t transactionId) : length_(0), checksum_(0), cmd_(command), trid_(transactionId) { }
	FrameHeader() : length_(0), checksum_(0), cmd_(0), trid_(0) { }

	void setCommand(boost::int32_t command) {
		cmd_ = command;	
	}
	boost::int32_t command() const {
		return cmd_;
	}

	void setChecksum(boost::int32_t checksum) {
		checksum_ = checksum;	
	}
	boost::int32_t checksum() const {
		return checksum_;
	}

	void setLength(boost::int32_t length) {
		length_ = length;	
	}
	boost::int32_t length() const {
		return length_;
	}

	void setTransactionId(boost::int32_t trid) {
		trid_ = trid;	
	}
	boost::int32_t transactionId() const {
		return trid_;
	}

	void makeChecksum(const void *payload, size_t size) {
		// TODO check sum logic need..
		checksum_ = 1;
	}

	void serialize(boost::shared_ptr<BufferedTransport> buffer, size_t payloadSize) {
		VirtualTransportHelper::writeInt32(buffer, cmd_);
		VirtualTransportHelper::writeInt32(buffer, trid_);
		VirtualTransportHelper::writeInt32(buffer, checksum_);
		// auto calculated..
		length_ = buffer->totalSize() + payloadSize + sizeof(length_);
		VirtualTransportHelper::writeInt32(buffer, length_);
	}

private:
	boost::int32_t length_;	// auto calculated
	boost::int32_t checksum_;	// auto calculated
	boost::int32_t cmd_;		// user definition
	boost::int32_t trid_;		// user definition
};


class COCONUT_API FrameProtocol : public BaseProtocol {
public:
	enum State{
		Init,
		Command,
		TransationID,
		Checksum,
		Length,
		Payload,
		Complete
	};

	FrameProtocol() : header_(), initHeader_(), state_(Init), payload_pos_(0), init_payload_pos_(0), payload_protocol_(NULL) { }

	~FrameProtocol() { 
	}

	const char* className() {
		return "FrameProtocol";
	}

	bool isReadComplete() {
		return state_ == Complete;
	}

	bool processSerialize(size_t bufferSize = 0);
	bool processRead(boost::shared_ptr<BaseVirtualTransport> transport);
	bool processWrite(boost::shared_ptr<BaseVirtualTransport> transport);

	size_t readPayloadSize() {
		return buffer_->readPos() - init_payload_pos_;
	}
	virtual const void * remainingBufferPtr();
	virtual size_t remainingBufferSize();

public:
	const FrameHeader &header() {
		return header_;
	}

	const void * payloadPtr();
	size_t payloadSize();

	void setFrame(const FrameHeader &header, BaseProtocol *protocol) {
		header_ = header;
		payload_protocol_ = protocol;

		state_ = Complete;
	}

	void setFrame(const FrameHeader &header, boost::shared_ptr<BaseProtocol> protocol) {
		header_ = header;
		payload_protocol_shared_ptr_ = protocol;

		state_ = Complete;
	}

	void setFrame(const FrameHeader &header, const void *payload, size_t size) {
		header_ = header;
		payload_.assign((char *)payload, size);

		state_ = Complete;
	}

private:
	FrameHeader header_;
	FrameHeader initHeader_;
	State state_;
	std::string payload_;
	int payload_pos_;
	int init_payload_pos_;
	BaseProtocol *payload_protocol_;
	boost::shared_ptr<BaseProtocol> payload_protocol_shared_ptr_;
};

} }
