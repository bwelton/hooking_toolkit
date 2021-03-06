#include "StraceThirdPartyLauncher.h"
//#include <boost/interprocess/shared_memory_object.hpp>
#include <signal.h>
std::shared_ptr<STraceThirdPartyLauncher> StraceThird;

STraceThirdPartyLauncher::STraceThirdPartyLauncher(StraceOutpurLocation out, char * fname) {
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

STraceThirdPartyLauncher::~STraceThirdPartyLauncher() {
    if (_storage_location == TOFILE) {
        fclose(_fd_out);
    }
}

void STraceThirdPartyLauncher::flush() {
    if (_storage_location == TOFILE)
        fflush(_fd_out);
}

void STraceThirdPartyLauncher::LogOut(const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(_fd_out, fmt, ap);
    va_end(ap);
}

void STraceThirdPartyLauncher::WriteStacks() {
    for(auto i : _storedStacks) {
        LogOut("======================\nSTACK:\n%s======================", i.second.c_str());
    }   
    _storedStacks.clear();
    flush();
}

void STraceThirdPartyLauncher::AddTrace(std::string i) {
    hash<std::string> hasher;
    size_t hash = hasher(i);
    _storedStacks[hash] = i;
    if (_storedStacks.size() > 100) {
        WriteStacks();
    }
}

void STraceThirdPartyLauncher::ContinueExec() {
    _proc->continueExecution();
}

void STraceThirdPartyLauncher::waitUntilStopped(BPatch *bpatch, BPatch_process *appProc, int proc_id)
{
  size_t count = 0;
  fprintf(stderr, "%s\n", "Entering waiting loop....");
  _proc = appProc;
  appProc->continueExecution();
  while (!appProc->isTerminated()) {
    fprintf(stderr, "%s\n", "loop iter....");
      while (!appProc->isStopped() && !appProc->isTerminated()) {
            bpatch->pollForStatusChange();
      }
      if (appProc->isTerminated())
        break;
      count++;
      fprintf(stderr, "%s\n", "Process has returned from being stopped");
      std::vector<std::string> ret = GenStackTrace(bpatch, appProc, proc_id);
      for(auto i : ret) {
            if(i.find("DYNINSTbreakPoint") != std::string::npos){
                AddTrace(i);
            }
      }
      if (count % 500 == 0)
        WriteStacks();
      if (!appProc->isTerminated())
        appProc->continueExecution();
  }
  WriteStacks();
}

std::vector<std::string> STraceThirdPartyLauncher::GenStackTrace(BPatch *bpatch, BPatch_process *appProc, int proc_id) {
    std::vector<std::string> rvec;
    BPatch_Vector<BPatch_thread *>  threads;
    appProc->getThreads(threads);
    char name[512];
    for (BPatch_thread * t : threads) {
        std::stringstream ret;
        BPatch_Vector<BPatch_frame> frames;
        t->getCallStack(frames);
        for (BPatch_frame f : frames) {
            BPatch_function *func = f.findFunction();
            if (func != NULL) {
                func->getName(name, 512);
                ret << name << std::endl;
            }
            else {
                //ret << "[UNKNOWN]" << std::endl;
            }
        }
        rvec.push_back(ret.str());
    } 
    return rvec;
}

