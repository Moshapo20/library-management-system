# Community Library System - Group Report

**Course/Module:** [Insert Course Name]
**Date:** [Insert Date]

## Group Members
| Student Name | Student Number | Module Responsibility |
| :--- | :--- | :--- |
| [Name 1] | [Number 1] | Student 1: Menu Module |
| [Name 2] | [Number 2] | Student 2: Book Module |
| [Name 3] | [Number 3] | Student 3: Borrower Module |
| [Name 4] | [Number 4] | Student 4: Loan Module |
| [Name 5] | [Number 5] | Student 5: Return Module |
| [Name 6] | [Number 6] | Student 6: Dynamic Memory Module |
| [Name 7] | [Number 7] | Student 7: File Module |
| [Name 8] | [Number 8] | Student 8: Report Module |
| [Name 9] | [Number 9] | Student 9: Template Module |
| [Name 10] | [Number 10] | Student 10: Testing & Integration Module |

*(Note: If you have fewer than 10 members, simply assign multiple modules to some members and adjust the table accordingly).*

---

## Technical Implementation Details

We built the library system exactly as requested in the rubric, avoiding vectors and handling all memory dynamically. Here is how we implemented the required features:

### 1. Pointer Usage
We used pointers a lot to manage arrays and link our structs together:
*   Instead of copying massive arrays, we passed pointers by reference into our functions (like `Book*& books`). This let us modify the main arrays directly.
*   Inside the `Book` struct, we used a `LoanInfo* loan` pointer. If the book is available, this pointer is just set to `nullptr` so it doesn't waste memory. If it gets checked out, we point it to a new loan record.

### 2. Dynamic Memory Allocation
Since we couldn't use `std::vector`, we managed the arrays manually with `new` and `delete`:
*   **Growing Arrays:** Whenever we added a new book or borrower, we dynamically allocated a slightly larger array on the heap using `new`. We copied the old data over, called `delete[]` on the old array to avoid memory leaks, and updated our pointer.
*   **Memory Release:** We wrote a `releaseMemory()` function that runs when the program exits. It loops through all the books and deletes any active `loan` pointers first, and then it calls `delete[]` on the main books and borrowers arrays to clean everything up.

### 3. Nested Structures
Our `LoanInfo` struct was designed using nested structures to keep the data organized. It holds a `Borrower` struct directly inside of it, along with two `Date` structs for the issue and due dates. This made it really easy to grab data, like typing `books[i].loan->borrower.fullName`.

### 4. Function Templates
For the generic template requirement, we fixed the bug in the `swapValues` template, and we also used template specialization for `linearSearchId`. Instead of writing two completely separate search functions, we made one template and then specialized it to search for `bookId` in the Book array and `borrowerId` in the Borrower array.
