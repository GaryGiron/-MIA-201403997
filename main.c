#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo{

int tam;
}Disco;

int main(void){
 char cadena [100];

 int a=0;
 while(a==0){
  printf ("File System ext2/ext3:#~$ ");
  fgets (cadena, 100, stdin);
  char delimit[]=" ";
  int len;
  int i;

char *token, *temp;

int estado=0;
    len = strlen(cadena);
    if((token = strtok(cadena, " "))!=NULL){
    printf("analizando comando: %s\n", token);
    estado=funciona(estado, token);
    }
    while((token=strtok(NULL, " "))!=NULL) {
        printf("analizando comando: %s\n", token);
        estado=funciona(estado, token);
    }
    //accion(estado);

 }return 0;

}
int funciona(int val, char* token){
    if(val==0){//inicia el comando
        if(strcmp(token,"mkdisk")==0){
            return 1;
        }else if(strcmp(token,"rmdisk")==0){
            return 8;
        }else if(strcmp(token,"fdisk")==0){
            return 10;
        }else if(strcmp(token,"mount")==0){

        }else if(strcmp(token,"umount")==0){

        }
        else{
            printf("\n Error no se ha diseñado otro comando en esta aplicacion puede usar los siguientes:\n");
            printf("\n - mkdisk    -rmdisk     -fdisk      -mount      -umount \n ");
            return 0;
        }
    }else if(val==1){//acaba de pasar por mkdisk
        if(strcmp(token,"-size")==0){
            return 2;
        }else{
            printf("\n Debe de ingresar el tamaño del disco:\n");
            return 0;
        }
    }else if(val==2){//valor del tamaño
        if(isdigit(token)){
            return 3;
        }else{
            printf("\n Debe ingresar un valor numerico para indicar el tamaño \n");
        }
    }else if(val==3){//comando unit
        if(strcmp(token,"+unit")==0){
            return 4;
        }else if(strcmp(token,"-path")==0){
            return 5;
        }else{
            printf("\n se ha ingresado un parametro que no corresponde en este instante pruebe usando: \n");
            printf("\n +unit  --> para indicar la medida del tamaño del disco \n");
            printf("\n -path  --> para indicar la ruta del disco \n");
        }
    }else if(val==4){//unidad en que se guardara
        if(token=="k" || token =="K" || token=="m" || token=="M"){
            return 3;
        }else{
            printf("\n He sido diseñado para crear archivos con tamaño en Mb y Kb\n");
            printf("\n intente nuevamente pero usando de unidad m o k \n");
        }
    }else if(val==5){//ruta para crear
        if(token[0]=="\""){
            return 6;
        }else{
            printf("\n debe indicar en una cadena cual es la ruta de su archivo \n");
        }
    }else if(val==6){//nombrre para crear
        if(strcmp(token,"-name")==0){
            return 7;
        }else{
            printf("\n se necesita el nombre que recibira el archivo \n");
            printf("\n ingrese el comando -name");
        }
    }else if(val==7){//ruta creada
        if(token[0]=="\""){
            return 0;
        }else{
            printf("\n debe indicar en una cadena cual es el nombre del archivo \n");
        }
    }else if(val== 8){
        if(strcmp(token,"-path")==0){
            return 9;
        }else{
            printf("se requiere el parametro path(-path)");
        }
    }else if(val==9){
        if(token[0]=="\""){
            return 0;
        }else{
            printf("\n debe indicar en una cadena cual es la ruta que desea eliminar \n");
        }
    }else if(val==10){

    }
}


char* CrearArchivo(char* ruta, char* nombre){
    char* path=ruta;//+ nombre
    FILE *archivo=fopen(path,"w+b");
    if(archivo){
        printf("El Disco ha sido creado exitosamente...\n");
        return path;
    }else{
        printf("Error al crear el Disco...\n");
        return "";
    }

}

void Guardar(Disco dev, char* path){
FILE* file = fopen(path,"ab");
        if(path ==NULL){
            printf("No se pudo acceder al Disco");

        }
        else{
            fwrite(&dev, sizeof(Disco),1,file);
            printf("Particion Creada");
            fclose(file);
        }
}
