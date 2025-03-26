#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int pipefd[2];  // pipefd[0] = lecture, pipefd[1] = écriture
    
    // Créer le pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    printf("Pipe créé: fd[0]=%d (lecture), fd[1]=%d (écriture)\n", pipefd[0], pipefd[1]);
    
    // Premier fork pour ls
    pid_t pid1 = fork();
    
    if (pid1 == 0) {  // Premier enfant (ls)
        printf("\nPREMIER ENFANT (ls) - PID: %d\n", getpid());
        
        close(pipefd[0]);  // Ferme l'extrémité lecture du pipe
        dup2(pipefd[1], STDOUT_FILENO);  // Redirige stdout vers le pipe
        close(pipefd[1]);  // Ferme l'original après dup2
        
        char *args[] = {"ls", NULL};
        execvp("ls", args);
        perror("execvp ls");
        return 1;
    }
    
    // Deuxième fork pour wc -l
    pid_t pid2 = fork();
    
    if (pid2 == 0) {  // Deuxième enfant (wc -l)
        printf("\nDEUXIÈME ENFANT (wc) - PID: %d\n", getpid());
        
        close(pipefd[1]);  // Ferme l'extrémité écriture du pipe
        dup2(pipefd[0], STDIN_FILENO);  // Redirige stdin vers le pipe
        close(pipefd[0]);  // Ferme l'original après dup2
        
        char *args[] = {"wc", "-l", NULL};
        execvp("wc", args);
        perror("execvp wc");
        return 1;
    }
    
    // Parent ferme les deux extrémités du pipe
    printf("\nPARENT - Je ferme le pipe et j'attends\n");
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Attend les deux enfants
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    printf("Terminé!\n");
    return 0;
}