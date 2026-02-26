#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"view.h"

// Validate command line arguments and check .mp3 extension
Status read_and_validate_view(char *argv[],View *viewinfo)
{
    // Check if audio file argument is provided
    if (argv[2] == NULL)
    {
        printf("No Audio file found\n");
        return e_failure;
    }

    // Get length of filename
    int len = strlen(argv[2]);

    // Verify file extension is .mp3
    if(strcmp(&argv[2][len - 4],".mp3") != 0)
    {
        printf("Error : Audio file extension is not valid\n");
        return e_failure;
    }

    // Store source audio filename
    viewinfo -> src_audio_fname = argv[2];

    return e_success;
}

// Open the source audio file
Status open_files(View *viewinfo)
{
    // Open file in read mode
    viewinfo-> fptr_src_file = fopen(viewinfo-> src_audio_fname, "r");

    // Check if file opening failed
    if (viewinfo-> fptr_src_file == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", viewinfo-> src_audio_fname);

        return e_failure;
    }

    return e_success;
}

// Skip first 10 bytes (ID3 header)
Status skip_header(View *viewinfo)
{
    fseek(viewinfo -> fptr_src_file,10,SEEK_SET);
    return e_success;
}

// Read ID3 frames and print required tag data
Status read_and_print_data(View *viewinfo)
{
    printf("----------------------------------\n");
    printf("\tSONG INFO\n");
    printf("----------------------------------\n");

    int i = 0;

    // Loop until 6 required tags are printed
    while(i < 6)
    {
        char buff[5];  // Buffer to store tag name

        // Read 4-byte frame ID
        if(fread(buff, 4, 1, viewinfo->fptr_src_file) != 1)
        {
            return e_failure;
        }

        buff[4] = '\0';  // Null terminate tag string

        unsigned int size = 0;

        // Read 4-byte frame size
        fread(&size,4,1,viewinfo -> fptr_src_file);

        // Convert big-endian to little-endian
        endianess(&size,4);

        // Skip 3 flag bytes
        fseek(viewinfo -> fptr_src_file,3,SEEK_CUR);

        // Create buffer for frame data
        char data[size + 1];

        // Read frame data (excluding encoding byte)
        fread(data,size - 1,1, viewinfo->fptr_src_file);

        data[size - 1] = '\0';  // Null terminate data

        // Print data if tag matches required fields
        if(print_data(buff,data) == e_success)
        {
            i++;
        }
    }

    printf("----------------------------------\n");
    return e_success;
}

// Convert byte order (big-endian to little-endian)
void endianess(void *ptr,int size)
{
    char *cptr = (char*)ptr;

    // Swap bytes
    for(int i = 0;i < size/2;i++)
    {
        char temp = cptr[i];
        cptr[i] = cptr[size - 1 - i];
        cptr[size - 1 - i] = temp;
    }
}

// Print tag data based on frame ID
Status print_data(char *tag, char *data)
{
    if(strcmp(tag, "TIT2") == 0)   // Title
    {
        printf("TITLE\t :\t%s\n", data);
    }
    else if(strcmp(tag, "TYER") == 0)  // Year
    {
        printf("YEAR\t :\t%s\n", data);
    }
    else if(strcmp(tag, "TALB") == 0)  // Album
    {
        printf("ALBUM\t :\t%s\n", data);
    }
    else if(strcmp(tag, "TPE1") == 0)  // Artist
    {
        printf("ARTIST\t :\t%s\n", data);
    }
    else if(strcmp(tag, "COMM") == 0)  // Composer
    {
        printf("COMPOSER :\t%s\n", data);
    }
    else if(strcmp(tag, "TCON") == 0)  // Content/Genre
    {
        printf("CONTENT\t :\t%s\n", data);
    }
    else
    {
        return e_failure;  // Tag not required
    }

    return e_success;
}

// Main view operation controller
Status do_view(View *viewinfo)
{
    // Open file
    if(open_files(viewinfo) == e_failure)
    {
        return e_failure;
    }

    // Skip header
    if(skip_header(viewinfo) == e_failure)
    {
        return e_failure;
    }

    // Read and print metadata
    if(read_and_print_data(viewinfo) == e_failure)
    {
        fclose(viewinfo -> fptr_src_file);
        return e_failure;
    }

    // Close file
    fclose(viewinfo -> fptr_src_file);

    return e_success;
}