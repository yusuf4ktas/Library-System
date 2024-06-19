#include <stdio.h>
#include <string.h>

struct Customer {
    int C_ID;
    char name[25];
    char surname[25];
    int age;
    int wallet;
};
struct Books {
    int B_ID;
    char name[35];
    char author[25];
    int age_limit;
    int price_per_week;
    int rented;
};
struct Rented {
    int R_ID;
    int C_ID;
    int B_ID;
    char rented_date[12];
    int week;
};

FILE *custPtr,*bookPtr,*rentPtr;

void createNewCustomer() {
    struct Customer customer;
    int id = 0;
    char bufferName[25], bufferSurname[25];

    custPtr = fopen("customers.txt", "a+");
    if (custPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter customer name:\n ");
    fgets(bufferName, sizeof(bufferName), stdin);
    bufferName[strcspn(bufferName, "\n")] = 0; // Remove newline character

    printf("Enter customer surname:\n ");
    fgets(bufferSurname, sizeof(bufferSurname), stdin);
    bufferSurname[strcspn(bufferSurname, "\n")] = 0; // Remove newline character

    // Reading the existing customers to find the highest ID and check for duplicates
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet)!= EOF) {
        if (customer.C_ID > id) {
            id = customer.C_ID; // Update the highest ID found
        }
        // Checking for duplicates using the buffers
        if (strcmp(bufferName, customer.name) == 0 && strcmp(bufferSurname, customer.surname) == 0) {
            printf("Customer with the same name and surname already exists!\n");
            fclose(custPtr);
            return; // Terminate the process if a duplicate is found
        }
    }
    id++; // Incrementing ID for the new customer

    fclose(custPtr);
    // To avoid asking for name and surname again, transferring the inputs stored in buffers
    strcpy(customer.name, bufferName);
    strcpy(customer.surname, bufferSurname);
    printf("Enter customer age:\n ");
    scanf("%d", &customer.age);
    customer.wallet = 0; // Initialize wallet to zero for new customers

    // Appending new customer to customers.txt file
    custPtr = fopen("customers.txt", "a");
    if (custPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(custPtr, "%d %s %s %d %d\n", id, customer.name, customer.surname,
            customer.age, customer.wallet);
    fclose(custPtr);

    printf("New customer added successfully!\n");
}

void depositMoney() {
    int id;
    int amount;

    struct Customer customer;
    custPtr = fopen("customers.txt", "r");
    if (custPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Creating a temporary file to hold the updated customer data
    FILE *tempPtr = fopen("temp.txt", "w");
    if (tempPtr == NULL) {
        printf("Error opening temporary file!\n");
        fclose(custPtr);
        return;
    }

    int found = 0; // Flag to indicate if the customer has been found

    printf("Enter the customer ID to deposit money: ");
    scanf("%d",&id);
    printf("Enter the money amount to deposit: ");
    scanf("%d",&amount);

    // Reading through the file to find the customer with the specified ID
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet) != EOF) {
        if (customer.C_ID == id) {
            found = 1; // Customer found
            customer.wallet += amount; // Update the wallet amount
            fprintf(tempPtr, "%d %s %s %d %d\n", customer.C_ID, customer.name, customer.surname,
                    customer.age, customer.wallet);
        } else {
            // Copying all other customers to the temporary file
            fprintf(tempPtr, "%d %s %s %d %d\n", customer.C_ID, customer.name, customer.surname,
                    customer.age, customer.wallet);
        }
    }

    if (!found) {
        printf("Customer with ID %d not found! \n", id);
    } else {
        printf("Money deposited successfully! \n");
    }

    fclose(custPtr);
    fclose(tempPtr);

    // Replace the original file with the updated temporary file
    remove("customers.txt");
    rename("temp.txt", "customers.txt");
}

