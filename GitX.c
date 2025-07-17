#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

// this is for test

int search(char* file_name){
    char cwd[1000];
    getcwd(cwd,sizeof(cwd));
    // puts(cwd);

    char temp_dir[1000];
    struct dirent* entery;
    bool exist = false;
    do  {
        DIR* dir = opendir(".");
        if (dir == NULL) return 1;

        while ((entery = readdir(dir)) != NULL)
        {
            if(entery->d_type == DT_DIR && strcmp(entery->d_name, file_name) == 0){
                exist = true;
                break;
            }
        }
        closedir(dir);
        // puts(temp_dir);

        getcwd(temp_dir,sizeof(temp_dir));
        if(strcmp(temp_dir,"C:\\") != 0){
            chdir("..");
        }
    } while (strcmp(temp_dir,"C:\\") != 0);

    chdir(cwd);
    if(exist) return 1;
    
    else return 0;
}

bool check_status(char filename[100]){

    FILE* fptr = fopen(".neogit/staging", "r");
    char line[100];
    while (fgets(line, sizeof(line), fptr))
    {
        line[strcspn(line, "\n")] = 0;
        if (!strcmp(line, filename))
        {
            return 1;
        }
    }
    return 0;
}

bool find_alias(char* argv[]){
    FILE* fptr = fopen(".neogit/alias","r");

    char line[1000];
    while (fgets(line, sizeof(line), fptr))
    {
        char* token = strtok(line, " ");
        if (!strcmp(token, argv[1]))
        {
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            token[strlen(token)-1] = 0;
            strcpy(argv[1], token);
            return true;
        }
        
    }
    return false;
}

int inc_last_commit_ID(){

    FILE* file = fopen(".neogit/config", "r");
    if (file == NULL) return 1;

    FILE* tmp_file = fopen(".neogit/tmp_config", "w");
    if (tmp_file == NULL) return 1;

    int last_commit_ID;
    char line[2000];
    while (fgets(line, sizeof(line), file))
    {
        if (strcmp(line, "last_commit_ID") == 0)
        {
            sscanf(line, "last commt: %d\n", last_commit_ID);
            last_commit_ID ++;
            fprintf(tmp_file, "last_commit_ID: %d\n", last_commit_ID);

        } else fprintf(tmp_file, "%s", line);
        fclose(file);
        fclose(tmp_file);

        remove(".neogit/config");
        remove("");
    }
}

int run_reset(int argc, char* argv[]){
    FILE* fptr = fopen(".neogit/staging", "r");

    char keep[100][100];
    char line[1000];
    int i = 0;
    while (fgets(line, sizeof(line), fptr))
    {
        line[strcspn(line, "\n")] = 0;
        strcpy(keep[i], line);
        if (!strcmp(line, argv[2]))
        {
          continue;  
        } 
        i++;
    }
    
    fclose(fptr);
    fptr = fopen(".neogit/staging", "w");
    for (int j = 0; j < i; j++)
    {
        fputs(keep[j], fptr);
        fprintf(fptr, "\n");
    }
    
    fclose(fptr);
    return 0;

}

int run_commit(int argc, char* argv[]){
    if (argv < 4)
    {
        perror("Please use correct format");
        return 1;
    }
    

}

int add_to_staging(char* filepath) {
    
    FILE* file = fopen(".neogit/staging", "r");

    char line[1000];

    while (fgets(line, sizeof(line), file))
    {
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] == 0;

        if (strcmp(line, filepath) == 0)
            return 0;
    }
    fclose(file);
    
    file = fopen(".neogit/staging", "a+");
    if (file == NULL) return 1;
    mkdir(filepath);

    fprintf(file, "%s\n", filepath);
    fprintf(stdout, "%s added successfully!\n",filepath);
    fclose(file);

    return 1;
}

