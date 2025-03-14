# tizio_query
I've made this library to allow faster management of queries.
I will keep on updating this for a while, since it is currently really basic.

## 📌 Requisites to use
- Needs PHP 7.0+ (because of static typing)
- PDF related functions require the [FPDF](http://www.fpdf.org/?lang=en) library to be imported beforehand

## ⚙️ How to use
Import the .inc file, then change the values of the variables (placed in the first 7 rows of the file):
```
$host = 'localhost';<br>
$password = '';<br>
$user = 'root';<br>
$db = 'tizio_dbName';<br>
```
If needed, override `$conn` using your own script.

## 🔧 What it does
This simple PHP library provides handy functions to work with a MySQL database. It offers several features:

- **Data Saving:** Quickly save data to your database using associative arrays (Really handy function to save data into a database directly from `$_POST`)
- **HTML Table Display:** Display query results in a neatly formatted HTML table, easily editable through css (using the built-in classes.
- **PDF Reporting:** Generate PDF reports using the **FPDF** library. Only generates the table with the report, leaving freedom to editing of the rest of the file.
- **Data Cleaning & Security:** Includes tools for cleaning and securing data before storage (Simple functions that make simple things (like securing an array) faster and more readable).

This lightweight solution is perfect for small projects that need easy database interaction and reporting.