void addNewBook(){
    struct Books book;
    int id = 0;
    char bufferName[35],bufferAuthor[25];

    bookPtr = fopen("books.txt", "a+");
    if (bookPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter the book name:\n ");
    fgets(bufferName, sizeof(bufferName), stdin);
    bufferName[strcspn(bufferName, "\n")] = 0; // Remove newline character

    printf("Enter the author:\n ");
    fgets(bufferAuthor, sizeof(bufferAuthor), stdin);
    bufferAuthor[strcspn(bufferAuthor, "\n")] = 0; // Remove newline character

    // Read existing books to find the highest ID
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author,
                  &book.age_limit, &book.price_per_week,&book.rented) != EOF) {
        if (book.B_ID > id) {
            id = book.B_ID; // Update the highest ID found
        }
        // Checking for duplicates
        if (strcmp(bufferName, book.name) == 0 && strcmp(bufferAuthor, book.author) == 0) {
            printf("Book with the same name and author already exists!\n");
            fclose(bookPtr);
            return; // Terminating the process if a duplicate is found
        }
    }
    id++; // Incrementing the ID for the new book

    fclose(bookPtr);


    strcpy(book.name, bufferName);
    strcpy(book.author, bufferAuthor);

    printf("Enter the age limit for the book:\n ");
    scanf("%d", &book.age_limit);

    book.rented = 0; // Initializing the rented status to zero(false)

    printf("Enter the price per week");
    scanf("%d",&book.price_per_week);

    // Appending the new book to books.txt file
    bookPtr = fopen("books.txt", "a");
    if (bookPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(bookPtr, "%d %s %s %d %d %d\n", id, book.name, book.author,
            book.age_limit, book.price_per_week,book.rented);
    fclose(bookPtr);

    printf("New book added successfully!\n");
}

void rentBook() {

    int customerID, bookID, rentalWeeks;

    FILE *tempCustPtr, *tempBookPtr;

    struct Customer customer;
    struct Books book;
    struct Rented rent;
    int customerFound = 0, bookFound = 0;
    int id;

    printf("Enter customer ID:\n ");
    scanf("%d", &customerID);

    printf("Enter book ID:\n ");
    scanf("%d", &bookID);

    printf("Enter the date (DD/MM/YYYY):\n");
    scanf("%s",rent.rented_date);

    printf("Enter rental weeks:\n ");
    scanf("%d", &rentalWeeks);


    // Find the customer
    custPtr = fopen("customers.txt", "r");
    if (custPtr == NULL) {
        printf("Error opening customers.txt!\n");
        return;
    }
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet) != EOF) {
        if (customer.C_ID == customerID) {
            customerFound = 1;
            break;
        }
    }
    fclose(custPtr);

    if (!customerFound) {
        printf("Customer not found!\n");
        return;
    }

    // Find the book
    bookPtr = fopen("books.txt", "r");
    if (bookPtr == NULL) {
        printf("Error opening books.txt!\n");
        return;
    }
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID == bookID) {
            bookFound = 1;
            break;
        }
    }
    fclose(bookPtr);

    if (!bookFound) {
        printf("Book not found!\n");
        return;
    }

    // Checking the conditions
    if (book.rented) {
        printf("Book is already rented!\n");
        return;
    }
    if (customer.age < book.age_limit) {
        printf("Customer is under the age limit for this book!\n");
        return;
    }
    if (customer.wallet < book.price_per_week * rentalWeeks) {
        printf("Insufficient balance to rent the book!\n");
        return;
    }

    customer.wallet -= book.price_per_week * rentalWeeks;

    // Updating the books.txt file and writing the other book information to temporary file
    bookPtr = fopen("books.txt", "r");
    tempBookPtr = fopen("temp_books.txt", "w");
    if (bookPtr == NULL || tempBookPtr == NULL) {
        printf("Error opening files for updating books!\n");
        if (bookPtr) fclose(bookPtr);
        if (tempBookPtr) fclose(tempBookPtr);
        return;
    }
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID == bookID) {
            fprintf(tempBookPtr, "%d %s %s %d %d 1\n", book.B_ID, book.name, book.author, book.age_limit,
                    book.price_per_week, book.rented);
        } else {
            fprintf(tempBookPtr, "%d %s %s %d %d %d\n", book.B_ID, book.name, book.author, book.age_limit,
                    book.price_per_week, book.rented);
        }
    }
    fclose(bookPtr);
    fclose(tempBookPtr);
    remove("books.txt");
    rename("temp_books.txt", "books.txt");

    // Updating the customers.txt file
    custPtr = fopen("customers.txt", "r");
    tempCustPtr = fopen("temp_customers.txt", "w");
    if (custPtr == NULL || tempCustPtr == NULL) {
        printf("Error opening files for updating customers!\n");
        if (custPtr) fclose(custPtr);
        if (tempCustPtr) fclose(tempCustPtr);
        return;
    }
    // Reading through the file to find the customer with the specified ID
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet) != EOF) {
        if (customer.C_ID == customerID) {
            fprintf(tempCustPtr, "%d %s %s %d %d\n", customer.C_ID, customer.name, customer.surname,
                    customer.age, customer.wallet);
        }
            //Printing all other customers to temporary file
        else {
            fprintf(tempCustPtr, "%d %s %s %d %d\n", customer.C_ID, customer.name, customer.surname,
                    customer.age, customer.wallet);
        }
    }
    fclose(custPtr);
    fclose(tempCustPtr);
    remove("customers.txt");
    rename("temp_customers.txt", "customers.txt");

    rent.C_ID = customerID;
    rent.B_ID = bookID;
    rent.week = rentalWeeks;

    rentPtr = fopen("rented.txt", "a+");
    if (rentPtr == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Read existing rental records to find the highest ID
    while (fscanf(rentPtr, "%d %d %d %s %d", &rent.R_ID, &rent.C_ID, &rent.B_ID,
                  rent.rented_date, &rent.week) != EOF) {
        if (rent.R_ID > id) {
            id = rent.R_ID; // Update the highest ID found
        }
    }
    id++;
    // Recording the rental information
    rentPtr = fopen("rented.txt", "a");
    if (rentPtr == NULL) {
        printf("Error opening rented.txt for writing!\n");
        return;
    }
    fprintf(rentPtr, "%d %d %d %s %d\n", rent.R_ID, rent.C_ID, rent.B_ID, rent.rented_date, rent.week );
    fclose(rentPtr);

    printf("Book rented successfully!\n");
}

