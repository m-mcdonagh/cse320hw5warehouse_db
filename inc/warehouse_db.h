struct artEntry {
	char valid;
	char client;
	int id;
	char* name;
};

void initializeDatabase(int maxRecords);
void newConnectionCheck(void);
void checkRequest(void);
void processRequest(int client, char* msg);
void print_entry(int i);
void list(void);
void cleanUp(void);
