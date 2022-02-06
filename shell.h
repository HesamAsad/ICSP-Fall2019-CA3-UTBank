#ifndef SHELL_H
#define SHELL_H

#define C_MAXVERB 15 // -> maximum verb length, "withdraw", "deposit", ...

#define CERR_NOAUTH -1
#define CERR_NOTADMIN -2
#define CERR_NOUSER -3
#define CERR_INVALIDCMD -4
#define CERR_LOGGEDIN -5

void HandleCommand(account_t *head);

#endif