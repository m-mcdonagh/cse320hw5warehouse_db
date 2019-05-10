all:
	gcc src/client.c -I./inc -o client
	gcc src/warehouse_db.c src/error_checking.c -I./inc -o warehouse_db

client:
	gcc src/client.c -I./inc -o client

warehouse_db:
	gcc src/warehouse_db.c src/error_checking.c -I./inc -o warehouse_db

clean:
	rm client warehouse_db