int run_config(int argc, char* argv[]){
    FILE *fptr = fopen(".neogit/config", "a+");

    if(search(".neogit"))
    {
        if (fptr == NULL) {
            perror("Error opening file");
            return 1;
        }

        puts(argv[2]);
        if(!strcmp(argv[2], "user.name")){
            fprintf(fptr, "username: %s\n", argv[3]);
        }
        if(!strcmp(argv[2], "user.email")){
            fprintf(fptr, "email: %s\n", argv[3]);
        } 
    }
    else{
        perror(".neogit was not found");
        return 1;
    }

    fclose(fptr);

    fprintf(stdout,"succesfully configed!");
    return 0;

    // fptr = fopen(".neogit/tracks", "w");
    // fptr = fopen(".neogit/staging", "w");
}

int run_global_config(int argc , char *argv[]){
    FILE *fptr = fopen("C:/global/config", "a+");

    puts(argv[3]);
    if(! strcmp(argv[3], "user.name")){
        fprintf(fptr, "username: %s\n", argv[4]);
    }
    else if(! strcmp(argv[3], "user.email")){
        fprintf(fptr, "email: %s\n", argv[4]);
    }
    else {
        perror("invalid command");
        return 1;
    }

    fclose(fptr);
    printf("succesfully configed in global!");

    return 1;
}

int run_add(int argc, char* argv){

    char* filepath = argv; 
    if(!search(filepath)){
        fprintf(stdout,"%s doesn't exist!\n", filepath);
        return 1;
    }
    add_to_staging(filepath);
}

int run_status(){

    struct dirent *entry;
    DIR *dir = opendir("."); 
    while((entry = readdir(dir)) != NULL){
        if(check_status(entry->d_name)){
            puts(entry->d_name);
            return 1;
        }   
    }
    closedir(dir);
    return 1;
}

int run_init(int argc, char* argv[]){

    if (search(".neogit"))
    {
        perror(".neogit is already existed!");
        return 1;
    } else{
        mkdir(".neogit");
        mkdir("Git");
        // run_config("arash","a.sh.138495@gmail.com");
    }
}

int run_alias(int argc, char* argv[], char* token){
    token = strtok(NULL, ".");

    if (strcmp(argv[3], "-global")){

        FILE* fptr = fopen(".neogit/alias","a+");
        fprintf(fptr, "%s %s\n", token, argv[3]);
        fprintf(stdout, "alias done successfully");
        return 1;
    }
    //else
}

int main(int argc, char* argv[]){
    
    if (argc < 2)
    {
        perror(".!");
        return 1;
    }

    find_alias(argv);
    
    if (strcmp(argv[1],"config") == 0)
    {
        if (!strcmp(argv[2], "-global") && (!strcmp(argv[3], "user.name") || !strcmp(argv[3], "user.email")))
        {
            run_global_config(argc, argv);
            return 1;
        }
        run_config(argc, argv);

        char* token;
        token = strtok(argv[2], ".");
        if (!strcmp(token, "alias"))
        {
            if (argc < 4)
            {
                perror("Invalid command!");
                return 1;
            }
            run_alias(argc, argv, token);
            return 1;
        }
        
    } 
    if (strcmp(argv[1],"init") == 0)
    {
        run_init(argc, argv);
    }
    if (strcmp(argv[1],"reset") == 0)
    {
        if (argc < 3)
        {
            fprintf(stdout, "Please enter file name or address");
            return 1;
        }
        run_reset(argc, argv);
    }
    if (strcmp(argv[1], "add") == 0)
    {
        if(argc < 3){
        perror("Please enter file name or address!");
        return 1;
        }

        if (!strcmp(argv[2], "-f"))
        {
            if (argc < 4)
            {
                perror("Please enter file name or address!");
                return 1;
            }
            
            for (int i = 3; i < argc; i++)
            {
                run_add(argc, argv[i]);
            }
            return 1;
        }
        
        run_add(argc, argv[2]);
        return 1;
    }
    if (strcmp(argv[1], "status") == 0)
    {
        run_status();
    }
    if (strcmp(argv[1], "commit") == 0)
    {
        run_commit(argc, argv);
    }
    // else
    //     perror("Invalid command");

}