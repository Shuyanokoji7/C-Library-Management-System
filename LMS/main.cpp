#include "Library.h"
#include <limits> // For std::numeric_limits

void displayMenu_Lib()
{
    cout << "\nLibrary Management System" << endl;
    cout << "1. Borrow Book" << endl;
    cout << "2. Return Book" << endl;
    cout << "3. View My Transactions" << endl;
    cout << "4. Fine Details and Payment" << endl;
    cout << "5. Add Book" << endl;
    cout << "6. Remove Book" << endl;
    cout << "7. Add User" << endl;
    cout << "8. Remove User" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter your choice: ";
}

int main()
{
    Library library;
    string userID, userRole;
    int incorrectAttempt = 0;

    while (incorrectAttempt < 3)
    {
        cout << "Enter User ID: ";
        cin >> userID;

        userRole = library.getUserRole(userID);
        if (userRole == "INVALID")
        {
            cout << "Invalid User ID, please re-enter.\n";
            incorrectAttempt++;
            if (incorrectAttempt == 3)
            {
                cout << "Program terminated due to 3 unsuccessful attempts.\n";
                return 1; // Exit the program
            }
        }
        else
        {
            break; // Valid user, proceed
        }
    }

    int choice;
    do
    {
        displayMenu_Lib();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (choice)
        {
        case 1: // Borrow Book
            if (userRole == "Librarian")
                cout << "You are not allowed to perform this action.\n";
            else
            {
                string ISBN;
                cout << "Enter ISBN to borrow: ";
                cin >> ISBN;
                library.borrowBook(userID, ISBN);
            }
            break;

        case 2: // Return Book
            if (userRole == "Librarian")
                cout << "You are not allowed to perform this action.\n";
            else
            {
                string ISBN;
                cout << "Enter ISBN to return: ";
                cin >> ISBN;
                library.returnBook(userID, ISBN);
            }
            break;

        case 3: // View Transactions

            library.displayUserTransactions(userID);
            break;

        case 4: // Fine Payment
            if (userRole == "Librarian" || userRole == "Faculty")
                cout << "You are not allowed to perform this action.\n";
            else
                library.simulateFinePayment(userID);
            break;

        case 5: // Add Book
            if (userRole != "Librarian")
                cout << "You are not allowed to perform this action.\n";
            else
            {
                string title, author, publisher, ISBN;
                unsigned short year;

                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter author: ";
                getline(cin, author);
                cout << "Enter publisher: ";
                getline(cin, publisher);
                cout << "Enter publication year: ";
                cin >> year;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Prevent input buffer issues
                cout << "Enter ISBN: ";
                cin >> ISBN;

                library.addBook(userID, title, author, publisher, year, ISBN);
            }
            break;

        case 6: // Remove Book
            if (userRole != "Librarian")
                cout << "You are not allowed to perform this action.\n";
            else
            {
                string ISBN;
                cout << "Enter ISBN of the book to remove: ";
                cin >> ISBN;
                library.removeBook(userID, ISBN);
            }
            break;

        case 7: // Add User
            if (userRole != "Librarian")
                cout << "You are not allowed to perform this action.\n";
            else
            {
                string newUserID, name, role;
                cout << "Enter new user ID: ";
                cin >> newUserID;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter name: ";
                getline(cin, name);
                cout << "Enter role (Student/Faculty/Librarian): ";
                cin >> role;

                library.addUser(userID, role, newUserID, name);
            }
            break;

        case 8: // Remove User
            if (userRole != "Librarian")
                cout << "You are not allowed to perform this action.\n";
            else
            {
                string removeUserID;
                cout << "Enter User ID to remove: ";
                cin >> removeUserID;
                library.removeUser(userID, removeUserID);
            }
            break;

        case 9: // Exit
            cout << "Exiting system...\n";
            break;

        default:
            cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 9); // Continue until user chooses to exit

    return 0;
}
