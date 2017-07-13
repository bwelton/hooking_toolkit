#include "deduplicator.h"
std::shared_ptr<Deduplicate> Dedup;

Deduplicate::Deduplicate() {
	_collisionCount = 0;
	_collisionSize = 0;
}

Deduplicate::~Deduplicate() {

}
void Deduplicate::TrackTransfer(int id, int64_t size) {
	if (id == -1)
		return;
	boost::recursive_mutex::scoped_lock lock(_mtx);
	if(_previousTransfers.find(id) != _previousTransfers.end()) {
		_collisionCount++;
		_collisionSize += size;
	} else {
		_previousTransfers[id] = size;
	}
}