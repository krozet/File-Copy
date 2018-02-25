
/*
Keawa Rozet
CSC 415
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 27 //27 bytes

int copyFromFileToFile(char *from, char *to);
void printDebugRead(char *buf, int count);
void printDebugWrite(char *buf, int count);
void printDebugSynopsis(int fd_input, int fd_output, int bytes);
void printWriteToScreen(char *msgToPrint);
void correctFilePath(char *path);
void setUserInput(char *input, int size);
void printSuccess(int bytes);

//used for error checking
int bytes_formatted, bytes_printed = -1;

int main(char *argv[], int* argc) {
  char copyFrom[256], copyTo[256];

  char msgGreeting[] = "Welcome to the File Copy Program by Keawa Rozet!\n";
  char msgCopyFrom[] = "Enter the name of the file to copy from:\n";
  char msgCopyTo[] = "Enter the name of the file to copy to:\n";

  //write greeting and copy from
  printWriteToScreen(msgGreeting);
  printWriteToScreen(msgCopyFrom);

  //read user input for copy from
  char *pCopyFrom = copyFrom;
  setUserInput(pCopyFrom, sizeof(copyFrom));

  //write prompt user for file to copy to
  printWriteToScreen(msgCopyTo);

  //read user input for copy from
  char *pCopyTo = copyTo;
  setUserInput(pCopyTo, sizeof(copyTo));

  //copies file and prints success if everything copied correctly
  printSuccess(copyFromFileToFile(pCopyFrom, pCopyTo));

  return 0;
}

//copites data from one file to another and returns the number of bytes copied
int copyFromFileToFile(char *from, char *to) {
  int fd_input, fd_output = 1;
  ssize_t bytes_input, bytes_output = -1;
  char buf[BUFFER_SIZE];

  //removes trailing '\n'
  correctFilePath(from);
  correctFilePath(to);

  //opens from file
  fd_input = open(from, O_RDONLY);
  if (fd_input < 0)
    perror ("error openning from...");

  //checks if to file is already created; returns if it is
  fd_output = open(to, O_RDONLY | O_WRONLY | O_RDWR);
    if (fd_output > 0) {
      close(fd_output);
      close(fd_input);
      return -1;
    }

  //creates to file with permissions 644
  fd_output = open(to, O_CREAT | O_WRONLY, 0644);
  if (fd_output < 0)
    perror ("error openning to...");

  int count = 0;
  while((bytes_input = read(fd_input, &buf, BUFFER_SIZE)) > 0){
    //uncomment for debugging
    //printDebugRead(buf, bytes_input);

    bytes_output = write (fd_output, &buf, bytes_input);
      if(bytes_output != bytes_input)
        perror("error writing file to file...");

    //uncomment for debugging
    //printDebugWrite(buf, bytes_output);

    count += bytes_output;
  }

  //uncomment for debugging
  //printDebugSynopsis(fd_input, fd_output, count);

  close(fd_input);
  close(fd_output);

  return count;
}

//prints fail if to-file is already in the directory
//prints success if all bytes were written over
void printSuccess(int bytes) {
  char msgFailed[] = "File Copy Failed. Destination file already exists.\n";

  if (bytes == -1) {
    char *pFailed = msgFailed;
    printWriteToScreen(msgFailed);
    return;
  }

  char msg[256];
  bytes_formatted = bytes_printed = -1;
  bytes_formatted = sprintf(msg, "File Copy Successful, %d bytes copied\n", bytes);
  if (bytes_formatted < 0)
    perror ("error running sprintf..");
  bytes_printed = write(STDOUT_FILENO, &msg, bytes_formatted);
  if (bytes_printed < 0)
    perror ("error running sprintf..");
}

//removes '\n' at the end of the string if there is one
void correctFilePath(char *path) {
  for (int i=0; path[i] != '\0'; i++)
    if (path[i] == '\n')
      path[i] = '\0';
}

//print message to the user
void printWriteToScreen(char *msgToPrint) {
  char msg[256];
  bytes_formatted = bytes_printed = -1;

  bytes_formatted = sprintf(msg, msgToPrint);
  if (bytes_formatted < 0)
    perror ("error running sprintf..");
  bytes_printed = write(STDOUT_FILENO, &msg, bytes_formatted);
  if (bytes_printed < 0)
    perror ("error running sprintf..");
}

//prints a debug log of the read function to see what was read
void printDebugRead(char *buf, int count) {
  char debugMsg[22];
  bytes_formatted = bytes_printed = -1;

  bytes_formatted = sprintf(debugMsg, "\nDebug read log ---> ");
  if (bytes_formatted < 0)
    perror ("error running sprintf in printDebugRead..");
  bytes_printed = write(STDOUT_FILENO, &debugMsg, bytes_formatted);
  if (bytes_printed < 0)
    perror ("error running write in printDebugRead..");
  bytes_printed = write(STDOUT_FILENO, buf, count);
  if (bytes_printed < 0)
    perror ("error running write in printDebugRead for char *buf..");
  }

//prints a debug log of the write function to see what was written
void printDebugWrite(char *buf, int count) {
  char debugMsg[23];
  bytes_formatted = bytes_printed = -1;

  bytes_formatted = sprintf(debugMsg, "\nDebug write log ---> ");
  if (bytes_formatted < 0)
    perror ("error running sprintf in printDebugRead..");
  bytes_printed = write(STDOUT_FILENO, &debugMsg, bytes_formatted);
  if (bytes_printed < 0)
    perror ("error running write in printDebugRead..");
  bytes_printed = write(STDOUT_FILENO, buf, count);
  if (bytes_printed < 0)
    perror ("error running write in printDebugRead for char *buf..");
}

//prints debug log for the end of the copy function
void printDebugSynopsis(int fd_input, int fd_output, int bytes) {
  char debugMsg[256];
  bytes_formatted = bytes_printed = -1;

  bytes_formatted = sprintf(debugMsg, "Debug synopsis log ---> fd_input: %d\tfd_output: %d\ttotal bytes output: %d\n", fd_input, fd_output, bytes);
  if (bytes_formatted < 0)
    perror ("error running sprintf in printDebugRead..");
  bytes_printed = write(STDOUT_FILENO, &debugMsg, bytes_formatted);
  if (bytes_printed < 0)
    perror ("error running write in printDebugRead..");
}

//stores the user input into char array
void setUserInput(char *input, int size) {
  bytes_formatted = bytes_printed = -1;

  bytes_formatted = read(STDOUT_FILENO, input, size);
  if (bytes_formatted < 0)
    perror ("error running read for first user input...");

  //uncomment for debugging
  //printDebugRead(input, bytes_formatted);
  }
