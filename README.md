**Library System**
------------------

A simple library system written in C to manage books, customers and rented books. The system writes the information to text files.

**Features**
------------

*   **Customer Management**: Add new customers.
    
*   **Book Management**: Add new books.
    
*   **Rental Management**: Record and manage book rentals.
    
*   **Data Storage**: Information is stored in text files.
    

**File Structure**
------------------

*   ``Library System.c``: Main source code file for the library system.
    

**Data Files**
--------------

*   customers.txt: Stores customer information.
    
*   books.txt: Stores book information.
    
*   rented.txt: Stores rental information.
    

**Compilation**
---------------

To compile the program, use a C compiler like gcc:

```bash
gcc -o library\_system Library\\ System.c
```
or use CLion IDE by JetBrains

**Usage**
---------

Run the compiled executable:

```bash
./library\_system
```

**Functions**
-------------



**Customer Management**

*   `void createNewCustomer()`: Adds a new customer to the customers.txt file.
    

**Book Management**

*  `void addNewBook()`: Adds a new book to the books.txt file.
    

**Rental Management**

*   `void rentBook()`: Records a new rental in the rented.txt file.
    

**Data Structures**
-------------------

**struct Customer**

*  `int C_ID` : Customer ID.
    
*   `char name[25]`: Customer's first name.
    
*   `char surname[25]`: Customer's last name.
    
*   `int age`: Customer's age.
    
*   `int wallet`: Amount in customer's wallet.
    

**struct Books**

*   ``int B_ID``: Book ID.
    
*   ``char name[35]``: Book name.
    
*   ``char author[25]``: Author's name.
    
*   ``int age_limit``: Age limit for renting the book.
    
*   ``int price_per_week``: Price per week for renting the book.
    
*   ``int rented``: Rental status (0 for available, 1 for rented).
    

**struct Rented**

*   ``int R_ID``: Rental ID.
    
*   ``int C_ID``: Customer ID.
    
*   ``int B_ID``: Book ID.
    
*   ``char rented_date[12]``: Rental date.
    
*   ``int week``: Number of weeks rented.
    

**Notes**
---------

*   Ensure that the data files (customers.txt, books.txt, rented.txt) exist in the same directory as the executable. If the files do not exist, the program will attempt to create them.
    
*   The program uses standard input (stdin) for receiving user inputs. Make sure to provide valid data to avoid unexpected behaviour.
    

**Contributions**
-----------------
Contributions are welcome! Please fork this repository and submit pull requests for any features, bug fixes, or improvements.
