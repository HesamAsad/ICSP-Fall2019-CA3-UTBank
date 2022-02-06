#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "account.h"
#include "transaction.h"

#define DBNAME "ledger.txt"

int NewAccount(account_t *head, char *userName, char *password, char *number, account_t **out) {
	account_t *acc = malloc(sizeof(account_t));
	account_t *tail = NULL; // Last account

	int uid = 1;

	while (head != NULL)
	{
		if (strcmp(head->user, userName) == 0)
		{
			free(acc);
			return AERR_USEREXISTS;
		}

		if (strcmp(head->phoneNum, number) == 0)
		{
			free(acc);
			return AERR_PHONEEXISTS;
		}

		while (head->id == uid)
			uid++;

		tail = head;
		head = head->next;
	}

	acc->id = uid;

	acc->balance = 0;

	acc->transNum = 0;
	acc->trans = NULL;

	acc->next = NULL;
	acc->previous = tail;

	strcpy(acc->user, userName);
	strcpy(acc->pass, password);
	strcpy(acc->phoneNum, number);

	if (tail != NULL)
		tail->next = acc;

	*out = acc;
	return 1; //SUCCESS
}

account_t* GetAccount(account_t *head, char *userName)
{
	while (head != NULL)
	{
		if (strcmp(head->user, userName) == 0)
			return head;
		else
			head = head->next;
	}

	return NULL;
}

void ViewAccount(account_t *acc)
{
	printf("[Account info]\nID: %d\nUsername: %s\nPassword: %s\nPhone No.: %s\nBalance: $%u\n", acc->id, acc->user, acc->pass, acc->phoneNum, acc->balance);
	PrintTransactions(acc);
}

void DeleteAccount(account_t *acc)
{
	account_t *prevAcc = acc->previous;
	account_t *nextAcc = acc->next;

	if (prevAcc != NULL)
		prevAcc->next = nextAcc;

	if (nextAcc != NULL)
		nextAcc->previous = prevAcc;

	DeleteTransactions(acc);
	free(acc);
}

int LoginAccount(account_t *head, char *userName, char *password, account_t **out)
{
	account_t *acc = GetAccount(head, userName);

	if (acc == NULL)
		return AERR_NOUSER;

	if (strcmp(acc->pass, password) == 0)
	{
		*out = acc;
		return 1;
	}
	else
		return AERR_WRONGPASS;
}

void WriteAccounts(account_t *head)
{
	FILE *database = fopen(DBNAME, "w");

	while (head != NULL)
	{
		transaction_t *trans = head->trans;
		fprintf(database, "%d\n%s\n%s\n%s\n%d\n", head->id, head->user, head->pass, head->phoneNum, head->transNum);

		while (trans != NULL)
		{
			fprintf(database, "%d\n%d\n", trans->operation, trans->amount);
			trans = trans->next;
		}

		head = head->next;
	}

	fclose(database);
}

account_t* ReadAccounts()
{
	FILE *database = fopen(DBNAME, "r");

	if (database == NULL)
		return NULL;

	account_t *head = NULL, *prev = NULL;

	for(;;) //infinite loop
	{
		int ret, transNum;

		account_t *newAcc = malloc(sizeof(account_t));

		newAcc->previous = prev;
		newAcc->trans = NULL;
		newAcc->transNum = 0;
		newAcc->balance = 0;
		newAcc->next = NULL;

		if (head == NULL)
			head = newAcc;

		ret = fscanf(database, "%d %s %s %s %d", &(newAcc->id), newAcc->user, newAcc->pass, newAcc->phoneNum, &transNum);

		if (ret != 5) // End of file
		{
			free(newAcc);
			break;
		}

		if (prev != NULL)
			prev->next = newAcc;

		prev = newAcc;

		while (transNum--)
		{
			int op;
			unsigned int amount;

			fscanf(database, "%d %u", &op, &amount);
			AddTransaction(newAcc, op, amount);
		}
	}

	fclose(database);

	return head;
}