# C++ DataBase Simulator

##The main idea of the project is to simulate a database using SQL commands through c++ language as follows :

- commands for data defining ( `CREATE` , `DROP` , `DISPLAY` ) and data modelling ( `SELECT`, `INSERT`, `DELETE` , `IMPORT` ) can be used in the console application or Command Prompt
- the structure of a table is saved in a particular `txt` file while the table datas are saved in a particular `bin' file. Only when importing a table is used a CSV file
- the `Attributes` folder contains the structures of the tables while the `Registers` folder contains the datas from the table
-in order to exit your database, type "exit"

##Knowledges used to create a Database management system :

- `boost` and `STL` libraries in order to create string vectors for command words and eventually values ( I used `trim()` function for eliminating the spaces)
- working with `txt` , `bin` and `CSV` files (reading from them and writing into them)
- OOP concepts such as `object`, `class`, `aggregation` , `polymorphism`, `abstraction`
