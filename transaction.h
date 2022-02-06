#ifndef TRANSACTION_H
#define TRANSACTION_H

#define T_DEPOSIT 0
#define T_WITHDRAW 1
#define T_TRANSFERIN 2
#define T_TRANSFEROUT 3

#define TERR_INSUFFICIENT_BALANCE -1

typedef struct transaction
{
	int operation; // 0 -> Deposit, 1 -> Withdraw, 2 -> Transfer in, 3 -> Transfer out
	unsigned int amount;

	struct transaction *next;
} transaction_t;

int AddTransaction(account_t *acc, int op, unsigned int amount);
void PrintTransactions(account_t *acc);
void DeleteTransactions(account_t *acc);

#endif