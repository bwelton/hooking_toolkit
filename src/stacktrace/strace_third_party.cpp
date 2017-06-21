#include "strace_third_party.h"
#include <boost/interprocess/shared_memory_object.hpp>
std::shared_ptr<STraceThirdParty> StraceThird;

STraceThirdParty::STraceThirdParty(StraceOutpurLocation out, char * fname) {
	_pid = getpid();
	if (fork() == 0) {
		// We are the child, we are going to become the stack trace watching process.
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
		ContinguousStackWalk();
	} else {
		return;
	}

}

STraceThirdParty::~STraceThirdParty() {
	if (_storage_location == TOFILE) {
		fclose(_fd_out);
	}
}

void STraceThirdParty::ContinguousStackWalk() {
	Walker * walker = Walker::newWalker(_pid);
	ProcessState * state = walker->getProcessState();
	
}


void STraceThirdParty::flush() {
	boost::recursive_mutex::scoped_lock lock(_mtx);
	fflush(_fd_out);
}

void STraceThirdParty::LogOut(const char * fmt, ...) {
	// Lock to ensure that output goes to file in some reasonable ordering
	boost::recursive_mutex::scoped_lock lock(_mtx);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(_fd_out, fmt, ap);
	va_end(ap);
}

void STraceThirdParty::WriteMyStack() {
	std::string stack = GenStackTrace();
	std::cerr << stack.c_str();
	LogOut("%s\n", stack.c_str());
}

std::string STraceThirdParty::GenStackTrace() {
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
