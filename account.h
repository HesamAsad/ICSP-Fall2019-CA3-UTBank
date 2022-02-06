#ifndef ACCOUNT_H
#define ACCOUNT_H

#define A_MAXNAME 60
#define A_MAXPASS 60
#define A_MAXNUM 60

#define AERR_USEREXISTS -1
#define AERR_PHONEEXISTS -2
#define AERR_NOUSER -3
#define AERR_WRONGPASS -4

typedef struct account
{
	int id;

	char user[A_MAXNAME];
	char pass[A_MAXPASS];
	char phoneNum[A_MAXNUM];

	unsigned int balance;
	int transNum;
	struct transaction *trans;

	struct account *next;
	struct account *previous;
} account_t;

int NewAccount(account_t *head, char *userName, char *password, char *number, account_t **out);
account_t* GetAccount(account_t *head, char *userName);
void ViewAccount(account_t *acc);
void DeleteAccount(account_t *acc);
int LoginAccount(account_t *head, char *userName, char *password, account_t **out);
void WriteAccounts(account_t *head);
account_t* ReadAccounts();


#endif