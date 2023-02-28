#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// From https://www.tutorialspoint.com/how-to-get-memory-usage-at-runtime-using-cplusplus
void mem_usage(double& vm_usage, double& resident_set) {
   vm_usage = 0.0;
   resident_set = 0.0;
   ifstream stat_stream("/proc/self/stat", ios_base::in); 

   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;
   unsigned long vsize;
   long rss;
   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
   >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
   >> utime >> stime >> cutime >> cstime >> priority >> nice
   >> O >> itrealvalue >> starttime >> vsize >> rss; 

   stat_stream.close();
   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
   vm_usage = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

double getMemory() {
   double vm, rss;
   mem_usage(vm, rss);
   return vm;
}
