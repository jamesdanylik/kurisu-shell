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

/* Type Declarations & Global Variables ------------------------------------- */

struct command_stream
{
  // The command stream should just be a tree of commands, so all we need is a
  // pointer to the root of the tree
  command_t root;
};

int line_number = 1;
// A global variable to track the line we're currently on.  A utilitarian
// concession; unly but efficient. We start on one cause most text editors do.

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
                         void *get_next_byte_argument,
                         bool subshell_opened );
  // Function to parse subshell commands, defined as complete commands in the
  // spec.  Specifically, creates commands that are of type SUBSHELL_COMMAND.
  // Added a function parameter to remember whether or not this is the first
  // subshell called easily.

command_t
parse_pipe_command ( int (*get_next_byte) (void *),
                     void *get_next_byte_argument,
                     command_t left_command );
  // Function to parse pipeline commands, defined as one or more commands
  // seperated by '|'.  Specifically, creates commands that are of type 
  // PIPE_COMMAND

command_t
parse_andor_command ( int (*get_next_byte) (void *),
                      void *get_next_byte_argument,
                      command_t left_command );
  // Function to parse andor commands, defined as one or more pipelines
  // seperated by '&&' or '||'.  This function will produce output of two
  // types, AND_COMMAND and OR_COMMAND; this is okay since they are equal
  // precedence as defined in the spec.

command_t
parse_complete_command ( int (*get_next_byte) (void *),
                         void * get_next_byte_argument,
                         command_t left_command );
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

  // Some flags to remember having seen tokens
  bool input_redir = false;
  bool output_redir = false;

  // Start the main read loop
  char next_byte;
  while ( (next_byte = get_next_byte(get_next_byte_argument)) != EOF )
  {
    // If '#', then its a comment and we can use our helper to just move to
    // the next byte, no harm no foul; first case of the loop.
    if ( next_byte == '#' ) 
    {
      // next_byte is now the next newline, which we'll throw away to continue
      // the loop.  But remember to incremet line_number!
      next_byte = get_byte_after_comment(get_next_byte, get_next_byte_argument);
      line_number++;
      continue;
    }
    // If ' ' or '/t' we can just restart the loop and check again.
    else if ( next_byte == ' ' || next_byte == '\t' )
      continue;
    // If '>', set output_redir to true; if it's already set its an error
    else if ( next_byte == '>' )
    {
      if ( input_redir || output_redir )
        error(1,0,"%d: Output redirection error. Double redirect?", line_number);
      else
        output_redir = true;
      continue;
    }
    else if ( next_byte == '<' )
    {
      if ( input_redir || output_redir )
        error(1,0,"%d: Input redirection error. Double redirect?", line_number);
      else
        input_redir = true;
      continue;
    }
    // If it's a word eligable character, this is a word, so start the
    // heavy lifting:  
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
          word = checked_grow_alloc(word, &word_size);
        strncat(word, &next_byte, 1);
      }
      // Put the last byte back for processing, it might not be a space!
      give_last_byte(get_next_byte_argument);

      // If output_redir is true, link this word to output and reset the flag.
      if ( output_redir )
      {
        // If command->output is NULL, this is the first redirect, so
        // set the word we just read to it, else its an error
        if ( command->output == NULL )
        {
          command->output = word;
          output_redir = false;
        }
        else
          error(1,0,"%d: Output was already redirected for this command!", line_number);
      }
      // Else if input_redir is true, link to that and reset the flag.
      else if ( input_redir )
      {
        // If command->input is NULL, this is the first redirect, so
        // set the word we just read to it, else its an error
        if ( command->input == NULL )
        {
          command->input = word;
          input_redir = false;
        }
        else
          error(1,0,"%d: Input was already redirected for this command!", line_number);
      }
      // Else this is just another argument or word
      else
      {
        if ( command->input != NULL || command->output != NULL)
          error(1,0,"%d: Extra words following I/O redirects.", line_number);

        // resize the words array if this word will make too many
        if ( (words_used*sizeof(char *)) >= (words_size-sizeof(char *)) )
          words = checked_grow_alloc(words, &words_size);

        // store the word in the array
        words[words_used++] = word;
      }
      // End of word processing, so restart reallocateword and restart  the loop.
      word_size = default_word_size;
      word = (char *) checked_malloc(word_size);
      word[0] = '\0';
      continue;
    }
    // If is any of the remaing possible POSIX characters, the simple command
    // is over, so put that back and return to a higher function.
    else if ( next_byte == ';' || next_byte == '|' || next_byte == '&'
           || next_byte == '\n'|| next_byte == ')')
    {
      // If either flag is still set, the command ended improperly
      if ( output_redir || input_redir )
        error(1,0,"%d: Incomplete redirect in simple commmand!", line_number);
      give_last_byte(get_next_byte_argument);
      break;
    }
    else
      error(1,0,"%d: Unknown character encountered in simple command.", line_number);
  }
  // If the command has a first word, everything should be good.  Else, we
  // either saw a blank command or... 
  if (words[0] != NULL )
    command->u.word = words;
  else
    error(1,0,"%d: Blank command encountered?", line_number);
  return command;
}

