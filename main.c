#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

//particiones
typedef struct PART{
int part_status;
char part_type;
char part_fit[3];
int part_start;
int part_size;
char part_name[16];
}PART;

//master boot record
typedef struct MBR{
int mbr_tamano;
char mbr_fecha_creacion[20];
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

typedef struct mount{
    char dir_disco[50];
    char nombre_part[20];
    char id[20];
    struct mount *sig;
    struct mount *ant;
}*h,*temp,*temp1,*temp2,*temp4;

//mkdisk
int tam_disco=0;
char* unit_disco;
char* ruta="\0";
char* nombre="\0";
int id;

//fdisk
int fdisk=0;
int tam_part=0;
char* unit_part='k';
char* ruta_disco="-";
char* type='p';
int start;
char* fit;
char* name_part="-";
char* tipo_del;
int add=0;
int del=0;
int add_tam=0;

char relleno[1024];

//montar



Disco creaRegistro(int tam){
Disco mbr_init;
time_t tiempo = time(0);
        struct tm *tlocal = localtime(&tiempo);
        char output[128];
        strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
        strcpy(mbr_init.mbr_fecha_creacion,output);


int i, j, num, dupl;

     printf("RANDOM\n");
     for (i = 0; i < 100; i++){
         num = 1 + rand();
         dupl = 0;
         printf("%-4d", num);
         //hacer algo para que no se duplique
         if (dupl == 1)
            i--;
         else
            break;
     }

mbr_init.mbr_tamano=tam;
mbr_init.mbr_disk_signature=num;
mbr_init.mbr_partition[0].part_status=0;
mbr_init.mbr_partition[1].part_status=0;
mbr_init.mbr_partition[2].part_status=0;
mbr_init.mbr_partition[3].part_status=0;

printf("tamaño del struct: %d",sizeof(mbr_init));
return mbr_init;

}

char* CrearArchivo(char* name, Disco mbr_init, int bits){
    int len = strlen(ruta);
        char path[len-2];
        int x;
        int i,j;
        i=0;
        j=0;

        while (ruta[i+1] != '\0')
  {
    if (isalpha(ruta[i])||isdigit(ruta[i]))
    {
      path[j] = ruta[i];
	  j++;
    }else{
    if(ruta[i]=='/'){
        path[j] = ruta[i];
	  j++;
    }
    }
    i++;
  }

  path[j]='\0';
        ruta=path;
    printf("Esta es la ruta: %s", ruta);
    int carp;
   carp= crearCarpeta(ruta);
 if(access(ruta, 0 ) == 0 ){
    char* dir=ruta;//+ nombre

    int len = strlen(name);
        char path2[len-2];
        i=0;
        j=0;

        while (name[i+1] != '\0')
  {
    if (isalpha(name[i])||isdigit(name[i]))
    {
      path2[j] = name[i];
	  j++;
    }else{
        if(name[i]=='.'){
           path2[j] = name[i];
            j++;
        }
    }
    i++;
  }
    path2[j]='\0';
        name=path2;

    strcat(dir,"/");
    strcat(dir,name);
    FILE *archivo=fopen(dir,"wb+");
    if(archivo){
        fwrite(&mbr_init, sizeof(Disco),1,archivo);
        int x;
        for(x=0; x<1024;x++){
            relleno[x]='\0';
        }
        int i;
        int valor=(bits-sizeof(Disco));
        if(valor<0){
            printf("El disco no puede ser creado con tan poco espacio... \n");
            return "";
        }
        for(i=0;i<valor;i++){
             fwrite(&relleno,sizeof(relleno),1,archivo);
        }
        fclose(archivo);
        printf("El Disco ha sido creado exitosamente...\n");
        return dir;
    }else{
        printf("Error al crear el Disco...\n");
        return "";
    }
 }else{
    printf("No se pudo crear la carpeta...");
    return "";
 }

}

void eliminarArchivo(char* route){
    int len = strlen(route);
        char path[len-2];
        int i=0;
        int j=0;

        while (route[i+1] != '\0')
  {
    if (isalpha(route[i])||isdigit(route[i]))
    {
      path[j] = route[i];
	  j++;
    }else{
        if(route[i]=='.'){
           path[j] = route[i];
            j++;
        }else if(route[i]=='/'){
            path[j] = route[i];
            j++;
        }
    }
    i++;
  }
    path[j]='\0';
        route=path;

            FILE *fichero;
        fichero = fopen( route, "r" );
        if(fichero!=NULL){
        printf("realmente desea eliminar el archivo que se encuentra en: %s \n",route);
        char opcion[50];
        scanf("%s",opcion);
        printf("su opcion %s \n",opcion);
        if(strcmp(opcion,"s")==0){
            printf("eliminando archivo...\n");
            remove(route);
        }else{
            printf("El archivo no fue eliminado...\n");
        }
    }else{
        printf("El Archivo no existe \n");
    }
}

int crearCarpeta(char* path){
 if(access(path, 0 ) == 0 ){
     printf("la ruta si existe");
    return 1;
}else{
    printf("ya salio");
    char* parms="mkdir ";
    char*dest;
    strcat(dest,parms);
    strcat(dest,path);

printf(dest);
system(dest);
printf("Carpeta Creada exitosamente\n");
return 0;
}

}

void crearParticion(char* archivo){
    Disco mbr_init;
    int codigo;
    int bandera = 0;
    char leer_arch[100];
    int x=0;

    while(archivo[x]!='\0'){
        leer_arch[x]=archivo[x];
        x=x+1;
    }
    leer_arch[x]='\0';
    int tam=calcularTam(leer_arch) ;
    FILE* file;
    file = fopen(leer_arch,"rb+");

        if(file== NULL){
            printf("no se pudo acceder al disco");
        }
        else{
            int x = fread(&mbr_init, sizeof(Disco),1,file);

            printf("mbr_tamano: %d \n", mbr_init.mbr_tamano);
                bandera = 1;
        }
    if(mbr_init.mbr_partition[0].part_status==0){
        mbr_init.mbr_partition[0].part_status=1;
        strcpy(mbr_init.mbr_partition[0].part_name,name_part);
        strcpy(mbr_init.mbr_partition[0].part_fit,fit);
        mbr_init.mbr_partition[0].part_size=tam_part;
        mbr_init.mbr_partition[0].part_start=start;
        mbr_init.mbr_partition[0].part_type=type;
        FILE *texto;
        texto=fopen(leer_arch,"w+b");
    if(texto){
        fwrite(&mbr_init, sizeof(Disco),1,texto);
        int x;
        int i;

        int nuevotam=(tam - sizeof(mbr_init))/1024;

        for(i=0;i<nuevotam  ;i++){
             fwrite(&relleno,sizeof(relleno),1,texto);
        }
        fclose(texto);
        printf("La particion 1 ha sido creada exitosamente...\n");
    }else{
        printf("Error al crear el Disco...\n");

    }
    }else if(mbr_init.mbr_partition[1].part_status==0){
        mbr_init.mbr_partition[1].part_status=1;
        strcpy(mbr_init.mbr_partition[1].part_name,name_part);
        strcpy(mbr_init.mbr_partition[1].part_fit,fit);
        mbr_init.mbr_partition[1].part_size=tam_part;
        mbr_init.mbr_partition[1].part_start=start;
        mbr_init.mbr_partition[1].part_type=type;
        FILE *texto=fopen(archivo,"w+b");
        if(texto){
        fwrite(&mbr_init, sizeof(Disco),1,texto);
        int x;
        int i;
         int nuevotam=(tam - sizeof(mbr_init))/1024;
        for(i=0;i<nuevotam ;i++){
             fwrite(&relleno,sizeof(relleno),1,texto);
        }
        fclose(texto);
        printf("La particion 1 ha sido creada exitosamente...\n");
        }else{
        printf("Error al crear el Disco...\n");

        }
    }else if(mbr_init.mbr_partition[2].part_status==0){
        mbr_init.mbr_partition[2].part_status=1;
        strcpy(mbr_init.mbr_partition[2].part_name,name_part);
        strcpy(mbr_init.mbr_partition[2].part_fit,fit);
        mbr_init.mbr_partition[2].part_size=tam_part;
        mbr_init.mbr_partition[2].part_start=start;
        mbr_init.mbr_partition[2].part_type=type;
        FILE *texto=fopen(archivo,"w+b");
        if(texto){
        fwrite(&mbr_init, sizeof(Disco),1,texto);
        int x;
        int i;
        int nuevotam=(tam - sizeof(mbr_init))/1024;
        for(i=0;i<nuevotam ;i++){
             fwrite(&relleno,sizeof(relleno),1,texto);
        }
        fclose(texto);
        printf("La particion 1 ha sido creada exitosamente...\n");
        }else{
        printf("Error al crear el Disco...\n");

        }
    }else if(mbr_init.mbr_partition[3].part_status==0){
        mbr_init.mbr_partition[3].part_status=1;
        strcpy(mbr_init.mbr_partition[3].part_name,name_part);
        strcpy(mbr_init.mbr_partition[3].part_fit,fit);
        mbr_init.mbr_partition[3].part_size=tam_part;
        mbr_init.mbr_partition[3].part_start=start;
        mbr_init.mbr_partition[3].part_type=type;
        FILE *texto=fopen(archivo,"w+b");
        if(texto){
        fwrite(&mbr_init, sizeof(Disco),1,texto);
        int x;
        int i;
        int nuevotam=(tam - sizeof(mbr_init))/1024;
        for(i=0;i< nuevotam ;i++){
             fwrite(&relleno,sizeof(relleno),1,texto);
        }
        fclose(texto);
        printf("La particion 1 ha sido creada exitosamente...\n");
        }else{
        printf("Error al crear el Disco...\n");
        }
    }else{
        printf("todas las particiones se encuentran ocupadas, si desea agregar debe eliminar una de las existentes \n");
    }

}

Disco buscarDato(char* directorio){
    int codigo;
    int bandera = 0;
    Disco usuario;
    char leer_arch[100];
    int x=0;

    while(directorio[x]!='\0'){
        leer_arch[x]=directorio[x];
        x=x+1;
    }
    leer_arch[x]='\0';
    FILE* file;
    file = fopen(leer_arch,"rb");

        if(file== NULL){
            printf("no se pudo acceder al disco");

       }
        else{

            fread(&usuario, sizeof(Disco),1,file);
              // printf("mbr_disk_signature: %-20s mbr_fecha_creacion: %-30d mbr_tamano: %-10d\n\n", (*usuario).mbr_disk_signature, (*usuario).mbr_fecha_creacion, (*usuario).mbr_tamano);
                bandera = 1;

        }
return usuario;
}

void ejecutar(char* path_exec){
    FILE* entrada;
    char comandos[150];
    int termino=0;
    entrada=fopen(path_exec,"r");
    if(entrada==NULL) printf("El archivo no existe o se encuentra en otro directorio \n");

    printf("Ejecutando comandos de entrada en archivo \n");
    termino=fscanf(entrada, " %[^\n]", comandos);
    int i;
    int estado=0;
    while(termino!=EOF){
        for(i=0; comandos[i]!='\0';i++){
            if(comandos[i]=='\\'){
                char tempo[150];
                termino=fscanf(entrada, " %[^\n]", tempo);
                strcat(comandos, " ");
                strcat(comandos, tempo);
            }
            if(comandos[i]=='#'){
                comandos[i]='\0';
            }
        }
        if(comandos[0]=='\0'){
            //no hace nada
        }else{
        analisislex(comandos);
        termino=fscanf(entrada, " %[^\n]", comandos);
        }
    }
    fclose(entrada);
}

int calcularTam(char* path){
    FILE *fich;

fich=fopen(path,"r");

  fseek(fich, 0L, SEEK_END);
  int num=ftell(fich);
  printf("el archivo ocupa %d bytes", num);
  fclose(fich);
  return num;
}

void analisislex(char* cadena){
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
    if(strcmp(ruta_disco,"-")!=0){
        adminDisco(ruta_disco);
    }
}

void addDisco(char* dir){
    Disco mbr_init;
    mbr_init = buscarDato(dir);
    if(strcmp(mbr_init.mbr_partition[0].part_name,name_part)==0){
        if((calcularTam(dir)+add_tam>=10)){
            mbr_init.mbr_partition[0].part_size= mbr_init.mbr_partition[0].part_size+add_tam;
            FILE *archivo=fopen(dir,"w+b");
            if(archivo){
            fwrite(&mbr_init, sizeof(Disco),1,archivo);
            int x;
            int i;
            for(i=0;i< (calcularTam(dir) - sizeof(Disco)) ;i++){
                fwrite(&relleno,sizeof(relleno),1,archivo);
            }
            fclose(archivo);
        printf("La particion 1 le ha sido adicionada la cantidad deseada...\n");
    }else{
        printf("Error al modificar el disco...\n");

    }
        }else{
            printf("no se puede realizar el add puesto que se disminuiria el tamaño del disco a un tamaño menos a 10kb");
        }
    }else if(strcmp(mbr_init.mbr_partition[1].part_name,name_part)==0){
        if((calcularTam(dir)+add_tam>=10)){
            mbr_init.mbr_partition[1].part_size= mbr_init.mbr_partition[1].part_size+add_tam;
            FILE *archivo=fopen(dir,"w+b");
            if(archivo){
            fwrite(&mbr_init, sizeof(Disco),1,archivo);
            int x;
            int i;
            for(i=0;i< (calcularTam(dir) - sizeof(Disco)) ;i++){
                fwrite(&relleno,sizeof(relleno),1,archivo);
            }
            fclose(archivo);
        printf("La particion 2 le ha sido adicionada la cantidad deseada...\n");
    }else{
        printf("Error al modificar el disco...\n");

    }
        }else{
            printf("no se puede realizar el add puesto que se disminuiria el tamaño del disco a un tamaño menos a 10kb");
        }
    }else if(strcmp(mbr_init.mbr_partition[2].part_name,name_part)==0){
        if((calcularTam(dir)+add_tam>=10)){
            mbr_init.mbr_partition[2].part_size= mbr_init.mbr_partition[2].part_size+add_tam;
            FILE *archivo=fopen(dir,"w+b");
            if(archivo){
            fwrite(&mbr_init, sizeof(Disco),1,archivo);
            int x;
            int i;
            for(i=0;i< (calcularTam(dir) - sizeof(Disco)) ;i++){
                fwrite(&relleno,sizeof(relleno),1,archivo);
            }
            fclose(archivo);
        printf("La particion 3 le ha sido adicionada la cantidad deseada...\n");
    }else{
        printf("Error al modificar el disco...\n");

    }
        }else{
            printf("no se puede realizar el add puesto que se disminuiria el tamaño del disco a un tamaño menos a 10kb");
        }
    }else if(strcmp(mbr_init.mbr_partition[3].part_name,name_part)==0){
        if((calcularTam(dir)+add_tam>=10)){
            mbr_init.mbr_partition[3].part_size= mbr_init.mbr_partition[3].part_size+add_tam;
            FILE *archivo=fopen(dir,"w+b");
            if(archivo){
            fwrite(&mbr_init, sizeof(Disco),1,archivo);
            int x;
            int i;
            for(i=0;i< (calcularTam(dir) - sizeof(Disco)) ;i++){
                fwrite(&relleno,sizeof(relleno),1,archivo);
            }
            fclose(archivo);
        printf("La particion 4 le ha sido adicionada la cantidad deseada...\n");
    }else{
        printf("Error al modificar el disco...\n");

    }
        }else{
            printf("no se puede realizar el add puesto que se disminuiria el tamaño del disco a un tamaño menos a 10kb");
        }
    }else{
        printf("no existe ninguna particion con ese nombre en el disco seleccionado");
    }
}

void crearLogica(char* dir){

}

//validacion para logicas
void agregaDisco(char* dir){
    if(type=='l'){
        crearLogica(dir);
    }else{
        Disco disk;
        disk = buscarDato(dir);
        if(disk.mbr_partition[0].part_type=='e'||disk.mbr_partition[1].part_type=='e'||disk.mbr_partition[2].part_type=='e'||disk.mbr_partition[3].part_type=='e'){
            if(type=='e'){
                printf("no se puede crear la particion puesto que ya existe una extendida");
            }else{
                addDisco(dir);
            }
        }else{
            addDisco(dir);
        }
    }

}

char* recorreGraph(char* dir){
char* graphviz;
Disco datos;
graphviz="digraph G {";
datos=buscarDato(dir);
char* graph;
strcat(graph, graphviz);
if(datos.mbr_partition[0].part_status=='1'){
    strcat(graph, "subgraph cluster1{label=\"Particion Extendida 1\" node [shape=box style=\"radial\" gradientangle=180]");
    if(datos.mbr_partition[0].part_type=='e' ){
        strcat(graph,"a0 [label=< <TABLE>	<TR>	<TD border=\"3\" colspan=\"4\" rowspan=\"6\">Particion Logica 1</TD><TD border=\"3\" colspan=\"6\" rowspan=\"6\">Particion Logica 2</TD><TD border=\"3\" colspan=\"4\" rowspan=\"8\">Particion Logica 3</TD></TR></TABLE>>];");
    }else{
        strcat(graph, "a0 [label=\"Extendida\"];");
    }
}
if(datos.mbr_partition[1].part_status=='1'){
    strcat(graph, "subgraph cluster1{label=\"Particion Extendida 1\" node [shape=box style=\"radial\" gradientangle=180]");
    if(datos.mbr_partition[1].part_type=='e' ){
        strcat(graph,"a0 [label=< <TABLE>	<TR>	<TD border=\"3\" colspan=\"4\" rowspan=\"6\">Particion Logica 1</TD><TD border=\"3\" colspan=\"6\" rowspan=\"6\">Particion Logica 2</TD><TD border=\"3\" colspan=\"4\" rowspan=\"8\">Particion Logica 3</TD></TR></TABLE>>];");
    }else{
        strcat(graph, "a0 [label=\"Extendida\"];");
    }
}
if(datos.mbr_partition[2].part_status=='1'){
    strcat(graph, "subgraph cluster1{label=\"Particion Extendida 1\" node [shape=box style=\"radial\" gradientangle=180]");
    if(datos.mbr_partition[2].part_type=='e' ){
        strcat(graph,"a0 [label=< <TABLE>	<TR>	<TD border=\"3\" colspan=\"4\" rowspan=\"6\">Particion Logica 1</TD><TD border=\"3\" colspan=\"6\" rowspan=\"6\">Particion Logica 2</TD><TD border=\"3\" colspan=\"4\" rowspan=\"8\">Particion Logica 3</TD></TR></TABLE>>];");
    }else{
        strcat(graph, "a0 [label=\"Extendida\"];");
    }
}
if(datos.mbr_partition[3].part_status=='1'){
    strcat(graph, "subgraph cluster1{label=\"Particion Extendida 1\" node [shape=box style=\"radial\" gradientangle=180]");
    if(datos.mbr_partition[3].part_type=='e' ){
        strcat(graph,"a0 [label=< <TABLE>	<TR>	<TD border=\"3\" colspan=\"4\" rowspan=\"6\">Particion Logica 1</TD><TD border=\"3\" colspan=\"6\" rowspan=\"6\">Particion Logica 2</TD><TD border=\"3\" colspan=\"4\" rowspan=\"8\">Particion Logica 3</TD></TR></TABLE>>];");
    }else{
        strcat(graph, "a0 [label=\"Extendida\"];");
    }

}
FILE *archivo=fopen("grafo.dot","w");
    if(archivo){
        fputs(graph, archivo);
        fclose(archivo);
        printf("Creando grafica...\n");

    }else{
        printf("Error al crear la grafica...\n");

    }
    system("dot -Tpng /home/stephen/[MIA]2016/EXT3/FILE_SISTEM_EXT2/grafo.dot -o /home/stephen/[MIA]2016/EXT3/FILE_SISTEM_EXT2/grafo.png");
    system("shotwell /home/stephen/[MIA]2016/EXT3/FILE_SISTEM_EXT2/grafo.png");
}

void borraDisco(Disco mbr_init, char* dir, int parti){
    if(strcmp(tipo_del,"full")==0){
            strcpy(mbr_init.mbr_partition[parti].part_fit,'\0');
            strcpy(mbr_init.mbr_partition[parti].part_name,'\0');
            strcpy(mbr_init.mbr_partition[parti].part_status,'0');
            mbr_init.mbr_partition[parti].part_size=0;
            mbr_init.mbr_partition[parti].part_start=0;
            mbr_init.mbr_partition[parti].part_type='\0';
        }else{
            strcpy(mbr_init.mbr_partition[parti].part_fit,"");
            strcpy(mbr_init.mbr_partition[parti].part_name,"");
            strcpy(mbr_init.mbr_partition[parti].part_status,'0');
            mbr_init.mbr_partition[parti].part_size=0;
            mbr_init.mbr_partition[parti].part_start=0;
            mbr_init.mbr_partition[parti].part_type="";
        }
        FILE *archivo=fopen(dir,"w+b");
            if(archivo){
            fwrite(&mbr_init, sizeof(Disco),1,archivo);
            int x;
            int i;
            for(i=0;i< (calcularTam(dir) - sizeof(Disco)) ;i++){
                fwrite(&relleno,sizeof(relleno),1,archivo);
            }
            fclose(archivo);
        printf("La particion fue eliminada...\n");
        }
}

void delDisco(char* dir){
    Disco mbr_init;
    int codigo;
    int bandera = 0;
    char leer_arch[100];
    int x=0;

    while(dir[x]!='\0'){
        leer_arch[x]=dir[x];
        x=x+1;
    }
    leer_arch[x]='\0';
    FILE* file;
    file = fopen(leer_arch,"rb");

        if(file== NULL){
            printf("no se pudo acceder al disco");
        }
        else{
            fread(&mbr_init, sizeof(Disco),1,file);
              // printf("mbr_disk_signature: %-20s mbr_fecha_creacion: %-30d mbr_tamano: %-10d\n\n", (*usuario).mbr_disk_signature, (*usuario).mbr_fecha_creacion, (*usuario).mbr_tamano);
                bandera = 1;
        }
    if(strcmp(mbr_init.mbr_partition[0].part_name,name_part)==0){
        borraDisco(mbr_init, dir, 0);
    }else if(strcmp(mbr_init.mbr_partition[1].part_name,name_part)==0){
        borraDisco(mbr_init, dir, 1);
    }else if(strcmp(mbr_init.mbr_partition[2].part_name,name_part)==0){
        borraDisco(mbr_init, dir, 2);
    }else if(strcmp(mbr_init.mbr_partition[3].part_name,name_part)==0){
        borraDisco(mbr_init, dir, 3);
    }else{
        printf("no existe ninguna particion con ese nombre en el disco seleccionado");
    }
}

void crearEBR(char* dir){

}

void mostrarDatos(char* dir){
    Disco mbr_init;
    char* graphviz;
    int codigo;
    int bandera = 0;
    char leer_arch[100];
    int x=0;

    while(dir[x]!='\0'){
        leer_arch[x]=dir[x];
        x=x+1;
    }
    leer_arch[x]='\0';
    FILE* file;
    file = fopen(leer_arch,"rb");

        if(file== NULL){
            printf("no se pudo acceder al disco");
        }
        else{
            fread(&mbr_init, sizeof(Disco),1,file);
              // printf("mbr_disk_signature: %-20s mbr_fecha_creacion: %-30d mbr_tamano: %-10d\n\n", (*usuario).mbr_disk_signature, (*usuario).mbr_fecha_creacion, (*usuario).mbr_tamano);
                bandera = 1;
        }
    graphviz="digraph G { label=\"Datos del Disco\" \n node [shape=box style=\"radial\" gradientangle=180] \n a0 [label=<  \n <TABLE><TR>";
    strcat(graphviz, "<TD border=\"3\">mbr_tamaño: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_tamano);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">mbr_fecha_creacion: </TD> <TD border=\"3\"> ");
    strcat(graphviz, mbr_init.mbr_fecha_creacion);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">mbr_disk_signature: </TD> <TD border=\"3\"> ");
    strcat(graphviz, mbr_init.mbr_disk_signature);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_name_1: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[0].part_name);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">Part_fit_1: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[0].part_fit);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_size_1: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[0].part_size);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_start_1: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[0].part_start);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_type_1: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[0].part_type);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_status_1: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[0].part_status);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_name_2: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[1].part_name);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">Part_fit_2: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[1].part_fit);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_size_2: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[1].part_size);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_start_2: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[1].part_start);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_type_2: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[1].part_type);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_status_2: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[1].part_status);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_name_3: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[2].part_name);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">Part_fit_3: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[2].part_fit);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_size_3: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[2].part_size);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_start_3: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[2].part_start);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_type_3: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[2].part_type);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_status_3: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[2].part_status);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_name_4: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[3].part_name);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">Part_fit_4: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[3].part_fit);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_size_4: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[3].part_size);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_start_4: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[3].part_start);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_type_4: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[3].part_type);
    strcat(graphviz, " </TD>");

    strcat(graphviz, "</TR><TR> ");
    strcat(graphviz, "<TD border=\"3\">part_status_4: </TD> <TD border=\"3\"> ");
    strcat(graphviz,mbr_init.mbr_partition[3].part_status);
    strcat(graphviz, " </TD>");

    strcat(graphviz, " </TABLE>>]; }");

    FILE *archivo=fopen("tabla.dot","w");
    if(archivo){
        fputs(graphviz, archivo);
        fclose(archivo);
        printf("Creando grafica...\n");

    }else{
        printf("Error al crear el Disco...\n");

    }
    system("dot -Tpng /home/stephen/[MIA]2016/EXT3/FILE_SISTEM_EXT2/tabla.dot -o /home/stephen/[MIA]2016/EXT3/FILE_SISTEM_EXT2/tabla.png");
    system("shotwell /home/stephen/[MIA]2016/EXT3/FILE_SISTEM_EXT2/tabla.png");
}

