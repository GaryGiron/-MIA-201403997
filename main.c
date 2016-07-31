#include <stdio.h>
#include <stdlib.h>


int main(void){
 char cadena [100];

 int a=0;
 while(a==0){
  printf ("File System ext2/ext3:#~$ ");
  fgets (cadena, 100, stdin);
  int len = strlen(cadena);
  int i;

  char str[]="/storage/SD:/storage/USB1";
    const char* del=":";
    char* arr[2];
    split(arr, str, del);
    return 0;
 }


return 0;
}

void split(char** dest, char* src, const char* del){
    char* token = strtok(src, del);
    while(token!=NULL){
       *dest++ = token;
       token = strtok(NULL, del);
    }
}