command_t
parse_subshell_command (int (*get_next_byte) (void *),
                        void *get_next_byte_argument,
                        bool subshell_opened )
{
  // Allocate a command and set but dont set it's type yet;
  command_t command = checked_malloc(sizeof(struct command));

  // Start the main read loop
  char next_byte;
  while ( (next_byte = get_next_byte(get_next_byte_argument)) != EOF )
  {
   // First check if it's a comment or whitespace and advance.
   if ( next_byte == '#' )
    {
      // next_byte is now the next newline, this time we need to handle it,
      // so push it back on the stack and continue
      next_byte = get_byte_after_comment(get_next_byte, get_next_byte_argument);
      give_last_byte(get_next_byte_argument);
      continue;
    }
    // If ' ' or '/t' we can just restart the loop and check again.
    else if ( next_byte == ' ' || next_byte == '\t' )
      continue;    
    // If its a word char, put it back and call simple command directly
    else if ( is_word_char(next_byte) )
    {
      give_last_byte(get_next_byte_argument);
      command = parse_simple_command(get_next_byte, get_next_byte_argument);
      continue;
    }
    // The next byte starts a subshell.  Nested call with subshell_open = true.
    // Why?  Because believe me, without that parameter handling nested subshells
    // is a huuuugge pain.
    else if ( next_byte == '(' )
    {
      // call a copy of ourself to handle it instead of just setting the flag here
      // this provides some memory for the state!
      command = parse_subshell_command(get_next_byte, get_next_byte_argument, true);
      if ( command->type != SUBSHELL_COMMAND )
        error(1,0, "%d: Subshell was opened, but never closed. Missing ')'?", line_number);
      continue;
    }
    // If this byte ends the subshell, either we'r done or something tanked if a
    // subshell never started.  Allocate a subshell command to nest the target 
    // command within, and return the subshell command.
    else if ( next_byte == ')' )
    {
      if ( subshell_opened )
      {
        command_t subshell_command = checked_malloc(sizeof(struct command));
        subshell_command->type = SUBSHELL_COMMAND;
        subshell_command->u.subshell_command = command;
        return subshell_command;
      }
      else
      {
      //  error(1,0,"%d: Subshell was closed, but not opened.  Missing '('?", line_number);
        give_last_byte(get_next_byte_argument);
        return command;
      }
    }
    // If it's a pipe, it could be a pipe or an or, but for now we'll just handle
    // pipes
    else if ( next_byte == '|' )
    {
      command = parse_pipe_command(get_next_byte, get_next_byte_argument, command);
    }
  } 
  if ( subshell_opened )
    error(1,0,"%d: Subshell was opened, but never closed. Missing ')'?", line_number);  
  return command;
} 

command_t
parse_pipe_command ( int (*get_next_byte) (void *),
                     void *get_next_byte_argument,
                     command_t left_command )
{
  // Make sure we have a proper left command to pipe from before building
  if ( left_command->type != SIMPLE_COMMAND 
    && left_command->type != SUBSHELL_COMMAND
    && left_command->type != PIPE_COMMAND )
    error(1,0,"%d: Improper pipe input, or command build error.", line_number);

  // Allocate a command, set its type equal to PIPE and set it's left command to
  // be the command passed in as an argument, then just parse the next part?
  command_t command = checked_malloc(sizeof(struct command));
  command->type = PIPE_COMMAND;
  command->u.command[0] = left_command;
  command->u.command[1] = parse_subshell_command(get_next_byte,get_next_byte_argument, false);
  return command;
}

/* Main Hook Functions ------------------------------------------------------ */

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  command_stream_t stream = checked_malloc(sizeof(stream));
  command_t root = checked_malloc(sizeof(struct command));

  root = parse_subshell_command(get_next_byte, get_next_byte_argument, false);

  if ( root != NULL )
    stream->root = root;
  else
    error(1,0,"%d: Failed stream creation.  No commands read.", line_number);
  
  return stream;
}

command_t
read_command_stream (command_stream_t s)
{
  command_t command = s->root;
  if ( command == NULL)
    return NULL;
  else if ( command->type == SUBSHELL_COMMAND )
  {
    while ( command->type == SUBSHELL_COMMAND )
    {
      if ( command->u.subshell_command != NULL )
        command = command->u.subshell_command;
      else
        error(1,0,"A subshell went wonky in storage.");
      s->root = command;
    }
  }
  s->root = NULL;
  return command;
}
