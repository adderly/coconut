#include "Coconut.h"
#include "FrameController.h"

using namespace coconut::protocol;

namespace coconut {
	
void FrameController::writeFrame(const protocol::FrameHeader &header, boost::shared_ptr<BufferedTransport> buffer) {
	writeFrame(header, buffer->currentPtr(), buffer->totalSize());
}

/*
static bool hasParentFrameProtocol(BaseProtocol *protocol) {
	BaseProtocol *parent = protocol->parentProtocol();
	while(parent) {
		if(strcmp(parent->className(), "FrameProtocol") == 0) {
			return true;
		}
		parent = parent->parentProtocol();
	}
	boost::shared_ptr<BaseProtocol> parent_share_ptr = protocol->parentProtocolSharedPtr();
	while(parent_share_ptr) {
		if(strcmp(parent_share_ptr->className(), "FrameProtocol") == 0) {
			return true;
		}
		parent_share_ptr = parent_share_ptr->parentProtocolSharedPtr();
	}
	return false;
}
*/


static bool disableWriteBufferOfParentFrameProtocol(BaseProtocol *protocol) {
	BaseProtocol *parent = protocol->parentProtocol();
	while(parent) {
		if(strcmp(parent->className(), "FrameProtocol") == 0) {
			parent->enableWriteBuffer(false);
			return true;
		}
		parent = parent->parentProtocol();
	}
	boost::shared_ptr<BaseProtocol> parent_share_ptr = protocol->parentProtocolSharedPtr();
	while(parent_share_ptr) {
		if(strcmp(parent_share_ptr->className(), "FrameProtocol") == 0) {
			parent_share_ptr->enableWriteBuffer(false);
			return true;
		}
		parent_share_ptr = parent_share_ptr->parentProtocolSharedPtr();
	}
	return false;
}


void FrameController::writeFrame(const protocol::FrameHeader &header, boost::shared_ptr<BaseProtocol> protocol) {
	
	//assert(!hasParentFrameProtocol(protocol.get()) && "Protocol must not have parent FrameProtocol.");
	disableWriteBufferOfParentFrameProtocol(protocol.get());

	boost::shared_ptr<FrameProtocol> prot 
		= boost::static_pointer_cast<FrameProtocol>(protocolFactory_->makeProtocol());

	prot->setFrame(header, protocol);
	prot->processSerialize();
	prot->processWrite(socket());
}

void FrameController::writeFrame(const protocol::FrameHeader &header, BaseProtocol *protocol) {
	//assert(!hasParentFrameProtocol(protocol) && "Protocol must not have parent FrameProtocol.");
	disableWriteBufferOfParentFrameProtocol(protocol);

	boost::shared_ptr<FrameProtocol> prot 
		= boost::static_pointer_cast<FrameProtocol>(protocolFactory_->makeProtocol());

	prot->setFrame(header, protocol);
	prot->processSerialize();
	prot->processWrite(socket());
}

void FrameController::writeFrame(const FrameHeader &header, const void *payload, size_t size) {
	boost::shared_ptr<FrameProtocol> prot 
		= boost::static_pointer_cast<FrameProtocol>(protocolFactory_->makeProtocol());

	prot->setFrame(header, payload, size);
	prot->processSerialize();
	prot->processWrite(socket());
}

void FrameController::onReceivedProtocol(boost::shared_ptr<protocol::BaseProtocol> protocol) {
	boost::shared_ptr<FrameProtocol> frame = boost::static_pointer_cast<FrameProtocol>(protocol);
	LOG_TRACE("FrameController received : %d", frame->header().command());
	onFrameReceived(frame);
}

} // end of namespace coconut