void deliverBook() {

    struct Customer customer;
    struct Books book;
    struct Rented rent;
    FILE *tempCustPtr, *tempBookPtr, *tempRentedPtr;
    int customerFound = 0, bookFound = 0, rentalFound = 0;

    int rentID,customerID, bookID, rentalWeeks, refundAmount,remainingWeeks;
    printf("Enter rental ID :\n ");
    scanf("%d", &rent.R_ID);
    printf("Enter remaining weeks:\n ");
    scanf("%d", &remainingWeeks);


    // Find the rental record
    rentPtr = fopen("rented.txt", "r");
    if (rentPtr == NULL) {
        printf("Error opening rented.txt!\n");
        return;
    }
    while (fscanf(rentPtr, " %d %d %d %d", &rentID, &customerID, &bookID, &rentalWeeks) != EOF) {
        if (rent.R_ID == rentID) {
            rentalFound = 1;
            break;
        }
    }
    fclose(rentPtr);

    if (!rentalFound) {
        printf("Rental record not found!\n");
        return;
    }

    // Checking if the customer exists
    custPtr = fopen("customers.txt", "r");
    if (custPtr == NULL) {
        printf("Error opening customers.txt!\n");
        return;
    }
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet) != EOF) {
        if (customer.C_ID == customerID) {
            customerFound = 1;
            break;
        }
    }
    fclose(custPtr);

    if (!customerFound) {
        printf("Customer not found!\n");
        return;
    }

    // Checking if the book exists
    bookPtr = fopen("books.txt", "r");
    if (bookPtr == NULL) {
        printf("Error opening books.txt!\n");
        return;
    }
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID == bookID) {
            bookFound = 1;
            break;
        }
    }
    fclose(bookPtr);

    if (!bookFound) {
        printf("Book not found!\n");
        return;
    }

    // Calculating the refund amount
    refundAmount = book.price_per_week * remainingWeeks;

    // Updating customer's wallet balance
    custPtr = fopen("customers.txt", "r");
    tempCustPtr = fopen("temp_customers.txt", "w");
    if (custPtr == NULL || tempCustPtr == NULL) {
        printf("Error opening files for updating customers!\n");
        if (custPtr) fclose(custPtr);
        if (tempCustPtr) fclose(tempCustPtr);
        return;
    }
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet) != EOF) {
        if (customer.C_ID == customerID) {
            customer.wallet += refundAmount; // Updating the wallet balance
            fprintf(tempCustPtr, "%d %s %s %d %d\n", customer.C_ID, customer.name, customer.surname,
                    customer.age, customer.wallet);
        } else {
            fprintf(tempCustPtr, "%d %s %s %d %d\n", customer.C_ID, customer.name, customer.surname,
                    customer.age, customer.wallet);
        }
    }
    fclose(custPtr);
    fclose(tempCustPtr);
    remove("customers.txt");
    rename("temp_customers.txt", "customers.txt");

    // Updating the book's rental status
    bookPtr = fopen("books.txt", "r");
    tempBookPtr = fopen("temp_books.txt", "w");
    if (bookPtr == NULL || tempBookPtr == NULL) {
        printf("Error opening files for updating books!\n");
        if (bookPtr) fclose(bookPtr);
        if (tempBookPtr) fclose(tempBookPtr);
        return;
    }
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID == bookID) {
            book.rented = 0; // Updating the rental status
            fprintf(tempBookPtr, "%d %s %s %d %d 0\n", book.B_ID, book.name, book.author, book.age_limit,
                    book.price_per_week, book.rented);
        } else {
            fprintf(tempBookPtr, "%d %s %s %d %d %d\n", book.B_ID, book.name, book.author, book.age_limit,
                    book.price_per_week, book.rented);
        }
    }
    fclose(bookPtr);
    fclose(tempBookPtr);
    remove("books.txt");
    rename("temp_books.txt", "books.txt");

    // Remove the rental record from rented.txt
    rentPtr = fopen("rented.txt", "r");
    tempRentedPtr = fopen("temp_rented.txt", "w");
    if (rentPtr == NULL || tempRentedPtr == NULL) {
        printf("Error opening files for updating rented records!\n");
        if (rentPtr) fclose(rentPtr);
        if (tempRentedPtr) fclose(tempRentedPtr);
        return;
    }
    while (fscanf(rentPtr, "%d %d %d %d", &rentID, &customerID, &bookID, &rentalWeeks) != EOF) {
        if (rentID != rent.R_ID) {
            fprintf(tempRentedPtr, "%d %d %d %d\n", rentID, customerID, bookID, rentalWeeks);
        }
    }
    fclose(rentPtr);
    fclose(tempRentedPtr);
    remove("rented.txt");
    rename("temp_rented.txt", "rented.txt");

    printf("Book delivered successfully! Refund amount: %d\n", refundAmount);
}

