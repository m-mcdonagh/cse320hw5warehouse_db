struct artEntry {
	char valid;
	int id;
	char* name;
	unsigned long int client;
};

void initializeDatabase(int maxRecords);
void newConnectionCheck(void);
void checkRequest(void);
void processRequest(int client, char* msg);
void cleanUp(void);
void list(int id);
void listAll(void);
void print_entry(int i);
void dump(void);
