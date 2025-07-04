#include "commands.h"


char *commands[] = {
  "cd",
  "help",
  "ls",
  "ls -a",
  "ls -l",
  "cat",
  "cp",
  "exit"
};


int ls_a(char* directory) {
  struct dirent **namelist;
  int n;
  n = scandir(directory, &namelist,0,alphasort);
  for(int i=0;i<n;i++){
    printf("%s\t\t",namelist[i]->d_name);
    free(namelist[i]);
  }
  printf("\n");
  free(namelist);
  
  /*  DIR* currdir = opendir(directory); //Ca en gros c'est un stream vers notre directory
  struct dirent *file; //Structure utilisée pour lire le stream
  while ((file=readdir(currdir)) != NULL){ //bon tu sais lire mec
    printf("%s",file->d_name); //le -> c'est pour acceder a un attribut de la struct, c'est comme un machin.name
    printf("\t\t"); //tu sais lire aussi
  }
  printf("\n");*/

  return EXIT_SUCCESS;
}

int is_visible(const struct dirent *file) {
  if(file->d_name[0]=='.'){  
    return 0;
  }
  else{
    return 1;
  }
}


int ls(char* directory) {
  struct dirent **namelist;
  int n;
  n = scandir(directory, &namelist,is_visible,alphasort);
  for(int i=0;i<n;i++){
    printf("%s\t\t",namelist[i]->d_name);
    free(namelist[i]);
  }
  printf("\n");
  free(namelist); 
  return EXIT_SUCCESS;
}

char* get_user(uid_t uid) {
    struct passwd *pwd = getpwuid(uid);
    if (pwd) {
        return pwd->pw_name;
    }
    return NULL; 
}

char* get_grp(gid_t gid) {
    struct group *grp = getgrgid(gid);
    if (grp) {
        return grp->gr_name; 
    }
    return NULL; 
}

unsigned char get_type(unsigned long mode) {
  switch (mode & __S_IFMT) {  //Masque (ET logique) qui ressort les bit de type en gros
    case __S_IFREG: //blabla chacune verif le type, renvoie true ou false tqt tu comprends
        return 'F';
    case __S_IFDIR:
        return 'D';
    case __S_IFCHR:
        return 'C';
    case __S_IFBLK:
        return 'B';
    case __S_IFIFO:
        return 'P';
    case __S_IFLNK:
        return 'L';
    case __S_IFSOCK:
        return 'S';
    default: //si rien n'est true, bah on sait po
        return '?';
}
}

void fmt_rights(mode_t mode, char* rights) {
  int x=0;
  for (int i=0;i<3;i++){
    for (int j=0;j<3;j++){
      /*
      on calcule le bit qu'on veut verifier
      Comme les perms sont sur les 9 derniers (de 8 à 0), on va prendre 8, enlever i*3 (donc l'user qu'on veut)+j(pour avoir chacun des trucs)
      */
     int bit = 8 - x; 
      /*
      En gros la on va faire un truc très stylé
      On décale 1 vers la gauche *bit* fois, donc on va crée un truc qui ressemble à ça 01000... (nb de 0 = bit)
      Ensuite l'opérande & va appliquer un masque sur *mode* avec ça
      Ce qui veut juste dire qu'on va faire un ET logique entre les deux
      Comme ça on peut savoir si le bit à la position est égal à 1 ou 0 (parce que y'a des 0 partout sauf au bit qu'on veut tester)
      Et on écrit en conséquence
      */
       if (mode & (1 << bit)) {
            rights[x] = 'o';
        } else {
            rights[x] = '-';
        }
      x++;
    }
  }
}

void print_line(char mode, char* rights, char* user, char* group, long long size, char* time, char* filename) {
  printf("%c ", mode);
  printf("%s ", rights);
  printf("%10s %10s ", user, group);
  printf("%8lld ", size);
  printf("%s ", time);
  printf("%s\n", filename);
}

int ls_l(char* directory) {
  struct dirent **namelist;
  struct stat stats; //Struct stat, pour avoir toutes les données a propos d'un fichier
  int n;

  n = scandir(directory, &namelist,is_visible,alphasort);
  if (n == -1) {
        return EXIT_FAILURE;
    }

  


  for(int i=0;i<n;i++){

    char relative_path[2048]="./";
    strcat(relative_path,namelist[i]->d_name);


    //On utilise la fonction stat, qui remplit la struct stat (technique hein tavu) avec les données du fichier (dont le chemin est fullpath)
    stat(relative_path, &stats);
    
    //On prend le type du fichier
    //sidenote : juste besoin d'un char, voir get_type
    char type = get_type(stats.st_mode);
    
    //On prend les droits de tt le monde dessus
    //sidenote : j'alloue 10 pour prendre tout, sans risque de depasser
    //j'ai passé 5 bonne minutes à me demander pq il me printait aussi des dates avec ca pour finalement me rendre compte
    //que je suis juste con et il dépassait
    char rights[10];
    fmt_rights(stats.st_mode, rights);

    //Bon je pense z'avez compris à ce niveau nan ?
    char* user = get_user(stats.st_uid);
    char* group = get_grp(stats.st_gid);

    //ctime c'est une fonction très cool qui vient nous formatter le temps direct depuis un time_t
    //et devinez ce qu'on a dans stat ? oooooh quel banger un time_t
    char* time = ctime(&stats.st_mtime);
    
    //Ca c'est juste parce que ctime viens foutre une retour ligne apres le temps, donc on le tej
    if (time) {
      time[strlen(time)-1] = '\0';
    }


    //bon j'utilise la fonction en gros
    print_line(type,rights,user,group,stats.st_size,time,namelist[i]->d_name);

    //toujours free, S. O. romain
    free(namelist[i]); 

  }
  printf("\n");
  free(namelist); 
  return EXIT_SUCCESS;
}





//-----------------CAT-----------------
int cat(const char* filename){
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("fopen");
    return 0;
  }
  
  //bon vrm juste des prints
  char line[1024];
  while (fgets(line,sizeof(line),file)){
    printf("%s",line);
  }
  
  printf("\n");
  fclose(file);
  return 1;
}


//-----------------CP-----------------
int cp(const char *src, const char *dst) {
    FILE *in = fopen(src, "r");
    if (!in) { 
      perror(src);
      fclose(in);  
      return 1; 
    }
    FILE *out = fopen(dst, "w");
    if (!out) { 
      perror(dst); 
      fclose(out); 
      return 1; 
    }


    char buffer[4096];
    int n=0;
    while (fgets(buffer, sizeof(buffer), in)) {
      n = strlen(buffer);
      fwrite(buffer, 1, n, out);
    }
    fclose(in);
    fclose(out);
    return 0;
}

