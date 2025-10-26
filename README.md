# OOP Store Simulation (Semester 3 Assignment)

## Project Overview

This repository contains the complete solution for the Object-Oriented Programming (OOP) course assignment from Semester 3. The project is a terminal-based online store simulation managing interactions between multiple buyers and sellers.

The primary objective of this assignment is to implement complex object relationships in C++ and manage data persistence across application sessions using serialization. The entire simulation is operated via a terminal interface.

## Course Information

* **Course:** Object-Oriented Programming (OOP)
* **Program:** S1 Computer Science
* **Institution:** Universitas Negeri Jakarta (UNJ)
* **Period:** Semester 3

### Store Capabilities

* Manages transaction statuses (Paid, Complete, Cancelled).
* Relies on an external banking system for payments.
* Lists all transactions from the last $k$ days.
* Lists all paid transactions that are not yet completed.
* Lists the $m$ most frequently transacted items.
* Lists the most active buyers and sellers by daily transaction count.

### Bank Capabilities

* Lists all transactions within a one-week backward-looking period.
* Lists all bank customers.
* Lists all dormant accounts (no transactions within one month).
* Lists the top $n$ users with the most transactions for today.

### Buyer Capabilities

* Can create a single banking account.
* Can register and log in to the store.
* Access to banking features: Top-up, Withdraw, and list cash flow (daily and monthly).
* Can purchase items, limited by bank balance.
* Can list all personal orders, filterable by status (Paid/Canceled/Completed).
* Can check total spending over the last $k$ days.

### Seller Capabilities

* Includes all features available to a `Buyer`.
* Manages item inventory: register new items, replenish stock, discard items, and set prices.
* Analytics: Can discover the top $k$ most popular items per month and identify loyal customers per month.

## OOP Concepts and Technical Implementation

* **Serialization:** The core technical requirement. Serialization is used to store the internal state of objects to disk, allowing data (users, items, transactions) to persist after the application is closed and re-opened.
* **High-Level Object Relationship:** Designed a hierarchical class structure where classes instantiate other classes (e.g., `Transaction` instantiating `Buyer`). The project also explores how deserialization can create new object references.
* **C++ Constructors:** Utilized colon-based initializer lists for class constructors, especially for reference members like `BankCustomer &account`.
* **Static Variables & Methods:** Implemented static members (variables and methods) to manage data and functionality shared across all instances of a class, such as an ID generator.
* **Dynamic Memory Allocation:** Used `new` and `delete` to manage the lifecycle of objects on the heap, as required for certain class relationships.

## Technology Stack

* **Language:** C++
* **Compiler:** [e.g., g++ / Clang / MSVC]
* **Core Concepts:** C++ Serialization (via `fstream` or similar), STL (`vector`), Pointers, and Class Inheritance.

## Project Status

Completed. This assignment has been finalized and meets all functional requirements. This repository is now archived.
