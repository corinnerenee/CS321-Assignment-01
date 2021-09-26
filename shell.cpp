/* Simple Shell program: g++ shell.cpp -o myshell -lcrypt */ 
/* include c header files */
#include <stdlib.h>
#include <unistd.h> // for function fork()
#include <sys/wait.h> // for function wait()
#include <stdio.h>
// include c++ header files
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream> //used for parsing file
#include <vector>
#include <iomanip>
#include <unistd.h>

#define N 10 //You may increase N later for the final testing
#define LOGOUTCODE 5

using namespace std;

void build_command();
void user_login();
void type_prompt();
int read_command(string command, string *parameters);
void exec_command(int opcode, string *parameters);
char *commands[7];

int main()
{
  int i=0,opcode=0;
  int pid=0, status=0;
  string command, parameters[3];
  build_command();
	/* read in the commands into a table. */ 
  user_login();          /* Authenticate the user */
  while (i < N) {        /* repeat maximum N times */
    type_prompt( );    /* display prompt */
    opcode = read_command(command, parameters);  
		/* input from terminal */
    if (opcode>0) { /* valid command */
      if (pid = fork() != 0) { /* Parent code */
          pid = wait( &status); /* wait for child to exit */
	     cout << "Parent: returned Child PID= " << pid << "\n";
		cout << "Parent: returned Child Status= " << status << "\n";
           if (status == 0x0500) exit(0); /* status must be a hex number */
           /* For example: LOGOUTCODE is 0x0500 is child terminated with the command exit(5) */
      } /* end of parent code */
      else { /* Child code */
		cout << "Child: returned PID= " << pid << "\n";
           exec_command (opcode, parameters);      /* execute command */ 
           exit(0); 
      } /* end of child code */
   } else { cout << "Invalid command, try again\n"; } 
   i++;
  } 
  return 1;
}

void build_command()
{
	cout << "Build command function: \n";

    ifstream commandFile;
    int i = 0;
    commandFile.open("shellcommands.txt");

    if(commandFile.is_open()){
      while (commandFile.eof()){
        commandFile >> commands[i];
        i += 1;
      }
      commandFile.close();
    }
    else cout << "Unable to open file";
    
	
}

void user_login()
{
	cout << "User login function: \n";
	  string password, user, encryptP;
    string vP, vU;
    const char* salt = "03";
    int tries = 3;
    int iFlag = 0; //for exiting once condition is met

     //open file
    ifstream myfile;
    myfile.open("info.txt");
    getline(myfile, vU);
    getline(myfile, vP);
  
    //take user input
    cout << "Username: ";
    cin >> user;

    cout << "Password:  ";
    cin >> password;

    //encrypt users input

    encryptP = crypt(password.c_str(), "22");
    //cout << "\n" << vP;
    

    //i didnt check the username since we can assume theres only one user
    if (vP == encryptP){
        cout << "Login Successful\n";
        return;
    }
    /*else if (vP != encryptP){

      for (int i = 0; i < tries; i++){
        //cout << password;
  
        cout << "\n Password incorrect, please try again";
        cin >> password;
        password = crypt(password.c_str(), "22");
        //cout << password;

        //check, if successful exit
        if(vP == encryptP){
          cout << "\nLogin Successful";
          //how do i exit the loop(early) once login is succesful
        }
      }
    }*/

   return;
}

void type_prompt()
{
	cout << "MSH> ";
}
void change_pw(){
	cout << "Change password function: \n";
	string password, encryptP;
	string vP, vU;
	const char* salt = "03";
	int tries = 3;
	int iFlag = 0; //for exiting once condition is met


	cout << "Enter new password: ";
	cin >> password;

	encryptP = crypt(password.c_str(), "22");
	
	 //open file
	ifstream myfile;
	myfile.open("info.txt");
	getline(myfile, vU); //Skip username
	getline(myfile, vP);
	myfile >> encryptP;

	cout << "Password successfully changed.\n";
}
int read_command(string command, string *parameter)
{
	int opcode=0;
	string commandString;
	
	cin >> commandString; //read in command
	istringstream cmdStream(commandString);

	cmdStream >> command;
	/*for (int i = 0; i < 7; i++) {
		string comp = (string)commands[i];
		if (command.compare(comp) == 0) {
			opcode = i + 1;
		}
	}*/
	
	if (command.compare("MSHpwd") == 0) {
		opcode = 1;
	}
	if (command.compare("MSHcopy") == 0) {
		opcode = 2;
	}
	if (command.compare("MSHps") == 0) {
		opcode = 3;
	}
	if (command.compare("MSHdf") == 0) {
		opcode = 4;
	}
	if (command.compare("MSHsearch") == 0) {
		opcode = 6;
	}
	if (command.compare("MSHhistory") == 0) {
		opcode = 7;
	}
	
	/* search the table to return the opcode */
	if (command.compare("MSHlogout") == 0) {
		opcode = LOGOUTCODE;
	}
	return opcode;	
}

void exec_command(int opcode, string *parameters)
{
	cout << "Child: Execute command function: 1" + (string)"\n";
  int cmdCount = 0;
	/* Using the case statement to run the simple shell commands */
	/* Write your code here  */
  //search 
	switch(opcode){
	case 1://MSHpwd - change password
		cmdCount++;
		change_pw();
		//printf("you are currently in %s", get_current_dir_name());
		break;
    case 2: //MSHcopy - system.cp
		//system("cp " + );
		break;
    case 3: //MSHps - ps -ef - displays status of processes
		system("ps -ef");
		break;
    case 4: //MSHdf - display disk space occupied by file system
		system("df");
		break;
    case 5: //MSHsearch - search word from file using grep
      //break;
    case 6: //MSHhistory - if cmd is valid, add history to table & displayTable() to list all previous commands
		system("history");
		break;
    case 7: //MSHlogin - use logout code?
      //break;
    default:
      cout << "Invalid command\n";
      break;
	}
  	if (opcode == LOGOUTCODE) {
    	  cout<<"Child: exit with status = LOGOUTCODE\n";
    	  exit(LOGOUTCODE);
  	}
	return;
}