void burnBook(){

    int B_ID;
    printf("Enter book ID :\n ");
    scanf("%d", &B_ID);

    struct Books book;
    FILE *tempBookPtr;
    int bookFound = 0, bookRented;

    // Open books.txt and search for the book
    bookPtr = fopen("books.txt", "r");
    if (bookPtr == NULL) {
        printf("Error opening books.txt!\n");
        return;
    }
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID == B_ID) {
            bookFound = 1;
            bookRented = book.rented;
            break;
        }
    }
    fclose(bookPtr);

    if (!bookFound) {
        printf("Book not found!\n");
        return;
    }

    if (bookRented) {
        printf("Book cannot be burned because it's currently rented.\n");
        return;
    }

    // Creating a temporary file and copy all books except the one to be deleted
    bookPtr = fopen("books.txt", "r");
    tempBookPtr = fopen("temp_books.txt", "w");
    if (bookPtr == NULL || tempBookPtr == NULL) {
        printf("Error opening files for updating books!\n");
        if (bookPtr) fclose(bookPtr);
        if (tempBookPtr) fclose(tempBookPtr);
        return;
    }
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID != B_ID) {
            fprintf(tempBookPtr, "%d %s %s %d %d %d\n", book.B_ID, book.name, book.author, book.age_limit,
                    book.price_per_week, book.rented);
        }
    }
    fclose(bookPtr);
    fclose(tempBookPtr);
    remove("books.txt");
    rename("temp_books.txt", "books.txt");

    printf("Book burned successfully!\n");
}