void adminDisco(char* dir){
if(fdisk==1){
    fdisk=0;
        if(add==1){
                add=0;
                addDisco(dir);
        }else if(del==1){
                del=0;
                delDisco(dir);
        }else{//crear particion
            if(type=='l'){
                crearEBR(dir);
            }else{
                crearParticion(dir);
            }
        }
    }
}

int main(void){
 char cadena [100];
 int a=0;
unit_disco="m";
fit="WF";
type=
 while(a==0){
  printf ("File System ext2/ext3:#~$ ");
  fgets (cadena, 100, stdin);
  analisislex(cadena);
 }return 0;

}
int funciona(int val, char* token){
    if(val==0){//inicia el comando
        if(strcmp(token,"mkdisk")==0){
            return 1;
        }else if(strcmp(token,"rmdisk")==0){
            return 8;
        }else if(strcmp(token,"fdisk")==0){
            fdisk=1;
            return 10;
        }else if(strcmp(token,"mount")==0){
            return 19;
        }else if(strcmp(token,"umount")==0){
            return 23;
        }else if(strcmp(token,"exec")==0){
            return 25;
        }else if(strcmp(token,"rep")==0){
            return 26;
        }else{
            printf("\n Error no se ha diseñado otro comando en esta aplicacion puede usar los siguientes:\n");
            printf("\n - mkdisk    -rmdisk   -rep  -fdisk      -mount      -umount  -exec \n ");
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
        Disco inicial;
        inicial=creaRegistro(tam_disco);
        char* dir=CrearArchivo(nombre,inicial ,tam);

        return 0;
    }else if(val== 8){
        if(strcmp(token,"path")==0){
            printf("esperando ruta del archivo a eliminar...");
            return 9;
        }else{
            printf("se requiere el parametro path(-path)");
        }
    }else if(val==9){
        eliminarArchivo(token);
        return 0;
    }else if(val==10){
        printf("--------ADMINISTRADOR DE PARTICIONES---------");
        if(strcmp(token,"size")==0){
            printf("esperando tamaño de la particion...\n");
            return 11;
        }else if(strcmp(token,"name")==0){
            printf("nombre de la particion...\n");
            return 12;
        }else if(strcmp(token,"unit")==0){
            printf("seleccionando la unidad de medida en que se guardara..\n");
            return 13;
        }else if(strcmp(token,"add")==0){
            add=1;
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
            tam_part=num;
            return 10;
        }else{
            printf("el tamaño de la particion debe ser mayor a cero \n");
            return 0;
        }
    }else if(val==12){
        printf("seleccionando nombre del archivo... \n");
        int lon_nombre = strlen(token);
        char particion_name[100];
        int a=0;
        int b=0;

        while (token[a+1] != '\0')
    {
    if (isalpha(token[a])||isdigit(token[a]))
    {
      particion_name[b] = token[a];
	  b++;
    }
    a++;
  }
    particion_name[b]='\0';
    name_part=particion_name;

        return 10;
    }else if(val==13){
        if(strcmp(token,"m")==0 || strcmp(token,"M")==0){
            unit_part='m';
            printf("unidades en Mb... \n");
            return 10;
        }else if(strcmp(token,"k")==0||strcmp(token,"K")==0){
            unit_part='k';
            printf("Unidades en Kb... \n");
            return 10;
        }else if(strcmp(token,"b")==0 || strcmp(token,"B")==0){
            unit_part='b';
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
        add_tam=num;
        return 10;
    }else if(val==15){
        printf("accediendo a la ruta del archivo... \n");
        int len = strlen(token);
        char nombre_prt[len-2];
        int i=0;
        int j=0;

        while (token[i+1] != '\0')
    {
    if (isalpha(token[i])||isdigit(token[i]))
    {
      nombre_prt[j] = token[i];
	  j++;
    }else if(token[i]=='/'){
        nombre_prt[j] = token[i];
	  j++;
    }else if(token[i]=='.'){
         nombre_prt[j] = token[i];
	  j++;
    }
    i++;
  }
    nombre_prt[j]='\0';
    ruta_disco=nombre_prt;

        return 10;
    }else if(val==16){
        if(strcmp(token,"l")==0 || strcmp(token,"L")==0){
           printf("creando particion logica");
           type='l';
        }else
        if(strcmp(token,"e")==0 || strcmp(token,"E")==0){//falta condicion  que aun no exista extendida y hallan 4
            printf("ingresando particion extendida... \n");
            type='e';
            return 10;
        }else if(strcmp(token,"p")==0 || strcmp(token,"P")==0){
            printf("ingresando particion primaria... \n");
            type='p';
            return 10;
        }else{
            printf("no se reconoce el parametro enviado a la funcion \n");
            return 0;
        }

    }else if(val==17){
        if(strcmp(token,"BF")==0){
            printf("determinado como mejor ajuste... \n");
            fit="BF";
            return 10;
        }else if(strcmp(token,"WF")==0){
            printf("determinado como peor ajuste... \n");
            fit="WF";
            return 10;
        }else if(strcmp(token,"FF")==0){
            printf("determinado como primer ajuste... \n");
            fit="FF";
            return 10;
        }else{
            printf("no se reconoce el tipo de ajuste... \n");
            return 0;
        }
    }else if(val==18){
        if(strcmp(token,"full")==0||strcmp(token,"FULL")==0){
                tipo_del="full";
            printf("se selecciono el eliminado completo");
        }else if(strcmp(token,"fast")==0||strcmp(token,"FAST")==0){
            printf("se selecciono el eliminado rapido");
            tipo_del="fast";
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
    }else if(val==25){
        int len = strlen(token);
        char ruta_exec[len-2];
        int i=0;
        int j=0;

        while (token[i+1] != '\0')
        {
            if (isalpha(token[i])||isdigit(token[i]))
            {
                ruta_exec[j] = token[i];
                j++;
            }else if(token[i]=='/'){
                ruta_exec[j] = token[i];
                j++;
            }else if(token[i]=='.'){
                ruta_exec[j] = token[i];
                j++;
            }
            i++;
        }
        ruta_exec[j]='\0';
        ejecutar(ruta_exec);

    }else if(val==26){
        if(strcmp(token,"mbr")==0){
            printf("buscando informacion del disco...");
            return 27;
        }else if(strcmp(token,"disk")==0){
            printf("creando representacion del disco...");
            return 28;
        }else{
            printf("No hay otro parametro que por el momento me reciba el comando rep");
        }

    }else if(val== 27){
        int len = strlen(token);
        char ruta_exec[len-2];
        int i=0;
        int j=0;

        while (token[i+1] != '\0')
        {
            if (isalpha(token[i])||isdigit(token[i]))
            {
                ruta_exec[j] = token[i];
                j++;
            }else if(token[i]=='/'){
                ruta_exec[j] = token[i];
                j++;
            }else if(token[i]=='.'){
                ruta_exec[j] = token[i];
                j++;
            }
            i++;
        }
        ruta_exec[j]='\0';
        mostrarDatos(ruta_exec);
    }else if(val==28){
        int len = strlen(token);
        char ruta_exec[len-2];
        int i=0;
        int j=0;

        while (token[i+1] != '\0')
        {
            if (isalpha(token[i])||isdigit(token[i]))
            {
                ruta_exec[j] = token[i];
                j++;
            }else if(token[i]=='/'){
                ruta_exec[j] = token[i];
                j++;
            }else if(token[i]=='.'){
                ruta_exec[j] = token[i];
                j++;
            }
            i++;
        }
        ruta_exec[j]='\0';
        recorreGraph(ruta_exec);
    }

}



