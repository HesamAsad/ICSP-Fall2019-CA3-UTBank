#include <stdlib.h>
#include <stdio.h>

#include "account.h"
#include "transaction.h"

int AddTransaction(account_t *acc, int op, unsigned int amount)
{
	transaction_t *newTrans = malloc(sizeof(transaction_t));

	newTrans->operation = op;
	newTrans->amount = amount;
	newTrans->next = NULL;

	acc->transNum++;

	if (op % 2 == 0) // Addition
	{
		acc->balance += amount;
	}
	else // Deduction
	{
		if (acc->balance >= amount)
		{
			acc->balance += -1 * amount;
		}
		else
		{
			return TERR_INSUFFICIENT_BALANCE;
		}
	}

	if (acc->trans == NULL)
	{
		acc->trans = newTrans;
	}
	else
	{
		transaction_t *tail = acc->trans;

		while (tail->next != NULL)
			tail = tail->next;

		tail->next = newTrans;
	}

	return 1; // SUCCESS
}

void PrintTransactions(account_t *acc)
{
	transaction_t *head = acc->trans;
	int num = 1;

	if (head != NULL)
		puts("[Transactions]");

	while (head != NULL)
	{
		switch (head->operation)
		{
		case 0:
			printf("%d. Deposited $%u\n", num, head->amount);
			break;
		
		case 1:
			printf("%d. Withdrew $%u\n", num, head->amount);
			break;
		
		case 2:
			printf("%d. Received $%u\n", num, head->amount);
			break;
		
		case 3:
			printf("%d:.Transferred out $%u\n", num, head->amount);
			break;
		}

		head = head->next;
		num++;
	}
}

void DeleteTransactions(account_t *acc)
{
	transaction_t *head = acc->trans;

	while (head != NULL)
	{
		transaction_t *tmp;

		tmp = head;
		head = head->next;

		free(tmp);
	}
}