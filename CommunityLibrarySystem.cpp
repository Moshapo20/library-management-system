#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
using namespace std;
// ===================== STRUCTURES =====================
struct Date {
 int day;
 int month;
 int year;
};
struct Borrower {
 int borrowerId;
 string fullName;
 string phone;
};
struct LoanInfo {
 Borrower borrower;
 Date issueDate;
 Date dueDate;
 bool returned;
};
struct Book {
 int bookId;
 string title;
 string author;
 int yearPublished;
 bool available;
 LoanInfo* loan;
};
// ===================== CONSTANTS =====================
// Define loan period so all modules use the same value
const int LOAN_PERIOD_DAYS = 14;
// ===================== FILE FORMAT SPECIFICATION =====================
// Prescribed text-file formats so all students write/read consistently.
//
// books.txt — one book per line, pipe-delimited:
// bookId|title|author|yearPublished|available
// e.g. 1|The Great Gatsby|F. Scott Fitzgerald|1925|1
// If the book is currently on loan, the next line holds the loan record:
// LOAN|borrowerId|borrowerName|borrowerPhone|issueDay|issueMonth|issueYear|dueDay|dueMonth|dueYear|returned
// A line containing "NOLOAN" means no active loan.
//
// borrowers.txt — one borrower per line, pipe-delimited:
// borrowerId|fullName|phone
// e.g. 101|Jane Doe|0712345678
// ===================== FUNCTION PROTOTYPES =====================
// Student 1 - Menu
void showMainMenu();
void processChoice(int choice, Book*& books, int& bookCount, Borrower*& borrowers, int& borrowerCount);
// Student 2 - Book Module
void addBook(Book*& books, int& bookCount);
void displayBooks(Book* books, int bookCount);
int findBookIndexById(Book* books, int bookCount, int id);
void searchBookByTitle(Book* books, int bookCount);
// Student 3 - Borrower Module
void addBorrower(Borrower*& borrowers, int& borrowerCount);
void displayBorrowers(Borrower* borrowers, int borrowerCount);
int findBorrowerIndexById(Borrower* borrowers, int borrowerCount, int id);
// Student 4 - Loan Module
void borrowBook(Book* books, int bookCount, Borrower* borrowers, int borrowerCount);
Date createDueDate(Date issueDate, int daysAllowed);
Date getCurrentDate(); // Prototype for current-date helper
// Student 5 - Return Module
void returnBook(Book* books, int bookCount);
bool isOverdue(Date dueDate, Date currentDate);
// Student 6 - Dynamic Memory Module
void resizeBooksArray(Book*& books, int oldSize, int newSize);
void resizeBorrowersArray(Borrower*& borrowers, int oldSize, int newSize);
void releaseMemory(Book*& books, int& bookCount, Borrower*& borrowers, int& borrowerCount);
// Student 7 - File Module
void saveBooksToFile(Book* books, int bookCount);
void loadBooksFromFile(Book*& books, int& bookCount);
void saveBorrowersToFile(Borrower* borrowers, int borrowerCount);
void loadBorrowersFromFile(Borrower*& borrowers, int& borrowerCount);
// Student 8 - Reports Module
void displayActiveLoans(Book* books, int bookCount);
void showSummary(Book* books, int bookCount, Borrower* borrowers, int borrowerCount);
// Student 9 - Template Module
template <typename T>
void swapValues(T& a, T& b) {
 T temp = a;
 a = b;
 b = temp;
}
// Hint — a plain template cannot access .bookId vs .borrowerId
// without specialisation. You MUST either:
// Option A) Give both structs a common field named 'id' and access arr[i].id, OR
// Option B) Create two separate specialisations:
// template<> int linearSearchId<Book>(...) and
// template<> int linearSearchId<Borrower>(...)
// Option A is the simpler approach at this level.
template <typename T>
int linearSearchId(T* arr, int size, int targetId);
// Student 10 - Integration and Testing
void runTests();
// ===================== MAIN =====================
int main() {
 Book* books = nullptr;
 Borrower* borrowers = nullptr;
 int bookCount = 0;
 int borrowerCount = 0;
 loadBooksFromFile(books, bookCount);
 loadBorrowersFromFile(borrowers, borrowerCount);
 int choice;
 do {
 showMainMenu();
 cin >> choice;
 processChoice(choice, books, bookCount, borrowers, borrowerCount);
 } while (choice != 0);
 saveBooksToFile(books, bookCount);
 saveBorrowersToFile(borrowers, borrowerCount);
 releaseMemory(books, bookCount, borrowers, borrowerCount);
 return 0;
}
// ===================== MENU =====================
void showMainMenu() {
 cout << "\n===== COMMUNITY LIBRARY SYSTEM =====\n";
 cout << "1. Add Book\n";
 cout << "2. Add Borrower\n";
 cout << "3. Borrow Book\n";
 cout << "4. Return Book\n";
 cout << "5. Search Book by Title\n";
 cout << "6. Display All Books\n";
 cout << "7. Display All Borrowers\n";
 cout << "8. Display Active Loans\n";
 cout << "9. Show Summary\n";
 cout << "0. Exit\n";
 cout << "Enter choice: ";
}
void processChoice(int choice, Book*& books, int& bookCount, Borrower*& borrowers, int& borrowerCount) {
 switch (choice) {
 case 1: addBook(books, bookCount); break;
 case 2: addBorrower(borrowers, borrowerCount); break;
 case 3: borrowBook(books, bookCount, borrowers, borrowerCount); break;
 case 4: returnBook(books, bookCount); break;
 case 5: searchBookByTitle(books, bookCount); break;
 case 6: displayBooks(books, bookCount); break;
 case 7: displayBorrowers(borrowers, borrowerCount); break;
 case 8: displayActiveLoans(books, bookCount); break;
 case 9: showSummary(books, bookCount, borrowers, borrowerCount); break;
 case 0: cout << "Exiting...\n"; break;
 default: cout << "Invalid choice.\n";
 }
}
// ===================== STUDENT TASKS =====================
void addBook(Book*& books, int& bookCount) {
 resizeBooksArray(books, bookCount, bookCount + 1);
 Book& newBook = books[bookCount];
 cout << "Enter Book ID: ";
 cin >> newBook.bookId;
 cin.ignore();
 cout << "Enter Title: ";
 getline(cin, newBook.title);
 cout << "Enter Author: ";
 getline(cin, newBook.author);
 cout << "Enter Year Published: ";
 cin >> newBook.yearPublished;
 newBook.available = true;
 newBook.loan = nullptr;
 bookCount++;
 cout << "Book added successfully!\n";
}
void displayBooks(Book* books, int bookCount) {
 cout << "\n--- Book List ---\n";
 for (int i = 0; i < bookCount; i++) {
  cout << "ID: " << books[i].bookId << ", Title: " << books[i].title
       << ", Author: " << books[i].author << ", Year: " << books[i].yearPublished
       << ", Status: " << (books[i].available ? "Available" : "Borrowed") << "\n";
 }
}
template <typename T>
int linearSearchId(T* arr, int size, int targetId) {
 return -1;
}

