#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "Helper_Functions.h"
using namespace std;

enum class BookStatus
{
    Available,
    Borrowed,
    Reserved
};

class Book
{
private:
    string title, author, publisher;
    unsigned short year;
    BookStatus status;
    string ISBN, borrowDate;
    string reservedByUserID;

public:
    Book(string t, string a, string p, unsigned short y, string isbn, BookStatus s = BookStatus::Available, string bd = "INVALID", string ID = "INVALID")
        : title(t), author(a), publisher(p), year(y), status(s), ISBN(isbn), borrowDate(bd), reservedByUserID(ID) {}

    string getISBN() { return ISBN; }
    BookStatus getStatus() { return status; }
    string getBorrowDate() { return borrowDate; }
    void setBorrowDate(string str) { borrowDate = str; }
    void setStatus(BookStatus newStatus) { status = newStatus; }

    void setReservedByUserID(string userID){
        reservedByUserID = userID;
    }
    string getReserevedByUserID(){
        return reservedByUserID;
    }

    void getDetails()
    {
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "Year: " << year << endl;
        cout << "ISBN: " << ISBN << endl;
        cout << "Status: " << (status == BookStatus::Available ? "Available" : "Borrowed") << endl;
        if (status == BookStatus::Borrowed)
            cout << "Borrow Date: " << borrowDate << endl;
        cout << "-------------------------" << endl;
    }

    string toFileString()
    {
        return title + "|" + author + "|" + publisher + "|" + to_string(year) + "|" + ISBN + "|" +
               to_string(static_cast<int>(status)) + "|" + borrowDate + "|" + reservedByUserID;
    }

    static Book *fromFileString(const string &data)
    {
        stringstream ss(data);
        string t, a, p, isbn, sStr, bd, user;
        unsigned short y;
        int s;

        // Read all fields correctly
        getline(ss, t, '|');
        getline(ss, a, '|');
        getline(ss, p, '|');

        string yearStr;
        getline(ss, yearStr, '|');
        y = static_cast<unsigned short>(stoi(yearStr)); // Convert year correctly

        getline(ss, isbn, '|');

        string statusStr;
        getline(ss, statusStr, '|');
        s = stoi(statusStr); // Convert status correctly

        getline(ss, bd, '|'); // Read borrow date
        getline(ss, user);

        return new Book(t, a, p, y, isbn, static_cast<BookStatus>(s), bd, user);
    }
};
