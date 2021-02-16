#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <signal.h>

#include <Env_Monitor.h>

using namespace std;

int main(int argc, char* argv[])
{
  pid_t pid = fork();
  cout << "Env_Monitor: Pid= " << pid << endl;
  
  if(pid<0)
    {
      cerr << "Fork error." << endl;
      exit(0);
    }
  else if(pid > 0)
    {
      cout << "Child process: "<< pid << ", parent process: " <<  getpid() << endl;
      exit(0);
    }
  else if(pid==0) cout << "Process: " << getpid() << endl;

  //to prevent sighup
  signal(SIGHUP, SIG_IGN);
  close(0);
  close(1);
  close(2);

  //change working dir.
  chdir("/");

  //become new session leader
  setsid();

  //Sensor
  Env_Monitor env_monitor;
  env_monitor.Run();
  
  return 0;
}//
