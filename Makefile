all:
	gcc src/client.c src/client_fifo.c src/error_checking.c -I./inc -o client -lpthread
	gcc src/server.c src/warehouse_db.c src/error_checking.c -I./inc -o warehouse_db
	gcc src/fifo_creator.c src/error_checking.c -I./inc -o fifo_creator

client:
	gcc src/client.c src/client_fifo.c src/error_checking.c -I./inc -o client -lpthread

fifo_creator:
	gcc src/fifo_creator.c src/error_checking.c -I./inc -o fifo_creator

warehouse_db:
	gcc src/server.c src/warehouse_db.c src/error_checking.c -I./inc -o warehouse_db

clean:
	rm client warehouse_db fifo_creator tmp/*
