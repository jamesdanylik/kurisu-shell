// UCLA CS 111 Lab 1 command reading

/* Skeleton Includes -------------------------------------------------------- */
#include "command.h"
#include "command-internals.h"

#include <error.h>

/*My Includes --------------------------------------------------------------- */
#include "alloc.h"
#include <stdio.h>

/* Type Declarations -------------------------------------------------------- */

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

/* Helper Functions Prototypes ---------------------------------------------- */

bool is_word_char ( char c); 

bool 
is_word_char ( char c )
{
  if ( isalnum(c) )
  {
    return true;
  }
  else if (      c == '!' || c == '%' || c == '+' || c == ',' || c == '-' 
  || c == '.' || c == '/' || c == ':' || c == '@' || c == '^' || c == '_' )
  {
    return true;
  }
  else
  {
    return false;
  }
}

/* Main Hook Functions ------------------------------------------------------ */

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* INPROGRESS: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */

  size_t line_count = 0;

  size_t buffer_size = 16; 
  size_t bytes_read = 0;    
  char *buffer = (char*) checked_malloc(buffer_size);
  char current_byte;

  size_t token_arr_byte_size = 16 * sizeof(token_t);
  size_t token_arr_size = 16;
  size_t tokens_read = 0;
  token_t *tokens = (token_t*) checked_malloc(token_arr_byte_size); 
  

  while ( (current_byte = get_next_byte(get_next_byte_argument)) != EOF )
  {
    if ( is_word_char(current_byte) ) 
    {
      tokens[tokens_read].type = WORD_T;
      buffer[bytes_read++] = current_byte;
      current_byte = get_next_byte(get_next_byte_argument);
      while ( is_word_char(current_byte) )
      {

      }
    }
    buffer[bytesRead++] = currentByte;
    if ( bytesRead == bufferSize)
    {
      buffer = (char*) checked_grow_alloc(buffer, &bufferSize);
    } 
  }
  // PROOF: 
  //printf ("%s \n", buffer);

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
