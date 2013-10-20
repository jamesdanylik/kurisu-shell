// UCLA CS 111 Lab 1 A/B

/* Skeleton Includes -------------------------------------------------------- */
#include "command.h"
#include "command-internals.h"

#include <error.h>

/*My Includes --------------------------------------------------------------- */
#include "alloc.h"
#include <stdio.h>
#include <ctype.h>

/* Type Declarations -------------------------------------------------------- */

// The command stream should just be a tree of commands, so all we need is a
// pointer to the root of the tree
struct command_stream
{
  command_t *root;
};

/* Helper Functions Prototypes ---------------------------------------------- */

bool is_word_char ( char c); 
  // Tests whether or not the next byte is an eligble word character.

char get_byte_after_comment ( int (*get_next_byte) (void *), 
                              void *get_next_byte_argument );
  // Gets the next eligble byte after comment ('#') has been encountered.

/* Helper Function Definitions --------------------------------------------- */

bool 
is_word_char ( char c )
{
  // Words can be alphanumeric or contain the prescribed symbols
  if ( isalnum(c) || c == '!' || c == '%' || c == '+' || c == ',' || c == '-' 
  || c == '.' || c == '/' || c == ':' || c == '@' || c == '^' || c == '_' )
    return true;
  // Otherwise it's not a word character at all, is it?
  else return false;
}

char 
get_byte_after_comment ( int (*get_next_byte) (void *), 
                         void *get_next_byte_argument )
{
  // Keep grabbing the next byte until we get to the next line, or the file
  // ends.
  char next_byte;
  while ( (next_byte = get_next_byte(get_next_byte_argument)),
          next_byte != EOF && next_byte != '\n' ) 
  { //Spin until the conditions are met, then return.
  } 
  return next_byte;
}

/* Parsing Function Protoypes ----------------------------------------------- */

command_t
parse_simple_command ( int (*get_next_byte) (void *), 
                       void *get_next_byte_argument );
  // Function to parse simple commands, defined as sequences of one or more 
  // words.  Specifically, creates commands that are of type SIMPLE_COMMAND

command_t 
parse_subshell_command ( int (*get_next_byte) (void *),
                         void *get_next_byte_argument );
  // Function to parse subshell commands, defined as complete commands in the
  // spec.  Specifically, creates commands that are of type SUBSHELL_COMMAND

command_t
parse_pipe_command ( int (*get_next_byte) (void *),
                         void *get_next_byte_argument );
  // Function to parse pipeline commands, defined as one or more commands
  // seperated by '|'.  Specifically, creates commands that are of type 
  // PIPE_COMMAND

command_t
parse_andor_command ( int (*get_next_byte) (void *),
                      void *get_next_byte_argument );
  // Function to parse andor commands, defined as one or more pipelines
  // seperated by '&&' or '||'.  This function will produce output of two
  // types, AND_COMMAND and OR_COMMAND; this is okay since they are equal
  // precedence as defined in the spec.

command_t
parse_complete_command ( int (*get_next_byte) (void *),
                         void * get_next_byte_argument );
 // Function to parse complete commands, and hence our entry point function.
 // which are defined as one or more AND/OR commands seperated by a SEMICOLON
 // or a NEWLINE, and optionally followed by a SEMICOLON.  This returns
 // commands of type SEQUENCE_COMMAND.


/* Parsing Function Defintions ---------------------------------------------- */

/* Main Hook Functions ------------------------------------------------------ */

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  (void) get_next_byte;
  (void) get_next_byte_argument;
  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  /* FIXME: Replace this with your implementation too.  */
  (void)s;

  error (1, 0, "command reading not yet implemented");
  return 0;
}
