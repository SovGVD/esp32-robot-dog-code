#define CLI_VALUES    4
#define CLI_VALUE_LEN 32
#define CLI_DP 6  // decimal point
#define CLI_GET 1
#define CLI_SET 2
#define CLI_RUN 3
#define CLI_TERMINAL_SIGN "> "
#define CLI_DELIMITER ":"

typedef double (*cliFunction)(double);

const typedef struct cliCommand_t {
  char commandName[CLI_VALUE_LEN];
  cliFunction func;
} cliCommand;
