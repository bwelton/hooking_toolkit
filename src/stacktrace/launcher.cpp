#include "StraceThirdPartyLauncher.h"
std::shared_ptr<STraceThirdPartyLauncher> StraceThirdLaunch;

std::vector<BPatch_function *> findFuncByName(BPatch_image * appImage, const char * funcName)
{
  /* fundFunctions returns a list of all functions with the name 'funcName' in the binary */
  BPatch_Vector<BPatch_function * >funcs;
  if (NULL == appImage->findFunction(funcName, funcs) || !funcs.size() || NULL == funcs[0])
  {
      std::cerr << "Failed to find " << funcName <<" function in the instrumentation library" << endl;
      return std::vector<BPatch_function *>();
  }
  std::cerr << "Found " << funcName << " this many times " << funcs.size() << endl;
  if (funcs.size() > 1) {
    for(int i = 0; i < funcs.size(); i++ )
    {
        std::cerr << "    " << funcs[i]->getName() << std::endl;
    }
  }
  return funcs;
}

static void handle_signal (int sig, siginfo_t *siginfo, void *context)
{
	BUILD_STORAGE_CLASS_ARGS(TOFILE, "StackTraces.txt")
	BPatch bpatch;
	fprintf(stderr,"we are here!\n");
	int id = siginfo->si_pid;
	BPatch_process * proc = bpatch.processAttach(NULL, id);
	if (proc == NULL) {
		fprintf(stderr, "%s\n", "Failed to attach to process, exiting\n");
		exit(-1);
	}
	fprintf(stderr, "%s\n", "TRACER: Attached to process");
	BPatch_image * appImage = proc->getImage();
	std::vector<BPatch_function *> rep_funcs = findFuncByName(appImage, "STRACE_RECORD_STACKTRACE");
	BPatch_Vector<BPatch_point *> points;
	rep_funcs[0]->getEntryPoints(points);
	BPatch_breakPointExpr bp;
	proc->insertSnippet(bp, points);
	proc->continueExecution();
	STORAGE_PTR->waitUntilStopped(&bpatch, proc, id);
	exit(0);
}
 
int main(int argc, char * argv[]) {
	int host_pid = getpid();
	if (fork() == 0){
		// we are the child, sleep for 5 seconds and spawn the process spawn the process
		fprintf(stderr, "%s\n", "In child process");
		sleep(10);
		execvp(argv[1], &(argv[1]));
	} else {
		fprintf(stderr, "%s\n", "In parent process");
		FILE * fd = fopen("tp_pid.pid","w");
		fprintf(fd, "%d ", host_pid);
		fclose(fd);
		struct sigaction handler;
		memset (&handler, '\0', sizeof(handler));
		handler.sa_sigaction = &handle_signal;
		handler.sa_flags = SA_SIGINFO;
		if( sigaction(SIGUSR1, &handler, NULL) < 0) {
			fprintf(stderr, "%s\n", "Could not set signal handler");
		}
		fprintf(stderr, "%s\n", "Signal handler set, now waiting...");
		while(1)
			sleep(10);
	}
}