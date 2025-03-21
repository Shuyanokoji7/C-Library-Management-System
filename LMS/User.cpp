#include <iostream>
#include "Library.h" // Include your library header file

using namespace std;

int main()
{
    Library lib; // Automatically loads books from file

    cout << "\n--- Initial Books Loaded from File ---\n";
    lib.loadBooksFromFile("books.txt");

    vector<Book *> books = lib.getBooks();
    for (Book *b : books)
    {
        b->getDetails();
    }

    // Adding a new book (Only a librarian can do this)
    cout << "\n--- Adding Book ---\n";
    lib.addBook("L300", "Dune", "Frank Herbert", "Chilton Books", 1965, "9780441013593");

    // Save books after addition
    lib.saveBooksToFile("books.txt");

    // Reload books to check persistence
    cout << "\n--- Books After Reloading from File ---\n";
    lib.loadBooksFromFile("books.txt");
    books = lib.getBooks();
    for (Book *b : books)
    {
        b->getDetails();
    }

    return 0;
}
