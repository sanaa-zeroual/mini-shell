#include "../minishell.h"

void validate_cmd(t_cmd *cmd) 
{
    if (ft_strchr(cmd->arguments[0], '/'))
        check_cmd_path(cmd);
    else
        check_command_name(cmd);
}

int count_commands(t_cmd *cmd)
{
    int count = 0;
    t_cmd *current = cmd;
    while(current)
    {
        count++;
        current = current->next;
    }
    return count;
}

char *allocate_folders(char *path, int i) 
{
    char *folders = malloc(i + 2);
    if (!folders)
    {
        perror("malloc failed");
        exit(1);
    }
    my_strncpy(folders, path, i + 1);
    return folders;
}

void check_cmd_path(t_cmd *cmd) 
{
    struct stat statbuf;
    if (stat(cmd->arguments[0], &statbuf) == 0) 
    {
        if (S_ISREG(statbuf.st_mode) && (statbuf.st_mode & S_IXUSR)) 
        {
            cmd->cmd_path = cmd->arguments[0];
        } 
        else 
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->arguments[0], 2);
            ft_putstr_fd(": Permission denied\n", 2);
            g_var.exit_status = 1;
        }
    }
    else 
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->arguments[0], 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        g_var.exit_status = 1;
    }
}

void my_strncpy(char *dest, const char *src, size_t n) 
{
    size_t i = 0;
    while (i < n && src[i] != '\0') 
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n) 
    {
        dest[i] = '\0';
        i++;
    }
}

int check_path(char *path, int builtin)
{
    struct stat statbuf;
    int i;

    if (builtin) 
        return 1;

    if (!ft_strchr(path, '/'))
        return 1;
    i = ft_strlen(path);
    while (i != 0 && path[i] != '/')
        i--;
    char *folders = allocate_folders(path, i);
    int status = (stat(folders, &statbuf) != -1);
    free(folders);
    return status;
}

void check_command_name(t_cmd *cmd)
{
    char *path_env = getenv("PATH");
    char **path_dirs = ft_split(path_env, ':'); 
    struct stat statbuf;
    int i = 0;

    while (path_dirs[i])
    {
        char *full_path = malloc(strlen(path_dirs[i]) + strlen(cmd->arguments[0]) + 2);
        if (!full_path)
        {
            perror("malloc failed");
            return;
        }
        strcpy(full_path, path_dirs[i]);
        strcat(full_path, "/");
        strcat(full_path, cmd->arguments[0]);

        if (stat(full_path, &statbuf) == 0 && S_ISREG(statbuf.st_mode) && (statbuf.st_mode & S_IXUSR))
        {
            cmd->cmd_path = full_path; 
            free(path_dirs);
            return;
        }

        free(full_path);
        i++;
    }
    ft_putstr_fd("minishell: command not found: ", 2);
    ft_putstr_fd(cmd->arguments[0], 2);
    ft_putstr_fd("\n", 2);
    g_var.exit_status = 127;
    free(path_dirs);
}

int check_file_errors(char *path, int builtin)
{
    if (path && (path[0] == '$' || (path[0] == '"' && path[1] == '$')))
    {
        g_var.red_error = 1;
        g_var.exit_status = 1;
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(" ambiguous redirect\n", 2);
        if (builtin)
            return 1;
        else
            exit(1);
    }
    return 0;
}

void handle_file_redirections(t_cmd *cmd, int btn) 
{
    files_redirections(cmd, btn != -1);
    if (btn == -1)
        validate_cmd(cmd);
    else if (g_var.pre_pipe_infd != -1 && !cmd->file->type)
        dup2(g_var.pre_pipe_infd, STDIN_FILENO);
}

