#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

typedef struct nodo{

int tam;
}Disco;

int main(void){
 char cadena [100];

 int a=0;
 while(a==0){
  printf ("File System ext2/ext3:#~$ ");
  fgets (cadena, 100, stdin);
  char delimit[]="";
  int len;
  int i;

char *token, *temp;

int estado=0;
    len = strlen(cadena);
    if((token = strtok(cadena, " :-+"))!=NULL){
    printf("Dato: %s \n",token);
    printf("analizando comando: %s\n", token);
    estado=funciona(estado, token);
    }
    while((token=strtok(NULL, " :-+"))!=NULL) {
        printf("Dato: %s \n",token);
        printf("analizando comando: %s\n", token);
        estado=funciona(estado, token);
    }


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

        if(strcmp(token,"size")==0){
        printf("esperando valor...\n");
        return 2;
        }else{
       printf("\n Debe de ingresar el tamaño del disco:\n");
       return 0;
    }
    }else if(val==2){//valor del tamaño
        int num=atoi(token);
        if(num>0){
                printf("valor guardado...\n");
            return 3;
        }
        else{
            printf("debe ingresar un dato numerico para indicar el espacio a utilizar \n");
        }
    }else if(val==3){//comando unit
        if(strcmp(token,"unit")==0){
        printf("esperando unidad en que se guardara[k/m]...\n");
        return 4;
        }else if(strcmp(token,"path")==0){
            printf("esperando ruta del archivo...\n");
            return 5;
        }
        else{
       printf("\n Debe de ingresar la unidad o la ruta del disco:\n");
       return 0;
    }

    }else if(val==4){//unidad en que se guardara
        if((strcmp(token,"k")==0)||(strcmp(token,"K")==0)){
        printf("se guardara en Kb...\n");
        return 3;
        }else if((strcmp(token,"m")==0)||(strcmp(token,"M")==0)){
            printf("se guardara en Mb...\n");
            return 3;
        }
        else{
       printf("\n Debe de ingresar la unidad del disco:\n");
       return 0;
    }
    }else if(val==5){//ruta para crear
        printf("ruta localizada");
        return 6;
    }else if(val==6){//nombrre para crear
        if(strcmp(token,"name")==0){
            return 7;
        }else{
            printf("\n se necesita el nombre que recibira el archivo \n");
            printf("\n ingrese el comando -name \n");
        }
    }else if(val==7){//ruta creada
        printf("nombre seleccionado");
        return 0;
    }else if(val== 8){
        if(strcmp(token,"path")==0){
            printf("esperando ruta del archivo a eliminar...");
            return 9;
        }else{
            printf("se requiere el parametro path(-path)");
        }
    }else if(val==9){
        printf("realmente desea eliminar el archivo que se encuentra en: %s ",token);
        char opcion[50];
        scanf("%s",opcion);
        printf("su opcion %s \n",opcion);
        if(strcmp(opcion,"s")==0){
            printf("eliminando archivo...\n");
        }else{
            printf("El archivo no fue eliminado...\n");
        }
        return 0;
    }else if(val==10){
        printf("--------ADMINISTRADOR DE PARTICIONES---------");
        if(strcmp(token,"size")==0){
            printf("esperando tamaño del disco...\n");
            return 11;
        }else if(strcmp(token,"name")==0){
            printf("nombre del disco...\n");
            return 12;
        }else if(strcmp(token,"unit")==0){
            printf("seleccionando la unidad de medida en que se guardara..\n");
            retun 13;
        }else if(strcmp(token,"add")==0){
            printf("esperando cantidad a añadir...\n");
            return 14;
        }else if(strcmp(token,"path")==0){
            printf("esperando ruta del archivo...\n");
            return 15;
        }else if(strcmp(token,"type")==0){
            printf("reconociendo tipo de particion...\n");
            return 16;
        }else if(strcmp(token,"fit")==0){
            printf("forma de colocacion: \n");
            return 17;
        }else if(strcmp(token,"delete")==0){
            printf("cantidad a eliminar en particion \n");
            return 18;
        }else{
            printf("parametro no reconocido, porfavor intente de nuevo \n");
            printf("el comando fdisk tiene los parametros: -size -name +unit +add -path +type +fit +delete \n");
        }
    }else if(val==11){
        int num=atoi(token);
        if(num>0){
            printf("encontrado el tamaño de la particion... \n");
            return 10;
        }else{
            printf("el tamaño de la particion debe ser mayor a cero \n");
            return 10;
        }
    }else if(val==12){
        if(strcmp(token,"m")==0 || strcmp(token,"M")==0){
            printf("unidades en Mb... \n");
            return 10;
        }else if(strcmp(token,"k")==0||strcmp(token,"K")==0){
            printf("Unidades en Kb... \n");
            return 10;
        }else if(strcmp(token,"b")==0 || strcmp(token,"B")==0){
            printf("Unidades en bytes... \n");
            return 10;
        }
    }else if(val==13){
        int num=atoi(token);
        if(num>0){
        printf("aumentando tamaño.. \n");
        }else{
            printf("disminuyendo tamaño... \n");
        }
        return 10;
    }else if(val==14){
        printf("accediendo a la ruta del archivo... \n");
        return 10;
    }else if(val==15){

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

void crearCarpeta(char* path){
//System("mkdir %s",path);
}
