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

void ptime(int index, std::vector<std::string>& hist, double time);
void history(int index, std::vector<std::string>& hist, double time);

int main() {

	double ctime=0;
	std::vector<std::string> hist;
	hist.push_back("History of input commands:");

//	std::cout<<"[cmd]:  ";

	//main approach: get a command, fork into command's program with execvp
	//execvp is replaced by the process it calls and will never terminate itself

	bool noexit = 1;	//subtle literary reference

	while (noexit) {

		//modus operandi

		std::cout << "[cmd]:  ";
		std::string instream = "";
		std::getline(std::cin, instream);
		hist.push_back(instream);

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
			}
			if(arguments[0]=="history"){
				history(0,hist,ctime);
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

