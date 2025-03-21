#include "User.h"
class Library
{
private:
    vector<Book *> books;
    vector<User *> users;
    vector<string> transactions;

public:
    Library()
    {
        loadBooksFromFile("books.txt");
        loadUsersFromFile("users.txt");
        loadTransactionsFromFile("transactions.txt");
    }

    ~Library()
    {
        saveBooksToFile("books.txt");
        saveUsersToFile("users.txt");

        ofstream outFile("transactions.txt", ios::out);
        if (!outFile)
        {
            cerr << "Error opening transactions file for writing.\n";
        }
        else
        {
            for (const string &log : transactions)
            {
                outFile << log << endl;
            }
            outFile.close();
        }

        for (Book *book : books)
            delete book;
        for (User *user : users)
            delete user;
    }
    string getUserRole(string myUserID)
    {
        for (User *user : users)
        {
            if (user->getUserID() == myUserID)
            {
                return user->getRole();
            }
        }
        return "INVALID";
    }
    void addUser(string myUserID, string role, string userID, string name)
    {
        // Check if the requester is a Librarian
        bool isLibrarian = false;
        for (User *user : users)
        {
            if (user->getUserID() == myUserID && user->getRole() == "Librarian")
            {
                isLibrarian = true;
                break;
            }
        }

        if (!isLibrarian)
        {
            cout << "User not allowed to perform the given operation" << endl;
            return;
        }

        // Create a new user based on role
        User *newUser = nullptr;
        Account *acc = new Account(); // Default account setup

        if (role == "Student")
            newUser = new Student(userID, name, acc);
        else if (role == "Faculty")
            newUser = new Faculty(userID, name, acc);
        else if (role == "Librarian")
            newUser = new Librarian(userID, name);

        if (newUser)
        {
            users.push_back(newUser);
            cout << "User added successfully: " << userID << " (" << role << ")" << endl;
            logTransaction(myUserID, "ADD_USER", userID);
        }
        else
        {
            cout << "Invalid role provided. User not added." << endl;
        }
    }

    void removeUser(string myUserID, string userID)
    {
        // Check if the requester is a Librarian
        bool isLibrarian = false;
        for (User *user : users)
        {
            if (user->getUserID() == myUserID && user->getRole() == "Librarian")
            {
                isLibrarian = true;
                break;
            }
        }

        if (!isLibrarian)
        {
            cout << "User not allowed to perform the given operation" << endl;
            return;
        }

        // Find and remove the user
        for (auto it = users.begin(); it != users.end(); ++it)
        {
            if ((*it)->getUserID() == userID)
            {
                delete *it; // Free memory
                users.erase(it);
                cout << "User removed successfully: " << userID << endl;
                logTransaction(myUserID, "REMOVE_USER", userID);
                return;
            }
        }

        cout << "User not found: " << userID << endl;
    }

    void addBook(string myUserID, string title,
                 string author,
                 string publisher,
                 unsigned short year,
                 string ISBN)
    {
        // Check if the user is a librarian
        for (User *user : users)
        {
            if (user->getUserID() == myUserID && user->getRole() == "Librarian")
            {
                // Add book to library collection
                books.push_back(new Book(title, author, publisher, year, ISBN));

                cout << "Book added successfully: " << title << endl;
                logTransaction(myUserID, "ADD_BOOK", ISBN);
                return;
            }
        }
        cout << "User not allowed to perform the given operation" << endl;
    }

    void removeBook(string myUserID, string ISBN)
    {
        // Check if the user is a librarian
        for (User *user : users)
        {
            if (user->getUserID() == myUserID && user->getRole() == "Librarian")
            {
                auto it = remove_if(books.begin(), books.end(), [&](Book *b)
                                    { return b->getISBN() == ISBN; });

                if (it != books.end())
                {
                    delete *it;
                    books.erase(it, books.end());
                    cout << "Book removed successfully: " << ISBN << endl;
                    logTransaction(myUserID, "REMOVE_BOOK", ISBN);
                }
                else
                {
                    cout << "Book not found." << endl;
                }
                return;
            }
        }
        cout << "User not allowed to perform the given operation" << endl;
    }

