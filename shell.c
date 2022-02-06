#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.h"
#include "transaction.h"
#include "shell.h"

void PrintError(int code)
{
	switch(code)
	{
	case CERR_NOAUTH:
		puts("Please authenticate with \"login <user> <pass>\".");
		break;
	case CERR_NOTADMIN:
		puts("You do not have enough privilege to use this command.");
		break;
	case CERR_INVALIDCMD:
		puts("Invalid command.");
		break;
	case CERR_NOUSER:
		puts("No such user exists.");
		break;
	}

	// Clear stdin - necessary for verbs with parameters
	scanf("%*[^\n]%*c");
}

void HandleCommand(account_t *head)
{
	account_t *userAcc = NULL;
	int isAdmin;
	char cmd[C_MAXVERB];

	for (;;)
	{
		if (userAcc == NULL)
		{
			printf("(anonymous)$ ");
		}
		else
		{
			printf("(%s)$ ", userAcc->user);
		}

		if (userAcc != NULL && userAcc == head)
			isAdmin = 1;
		else
			isAdmin = 0;

		scanf("%s", cmd);

		if (strcmp(cmd, "exit") == 0)
		{
			puts("Have a nice day!");
			break;
		}
		else if (strcmp(cmd, "login") == 0)
		{
			char name[A_MAXNAME];
			char password[A_MAXPASS];

			scanf("%s %s", name, password);

			switch (LoginAccount(head, name, password, &userAcc))
			{
			case AERR_NOUSER:
				puts("No such user exists.");
				break;
			case AERR_WRONGPASS:
				puts("Wrong password.");
				break;
			default:
				printf("Welcome %s!\n", name);
			}
		}
		else if (userAcc == NULL)
		{
			PrintError(CERR_NOAUTH); // Next commands require the user to be authenticated
		}
		else if (strcmp(cmd, "logout") == 0)
		{
			puts("Bye-bye!");
			userAcc = NULL;
		}
		else if (strcmp(cmd, "deposit") == 0 || strcmp(cmd, "withdraw") == 0)
		{
			if (!isAdmin)
			{
				PrintError(CERR_NOTADMIN);
				continue;
			}

			char name[A_MAXNAME];
			unsigned int amount;
			account_t *recipient = NULL;

			scanf("%s %u", name, &amount);

			recipient = GetAccount(head, name);

			if (recipient == NULL)
			{
				PrintError(CERR_NOUSER);
				continue;
			}

			if (cmd[0] == 'd')
			{
				AddTransaction(recipient, T_DEPOSIT, amount);
				printf("Deposited $%u to %s\n", amount, name);
			}
			else
			{
				if (AddTransaction(recipient, T_WITHDRAW, amount) == TERR_INSUFFICIENT_BALANCE)
					puts("Error: Insufficient balance!");
				else
					printf("Withdrew $%u from %s\n", amount, name);
			}
		}
		else if (strcmp(cmd, "transfer") == 0)
		{
			char name[A_MAXNAME];
			unsigned int amount;
			account_t *target;

			scanf("%s %u", name, &amount);
			target = GetAccount(head, name);

			if (target == NULL)
			{
				PrintError(CERR_NOUSER);
				continue;
			}

			if (AddTransaction(userAcc, T_TRANSFEROUT, amount) == TERR_INSUFFICIENT_BALANCE)
			{
				puts("Error: Insufficient balance!");
			}
			else
			{
				AddTransaction(target, T_TRANSFERIN, amount);
				printf("Transferred $%u to %s\n", amount, name);
			}
			
		}
		else if (strcmp(cmd, "view") == 0)
		{
			char end;

			scanf("%c", &end);

			if (end == '\n')
			{
				ViewAccount(userAcc);
			}
			else
			{
				if (!isAdmin)
				{
					PrintError(CERR_NOTADMIN);
					continue;
				}

				char user[A_MAXNAME];
				scanf("%s", user);

				account_t *target = GetAccount(head, user);

				if (target == NULL)
				{
					PrintError(CERR_NOUSER);
					continue;
				}

				ViewAccount(target);
			}
		}
		else if (strcmp(cmd, "add_user") == 0)
		{
			if (!isAdmin)
			{
				PrintError(CERR_NOTADMIN);
				continue;
			}

			char name[A_MAXNAME];
			char password[A_MAXPASS];
			char phone[A_MAXNUM];

			scanf("%s %s %s", name, password, phone);

			account_t *newAcc;
			int ret = NewAccount(head, name, password, phone, &newAcc);

			switch (ret)
			{
			case AERR_USEREXISTS:
				puts("Error: User already exists.");
				break;
			case AERR_PHONEEXISTS:
				puts("Error: An account with same phone number exists.");
				break;
			default:
				printf("Created user %s\n", name);
			}
		}
		else if (strcmp(cmd, "delete") == 0)
		{
			if (!isAdmin)
			{
				PrintError(CERR_NOTADMIN);
				continue;
			}

			char name[A_MAXNAME];
			account_t *target;

			scanf("%s", name);
			target = GetAccount(head, name);

			if (target == NULL)
			{
				PrintError(CERR_NOUSER);
			}
			else
			{
				DeleteAccount(target);
				printf("Deleted user %s\n", name);
			}
		}
		else
		{
			PrintError(CERR_INVALIDCMD);
		}
	}
}