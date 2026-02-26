#ifndef EDIT_H
#define EDIT_H

#include<stdio.h>
#include"types.h"

typedef struct _Edit_data
{

    char editing[5];         // To store title of the tag
    char new_data[50];       // To store new data
    char *audio_file_name;   // Audio file
    FILE *fptr_audio;         //file pointer audio file
    char *temp_file_name;    // Temporary audio file
    FILE *fptr_temp;


}Edit;


/* For read and validating edit operation */
Status read_and_validate_edit(char *argv[],Edit *edit,int argc);

/* Perform edit operation */
Status do_edit_operation(Edit *edit);

/* To open src file in read mode and edit file in write mode */
Status open_edit_file(Edit *edit);

/* To copy the header bytes */
Status copy_header_bytes(Edit *edit,int size);

/* To Overwrite the new data */
Status edit_new_data(Edit *edit);

/*convert littlr to big to littlr endian or vice verse*/
void endianess(void *ptr,int size);

#endif