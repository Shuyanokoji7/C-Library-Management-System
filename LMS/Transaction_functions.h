*

void logTransaction(const string &userID, const string &action, const string &ISBN)
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        string date = to_string(ltm->tm_mday) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(1900 + ltm->tm_year);
        string logEntry = date + " | " + userID + " | " + action + " | " + ISBN;

        transactions.push_back(logEntry);
        cout << "Transaction Logged: " << logEntry << endl;
    }

    // Save transactions to file (append mode)
    void saveTransactionToFile(const string &filename, const string &logEntry)
    {
        ofstream outFile(filename, ios::app); // Open in append mode
        if (!outFile)
        {
            cerr << "Error opening transactions file for writing.\n";
            return;
        }

        outFile << logEntry << endl;
        outFile.close();
        cout << "Transaction saved to file.\n";
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
            outFile << user->getUserID() << "|" << user->displayInfo() << "|"
                    << user->getRole() << "|";

            // Handle Students and Faculty who have an Account
            Account *acc = dynamic_cast<Account *>(user);
            if (acc)
            {
                outFile << acc->getFine() << "|";

                // Save borrowed book ISBNs
                vector<Book *> borrowed = acc->getBorrowedBook();
                for (Book *b : borrowed)
                {
                    outFile << b->getISBN() << ",";
                }
            }
            else
            {
                outFile << "0|"; // Default fine for Librarians (no Account object)
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

    vector<Book *> getBooks()
    {
        return books;
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
