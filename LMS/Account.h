#include "Book.h"

class Account
{
private:
    vector<Book *> borrowedBooks;
    int fineAmount;

public:
    Account() : fineAmount(0) {}

    vector<Book *> getBorrowedBook()
    {
        return borrowedBooks;
    }

    void addBorrowedBook(Book *book)
    {
        borrowedBooks.push_back(book);
    }
    void updateTotalFine(int maxBorrowingPeriod, int fineRate)
    {
        int fine = 0;
        for (Book *book : borrowedBooks)
        {
            int daysBookHeld = inBetweenDays(book->getBorrowDate(), getCurrentDate());
            if (daysBookHeld > maxBorrowingPeriod)
            {
                fine += (daysBookHeld - maxBorrowingPeriod) * fineRate;
            }
        }
        this->fineAmount = fine;
    }
    void removeBorrowedBook(string ISBN)
    {

        auto it = remove_if(borrowedBooks.begin(), borrowedBooks.end(), [&](Book *b)
                            { return b->getISBN() == ISBN; });

        if (it != borrowedBooks.end())
        {
            borrowedBooks.erase(it, borrowedBooks.end());
        }
        else
        {
            cout << "Book not found.\n";
        }
    }

    int getFine()
    {
        return fineAmount;
    }
    void setFine(int x)
    {
        fineAmount = x;
    }

    bool getEligibility()
    {

        if (fineAmount != 0)
            return false;

        for (Book *book : borrowedBooks)
        {
            if (inBetweenDays(book->getBorrowDate(), getCurrentDate()) > 60)
                return false;
        }

        return true;
    }
};