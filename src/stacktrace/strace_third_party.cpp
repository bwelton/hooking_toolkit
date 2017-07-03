#include "strace_third_party.h"
#include <signal.h>
#include <unistd.h>
std::shared_ptr<STraceThirdParty> StraceThird;

STraceThirdParty::STraceThirdParty() {
    int par_pid = 0;
    FILE * fd_in = fopen("tp_pid.pid","r");
    fprintf(stderr, "%s\n", "File opened");
    fscanf(fd_in, "%d", &par_pid);
    fprintf(stderr, "%s: %d\n", "Parent Process ID", par_pid);
    fclose(fd_in);
    kill(par_pid, SIGUSR1);
    sleep(30);
}

