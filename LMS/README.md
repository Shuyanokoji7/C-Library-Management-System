# Library Management System

## Overview
This Library Management System (LMS) is a command-line application implemented in C++ using Object-Oriented Programming (OOP) principles. It allows users to borrow and return books, view transaction history, manage fines, reserve books, and handle book and user administration.

## Features
- **Borrow Book**: Users can borrow books using their User ID and the book's ISBN.
- **Return Book**: Users can return books by providing the ISBN.
- **View Transactions**: Users can view their borrowing, returning, and reservation history.
- **Fine Details and Payment**: Users can check their fines and simulate payments.
- **Book Reservation**: Users can reserve books if they are currently borrowed by another user.
- **Reservation Notification**: Reserved books will notify the user when they become available.
- **Book Management**: Librarians can add and remove books from the system.
- **User Management**: Librarians can add and remove users.
- **Secure Access**: Users must enter a valid User ID to interact with the system.
- **Exit System**: Users can exit the system safely.

## User Roles
The system supports three different user roles:
- **Student**:
  - Can borrow up to 3 books for 15 days.
  - Fine rate: Rs 10 per overdue day.
  - Can reserve books if they are already borrowed by another user.
- **Faculty**:
  - Can borrow up to 5 books for 30 days.
  - Restricted from borrowing if overdue books exceed 60 days.
  - Can reserve books if they are already borrowed by another user.
- **Librarian**:
  - Cannot borrow or return books.
  - Manages books and users in the system.

## Fine Calculation
Fines are calculated dynamically based on overdue days:
- If a borrowed book exceeds the borrowing period, a fine is incurred based on the fine rate.
- Student cannot borrow new books if they have unpaid fines.
- If a book is overdue for more than 60 days, the Faculty is restricted from borrowing any new books.

## Book Borrowing System
- Users can only borrow books that are currently available.
- The system maintains borrowing records in the format: `Borrow Date|User ID|BORROW|ISBN`.

## Book Returning System
- Users can return borrowed books anytime given the fact that late returns will be charged for the students and respective penalties will be incurred by both faculty and students based on terms and conditions.
- The system maintains returning records in the format: `Return Date|User ID|BORROW|ISBN`.

## Book Reservation System
- Users can reserve books that are currently borrowed by another user.
- When a reserved book is returned, the book will be automatically borrowed by the first user who reserved it. automatically .
- The system maintains reservation records in the format: `Reservation Date|User ID|RESERVE|ISBN`.

## Usage
1. Run the program.
2. Enter your User ID.
3. Choose an option from the menu:
    - `1`: Borrow a book.
    - `2`: Return a book.
    - `3`: View transaction history.
    - `4`: View fine details and make payments.
    - `5`: Reserve a book.
    - `6`: Add a book (Librarians only).
    - `7`: Remove a book (Librarians only).
    - `8`: Add a user (Librarians only).
    - `9`: Remove a user (Librarians only).
    - `10`: Exit the system.

## Example Usage
**Borrowing a Book:**
```
Enter User ID: S123
Choose an option: 1
Enter ISBN: 9783161484100
Book borrowed successfully.
```
**Returning a Book:**
```
Enter User ID: S123
Choose an option: 2
Enter ISBN: 9783161484100
Book returned successfully.
```

### Handling File Dependencies
Ensure all `.h` and `.cpp` files are in the same directory. If compilation errors occur, check for missing header files or incorrect file names.

## Dependencies
- C++17 or later
- Standard Template Library (STL)

## File Structure
- `main.cpp`: Contains the main program logic and user menu.
- `Library.h`: Handles library operations such as borrowing, returning, and managing books and users.
- `User.h`: Manages user details and roles.
- `Book.h`: Represents book attributes and functionalities.
- `Account.h`: Manages user accounts, fines, reservations, and book transactions.
- `transactions.txt`: Stores user transactions, including borrowing, returning, and reservation records.
- `books.txt`: Stores book details, including reservation status.
- `users.txt`: Stores user details, inlcuding borrowed books and fine.

## Future Enhancements
- Implement authentication for better security.
- Add a GUI interface for better user experience.



