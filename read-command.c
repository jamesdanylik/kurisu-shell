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

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* INPROGRESS: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

struct command_stream
{
  command_t *commands;
};

/* Helper Functions Prototypes ---------------------------------------------- */

bool is_word_char ( char c); 
char get_byte_after_whitespace ( int (*get_next_byte) (void *),
                      void *get_next_byte_argument );
char get_byte_after_comment ( int (*get_next_byte) (void *),
                   void *get_next_byte_argument );

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
get_byte_after_whitespace ( int (*get_next_byte) (void *),
                            void *get_next_byte_argument )
{
  // Keep grabbing 
  char current_byte;
  while ( (current_byte = get_next_byte(get_next_byte_argument)),
          current_byte == ' ' || current_byte == '\t' )
  {
  }
  return current_byte;
}

char 
get_byte_after_comment ( int (*get_next_byte) (void *),
                              void *get_next_byte_argument )
{
  // Keep grabbing the next byte until we get a non whitespace character,
  // then return the byte we got.
  char current_byte;
  while ( (current_byte = get_next_byte(get_next_byte_argument)),
          current_byte != EOF && current_byte != '\n' ) 
  { //Spin until the conditions are met, then return.
  } 
  return current_byte;
}

/* Main Hook Functions ------------------------------------------------------ */

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* INPROGRESS: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
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
