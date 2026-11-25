# LABLEDGER
An offline C-based laboratory asset management system for equipment inventory, allocation, borrowing, returns and availability tracking.
### Laboratory Asset Lifecycle Management System

This is a lightweight, offline laboratory asset management system developed using the C programming language. It is designed to simplify laboratory equipment management through inventory tracking, equipment allocation, borrowing and return management, and availability monitoring.

The system provides a simple command-line interface and uses file handling to maintain records even after the application is closed.

## 🎯 Project Objective

To develop an efficient and reliable system that simplifies laboratory resource management, reduces manual record-keeping, and improves equipment availability tracking.

## 🚀 Key Features

- **Equipment Inventory:** View all available laboratory equipment and quantities.
- **Asset Registration:** Add new equipment to the inventory.
- **Equipment Allocation:** Record equipment borrowed by individuals.
- **Return Management:** Update equipment availability when items are returned.
- **Active Loan Tracking:** Monitor currently borrowed equipment.
- **Equipment Search:** Search for equipment by name.
- **Inventory Summary:** View total, available, and borrowed equipment quantities.
- **Persistent Storage:** Store inventory and borrowing records using text files.

## 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C Programming | Core application development |
| Structures | Organizing equipment and loan records |
| Arrays | Managing multiple records |
| Functions | Modular program implementation |
| File Handling | Persistent data storage |
| String Handling | Equipment and borrower information |

## ⚙️ System Requirements

- GCC Compiler
- Windows / Linux / macOS
- Command-line terminal
- Basic C development environment

## ▶️ How to Run

**Step 1: Clone the repository**

```bash
git clone https://github.com/YOUR-USERNAME/LABLEDGER.git
```

**Step 2: Navigate to the project directory**

```bash
cd LABLEDGER
```

**Step 3: Compile the program**

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c -o labledger
```

**Step 4: Execute the application**

Windows:

```bash
labledger.exe
```

Linux / macOS:

```bash
./labledger
```

## 📂 Project Structure

```text
Labledger/
│
├── main.c
├── README.md
├── Makefile
├── .gitignore
│
└── data/
    └── .gitkeep
```

The application automatically creates the inventory and loan data files when required.

## 💡 Working Principle

1. The application loads existing inventory and loan records.
2. Users can view or register laboratory equipment.
3. Borrowing requests are recorded against a unique loan ID.
4. Available equipment quantities are automatically updated.
5. Returned equipment is added back to the available inventory.
6. Updated records are stored in local text files.

## 🔮 Future Enhancements

- Due-date and overdue tracking.
- Equipment condition monitoring.
- Equipment category management.
- CSV report generation.
- Improved user interface.
- User authentication and access management.

## 👨‍💻 Project Information

**Project Name:** Labledger

**Project Category:** Systems Programming / Inventory Management

**Programming Language:** C

**Application Type:** Offline Command-Line Application

---

*Labledger — Simplifying Laboratory Resource Management Through C Programming.*
