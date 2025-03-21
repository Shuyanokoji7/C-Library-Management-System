#include "Account.h"

class User
{
protected:
    string userID;
    string name;

public:
    User(string userID, string name)
    {
        this->userID = userID;
        this->name = name;
    }
    virtual ~User() {};

    virtual string borrowBook(Book *book) = 0;
    virtual bool returnBook(string ISBN) = 0;

    virtual int getFineRate() { return 0; }

    virtual string toFileString() = 0;

    virtual vector<string> finePayment() = 0;
    virtual string getRole() = 0;
    string getUserID() { return userID; }
};

class Student : public User
{
private:
    int fineRate = 10;
    unsigned int maxBooks = 3;
    int maxBorrowingPeriod = 15;
    Account *userAccount;

public:
    Student(string ID, string n, Account *newAccount) : User(ID, n)
    {
        userAccount = newAccount;
        userAccount->updateTotalFine(maxBorrowingPeriod, fineRate);
    }
    Account *getAccount()
    {
        return userAccount;
    }
    int getMaxBorrowingPeriod() { return maxBorrowingPeriod; }
    int getFineRate() { return fineRate; }
    string getRole() override { return "Student"; }

    string toFileString()
    {
        return userID + "|" + name + "|" + getRole() + "|";
    }
    vector<string> finePayment()
    {
        if (userAccount->getFine() == 0)
        {
            cout << "No outstanding fines.\n";
            return {"0"};
        }
        int amount = userAccount->getFine();
        cout << "Your outstanding fine is: Rs " << userAccount->getFine() << endl;

        vector<Book *> dueBooks;
        for (Book *book : userAccount->getBorrowedBook())
        {
            int daysBookHeld = inBetweenDays(book->getBorrowDate(), getCurrentDate());
            if (daysBookHeld > 15)
            {
                dueBooks.push_back(book);
            }
        }

        cout << "You need to return following overdue books first before proceeding to payment: " << endl;
        for (Book *book : dueBooks)
        {
            cout << "ISBN: " << book->getISBN() << ", ";
        }
        cout << "\nWould you like to return them and pay your bill now?: (yes/no)" << endl;
        string c;
        cin >> c;
        if (c == "y" || c == "yes")
        {
            vector<string> overdueBooks;
            for (Book *book : dueBooks)
            {
                string ISBN = book->getISBN();
                userAccount->removeBorrowedBook(ISBN);
                overdueBooks.push_back(ISBN);
                book->setBorrowDate("INVALID");
                book->setStatus(BookStatus::Available);
            }
            userAccount->setFine(0);
            cout << "Fine paid successfully! Borrowing restrictions lifted." << endl;
            overdueBooks.push_back(to_string(amount));
            return overdueBooks;
        }
        else
        {
            cout << "Fine payment cancelled" << endl;
            return {"0"};
        }
    }

    string borrowBook(Book *book) override
    {
        vector<Book *> borrowedBook = userAccount->getBorrowedBook();
        if (!userAccount->getEligibility())
        {
            cout << "You have a book due over 15 days.\nKindly return it to further borrow any book" << endl;
            return "Transaction: Failed";
        }
        else if (book->getStatus() == BookStatus::Borrowed)
        {
            cout << "This book is already borrowed!" << endl;
            cout << "Would you like to reserve it for future? (yes/no)" << endl;
            string choice;
            cin >> choice;

            if (choice == "yes" || choice == "y")
            {
                book->setStatus(BookStatus::Reserved);
                book->setReservedByUserID(userID); // Ensure function exists
                return "Transaction: Reserved";
            }
            else
            {
                return "Transaction: Failed";
            }
        }
        else if (book->getStatus() == BookStatus::Reserved)
        {
            cout << "This book is already borrowed and reserved!" << endl;
            return "Transaction: Failed"; // Fixed typo
        }
        else if (borrowedBook.size() >= maxBooks)
        {
            cout << "Max borrowing limit reached" << endl;
            return "Transaction: Failed";
        }
        
        else
        {
            string str = getCurrentDate();
            book->setBorrowDate(str);
            book->setStatus(BookStatus::Borrowed);
            userAccount->addBorrowedBook(book);
            return "Transaction: Borrow";
        }
    }

