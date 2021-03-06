#include "strace.h"
#include <pthread.h>
std::shared_ptr<STrace> StraceStore;
STrace::STrace(StraceOutpurLocation out, char * fname) {
        pthread_self();
	switch (out) {
		case STDOUT:
			_fd_out = stdout;
			_storage_location = STDOUT;
			break;
		case STDERR:
			_fd_out = stderr;
			_storage_location = STDERR;
			break;
		case TOFILE:
			_fd_out = fopen(fname,"w");
			if (_fd_out == NULL) {
				_storage_location = STDERR;
				LogOut("ERROR: Could not open output file, defaulting to STDERR\n");
			} else {
				_storage_location = TOFILE;
			}
			break;
	}
}

STrace::~STrace() {
	WriteStacks();
	if (_storage_location == TOFILE) {
		fclose(_fd_out);
	}
}

void STrace::flush() {
	boost::recursive_mutex::scoped_lock lock(_mtx);
	fflush(_fd_out);
}

void STrace::WriteStacks() {
	boost::recursive_mutex::scoped_lock lock(_mtx);
	for (auto i : _stacks) {
		size_t count = _stackCounts[i.first];
		LogOut("===================== Trace ================\n==== Count = %llu\n%s\n===============\n", count, i.second.c_str());
	}
	_stackCounts.clear();
	_stacks.clear();
	flush();
}

void STrace::AddHash(std::string s) {
	size_t b =  boost::hash_value(s);
	boost::recursive_mutex::scoped_lock lock(_mtx);
	if (_stacks.find(b) != _stacks.end())
	{
		_stackCounts[b] += 1;
	} else {
		_stacks[b] = s;
		_stackCounts[b] = 1;
	}
	if (_stacks.size() > 100) {
		WriteStacks();
	}

		
}

void STrace::LogOut(const char * fmt, ...) {
	// Lock to ensure that output goes to file in some reasonable ordering
	boost::recursive_mutex::scoped_lock lock(_mtx);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(_fd_out, fmt, ap);
	va_end(ap);
}
void STrace::WriteMyStack() {
	std::string stack = GenStackTrace();
	//std::cerr << stack.c_str();
	AddHash(stack);
	//LogOut("===================\nTrace:\n%s\n=================\n", stack.c_str());
}

std::string STrace::GenStackTrace() {
	std::stringstream ret; 	
	std::vector<Frame> stackwalk;
	std::string s;
	void * sym; 
	Walker * walker = Walker::newWalker();
	walker->walkStack(stackwalk);
	Dyninst::Offset offset;
	for (int i = 0; i < stackwalk.size(); i++) {
		stackwalk[i].getName(s);
		ret << s << " - ";
		if(stackwalk[i].getLibOffset(s, offset, sym) == false){
		 	continue;
		}
		Symtab * curSym = static_cast<Symtab *>(sym);
		if (sym == NULL) {
			std::cerr << "we dont have a symtab" << std::endl;
			ret << "\n";
			continue;
		}
		std::vector<Statement *> lines;
		if((curSym)->getSourceLines(lines,offset) == false) {
			ret << "\n";
			continue;
		}
		for (int q = 0; q < lines.size(); q++) {
			ret << lines[q]->getFile() << ":" << std::to_string(lines[q]->getLine()) << " ";
			//ret << lines[i]->getFile() << " " << std::endl;// << std::to_string(lines[i]->getLine()) << std::endl;
		}
		// stackwalk[i].getObject(sym);
		ret << "\n";
	}
	// std::cerr << ret;
	return ret.str();
}

