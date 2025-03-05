# tizio_query
I've made this library to allow faster management of queries.
To update, since it is currently really basic.

## How to use
Import the .inc file, then change the values of the variables (placed in the first 7 rows of the file):

$host = 'localhost';
$password = '';
$user = 'root';
$db = 'tizio_dbName';
Into whatever your database uses.

If needed, override $conn using your own script.

## What it does
Allows to print in tables fast starting from queries, same goes for saving data starting from an array.
