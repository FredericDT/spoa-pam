#include <stdio.h>
#include <security/pam_appl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <spoa_pam.h>

int function_conversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr)
{
    *resp = (struct pam_response *) appdata_ptr;
    return PAM_SUCCESS;
}

int authenticate_system(const char *username, const char *password)
{
    struct pam_response *reply = (struct pam_response *)malloc(sizeof(struct pam_response));

    const struct pam_conv local_conversation = {function_conversation, reply};
    pam_handle_t *local_auth_handle = NULL;

    int retval;
    // TODO: custom pam service name su -> sth?
    retval = pam_start("fdt-pam-test-client", username, &local_conversation, &local_auth_handle); 
    
    if (retval != PAM_SUCCESS)
    {
        perror("Error when pam_start(): ");
        goto pam_cleanup;
    }

    reply[0].resp = strdup(password);
    reply[0].resp_retcode = 0;
    retval = pam_authenticate(local_auth_handle, 0);

    int pam_auth_retval = retval;

    if (retval != PAM_SUCCESS)
    {
        if (retval == PAM_AUTH_ERR)
        {
            printf("Authentication failure.\n");
        }
        perror("Error when pam_authenticate(): ");
        goto pam_cleanup;
    }

pam_cleanup:
    retval = pam_end(local_auth_handle, retval);

    if (retval != PAM_SUCCESS)
    {
        perror("Error when pam_end(): ");
    }

    return pam_auth_retval;
}

// int main(int argc, char **argv)
// {
//     char *login;
//     char *password;

//     printf("Authentication module\n");

//     if (argc != 3)
//     {
//         printf("Invalid count of arguments %d.\n", argc);
//         printf("./authModule <username> <password>");
//         return 1;
//     }

//     login = argv[1];
//     password = argv[2];

//     if (authenticate_system(login, password) == PAM_SUCCESS)
//     {
//         printf("Authenticate with %s - %s through system\n", login, password);
//         return 0;
//     }

//     printf("Authentication failed!\n");
//     return 1;
// }