    void borrowBook(string userID, string ISBN)
    {
        Book *bookToBeBorrowed = nullptr; // Ensure initialization
        for (Book *book : books)
        {
            if (book->getISBN() == ISBN)
            {
                bookToBeBorrowed = book;
                break;
            }
        }

        if (bookToBeBorrowed == nullptr)
        {
            cout << "Book not found in the system." << endl;
            return;
        }

        for (User *user : users)
        {
            if (user->getUserID() == userID && user->getRole() != "Librarian")
            {
                string transaction = user->borrowBook(bookToBeBorrowed);

                if (transaction == "Transaction: Borrow")
                {
                    logTransaction(userID, "BORROW", ISBN);
                    cout << "Book borrowed successfully by " << userID << endl;
                }
                else if (transaction == "Transaction: Reserved")
                {
                    logTransaction(userID, "RESERVE", ISBN);
                    cout << "Book reserved successfully" << endl;
                }
                return;
            }
        }
        cout << "User not allowed to perform the given operation" << endl;
    }

    void returnBook(string userID, string ISBN)
    {
        Book *book = nullptr;

        for (auto it = books.begin(); it != books.end(); ++it)
        {
            if ((*it)->getISBN() == ISBN)
            {
                book = (*it);
                break;
            }
        }
        if (book == nullptr)
        {
            cerr << "Error: Book not found in borrowed list.\n";
            return;
        }
        for (User *user : users)
        {
            if (user->getUserID() == userID && user->getRole() != "Librarian")
            {
                if (book->getStatus() == BookStatus::Reserved)
                {
                    if (user->returnBook(ISBN))
                    {
                        logTransaction(userID, "RETURN", ISBN);
                        string reserved_borrower = book->getReserevedByUserID();
                        borrowBook(reserved_borrower, ISBN);
                        book->setReservedByUserID("INVALID");
                    }
                }
                else
                {
                    if (user->returnBook(ISBN))
                        logTransaction(userID, "RETURN", ISBN);
                }

                return;
            }
        }
        cout << "User not allowed to perform the given operation" << endl;
    }

    void logTransaction(const string &userID, const string &action, const string &ISBN)
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        string date = to_string(ltm->tm_mday) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(1900 + ltm->tm_year);
        string logEntry = date + " | " + userID + " | " + action + " | " + ISBN;

