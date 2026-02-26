/**
 * ============================================================================
 *  Project Name : MP3Tag_Raeder_and_Editor
 *  Author       : Rajesh Bathula
 *  Batch        : 25036A-39
 *
 *  Description  :
 *  --------------
 *  This program is a command-line based MP3 Tag Reader and Editor.
 *  It allows the user to:
 *
 *      1. View MP3 tag details  (-v)
 *      2. Edit MP3 tag details  (-e)
 *      3. Display help menu     (-h)
 *
 *  Supported Editing Options:
 *      -t : Edit Title
 *      -y : Edit Year
 *      -a : Edit Album
 *      -A : Edit Artist
 *      -c : Edit Comment
 *      -g : Edit Genre
 *
 *  Usage:
 *  ------
 *      For Help:
 *          ./a.out -h
 *
 *      For Viewing MP3 Tags:
 *          ./a.out -v <file.mp3>
 *
 *      For Editing MP3 Tags:
 *          ./a.out -e <operation> <data_to_update> <file.mp3>
 *
 *  Dependencies:
 *      types.h  - Contains enum definitions and common data types
 *      view.h   - Contains declarations related to viewing MP3 tags
 *      edit.h   - Contains declarations related to editing MP3 tags
 *
 * ============================================================================
 */
#include<stdio.h>
#include<string.h>
#include"types.h"
#include"view.h"
#include"edit.h"

// Function to check operation type based on user argument
OperationType check_operation_type(char *argv[]);

int main(int argc, char *argv[])
{
    // Check if minimum arguments are provided
    if (argc < 2)
    {
        printf("Insufficient Arguments\n");
        printf("\nFor help  : ./a.out -h <file.mp3>\n");
        printf("\nFor viewing  : ./a.out -v <file.mp3>\n");
        printf("\nFor editing  : ./a.out -e <operation> <data_to_update> <file.mp3>\n");
        return e_failure;
    }

    // If user selects view operation
    if (check_operation_type(argv) == e_view)
    {
        View viewinfo;  // Structure for view operation

        // Validate arguments for view
        if (read_and_validate_view(argv,&viewinfo) == e_success)
        {
            // Perform view operation
            if (do_view(&viewinfo) == e_success)
                return e_success;
            else
                return e_failure;
        }
        else
        {
            printf("Read and validation Un-successful\n");
            return e_failure;
        }
    }

    // If user selects edit operation
    else if (check_operation_type(argv) == e_edit)
    {
        Edit edit;  // Structure for edit operation

        printf("You have chosen Editing\n");

        // Validate arguments for edit
        if(read_and_validate_edit(argv,&edit,argc)==e_success)
        {
            // Perform edit operation
            if(do_edit_operation(&edit) == e_success)
            {
                return e_success;
            }
            else
            {
                return e_failure;
            }
        }
    }

    // If user selects help operation
    else if (check_operation_type(argv) == e_help)
    {
        printf("\nFor viewing  : ./a.out -v <file.mp3>\n");
        printf("\nFor editing  : ./a.out -e <operation> <data_to_update> <file.mp3>\n");
        printf("\n\t\t\t-t : To edit Title\n");
        printf("\t\t\t-y : To edit Year\n");
        printf("\t\t\t-a : To edit Album\n");
        printf("\t\t\t-A : To edit Artist\n");
        printf("\t\t\t-c : To edit Comment\n");
        printf("\t\t\t-g : To edit Genre\n");
        return e_failure;
    }

    // If unsupported argument is given
    else
    {
        printf("Unsuported arguments!\n");
        printf("\nFor help  : ./a.out -h\n");
        printf("\nFor viewing  : ./a.out -v <file.mp3>\n");
        printf("\nFor editing  : ./a.out -e <operation> <data_to_update> <file.mp3>\n");
        return e_failure;
    }

}

// Determine which operation user selected
OperationType check_operation_type(char *argv[])
{
    // View operation
    if(strcmp(argv[1],"-v")==0)
    {
        return e_view;
    }
    // Edit operation
    else if(strcmp(argv[1],"-e")==0)
    {
        return e_edit;
    }
    // Help operation
    else if(strcmp(argv[1],"-h")==0)
    {
        return e_help;
    }
    // Unsupported operation
    else
    {
        return e_unsupported;
    }
}