void updateCustomerInfo(){
    int C_ID, newAge;
    char newName[25], newSurname[25];

    printf("Enter customer ID (C_ID):\n ");
    scanf("%d", &C_ID);


    struct Customer customer;
    FILE *tempCustPtr;
    int customerFound = 0;

    // Open customers.txt and search for the customer
    custPtr = fopen("customers.txt", "r");
    if (custPtr == NULL) {
        printf("Error opening customers.txt!\n");
        return;
    }
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet) != EOF) {
        if (customer.C_ID == C_ID) {
            customerFound = 1;
            break;
        }
    }
    fclose(custPtr);

    if (!customerFound) {
        printf("Customer not found!\n");
        return;
    }
    printf("Enter the new name:\n ");
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = 0; // Remove newline character

    printf("Enter the new surname:\n ");
    fgets(newSurname, sizeof(newSurname), stdin);
    newSurname[strcspn(newSurname, "\n")] = 0; // Remove newline character

    printf("Enter the new age:\n ");
    scanf("%d", &newAge);

    // Creating a temporary file and copy all customers except the one to be updated
    custPtr = fopen("customers.txt", "r");
    tempCustPtr = fopen("temp_customers.txt", "w");
    if (custPtr == NULL || tempCustPtr == NULL) {
        printf("Error opening files for updating customers!\n");
        if (custPtr) fclose(custPtr);
        if (tempCustPtr) fclose(tempCustPtr);
        return;
    }
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet) != EOF) {
        if (customer.C_ID == C_ID) {
            // Updating the customer information
            fprintf(tempCustPtr, "%d %s %s %d %d\n", C_ID, newName, newSurname, newAge, customer.wallet);
        } else {
            fprintf(tempCustPtr, "%d %s %s %d %d\n", customer.C_ID, customer.name, customer.surname,
                    customer.age, customer.wallet);
        }
    }
    fclose(custPtr);
    fclose(tempCustPtr);
    remove("customers.txt");
    rename("temp_customers.txt", "customers.txt");

    printf("Customer information updated successfully!\n");
}

void updateBookInfo(){
    int B_ID, newAgeLimit, newPricePerWeek;
    char newName[35], newAuthor[25];

    printf("Enter book ID (B_ID):\n ");
    scanf("%d", &B_ID);

    struct Books book;
    FILE *tempBookPtr;
    int bookFound = 0;

    // Open books.txt and search for the book
    bookPtr = fopen("books.txt", "r");
    if (bookPtr == NULL) {
        printf("Error opening books.txt!\n");
        return;
    }
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID == B_ID) {
            bookFound = 1;
            break;
        }
    }
    fclose(bookPtr);

    if (!bookFound) {
        printf("Book not found!\n");
        return;
    }
    printf("Enter the new book name:\n ");
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = 0; // Remove newline character

    printf("Enter the new author:\n ");
    fgets(newAuthor, sizeof(newAuthor), stdin);
    newAuthor[strcspn(newAuthor, "\n")] = 0; // Remove newline character

    printf("Enter the new age limit:\n ");
    scanf("%d", &newAgeLimit);

    printf("Enter the new price per week:\n ");
    scanf("%d", &newPricePerWeek);

    // Creating a temporary file and copy all books except the one to be updated
    bookPtr = fopen("books.txt", "r");
    tempBookPtr = fopen("temp_books.txt", "w");
    if (bookPtr == NULL || tempBookPtr == NULL) {
        printf("Error opening files for updating books!\n");
        if (bookPtr) fclose(bookPtr);
        if (tempBookPtr) fclose(tempBookPtr);
        return;
    }
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        if (book.B_ID == B_ID) {
            // Updating the book information
            fprintf(tempBookPtr, "%d %s %s %d %d %d\n", B_ID, newName, newAuthor, newAgeLimit, newPricePerWeek, book.rented);
        } else {
            fprintf(tempBookPtr, "%d %s %s %d %d %d\n", book.B_ID, book.name, book.author, book.age_limit,
                    book.price_per_week, book.rented);
        }
    }
    fclose(bookPtr);
    fclose(tempBookPtr);
    remove("books.txt");
    rename("temp_books.txt", "books.txt");

    printf("Book information updated successfully!\n");
}

void listCustomers() {
    struct Customer customer;

    // Open customers.txt for reading
    custPtr = fopen("customers.txt", "r");
    if (custPtr == NULL) {
        printf("Error opening customers.txt!\n");
        return;
    }

    // Header for the customer list
    printf("C_ID | Name | Surname | Age | Wallet\n");
    printf("-----------------------------------\n");

    // Read and print each customer's information
    while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                  &customer.age, &customer.wallet) != EOF) {
        printf("%d | %s | %s | %d | %d\n", customer.C_ID, customer.name, customer.surname,
               customer.age, customer.wallet);
    }
    // Closing the file
    fclose(custPtr);

    printf("\n");
}

