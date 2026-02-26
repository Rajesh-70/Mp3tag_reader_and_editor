#ifndef VIEW_H
#define VIEW_H

#include<stdio.h>
#include"types.h"

typedef struct _view
{
    char *src_audio_fname;//to store the file name
    FILE *fptr_src_file;//file pointer
}View;

/* Perform view operation */
Status do_view(View *viewinfo);

/* For read and validating view operation */
Status read_and_validate_view(char *argv[],View *viewinfo);

/* To open src file in read mode*/
Status open_files(View *viewinfo);

/*To skip header*/
Status skip_header(View *viewinfo);

/* To read and print tag details*/
Status read_and_print_data(View *viewinfo);


/*convert little to big endianess or vice verse*/
void endianess(void *ptr,int size);

/* To print song details*/
Status print_data(char *tag, char *data);

#endif