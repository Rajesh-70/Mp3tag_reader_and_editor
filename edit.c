#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"edit.h"

// Validate command line arguments for edit operation
Status read_and_validate_edit(char *argv[],Edit *edit,int argc)
{
    // Check if operation argument is provided
    if(argv[2] == NULL)
    {
        printf("\n\tError: Insufficient Arguments\n");
        printf("\n\t\t\t-t : To edit Title\n");
        printf("\t\t\t-y : To edit Year\n");
        printf("\t\t\t-a : To edit Album\n");
        printf("\t\t\t-A : To edit Artist\n");
        printf("\t\t\t-c : To edit Comment\n");
        printf("\t\t\t-g : To edit Genre\n");
        return e_failure;
    }

    // Identify which tag needs to be edited
    if(strcmp(argv[2], "-t") == 0)
    {
        strcpy(edit->editing, "TIT2");   // Title
    }
    else if(strcmp(argv[2], "-y") == 0)
    {
        strcpy(edit->editing, "TYER");   // Year
    }
    else if(strcmp(argv[2], "-a") == 0)
    {
        strcpy(edit->editing, "TALB");   // Album
    }
    else if(strcmp(argv[2], "-A") == 0)
    {
        strcpy(edit->editing, "TPE1");   // Artist
    }
    else if(strcmp(argv[2], "-c") == 0)
    {
        strcpy(edit->editing, "COMM");   // Comment
    }
    else if(strcmp(argv[2], "-g") == 0)
    {
        strcpy(edit->editing, "TCON");   // Genre
    }
    else
    {
        // Unsupported option
        printf("Unsuported arguments!\n");
        printf("\nFor editing  : ./a.out -e <operation> <data_to_update> <file.mp3>\n");
        printf("\n\t\t\t-t : To edit Title\n");
        printf("\t\t\t-y : To edit Year\n");
        printf("\t\t\t-a : To edit Album\n");
        printf("\t\t\t-A : To edit Artist\n");
        printf("\t\t\t-c : To edit Comment\n");
        printf("\t\t\t-g : To edit Genre\n");
        return e_failure;
    }

    // Check if new data is provided
    if(argv[3] == NULL)
    {
        printf("File not present\n");
        return e_failure;
    }

    // Store new data to be updated
    strcpy(edit->new_data, argv[3]);

    // Check if audio file name is provided
    if(argv[4] == NULL)
    {
        printf("Audio file not present\n");
        return e_failure;
    }

    // Validate .mp3 extension
    int len = strlen(argv[4]);
    if(strcmp(&argv[4][len - 4],".mp3") != 0)
    {
        printf("Error : Audio file extension is not valid\n");
        return e_failure;
    }

    // Store audio file name
    edit->audio_file_name = argv[4];

    return e_success;
}


// Open original audio file and temporary file
Status open_edit_file(Edit *edit)
{
    // Open audio file in read mode
    edit->fptr_audio = fopen(edit->audio_file_name, "r");
    if (edit->fptr_audio == NULL)
    {
        printf("Cannot open Audio file\n");
        return e_failure;
    }

    // Create temporary file in write mode
    edit->temp_file_name = "temp.mp3";

    edit->fptr_temp = fopen(edit->temp_file_name, "w");
    if (edit->fptr_temp == NULL)
    {
        printf("Temp file opening  failed\n");
        return e_failure;
    }

    return e_success;
}


// Copy header bytes (first 10 bytes of ID3)
Status copy_header_bytes(Edit *edit,int size)
{
    char buffer[size];

    // Read header from original file
    fread(buffer, size, 1, edit->fptr_audio);

    // Write header to temp file
    fwrite(buffer, size, 1, edit->fptr_temp);

    return e_success;
}


// Edit the required tag data
Status edit_new_data(Edit *edit)
{
    char tag[5];    // Store frame ID
    int size;       // Store converted size
    char flag[3];   // Store frame flags

    // Loop until end of file
    while(feof(edit->fptr_audio) == 0)
    {
        // Read frame ID
        fread(tag, 4, 1, edit->fptr_audio);
        tag[4] = '\0';

        int original_size;

        // Read original size (big-endian)
        fread(&original_size, 4, 1, edit->fptr_audio);

        size = original_size;

        // Convert size to little-endian
        endianess(&size, 4);

        // Allocate memory for frame data
        char *data = malloc(size);
        if(data == NULL) {
            printf("Memory allocation failed\n");
            return e_failure;
        }

        // Read flag bytes
        fread(flag, 3, 1, edit->fptr_audio);

        // Read frame data
        fread(data, (size - 1), 1, edit->fptr_audio);
        data[size-1] = '\0';

        // Check if this is the tag to edit
        if(strcmp(tag, edit->editing) == 0)
        {
            // Write tag to temp file
            fwrite(tag, 4, 1, edit->fptr_temp);

            // Calculate new size
            int new_size = strlen(edit->new_data) + 1;
            int new_size_be = new_size;

            // Convert new size to big-endian
            endianess(&new_size_be, 4); 
            
            // Write updated size and data
            fwrite(&new_size_be, 4, 1, edit->fptr_temp);
            fwrite(flag, 3, 1, edit->fptr_temp);
            fwrite(edit->new_data, new_size - 1, 1, edit->fptr_temp);

            free(data);
            break;
        }
        else
        {
            // Copy existing frame as it is
            fwrite(tag, 4, 1, edit->fptr_temp);
            fwrite(&original_size, 4, 1, edit->fptr_temp);
            fwrite(flag, 3, 1, edit->fptr_temp);
            fwrite(data, size - 1, 1, edit->fptr_temp);

            free(data);
        }
    }

    // Copy remaining bytes after editing
    char buff[1];
    while(fread(buff, 1, 1, edit->fptr_audio) == 1)
    {
        fwrite(buff, 1, 1, edit->fptr_temp);
    }

    // Close both files
    fclose(edit->fptr_temp);
    fclose(edit->fptr_audio);

    // Replace original file with updated temp file
    remove(edit->audio_file_name);
    rename("temp.mp3", edit->audio_file_name);

    return e_success;
}


// Main edit operation controller
Status do_edit_operation(Edit *edit)
{
    // Open files
    if(open_edit_file(edit) == e_failure)
    {
        return e_failure;
    }

    // Copy header
    copy_header_bytes(edit,10);

    // Edit selected tag
    if(edit_new_data(edit) == e_failure)
    {
        return e_failure;
    }

    printf("Edited successfully\n");
    return e_success;
}