void files_redirections(t_cmd *cmd, int builtin)
{
    t_file *curr_red = cmd->file;
    while (curr_red) 
    {
        if (check_file_errors(curr_red->filename, builtin))
            return;
        if (cmd->type == RE_IN)
        {
            if(builtin)
                g_var.in_fd = open(curr_red->filename, O_RDONLY, 0644);
            else
                in_file_prep(curr_red->filename);
        }
        else if (cmd->type == RE_OUT)
        {
            if (builtin)
                g_var.out_fd = open(curr_red->filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
            else
                out_file_prep(curr_red->filename);
        }
        else if (curr_red->type == RE_APPEND)
        {
            if (builtin)
                g_var.out_fd = open(curr_red->filename, O_CREAT | O_WRONLY | O_APPEND, 0777);
            else
                append_file_prep(curr_red->filename);
        }
        curr_red = curr_red->next;
    }
}

void append_file_prep(char *path) 
{
    int fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0777);
    if (fd == -1) 
    {
        perror(path);
        exit(1);
    } 
    else 
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

void in_file_prep(char *path) 
{
    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror(path);
        exit(1);
    } 
    else 
    {
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
}

void out_file_prep(char *path) 
{
    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (fd == -1)
    {
        perror(path);
        exit(1);
    } 
    else
    {
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

int check_builtin(t_cmd *cmd)
{
    if (ft_strcmp(cmd->arguments[0], "cd") == 0)
        return 1;
    else if (ft_strcmp(cmd->arguments[0], "echo") == 0)
        return 2;
    else if (ft_strcmp(cmd->arguments[0], "env") == 0)
        return 3;
    else if (ft_strcmp(cmd->arguments[0], "exit") == 0)
        return 4;
    else if (ft_strcmp(cmd->arguments[0], "export") == 0)
        return 5;
    else if (ft_strcmp(cmd->arguments[0], "pwd") == 0)
        return 6;
    return -1;
}

void exec_builtin(int btn, t_cmd *cmd, t_mini *box)
{
    if (btn == 1 && !g_var.red_error)
        ft_cd(cmd->arguments, box->env);
    else if (btn == 2 && !g_var.red_error)
        ft_echo(cmd->arguments);
    else if (btn == 3 && !g_var.red_error)
        ft_env(box->env);
    else if (btn == 4 && !g_var.red_error)
        ft_exit(box->shell);
    else if (btn == 5 && !g_var.red_error)
        ft_export(cmd->arguments, box->env);
    else if (btn == 6 && !g_var.red_error)
        ft_pwd(box->env);

    if (g_var.out_fd > 2)
        close(g_var.out_fd);
    g_var.out_fd = 1;
}

void child_process(t_cmd *cmd, int pipe_nb, int btn, t_mini *box) 
{
    g_var.last_child_id = fork();
    if (g_var.last_child_id == 0) 
    {
        if (g_var.pre_pipe_infd != -1)
            dup2(g_var.pre_pipe_infd, STDIN_FILENO);
        if (pipe_nb < g_var.size - 1 && cmd->pipe_fd[1] > 2)
            dup2(cmd->pipe_fd[1], STDOUT_FILENO); 
        handle_file_redirections(cmd, btn); 
        execs(cmd, btn, box);
        exit(0);
    }
}

void execs(t_cmd *cmd, int btn, t_mini *box) 
{
    if (btn != -1)
    {
        exec_builtin(btn, cmd, box);
        exit(0);
    }
    else if (cmd->cmd_path)
    {
        execve(cmd->cmd_path, cmd->arguments, g_var.envp);
        perror(cmd->cmd_path);
        exit(errno);
    }
}

void child(t_cmd *cmd, int pipe_nb,int btn,  t_mini *box)
{
    if(g_var.last_child_id == 0)
    {
        if(g_var.pre_pipe_infd != - 1)
            dup2(g_var.pre_pipe_infd, STDIN_FILENO);
        if(pipe_nb < g_var.size - 1 && cmd->pipe_fd[1] > 2)
            dup2(cmd->pipe_fd[1], STDOUT_FILENO);
        handle_file_redirections(cmd, btn);
        execs(cmd, btn, box);
        exit(0);        
    }
}

void execute_pipes(t_cmd *cmd, int pipe_nb, t_mini *box) 
{
    int btn = check_builtin(cmd);
    if (g_var.size == 1 && btn != -1) 
    {
        files_redirections(cmd, 1);
        exec_builtin(btn, cmd, box);
    }
    else 
    {
        if (g_var.size != pipe_nb + 1 && pipe(cmd->pipe_fd) == -1) 
        {
            perror("pipe");
            exit(1);
        }
        child_process(cmd, pipe_nb, btn, box); 
        close(cmd->pipe_fd[1]);
        g_var.pre_pipe_infd = cmd->pipe_fd[0];
    }
}


void sig_wait(t_cmd *cmd)
{
    int status;
    while (cmd)
    {
        waitpid(cmd->pid, &status, 0);
        if (WIFSIGNALED(status))
            g_var.exit_status = 128 + WTERMSIG(status);
        if (WIFEXITED(status))
            g_var.exit_status = WEXITSTATUS(status);
        cmd = cmd->next;
    }
}

void execute_arguments(t_cmd *cmd, t_mini *box)
{
    g_var.size = count_commands(cmd);
    g_var.pipe_nb = g_var.size - 1;
    int i = 0;
    g_var.exit_status = 0;
    g_var.pre_pipe_infd = -1;
    while (cmd && g_var.exit_status == 0) 
    {
        execute_pipes(cmd, i, box);
        cmd = cmd->next;
        i++;
    }
    if (g_var.pre_pipe_infd > 2)
        close(g_var.pre_pipe_infd);
    sig_wait(cmd);
}