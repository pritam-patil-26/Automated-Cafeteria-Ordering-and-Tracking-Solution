# Automated-Cafeteria-Ordering-and-Tracking-Solution
An automated system designed to streamline cafeteria operations by integrating digital ordering, inventory management, and sales tracking. It reduces manual errors, improves efficiency, and enables real-time monitoring for faster service and better customer experience.




☕ Cafe Ordering and Tracking System (C++)

A console-based Cafe Management System developed in C++ that allows customers to order food, reserve tables, and manage billing, while admins can manage menu items and view daily sales.

🚀 Features
👤 User Roles
Admin
View all menu items
Add new menu items
Modify price and availability
View table status
View daily sales logs
Customer
Select cafe
Reserve table
View menu
Order food items
View current order
Generate and pay bill
🧱 System Design

The system follows Object-Oriented Programming (OOP) concepts:

Classes Used:
MenuItem → Stores item details (id, name, price, category)
Order → Handles order items, total calculation, billing
Table → Manages table availability and assignment
Cafe → Core system (menu, tables, orders, billing)
User → Base class for login system
Admin → Derived class with admin privileges
Customer → Derived class for customer operations
UserManager → Handles authentication
💻 Technologies Used
C++
STL (Vector, Map)
File Handling (fstream)
OOP Concepts:
Encapsulation
Inheritance
Polymorphism
Abstraction