template <>
int linearSearchId<Book>(Book* arr, int size, int targetId) {
 for (int i = 0; i < size; i++) {
  if (arr[i].bookId == targetId) return i;
 }
 return -1;
}

template <>
int linearSearchId<Borrower>(Borrower* arr, int size, int targetId) {
 for (int i = 0; i < size; i++) {
  if (arr[i].borrowerId == targetId) return i;
 }
 return -1;
}

void addBorrower(Borrower*& borrowers, int& borrowerCount) {
 resizeBorrowersArray(borrowers, borrowerCount, borrowerCount + 1);
 Borrower& newBorrower = borrowers[borrowerCount];
 cout << "Enter Borrower ID: ";
 cin >> newBorrower.borrowerId;
 cin.ignore();
 cout << "Enter Full Name: ";
 getline(cin, newBorrower.fullName);
 cout << "Enter Phone: ";
 getline(cin, newBorrower.phone);
 borrowerCount++;
 cout << "Borrower added successfully!\n";
}
void displayBorrowers(Borrower* borrowers, int borrowerCount) {
 cout << "\n--- Borrower List ---\n";
 for (int i = 0; i < borrowerCount; i++) {
  cout << "ID: " << borrowers[i].borrowerId << ", Name: " << borrowers[i].fullName
       << ", Phone: " << borrowers[i].phone << "\n";
 }
}
int findBookIndexById(Book* books, int bookCount, int id) {
 return linearSearchId<Book>(books, bookCount, id);
}
int findBorrowerIndexById(Borrower* borrowers, int borrowerCount, int id) {
 return linearSearchId<Borrower>(borrowers, borrowerCount, id);
}
void searchBookByTitle(Book* books, int bookCount) {
 cin.ignore();
 string searchTitle;
 cout << "Enter title to search: ";
 getline(cin, searchTitle);
 bool found = false;
 for (int i = 0; i < bookCount; i++) {
  if (books[i].title == searchTitle) {
   cout << "Found: ID: " << books[i].bookId << ", Title: " << books[i].title
        << ", Author: " << books[i].author << ", Status: " 
        << (books[i].available ? "Available" : "Borrowed") << "\n";
   found = true;
  }
 }
 if (!found) cout << "No book found with that title.\n";
}
Date createDueDate(Date issueDate, int daysAllowed) {
 Date due = issueDate;
 due.day = issueDate.day + daysAllowed;
 return due;
}
// getCurrentDate() — uses <ctime> to get today's date from the system clock.
// Student 4 must call this to populate issueDate when borrowing a book.
Date getCurrentDate() {
 time_t now = time(nullptr);
 tm* localTime = localtime(&now);
 Date today;
 today.day = localTime->tm_mday;
 today.month = localTime->tm_mon + 1; // tm_mon is 0-based
 today.year = localTime->tm_year + 1900;
 return today;
}
void borrowBook(Book* books, int bookCount, Borrower* borrowers, int borrowerCount) {
 int bookId, borrowerId;
 cout << "Enter Book ID to borrow: ";
 cin >> bookId;
 cout << "Enter Borrower ID: ";
 cin >> borrowerId;
 
 int bookIdx = findBookIndexById(books, bookCount, bookId);
 int borrowerIdx = findBorrowerIndexById(borrowers, borrowerCount, borrowerId);
 
 if (bookIdx == -1) {
  cout << "Error: Book not found.\n";
  return;
 }
 if (borrowerIdx == -1) {
  cout << "Error: Borrower not found.\n";
  return;
 }
 if (!books[bookIdx].available) {
  cout << "Error: Book is currently not available.\n";
  return;
 }
 
 LoanInfo* newLoan = new LoanInfo;
 newLoan->borrower = borrowers[borrowerIdx];
 newLoan->issueDate = getCurrentDate();
 newLoan->dueDate = createDueDate(newLoan->issueDate, LOAN_PERIOD_DAYS);
 newLoan->returned = false;
 
 books[bookIdx].loan = newLoan;
 books[bookIdx].available = false;
 cout << "Book borrowed successfully! Due date: " << newLoan->dueDate.day << "/" 
      << newLoan->dueDate.month << "/" << newLoan->dueDate.year << "\n";
}
void returnBook(Book* books, int bookCount) {
 int bookId;
 cout << "Enter Book ID to return: ";
 cin >> bookId;
 
 int bookIdx = findBookIndexById(books, bookCount, bookId);
 if (bookIdx == -1) {
  cout << "Error: Book not found.\n";
  return;
 }
 if (books[bookIdx].available || books[bookIdx].loan == nullptr) {
  cout << "Error: Book is not currently on loan.\n";
  return;
 }
 
 books[bookIdx].loan->returned = true;
 delete books[bookIdx].loan;
 books[bookIdx].loan = nullptr;
 books[bookIdx].available = true;
 cout << "Book returned successfully!\n";
}
bool isOverdue(Date dueDate, Date currentDate) {
 if (currentDate.year > dueDate.year) return true;
 if (currentDate.year < dueDate.year) return false;
 if (currentDate.month > dueDate.month) return true;
 if (currentDate.month < dueDate.month) return false;
 return currentDate.day > dueDate.day;
}
void resizeBooksArray(Book*& books, int oldSize, int newSize) {
 Book* newArray = new Book[newSize];
 for (int i = 0; i < oldSize; i++) {
  newArray[i] = books[i];
 }
 for (int i = oldSize; i < newSize; i++) {
  newArray[i].loan = nullptr; // Initialize pointers for new slots
 }
 delete[] books;
 books = newArray;
}
void resizeBorrowersArray(Borrower*& borrowers, int oldSize, int newSize) {
 Borrower* newArray = new Borrower[newSize];
 for (int i = 0; i < oldSize; i++) {
  newArray[i] = borrowers[i];
 }
 delete[] borrowers;
 borrowers = newArray;
}
void saveBooksToFile(Book* books, int bookCount) {
 ofstream outFile("books.txt", ios::out);
 if (!outFile) {
  cout << "Error opening books.txt for writing.\n";
  return;
 }
 for (int i = 0; i < bookCount; i++) {
  outFile << books[i].bookId << "|" << books[i].title << "|"
          << books[i].author << "|" << books[i].yearPublished << "|"
          << books[i].available << "\n";
  if (books[i].loan != nullptr) {
   outFile << "LOAN|" << books[i].loan->borrower.borrowerId << "|"
           << books[i].loan->borrower.fullName << "|" << books[i].loan->borrower.phone << "|"
           << books[i].loan->issueDate.day << "|" << books[i].loan->issueDate.month << "|" << books[i].loan->issueDate.year << "|"
           << books[i].loan->dueDate.day << "|" << books[i].loan->dueDate.month << "|" << books[i].loan->dueDate.year << "|"
           << books[i].loan->returned << "\n";
  } else {
   outFile << "NOLOAN\n";
  }
 }
}
void loadBooksFromFile(Book*& books, int& bookCount) {
 ifstream inFile("books.txt");
 if (!inFile) return; // File does not exist, safe to return
 
 string line;
 while (getline(inFile, line)) {
  if (line.empty()) continue;
  stringstream ss(line);
  string token;
  
  Book b;
  getline(ss, token, '|'); b.bookId = stoi(token);
  getline(ss, b.title, '|');
  getline(ss, b.author, '|');
  getline(ss, token, '|'); b.yearPublished = stoi(token);
  getline(ss, token, '|'); b.available = (stoi(token) != 0);
  b.loan = nullptr;
  
  resizeBooksArray(books, bookCount, bookCount + 1);
  books[bookCount] = b;
  
  if (getline(inFile, line)) {
   if (line.substr(0, 4) == "LOAN") {
    stringstream lss(line);
    getline(lss, token, '|'); // skip "LOAN"
    
    LoanInfo* newLoan = new LoanInfo;
    getline(lss, token, '|'); newLoan->borrower.borrowerId = stoi(token);
    getline(lss, newLoan->borrower.fullName, '|');
    getline(lss, newLoan->borrower.phone, '|');
    
    getline(lss, token, '|'); newLoan->issueDate.day = stoi(token);
    getline(lss, token, '|'); newLoan->issueDate.month = stoi(token);
    getline(lss, token, '|'); newLoan->issueDate.year = stoi(token);
    
    getline(lss, token, '|'); newLoan->dueDate.day = stoi(token);
    getline(lss, token, '|'); newLoan->dueDate.month = stoi(token);
    getline(lss, token, '|'); newLoan->dueDate.year = stoi(token);
    
    getline(lss, token, '|'); newLoan->returned = (stoi(token) != 0);
    
    books[bookCount].loan = newLoan;
   }
  }
  bookCount++;
 }
}
void saveBorrowersToFile(Borrower* borrowers, int borrowerCount) {
 ofstream outFile("borrowers.txt", ios::out);
 if (!outFile) {
  cout << "Error opening borrowers.txt for writing.\n";
  return;
 }
 for (int i = 0; i < borrowerCount; i++) {
  outFile << borrowers[i].borrowerId << "|" << borrowers[i].fullName << "|"
          << borrowers[i].phone << "\n";
 }
}
void loadBorrowersFromFile(Borrower*& borrowers, int& borrowerCount) {
 ifstream inFile("borrowers.txt");
 if (!inFile) return;
 
 string line;
 while (getline(inFile, line)) {
  if (line.empty()) continue;
  stringstream ss(line);
  string token;
  
  Borrower b;
  getline(ss, token, '|'); b.borrowerId = stoi(token);
  getline(ss, b.fullName, '|');
  getline(ss, b.phone, '|');
  
  resizeBorrowersArray(borrowers, borrowerCount, borrowerCount + 1);
  borrowers[borrowerCount] = b;
  borrowerCount++;
 }
}
void displayActiveLoans(Book* books, int bookCount) {
 cout << "\n--- Active Loans ---\n";
 bool found = false;
 Date current = getCurrentDate();
 for (int i = 0; i < bookCount; i++) {
  if (!books[i].available && books[i].loan != nullptr && !books[i].loan->returned) {
   found = true;
   bool overdue = isOverdue(books[i].loan->dueDate, current);
   cout << "Book: " << books[i].title << " (ID: " << books[i].bookId << ")\n"
        << " Borrower: " << books[i].loan->borrower.fullName << "\n"
        << " Due Date: " << books[i].loan->dueDate.day << "/" 
        << books[i].loan->dueDate.month << "/" << books[i].loan->dueDate.year << "\n"
        << " Overdue Status: " << (overdue ? "OVERDUE!" : "On time") << "\n\n";
  }
 }
 if (!found) cout << "No active loans.\n";
}
void showSummary(Book* books, int bookCount, Borrower* borrowers, int borrowerCount) {
 int borrowedCount = 0;
 int availableCount = 0;
 for (int i = 0; i < bookCount; i++) {
  if (books[i].available) availableCount++;
  else borrowedCount++;
 }
 cout << "\n--- Library Summary ---\n";
 cout << "Total Books: " << bookCount << "\n";
 cout << "Total Borrowers: " << borrowerCount << "\n";
 cout << "Borrowed Books: " << borrowedCount << "\n";
 cout << "Available Books: " << availableCount << "\n";
}
void releaseMemory(Book*& books, int& bookCount, Borrower*& borrowers, int& borrowerCount) {
 for (int i = 0; i < bookCount; i++) {
  if (books[i].loan != nullptr) {
   delete books[i].loan;
  }
 }
 delete[] books;
 books = nullptr;
 delete[] borrowers;
 borrowers = nullptr;
 bookCount = 0;
 borrowerCount = 0;
}
void runTests() {
 cout << "\n--- Running Tests ---\n";
 int x = 5, y = 10;
 cout << "Before swapValues(x, y): x=" << x << " y=" << y << "\n";
 swapValues(x, y);
 cout << "After swapValues(x, y): x=" << x << " y=" << y << "\n";
 cout << "Expected: x=10 y=5\n";
}
