#ifndef TYPES_H
#define TYPES_H

// This file contains common data types used in the project

// Status enum is used as return type for functions
// e_failure indicates operation failure
// e_success indicates operation success
typedef enum
{
    e_failure,
    e_success
} Status;


// OperationType enum is used to identify user-selected operation
// e_view        -> View MP3 tags (-v)
// e_edit        -> Edit MP3 tags (-e)
// e_help        -> Display help menu (-h)
// e_unsupported -> Invalid argument
typedef enum
{
    e_view,
    e_edit,
    e_help,
    e_unsupported
} OperationType;

#endif