// UCLA CS 111 Lab 1 A/B

/* Skeleton Includes -------------------------------------------------------- */
#include "command.h"
#include "command-internals.h"

#include <error.h>

/*My Includes --------------------------------------------------------------- */
#include "alloc.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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

void give_last_byte( void *get_next_byte_argument );
  // A helper function to move back the last byte for further processing.

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

void
give_last_byte( void *get_last_byte_argument)
{
  fseek(get_last_byte_argument, -1, SEEK_CUR);
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

command_t
parse_simple_command ( int (*get_next_byte) (void *),
                       void *get_next_byte_argument )
{
  // Allocate a command and set its type to SIMPLE_COMMAND
  command_t command = checked_malloc(sizeof(struct command));
  command->type = SIMPLE_COMMAND; 

  // Prepare a dynamic array of C strings (char arrays) to hold the words of
  // the command.  Both the array of words and words themselves must be randomly
  // resize, so their sizes must be traced with variables per command as well.
  // First, the words array:
  const size_t default_words_size = 4 * sizeof(char *);
  size_t words_size = default_words_size;
  size_t words_used = 0;
  char **words = (char **) checked_malloc(words_size);
  // Now allocate and initialize the first word, and variable for tokens in
  // the current word:
  const size_t default_word_size = 8 * sizeof(char);
  size_t word_size = default_word_size;
  char *word = (char *) checked_malloc(word_size);
  word[0] = '\0';

  // Start the main read loop
  char next_byte;
  while ( (next_byte = get_next_byte(get_next_byte_argument)) != EOF )
  {
    // If '#', then its a comment and we can use our helper to just move to
    // the next byte, no harm no foul; first case of the loop.
    if ( next_byte == '#' ) 
      next_byte = get_byte_after_comment(get_next_byte, get_next_byte_argument);
    // If ' ' or '/t' we can just restart the loop and check again.
    else if ( next_byte == ' ' || next_byte == '\t' )
      continue;
    // If it's a word eligable character, this is a word  
    else if ( is_word_char(next_byte) )
    {
      // Write the first byte in.
      strncat(word, &next_byte, 1);
  
      // Read in the rest of the word
      while ( (next_byte = get_next_byte(get_next_byte_argument)),
              is_word_char(next_byte) )
      {
        // Resize the word if it gets too big
        if ( strlen(word) == (word_size-1) )
        {
          word = checked_grow_alloc(word, &word_size);
        }
        strncat(word, &next_byte, 1);
      }
      // Put the last byte back for processing.
      give_last_byte(get_next_byte_argument);

      // resize the words array if this word will make too many
      if ( (words_used*sizeof(char *)) >= (words_size-sizeof(char *)) )
      {
        words = checked_grow_alloc(words, &words_size);
      }

      // store the word in the array and allocate a new one
      words[words_used++] = word;
      word_size = default_word_size;
      word = (char *) checked_malloc(word_size);
      word[0] = '\0';
      continue;
    }
  }
  command->u.word = words;
  return command;
}

/* Main Hook Functions ------------------------------------------------------ */

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  parse_simple_command(get_next_byte, get_next_byte_argument);
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
