#include "deduplicator.h"
std::shared_ptr<Deduplicate> Dedup;

Deduplicate::Deduplicate() {
	_collisionCount = 0;
	_collisionSize = 0;
	_totalCount =0;
	_totalSize = 0;
}

Deduplicate::~Deduplicate() {
  fprintf(stderr, "Collisions: %llu, Size: %lld\n", _collisionCount, _collisionSize); 
  
  fprintf(stderr, "Total Transfer: %llu, Total Size: %lld\n", _totalCount, _totalSize); 
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
		_totalSize += size;
		_totalCount++;
}
