// Main Function

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>
#include <cmath>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include <signal.h>
#include <cstdio>
#include <cstdlib>

//new requirements: piping (2 commands), prevent ctrl+C termination, change directories
// accept cd [dir] and pwd, change command prompt to show the current directory in the prompt

void ptime(int index, std::vector<std::string>& hist, double time);
void history(int index, std::vector<std::string>& hist, double time);
void changedir(std::string stringdir);
void doPipe(std::string instream);

int main() {
	signal(SIGINT, SIG_IGN);

	double ctime=0;
	std::vector<std::string> hist;
	hist.push_back("History of input commands:");

	//main approach: get a command, fork into command's program with execvp
	//execvp is replaced by the process it calls and will never terminate itself

	bool noexit = 1;	//subtle literary reference

	while (noexit) {

		//modus operandi
		char *dirpath = (char*)get_current_dir_name();
		std::cout << "["<< dirpath <<"]:  ";
		std::string instream = "";
		std::getline(std::cin, instream);
		if (instream==""){
			continue;
		}
		else{

			hist.push_back(instream);

			//look for piping
			if (instream.find("|") != std::string::npos){
				//std::cout<<"pipe found; piping\n";
				//call to piping
				doPipe(instream);
				//continue;
			}

			//parse instream
			std::istringstream iss(instream);
			std::string word;
			std::vector<std::string> arguments;
			while(iss>>word){
				arguments.push_back(word);
			}

			if (arguments[0] == "exit"){
				noexit = 0;
				break;
			}

			if(arguments[0] == "^"){
				//reassign arguments to be the history command
				std::string histstream=hist[std::stoi(arguments[1])];
				std::istringstream hss(histstream);
				std::string wrd;
				arguments.clear();
				arguments.resize(0);
				while(hss>>wrd){
					arguments.push_back(wrd);
				}
			}
			if(arguments[0] == "cd"){
				//std::cout<<arguments[1]<<std::endl;
				changedir((std::string)arguments[1]);
				continue;
			}

			int pid=fork();	
			int exitstatus=0;
			if (pid>0) {	//parent
				//do fork things
				std::chrono::time_point<std::chrono::high_resolution_clock> start =
					std::chrono::high_resolution_clock::now();
				waitpid(pid, &exitstatus, 0);
				std::chrono::time_point<std::chrono::high_resolution_clock> end =
					std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> time = end-start;
				ctime+=time.count();
				//ptime+=static_cast<double>(time.count());
			}
			else if(pid==0) {
				//do child things
				if(arguments[0]=="ptime"){
					ptime(0,hist,ctime);
					break;
				}
				if(arguments[0]=="history"){
					history(0,hist,ctime);
					break;
				}
				//execvp the command
				int SIZE=instream.size()+2;
				char** argv = new char*[SIZE];
				argv[0]=new char[arguments[0].length()+1];
				strcpy(argv[0], arguments[0].c_str());
				for(int i=1;i<arguments.size();i++){
					argv[i]=new char[arguments[i].length()+1];
					strcpy(argv[i], arguments[i].c_str());
				}
				argv[arguments.size()]=NULL;
				//first argument: int index
				//second argument: vector hist
				//third argument: double time

				if(execvp (argv[0],argv)<0){
					std::cout<<"invalid command\n";
				}
				break;
			}
			else{
				std::cout<<"fork function failure \n";
			}
		}
	}
}

void ptime(int index, std::vector<std::string>& hist, double time) {
	std::cout << "Total execution time of child processes: ";
	std::cout << time << "seconds.\n";
}


void history(int index, std::vector<std::string>&  hist, double time){
	//store commands and arguments as a vector, then print them out
	//start record at index 1
	for (int i=0; i<hist.size(); i++){
		std::cout<<i<<"  "<<hist[i]<<std::endl;
	}
}

void changedir(std::string stringdir){
	int newdir;
	//stringdir = "/$HOME/" + stringdir;
	//const char *argdir  = stringdir.c_str();
	newdir = chdir(stringdir.c_str());
	if (newdir <0){
		std::cout<<"Ya done messed up, A-ARON (no such directory)\n";
	}
}

void doPipe(std::string instream){
	const int PIPE_COUNT = 2;
	const int PIPE_READ_END = 0;
	const int PIPE_WRITE_END = 1;

	const int STDIN = 0;
	const int STDOUT = 1;

	int pids [PIPE_COUNT];
	pipe(pids);

	int savedstdout = dup(STDOUT);
	int savedstdin = dup(STDIN);

	std::istringstream iss(instream);
	std::string word;
	std::vector<std::string> prePipe;
	std::vector<std::string> postPipe;
	while(iss >> word && word != "|"){
		prePipe.push_back(word);
	}
	while(iss>>word){
		postPipe.push_back(word);
	}

	pid_t pid = fork();
	if (pid == 0){
		//std::cout<<"first child\n";
		//first child: first program in the pipe
		dup2(pids[PIPE_WRITE_END], STDOUT);
		int size=prePipe.size();
		char** argv = new char*[size+2];
		for(int i=0; i<size; i++){
			argv[i] = new char[(prePipe[i].size())];	//check size is word+1
			strcpy(argv[i], prePipe[i].c_str());
		}
		argv[size+1] = NULL;

		execvp(argv[0], argv);
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}

	pid_t pid2 = fork();
	if (pid2 == 0){
		//std::cout<<"second child\n";
		//second child: second argument in the pipe
		dup2(pids[PIPE_READ_END], STDIN);
		close(pids[PIPE_WRITE_END]);
		int size=postPipe.size();
		char** argv = new char*[size+2];
		for(int i=0; i<size; i++){
			argv[i] = new char [postPipe[i].size()+1];
			strcpy(argv[i], postPipe[i].c_str());
		}
		argv[size+1] = NULL;

		execvp(argv[0], argv);
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}

	int status;
	waitpid(pid, &status, 0);

	close(pids[PIPE_WRITE_END]);
	close(pids[PIPE_READ_END]);

	waitpid(pid2, &status, 0);

	dup2(savedstdout, STDOUT);
	dup2(savedstdin, STDIN);

	prePipe.clear();
	postPipe.clear();

}
