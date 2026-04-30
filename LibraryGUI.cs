using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace LibrarySystemGUI
{
    public class Borrower
    {
        public int Id { get; set; }
        public string FullName { get; set; }
        public string Phone { get; set; }
    }

    public class Book
    {
        public int Id { get; set; }
        public string Title { get; set; }
        public string Author { get; set; }
        public int Year { get; set; }
        private bool available = true;
        public bool Available { get { return available; } set { available = value; } }
        
        public string Status { get { return Available ? "Available" : "Borrowed"; } }
    }

    public class Loan
    {
        public Book BookItem { get; set; }
        public Borrower BorrowerItem { get; set; }
        public DateTime IssueDate { get; set; }
        public DateTime DueDate { get; set; }
        public bool Returned { get; set; }

        public string BookTitle { get { return BookItem.Title; } }
        public string BorrowerName { get { return BorrowerItem.FullName; } }
        public string Due { get { return DueDate.ToShortDateString(); } }
        public string Overdue { get { return DateTime.Now > DueDate ? "Yes" : "No"; } }
    }

    public class LibraryForm : Form
    {
        private List<Book> books = new List<Book>();
        private List<Borrower> borrowers = new List<Borrower>();
        private List<Loan> loans = new List<Loan>();

        private TabControl tabControl;
        
        private DataGridView dgvBooks;
        private DataGridView dgvBorrowers;
        private DataGridView dgvLoans;

        private TextBox txtBookId, txtBookTitle, txtBookAuthor, txtBookYear;
        private TextBox txtBorrowerId, txtBorrowerName, txtBorrowerPhone;
        private TextBox txtLoanBookId, txtLoanBorrowerId, txtReturnBookId;

        public LibraryForm()
        {
            this.Text = "Community Library System - C# GUI";
            this.Size = new Size(850, 600);
            this.StartPosition = FormStartPosition.CenterScreen;
            this.Font = new Font("Segoe UI", 10);

            InitializeUI();
            
            books.Add(new Book { Id = 1, Title = "The Great Gatsby", Author = "F. Scott Fitzgerald", Year = 1925 });
            books.Add(new Book { Id = 2, Title = "1984", Author = "George Orwell", Year = 1949 });
            borrowers.Add(new Borrower { Id = 100, FullName = "John Doe", Phone = "555-0100" });
            
            RefreshData();
        }

        private void InitializeUI()
        {
            tabControl = new TabControl { Dock = DockStyle.Fill, Padding = new Point(15, 10) };

            // === TAB 1: BOOKS ===
            TabPage tabBooks = new TabPage("Manage Books");
            dgvBooks = CreateGrid();
            tabBooks.Controls.Add(dgvBooks);
            Panel pnlBookTop = new Panel { Dock = DockStyle.Top, Height = 80, Padding = new Padding(10) };
            
            pnlBookTop.Controls.Add(new Label { Text = "ID:", Location = new Point(10, 25), AutoSize = true });
            txtBookId = new TextBox { Location = new Point(40, 22), Width = 50 };
            pnlBookTop.Controls.Add(txtBookId);

            pnlBookTop.Controls.Add(new Label { Text = "Title:", Location = new Point(100, 25), AutoSize = true });
            txtBookTitle = new TextBox { Location = new Point(140, 22), Width = 150 };
            pnlBookTop.Controls.Add(txtBookTitle);

            pnlBookTop.Controls.Add(new Label { Text = "Author:", Location = new Point(300, 25), AutoSize = true });
            txtBookAuthor = new TextBox { Location = new Point(360, 22), Width = 150 };
            pnlBookTop.Controls.Add(txtBookAuthor);

            pnlBookTop.Controls.Add(new Label { Text = "Year:", Location = new Point(520, 25), AutoSize = true });
            txtBookYear = new TextBox { Location = new Point(560, 22), Width = 60 };
            pnlBookTop.Controls.Add(txtBookYear);

            Button btnAddBook = new Button { Text = "Add Book", Location = new Point(640, 20), Width = 100, BackColor = Color.LightGreen };
            btnAddBook.Click += BtnAddBook_Click;
            pnlBookTop.Controls.Add(btnAddBook);

            tabBooks.Controls.Add(pnlBookTop);

            // === TAB 2: BORROWERS ===
            TabPage tabBorrowers = new TabPage("Manage Borrowers");
            dgvBorrowers = CreateGrid();
            tabBorrowers.Controls.Add(dgvBorrowers);
            Panel pnlBorrowerTop = new Panel { Dock = DockStyle.Top, Height = 80, Padding = new Padding(10) };

            pnlBorrowerTop.Controls.Add(new Label { Text = "ID:", Location = new Point(10, 25), AutoSize = true });
            txtBorrowerId = new TextBox { Location = new Point(40, 22), Width = 50 };
            pnlBorrowerTop.Controls.Add(txtBorrowerId);

            pnlBorrowerTop.Controls.Add(new Label { Text = "Name:", Location = new Point(100, 25), AutoSize = true });
            txtBorrowerName = new TextBox { Location = new Point(150, 22), Width = 150 };
            pnlBorrowerTop.Controls.Add(txtBorrowerName);

            pnlBorrowerTop.Controls.Add(new Label { Text = "Phone:", Location = new Point(310, 25), AutoSize = true });
            txtBorrowerPhone = new TextBox { Location = new Point(360, 22), Width = 120 };
            pnlBorrowerTop.Controls.Add(txtBorrowerPhone);

            Button btnAddBorrower = new Button { Text = "Add Borrower", Location = new Point(500, 20), Width = 120, BackColor = Color.LightGreen };
            btnAddBorrower.Click += BtnAddBorrower_Click;
            pnlBorrowerTop.Controls.Add(btnAddBorrower);

            tabBorrowers.Controls.Add(pnlBorrowerTop);

            // === TAB 3: LOANS ===
            TabPage tabLoans = new TabPage("Checkout & Returns");
            dgvLoans = CreateGrid();
            tabLoans.Controls.Add(dgvLoans);
            Panel pnlLoanTop = new Panel { Dock = DockStyle.Top, Height = 120, Padding = new Padding(10) };

            // Borrow section
            GroupBox gbBorrow = new GroupBox { Text = "Borrow a Book", Location = new Point(10, 10), Size = new Size(350, 90) };
            gbBorrow.Controls.Add(new Label { Text = "Book ID:", Location = new Point(10, 35), AutoSize = true });
            txtLoanBookId = new TextBox { Location = new Point(70, 32), Width = 50 };
            gbBorrow.Controls.Add(txtLoanBookId);

            gbBorrow.Controls.Add(new Label { Text = "Borrower ID:", Location = new Point(130, 35), AutoSize = true });
            txtLoanBorrowerId = new TextBox { Location = new Point(215, 32), Width = 50 };
            gbBorrow.Controls.Add(txtLoanBorrowerId);

            Button btnBorrow = new Button { Text = "Borrow", Location = new Point(275, 30), Width = 65, BackColor = Color.LightSkyBlue };
            btnBorrow.Click += BtnBorrow_Click;
            gbBorrow.Controls.Add(btnBorrow);
            pnlLoanTop.Controls.Add(gbBorrow);

            // Return section
            GroupBox gbReturn = new GroupBox { Text = "Return a Book", Location = new Point(380, 10), Size = new Size(200, 90) };
            gbReturn.Controls.Add(new Label { Text = "Book ID:", Location = new Point(10, 35), AutoSize = true });
            txtReturnBookId = new TextBox { Location = new Point(70, 32), Width = 50 };
            gbReturn.Controls.Add(txtReturnBookId);

            Button btnReturn = new Button { Text = "Return", Location = new Point(130, 30), Width = 60, BackColor = Color.LightCoral };
            btnReturn.Click += BtnReturn_Click;
            gbReturn.Controls.Add(btnReturn);
            pnlLoanTop.Controls.Add(gbReturn);

            tabLoans.Controls.Add(pnlLoanTop);

            // Add tabs to control
            tabControl.TabPages.Add(tabBooks);
            tabControl.TabPages.Add(tabBorrowers);
            tabControl.TabPages.Add(tabLoans);
            this.Controls.Add(tabControl);
        }

        private DataGridView CreateGrid()
        {
            return new DataGridView
            {
                Dock = DockStyle.Fill,
                AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill,
                AllowUserToAddRows = false,
                AllowUserToDeleteRows = false,
                ReadOnly = true,
                SelectionMode = DataGridViewSelectionMode.FullRowSelect,
                BackgroundColor = Color.White
            };
        }

        private void RefreshData()
        {
            dgvBooks.DataSource = null;
            dgvBooks.DataSource = books;
            
            dgvBorrowers.DataSource = null;
            dgvBorrowers.DataSource = borrowers;

            dgvLoans.DataSource = null;
            dgvLoans.DataSource = loans.Where(l => !l.Returned).ToList();
        }

        private void BtnAddBook_Click(object sender, EventArgs e)
        {
            try
            {
                int id = int.Parse(txtBookId.Text);
                if (books.Any(b => b.Id == id)) { MessageBox.Show("Book ID already exists!"); return; }

                books.Add(new Book
                {
                    Id = id,
                    Title = txtBookTitle.Text,
                    Author = txtBookAuthor.Text,
                    Year = int.Parse(txtBookYear.Text)
                });
                
                txtBookId.Clear(); txtBookTitle.Clear(); txtBookAuthor.Clear(); txtBookYear.Clear();
                RefreshData();
            }
            catch { MessageBox.Show("Invalid input. Please ensure ID and Year are numbers."); }
        }

        private void BtnAddBorrower_Click(object sender, EventArgs e)
        {
            try
            {
                int id = int.Parse(txtBorrowerId.Text);
                if (borrowers.Any(b => b.Id == id)) { MessageBox.Show("Borrower ID already exists!"); return; }

                borrowers.Add(new Borrower
                {
                    Id = id,
                    FullName = txtBorrowerName.Text,
                    Phone = txtBorrowerPhone.Text
                });

                txtBorrowerId.Clear(); txtBorrowerName.Clear(); txtBorrowerPhone.Clear();
                RefreshData();
            }
            catch { MessageBox.Show("Invalid input. Please ensure ID is a number."); }
        }

        private void BtnBorrow_Click(object sender, EventArgs e)
        {
            try
            {
                int bookId = int.Parse(txtLoanBookId.Text);
                int borrowerId = int.Parse(txtLoanBorrowerId.Text);

                Book book = books.FirstOrDefault(b => b.Id == bookId);
                Borrower borrower = borrowers.FirstOrDefault(b => b.Id == borrowerId);

                if (book == null) { MessageBox.Show("Book not found."); return; }
                if (borrower == null) { MessageBox.Show("Borrower not found."); return; }
                if (!book.Available) { MessageBox.Show("Book is already borrowed."); return; }

                book.Available = false;
                loans.Add(new Loan
                {
                    BookItem = book,
                    BorrowerItem = borrower,
                    IssueDate = DateTime.Now,
                    DueDate = DateTime.Now.AddDays(14), // 14-day loan period
                    Returned = false
                });

                txtLoanBookId.Clear(); txtLoanBorrowerId.Clear();
                RefreshData();
                MessageBox.Show(string.Format("Book borrowed successfully! Due: {0}", DateTime.Now.AddDays(14).ToShortDateString()));
            }
            catch { MessageBox.Show("Invalid input. IDs must be numbers."); }
        }

        private void BtnReturn_Click(object sender, EventArgs e)
        {
            try
            {
                int bookId = int.Parse(txtReturnBookId.Text);
                Loan activeLoan = loans.FirstOrDefault(l => l.BookItem.Id == bookId && !l.Returned);

                if (activeLoan == null) { MessageBox.Show("No active loan found for this book."); return; }

                activeLoan.Returned = true;
                activeLoan.BookItem.Available = true;

                txtReturnBookId.Clear();
                RefreshData();
                MessageBox.Show("Book returned successfully!");
            }
            catch { MessageBox.Show("Invalid input. ID must be a number."); }
        }

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new LibraryForm());
        }
    }
}