        transactions.push_back(logEntry);
    }

    // Save transactions to file (append mode)
    void saveTransactionToFile(const string &filename, const string &logEntry)
    {
        ofstream outFile(filename, ios::app); // Append mode
        if (!outFile)
        {
            cerr << "Error opening transactions file for writing.\n";
            return;
        }

        outFile << logEntry << endl;
        outFile.close();
    }

    // Load transactions from a file (does not clear existing ones)
    void loadTransactionsFromFile(const string &filename)
    {
        ifstream inFile(filename);
        if (!inFile)
        {
            cerr << "Error opening transactions file for reading.\n";
            return;
        }

        transactions.clear(); // Clear only in memory, not the file
        string line;
        while (getline(inFile, line))
        {
            transactions.push_back(line);
        }

        inFile.close();
        cout << "Transactions loaded successfully.\n";
    }

    // Display transactions for a specific user
    void displayUserTransactions(const string &userID)
    {
        cout << "Transactions for User ID: " << userID << "\n";
        cout << "-------------------------------\n";
        for (const string &log : transactions)
        {
            if (log.find(userID) != string::npos) // Match user's transactions
                cout << log << endl;
        }
    }

    void saveUsersToFile(const std::string &filename)
    {
        std::ofstream outFile(filename);
        if (!outFile)
        {
            std::cerr << "Error opening file for writing.\n";
            return;
        }

        for (User *user : users)
        {
            // Write common user data
            outFile << user->toFileString();

            // Check if user is a Student or Faculty
            Student *student = dynamic_cast<Student *>(user);
            Faculty *faculty = dynamic_cast<Faculty *>(user);

            if (student)
            {
                Account *acc = student->getAccount();
                acc->updateTotalFine(student->getMaxBorrowingPeriod(), student->getFineRate());
                outFile << acc->getFine() << "|"; // Write fine

                // Save borrowed book ISBNs
                vector<Book *> borrowed = acc->getBorrowedBook();
                for (Book *b : borrowed)
                {
                    outFile << b->getISBN() << ",";
                }
            }
            else if (faculty)
            {
                Account *acc = faculty->getAccount();
                outFile << acc->getFine() << "|"; // Write fine

                // Save borrowed book ISBNs
                vector<Book *> borrowed = acc->getBorrowedBook();
                for (Book *b : borrowed)
                {
                    outFile << b->getISBN() << ",";
                }
            }
            else
            {
                outFile << "0|"; // Default fine for Librarians (no Account)
            }

            outFile << "\n"; // End of user record
        }

        outFile.close();
    }

    // ----------------- LOAD USERS FROM FILE -----------------
    void loadUsersFromFile(const std::string &filename)
    {
        std::ifstream inFile(filename);
        if (!inFile)
        {
            std::cerr << "Error opening file for reading.\n";
            return;
        }

        users.clear(); // Clear existing users before loading

        string userID, name, role, fineStr, borrowedBooksStr;
        while (getline(inFile, userID, '|'))
        {
            getline(inFile, name, '|');
            getline(inFile, role, '|');
            getline(inFile, fineStr, '|');
            getline(inFile, borrowedBooksStr); // Remaining line has borrowed books

            Account *account = nullptr;
            int fine = 0;

            // Assign account only to Students and Faculty
            if (role == "Student" || role == "Faculty")
            {
                fine = stoi(fineStr);
                account = new Account();
                account->setFine(fine);

                // Re-add borrowed books based on ISBN
                stringstream ss(borrowedBooksStr);
                string ISBN;
                while (getline(ss, ISBN, ','))
                {
                    for (Book *b : books)
                    {
                        if (b->getISBN() == ISBN)
                        {
                            account->addBorrowedBook(b);
                            break;
                        }
                    }
                }
            }

            // Dynamically create a new user based on role
            User *user = nullptr;
            if (role == "Student")
                user = new Student(userID, name, account);
            else if (role == "Faculty")
                user = new Faculty(userID, name, account);
            else
                user = new Librarian(userID, name); // Librarians do not need an account

            users.push_back(user);
        }

        inFile.close();
    }

    void saveBooksToFile(const std::string &filename)
    {
        std::ofstream outFile(filename);
        if (!outFile)
        {
            std::cerr << "Error opening file for writing.\n";
            return;
        }

        for (Book *book : books)
        {
            outFile << book->toFileString() << "\n"; // Ensure each book is on a new line
        }

        outFile.close();
    }

    void loadBooksFromFile(const std::string &filename)
    {
        std::ifstream inFile(filename);
        if (!inFile)
        {
            std::cerr << "Error opening file for reading.\n";
            return;
        }

        books.clear(); // Clear existing books

        std::string line;
        while (std::getline(inFile, line))
        {
            if (!line.empty())
            {
                Book *newBook = Book::fromFileString(line);
                books.push_back(newBook);
            }
        }

        inFile.close();
    }

    void simulateFinePayment(string userID)
    {
        for (User *user : users)
        {
            if (user->getUserID() == userID && user->getRole() == "Student")
            {
                vector<string> paymentDetails = user->finePayment();
                int n = paymentDetails.size();
                if (n != 1)
                {
                    for (int i = 0; i < n - 1; i++)
                    {
                        logTransaction(userID, "RETURN", paymentDetails[i]);
                    }
                    logTransaction(userID, "FINE_PAYMENT", paymentDetails[n - 1]);
                }
                return;
            }
        }
        cout << "User not allowed to perform the given operation" << endl;
    }
};