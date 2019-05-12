---------------------------How to Compile/Run---------------------------------

To initialize the Client/Server System, call "make fifo\_creator" and run "./fifo\_creator" which creates the pipes through which the clients and server will communicate.

To compile the Client (Part 2.2.1), call "make client" which will create an out file named "client"  
To compile the remote Warehouse Database (Part 2.2.2), call "make warehouse\_db" which will create an out file named "warehouse\_db"  

To compile everything (including fifo\_creator), no argument is needed (just call "make"), which will create both "client" and "warehouse\_db" out files  

Calling "make clean" deletes all executables and FIFO files.

----------------------------Design Decisions-------------------------------

The design doc says, "Initially, \[infotab] should print the first level tables and prompt user to choose one of these tables. Then the user should be able to navigate from that first level table to the second level table." Thus my infotab prints "the first level tables and prompt user to choose one of these tables. Then the user should be able to navigate from that first level table to the second level table."

