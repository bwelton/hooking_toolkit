#include "deduplicator.h"
std::shared_ptr<Deduplicate> Dedup;

Deduplicate::Deduplicate() {
	_collisionCount = 0;
	_collisionSize = 0;
	_totalCount =0;
	_totalSize = 0;
	_missedHashes = 0;
	_missedSize = 0;
}

Deduplicate::~Deduplicate() {
  fprintf(stderr, "Collisions: %llu, Size: %lld\n", _collisionCount, _collisionSize); 
  fprintf(stderr, "Total Transfer: %llu, Total Size: %lld\n", _totalCount, _totalSize); 
  fprintf(stderr, "Missed Collisions: %llu, Total Size: %lld\n",_missedHashes, _missedSize );
}

uint32_t Deduplicate::HashData(char * data, size_t size) {
	uint32_t result = 0;
#ifdef USE_PRIVATE 
	result = XXHash32::hash(data, size, 0);
#endif
	return result;
}

void Deduplicate::TrackTransfer(int id, int64_t size, char * data) {
	if (id == -1 || size == 0)
		return;
	uint32_t hash = HashData(data, size);
	bool prev_found = false;
	boost::recursive_mutex::scoped_lock lock(_mtx);
	if(_previousTransfers.find(id) != _previousTransfers.end()) {
		_collisionCount++;
		_collisionSize += size;
		prev_found = true;
	} else {
		_previousTransfers[id] = size;
	}
	if (_previousHash.find(hash) != _previousHash.end()) {
		// Matched via hash
		if (prev_found == false){
			// Not matched via ID
			_missedHashes++;
			_missedSize += size;
		}
	} else {
	  	_previousHash[hash] = size;
	}
	_totalSize += size;
	_totalCount++;	
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
