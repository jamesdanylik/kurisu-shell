// UCLA CS 111 Lab 1 command reading

/* Skeleton Includes */
#include "command.h"
#include "command-internals.h"

#include <error.h>

/*My Includes */
#include "alloc.h"
#include <stdio.h>

/* Type Declarations */

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* INPROGRESS: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

struct command_stream
{
  int size;
  command_t *commands;
};

typedef enum 
{
  WORD_T,
  PIPE_T,
  AND_T,
  OR_T,
  OPEN_PAREN_T,
  CLOSE_PAREN_T,
  L_REDIR_T,
  R_REDIR_T,
  NEWLINE_T,
  SEMICOLON_T
} token_type_t;

typedef struct
{
  token_type_t type;
  char *wordData;
} token_t;

/* Main Hook Functions */

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* INPROGRESS: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */

  /* Begin by reading the shell script into a buffer for processing */

  /* Some variables to hold the buffer and track its size */
  size_t bufferSize = 256; 
  size_t bytesRead = 0;    
  int currentByte;  
  char *buffer = (char*) checked_malloc(bufferSize);

  /* Read the entire script into the buffer.  Double in size if the script
     exceeds what we currently have allocated.  Terminate the buffer with a
     null byte.  */
  while ( (currentByte = get_next_byte(get_next_byte_argument)) != EOF )
  {
    buffer[bytesRead++] = currentByte;
    if ( bytesRead == bufferSize)
    {
      buffer = (char*) checked_grow_alloc(buffer, &bufferSize);
    } 
  }
  buffer[bytesRead] = 0;
  // PROOF: 
  // printf ("%s \n", buffer);

  error (1, 0, "command reading not FULLY implemented");
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