void listCustomersWhoRentedBook() {
    struct Customer customer;
    struct Rented rent;
    int customerFound;

    // Open rented.txt for reading
    rentPtr = fopen("rented.txt", "r");
    if (rentPtr == NULL) {
        printf("Error opening rented.txt!\n");
        return;
    }

    // Header for the customer list
    printf("C_ID | Name | Surname | Age | Wallet\n");
    printf("-----------------------------------\n");

    // Read each rental record
    while (fscanf(rentPtr, "%d %d %d %s %d", &rent.R_ID, &rent.C_ID, &rent.B_ID, rent.rented_date, &rent.week) != EOF) {
        // Open customers.txt for reading to find the customer details
        custPtr = fopen("customers.txt", "r");
        if (custPtr == NULL) {
            printf("Error opening customers.txt!\n");
            fclose(rentPtr);
            return;
        }

        customerFound = 0;
        // Search for the customer in customers.txt
        while (fscanf(custPtr, "%d %s %s %d %d", &customer.C_ID, customer.name, customer.surname,
                      &customer.age, &customer.wallet) != EOF) {
            if (customer.C_ID == rent.C_ID) {
                customerFound = 1;
                break;
            }
        }

        if (customerFound) {
            // Print the customer's details
            printf("%d | %s | %s | %d | %d\n", customer.C_ID, customer.name, customer.surname,
                   customer.age, customer.wallet);
        }

        // Close customers.txt
        fclose(custPtr);
    }

    // Close rented.txt
    fclose(rentPtr);

    printf("\n");
}
void listBooks() {
    struct Books book;

    // Open books.txt for reading
    bookPtr = fopen("books.txt", "r");
    if (bookPtr == NULL) {
        printf("Error opening books.txt!\n");
        return;
    }

    // Print header for the book list
    printf("B_ID | Name | Author | Age Limit | Price per Week | Rented\n");
    printf("----------------------------------------------------------\n");

    // Reading and printing each book's information
    while (fscanf(bookPtr, "%d %s %s %d %d %d", &book.B_ID, book.name, book.author, &book.age_limit,
                  &book.price_per_week, &book.rented) != EOF) {
        printf("%d | %s | %s | %d | %d | %s\n", book.B_ID, book.name, book.author, book.age_limit,
               book.price_per_week, book.rented ? "Yes" : "No");
    }

    // Close the file
    fclose(bookPtr);

    printf("\n");
}

void displayMenu() {
    printf("\n--- Library Management System Menu ---\n");
    printf("1. Create New Customer\n");
    printf("2. Deposit Money to the Customer\n");
    printf("3. Add New Book\n");
    printf("4. Rent A Book\n");
    printf("5. Delivery A Book\n");
    printf("6. Burn Book\n");
    printf("7. Update Customer Information\n");
    printf("8. Update Book Information\n");
    printf("9. List of Customers Who Rent a Book\n");
    printf("10. List of Customers\n");
    printf("11. List of Books\n");
    printf("0. Exit\n");

}

int main(){

    int operation;
    do{
        displayMenu();
        printf("Enter the operation you want");
        scanf("%d",&operation);

        switch (operation) {

            case 1:
            {
                createNewCustomer();
                break;
            }
            case 2:
            {
                depositMoney();
                break;
            }
            case 3:
            {
                addNewBook();
                break;
            }
            case 4:
            {
                rentBook();
                break;
            }
            case 5:
            {
                deliverBook();
                break;
            }
            case 6:
            {
                burnBook();
                break;
            }
            case 7:
            {
                updateCustomerInfo();
                break;
            }
            case 8:
            {
                updateBookInfo();
                break;
            }
            case 9:
            {
                listCustomersWhoRentedBook();
                break;
            }
            case 10:
            {
                listCustomers();
                break;
            }
            case 11:
            {
                listBooks();
                break;
            }
            case 0:
            {
                printf("Exitting from the library system...");
                break;
            }
            default:
            {
                printf("Invalid operation, try again");
            }
        }
    }while(operation != 0);

    return 0;
}
