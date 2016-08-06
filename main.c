#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

//particiones
typedef struct PART{
char part_status;
char part_type;
char part_fit[3];
int part_start;
int part_size;
char part_name[16];
}PART;

//master boot record
typedef struct MBR{

int mbr_tamano;
char* mbr_fecha_creacion;
int mbr_disk_signature; //numero random que identifica cada disco
PART mbr_partition[4];
}Disco;

//extended boot record
typedef struct EBR{
char part_status;
char part_fit;
int part_start;
int part_size;
int part_next;
char part_name[16];
};
//mkdisk
int tam_disco=0;
char* unit_disco;
char* ruta="/0";
char* nombre="/0";

char relleno[1024];

Disco creaRegistro(int tam, char* date, int num){

Disco mbr_init;

mbr_init.mbr_tamano=tam;
mbr_init.mbr_fecha_creacion=date;
mbr_init.mbr_disk_signature=num;

return mbr_init;

}


char* CrearArchivo(char* name, Disco mbr_init, int bits){
    int len = strlen(ruta);
        char path[len-2];
        int x;
        for(x=0;x<len-2;x++){
            path[x]=ruta[x+1];
        }

        ruta=path;
    printf("Esta es la ruta: %s", ruta);
 if(access(ruta, 0 ) == 0 ){
    char* path=ruta;//+ nombre
    int longit = strlen(name);
        char tempo[longit-2];
        for(x=0;x<len-2;x++){
            tempo[x]=name[x+1];
        }

        name=tempo;
    strcat(path,"/");
    strcat(path,name);
    FILE *archivo=fopen(path,"w+b");
    if(archivo){
        fwrite(&mbr_init, sizeof(Disco),1,archivo);
        int x;
        for(x=0; x<1024;x++){
            relleno[x]="/0";
        }
        int i;
        for(i=0;i< (bits - sizeof(Disco)) ;i++){
             fwrite(&relleno,sizeof(relleno),1,archivo);
        }
        fclose(archivo);
        printf("El Disco ha sido creado exitosamente...\n");
        return path;
    }else{
        printf("Error al crear el Disco...\n");
        return "";
    }
 }else{
    printf("La ruta ingresada no existe...");
    return "";
 }

}




char* horario(){

        time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
        printf("%s\n",output);
  return output;
}

int crearCarpeta(char* path){
 if(access(path, 0 ) == 0 ){
     printf("ya la carlitos");
    return 1;
}else{
    printf("ya salio");
    char* parms="mkdir ";
    strcat(parms,path);

printf(parms);
system(parms);
printf("Carpeta Creada exitosamente\n");
return 0;
}

}

int generarRandom()
{
     int i, j, num, dupl;

     printf("RANDOM\n");
     for (i = 0; i < 100; i++){
         num = 1 + rand() % 100;
         dupl = 0;
         printf("%-4d", num);
         //hacer algo para que no se duplique
         if (dupl == 1)
            i--;
         else
            return num;
     }
}

int main(void){
 char cadena [100];
 int a=0;
unit_disco="m";

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
            return 19;
        }else if(strcmp(token,"umount")==0){
            return 23;
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
                tam_disco=num;
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
        unit_disco="k";
        return 3;
        }else if((strcmp(token,"m")==0)||(strcmp(token,"M")==0)){
            printf("se guardara en Mb...\n");
            unit_disco="m";
            return 3;
        }
        else{
       printf("\n Debe de ingresar la unidad del disco:\n");
       return 0;
    }
    }else if(val==5){//ruta para crear
        int len = strlen(token);
        char path[len];
        int x;
        for(x=0;x<len-2;x++){
            path[x]=token[x+1];
        }

        ruta=token;
        printf("%s",ruta);
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
        nombre=token;
        printf("%s",nombre);
        printf("nombre seleccionado");
        int tam;
        if(strcmp(unit_disco,"m")==0){
            tam=tam_disco*1024;
        }else{
            tam=tam_disco;
        }
        CrearArchivo(nombre, creaRegistro(tam_disco,horario(),generarRandom()),tam);

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
            return 13;
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
            return 0;
        }
    }else if(val==12){
        printf("seleccionando nombre del archivo... \n");
        return 10;
    }else if(val==13){
        if(strcmp(token,"m")==0 || strcmp(token,"M")==0){
            printf("unidades en Mb... \n");
            return 10;
        }else if(strcmp(token,"k")==0||strcmp(token,"K")==0){
            printf("Unidades en Kb... \n");
            return 10;
        }else if(strcmp(token,"b")==0 || strcmp(token,"B")==0){
            printf("Unidades en bytes... \n");
            return 10;
        }else{
            printf("no se reconoce ese tipo de unidad.. \n");
        }
    }else if(val==14){
        int num=atoi(token);
        if(num>0){
        printf("aumentando tamaño.. \n");
        }else{
            printf("disminuyendo tamaño... \n");
        }
        return 10;
    }else if(val==15){
        printf("accediendo a la ruta del archivo... \n");
        return 10;
    }else if(val==16){
        //if(existe extendida){
         //  printf("creando particion logica");
        //}else
        if(strcmp(token,"e")==0 || strcmp(token,"E")==0){//falta condicion  que aun no exista extendida y hallan 4
            printf("ingresando particion extendida... \n");
            return 10;
        }else if(strcmp(token,"p")==0 || strcmp(token,"P")==0){
            printf("ingresando particion primaria... \n");
            return 10;
        }else{
            printf("no se reconoce el parametro enviado a la funcion \n");
            return 0;
        }

    }else if(val==17){
        if(strcmp(token,"BF")==0){
            printf("determinado como mejor ajuste... \n");
            return 10;
        }else if(strcmp(token,"WF")==0){
            printf("determinado como peor ajuste... \n");
            return 10;
        }else if(strcmp(token,"FF")==0){
            printf("determinado como primer ajuste... \n");
            return 10;
        }else{
            printf("no se reconoce el tipo de ajuste... \n");
            return 0;
        }
    }else if(val==18){
        if(strcmp(token,"full")==0||strcmp(token,"FULL")==0){
            printf("se selecciono el eliminado completo");
        }else if(strcmp(token,"fast")==0||strcmp(token,"FAST")==0){
            printf("se selecciono el eliminado rapido");
        }else{
            printf("el tipo de parametro para la eliminacion es incorrecto... \n");
            return 0;
        }
        return 10;
    }else if(val==19){
        if(strcmp(token,"path")==0){
            printf("reconociendo ruta del archivo... \n");
            return 20;
        }else{
            printf("debe ingresar la ruta (-path) \n");
            return 0;
        }
    }else if(val==20){
        printf("guardando la ruta del archivo...\n");
        return 21;
    }else if(val==21){
        if(strcmp(token,"name")==0){
            printf("reconociendo nombre del archivo... \n");
            return 22;
        }else{
            printf("debe ingresar el nombre(-name) \n");
            return 0;
        }
    }else if(val==22){
        printf("guardando el nombre del archivo...\n");
        return 0;
    }else if(val==23){
        char id[3];
        id[0]=token[0];
        id[1]=token[1];
        if(strcmp(id,"id")==0){
            printf("ingresando al %s ... \n",token);
            return 24;
        }else{
            printf("debe de ingresar un id para llegar al disco... \n");
        }
    }else if(val==24){
        char id[3];
        id[0]=token[0];
        id[1]=token[1];
        if(strcmp(id,"VD")==0 || strcmp(id,"vd")==0){
            printf("ingresando al %s ... \n",token);
            return 23;
        }else{
            printf("debe de ingresar un VD del disco... \n");
            return 0;
        }
    }

}