    bool returnBook(string ISBN) override
    {
        vector<Book *> borrowedBook = userAccount->getBorrowedBook();
        Book *book = nullptr;

        for (auto it = borrowedBook.begin(); it != borrowedBook.end(); ++it)
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
            return false;
        }

        int daysBookHeld = inBetweenDays(book->getBorrowDate(), getCurrentDate());
        int fine = 0;

        cout << "Book Returned Successfully" << endl;
        if (daysBookHeld > maxBorrowingPeriod)
        {
            fine = (daysBookHeld - maxBorrowingPeriod) * fineRate;
            cout << "Book overheld for " << daysBookHeld - maxBorrowingPeriod << " days" << endl;
        }

        userAccount->updateTotalFine(maxBorrowingPeriod, fineRate); // Updates the total outstanding fine

        cout << "Fine for this book: Rs " << fine << endl;
        cout << "Total Outstanding Fine: Rs " << userAccount->getFine() << endl;

        userAccount->removeBorrowedBook(ISBN);
        book->setBorrowDate("INVALID");
        book->setStatus(BookStatus::Available);
        return true;
    }
};
class Faculty : public User
{
private:
    unsigned maxBooks = 5;
    int maxBorrowingPeriod = 30;
    Account *userAccount;

public:
    Faculty(string ID, string n, Account *newAccount) : User(ID, n)
    {
        userAccount = newAccount;
    }
    string getRole() override { return "Faculty"; }
    string toFileString()
    {
        return userID + "|" + name + "|" + getRole() + "|";
    }
    Account *getAccount()
    {
        return userAccount;
    }

    vector<string> finePayment() { return {"0"}; }
    string borrowBook(Book *book) override
    {
        vector<Book *> borrowedBook = userAccount->getBorrowedBook();
        if (!userAccount->getEligibility())
        {
            cout << "You have a book due over 60 days\nKindly return it to futher borrow any book" << endl;
            return "Transation: Failed";
        }
        else if (book->getStatus() == BookStatus::Borrowed)
        {

            cout << "This book is already borrowed!" << endl;
            cout << "Would you like to reserve it for future? (yes/no)" << endl;
            string choice;
            cin >> choice;

            if (choice == "yes" || choice == "y")
            {
                book->setStatus(BookStatus::Reserved);
                book->setReservedByUserID(userID);
            }
            return "Transation: Reserved";
        }
        else if (book->getStatus() == BookStatus::Reserved)
        {
            cout << "This book is already borrowed and reserved!" << endl;
            return "Transation: Failed";
        }
        else if (borrowedBook.size() == maxBooks)
        {
            cout << "Max borrowing limit reached" << endl;
            return "Transation: Failed";
        }
        else
        {
            string str = getCurrentDate();
            book->setBorrowDate(str);
            book->setStatus(BookStatus::Borrowed);
            userAccount->addBorrowedBook(book);
            return "Transaction: Borrow";
        }
    }
    bool returnBook(string ISBN) override
    {
        vector<Book *> borrowedBook = userAccount->getBorrowedBook();
        Book *book = nullptr;
        for (auto it = borrowedBook.begin(); it != borrowedBook.end(); it = it + 1)
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
            return false;
        }
        userAccount->removeBorrowedBook(ISBN);
        book->setStatus(BookStatus::Available);
        book->setBorrowDate("INVALID");

        int daysBookHeld = inBetweenDays(book->getBorrowDate(), getCurrentDate());
        cout << "Book Returned Successfully" << endl;
        if (daysBookHeld > maxBorrowingPeriod)
        {
            cout << "Book overheld for " << daysBookHeld - maxBorrowingPeriod << " days" << endl;
        }
        return true;
    }
};
class Librarian : public User
{

public:
    Librarian(string ID, string n) : User(ID, n) {}
    string getRole() override { return "Librarian"; }
    string borrowBook(Book *book) override
    {
        (void)book;
        cout << "Librarian can't borrow books" << endl;
        return "Transation: Failed";
    }
    bool returnBook(string ISBN) override
    {
        (void)ISBN;
        cout << "Librarian can't return books" << endl;
        return false;
    }
    string toFileString()
    {
        return userID + "|" + name + "|" + getRole() + "|";
    }
    vector<string> finePayment() { return {"0"}; }
};