#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.h"
#include "transaction.h"
#include "shell.h"

int main()
{
	account_t *head = ReadAccounts(); // First account (always admin)

	if (head == NULL)
	{
		NewAccount(NULL, "admin", "123", "0000000000", &head);
	}

	puts("Welcome to UT Bank!\n");
	HandleCommand(head);

	WriteAccounts(head);
	return 0;
}