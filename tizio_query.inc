<?php

// All the variables needed for the connection, CHANGE WHERE NEEDED.
$host = 'localhost';
$password = '';
$user = 'root';
$db = 'tizio_dbName';
$conn = mysqli_connect($host, $user, $password, $db);

/**
 * Saves all the data inside the SQL database.
 * @param mysqli $conn is the connection to the database
 * @param String $tabName the name of the table to which you add values
 * @param Array $assArray An associative array with all the attributes and values to add to the table
 * @return Boolean True or false based on if it was a successful operation or not
 */

function save_data(mysqli $conn, string $tabName, array $assArray): bool {
    // Adds the keys to the command
    $command = "INSERT INTO $tabName(";
    $i = 0;
    foreach($assArray as $key => $value) {
        if($i++) $command .= ","; // If it's not the first loop, add ','
        $command .= "$key";
        // Format:
        // table($key,$key,$key)
    } // Foreach key
    // Adds the values to the command
    $i = 0;
    $command .= ") VALUES(";
    foreach($assArray as $value) { // For each value passed
        $isNumber = gettype($value) != 'integer' && gettype($value) != 'double';
        if($i++) $command .= ','; // If it's not the first loop, add ','
        // If the value is not a number, add '
        if ($isNumber) $command .= "'";
        $command .= "$value";
        if ($isNumber) $command .= "'";
        // Format:
        // VALUES(value,'strvalue','value')
    } // Foreach value

    $command .= ")"; // Close the command

    $query = mysqli_query($conn, $command);
    if ($query) { // If query was successful
        return true;
    } else {
        return false;
    }
} // save_data

/**
 * Prints all the data that comes from a query in a table <br>
 * \<TABLE\> uses class "tizio_formattedTable" <br>
 * \<TR\> uses class "tizio_formattedTr" <br>
 * \<TH\> uses class "tizio_formattedTh" <br>
 * \<TD\> uses class "tizio_formattedTd"
 * @param mysqli $conn is the connection to the database
 * @param String $queryString A string of the query
 * @param string $noResultMessage Is the message that will be printed in case no results appear
 * @param string $failMessage Is the message that will be printed in case some error happens.
 * @return int The number of results of the query
 */
function print_query(mysqli $conn, string $queryString,
                            string $noResultMessage = "No results found according to the search criteria...",
                            string $failMessage = "The query could not be completed..."): int {

    $query = mysqli_query($conn, $queryString);

    if ($query) {
        $resultsNum = mysqli_num_rows($query);
        if ($resultsNum != 0) { // If there is at least 1 result
            $i = 0;
            echo "<table class='tizio_formattedTable'>";
            while($row = mysqli_fetch_assoc($query)) {
                echo "<tr class='tizio_formattedTr'>";
                if($i++ == 0) { // Only on the first cycle (Header)
                    foreach($row as $key => $value) { // Prints each key
                        echo "<th class='tizio_formattedTh'>$key</th>";
                    }
                    echo "</tr><tr>";
                } // If first cycle

                foreach($row as $value) { // Prints all the values (Body)
                    echo "<td class='tizio_formattedTd'>$value</td>";
                } // foreach
                echo "</tr>";
            } // while
            echo "</table>";
            return $resultsNum;
        } else { // If the result is empty
            echo "<i>$noResultMessage</i>";
        }
    } else {
        echo "<i>$failMessage</i>";
    } // else closes
    return 0;
} // print_query function

/**
 * Trims all the strings and makes all the first letters in the strings in the array capital
 * @param Array $assArray The array to format
 * @return Array The new formatted array
 */
function format_array(array $assArray): array {
    $newArray = array();
    foreach($assArray as $key => $value) {
        $newArray[$key] =  trim(ucwords($value));
    }
    return $newArray;
}

/**
 * Runs mysqli_real_escape_string and stripslashes for each value in the array.
 * @param $assArray Array The array to secure
 * @return Array The new secured array
 */
function secure_array(mysqli $conn, array $assArray): array {
    $newArray = array();
    foreach($assArray as $key => $value) {
        $newArray[$key] =  mysqli_real_escape_string($conn, stripslashes($value));
    }
    return $newArray;
}


/**
 * <b>Has to be run before anything gets written into the page <br>
 * Also requires the library "FPDF" to be imported beforehand </b><br>
 * <br>
 * Will add a table with the result of the query to the pdf <br>
 * This function does not add any pages, it just prints the results of the query
 * @param mysqli $conn is the connection to the database (not the object)
 * @param String $queryString A string of the query
 * @param FPDF $pdf The pdf file in which the query will be printed
 * @param int $margin Decides the margin of the table. If -1, center the table.
 * @param int $cellWidth The width of a cell (crazy right?)
 * @param int $fontSize The size of the fonts in the table. The header font will be 1.25x
 * @param int $border The size of the border
 * @param string $noResultMessage Is the message that will be printed in case no results appear
 * @param string $failMessage Is the message that will be printed in case some error happens.
 */
function pdf_query(mysqli $conn, $queryString, FPDF $pdf, int $margin = -1, int $cellWidth = 60, int $fontSize = 12, int $border = 1,
                          string $noResultMessage = "No results found according to the search criteria...",
                          string $failMessage = "The query could not be completed..."): FPDF {

    if (class_exists("FPDF")) { // Checks if FPDF has been correctly imported

        $query = mysqli_query($conn, $queryString);

        if ($query) {
            $resultNum = mysqli_num_rows($query);
            if ($resultNum != 0) { // If there is at least 1 result

                if ($margin == -1) { // Calculates the margin
                    $colNum = mysqli_num_fields($query);
                    $tableWidth = $colNum * $cellWidth;
                    $pageWidth = $pdf->GetPageWidth();
                    $margin = ($pageWidth - $tableWidth) / 2;
                }

                $i = 0;
                while($row = mysqli_fetch_assoc($query)) {
                    if($i++ == 0) { // Only in the first cycle
                        $pdf->SetX($margin);
                        $pdf->setfont('Arial','B',$fontSize*1.25); // Sets the font of the header
                        foreach($row as $key => $value) { // Sets the table header
                            $pdf->cell($cellWidth, 10, $key, $border, 0, 'C');
                        }
                        $pdf->ln(); // New line (only first cycle)
                    } // Close if first cycle
                    $pdf->SetX($margin);
                    $pdf->setfont('Arial','',$fontSize); // Sets the font of the body
                    foreach($row as $value) { // For each value in the query
                        $pdf->cell($cellWidth, 10, $value, $border, 0, 'C');
                    } // foreach
                    $pdf->ln(); // New line
                } // while

                // Returns the pdf file
                return $pdf;

            } else { // In case the result is empty
                echo "<i>$noResultMessage</i>";
            }
        } else {
            echo "<i>$failMessage</i>";
        } // else

    } else { // In case the library for pdf hasn't been imported, give an error
        die("FPDF HAS NOT BEEN IMPORTED!");
    }

    return $pdf; // In case something goes wrong, return the pdf unchanged
} // PDF query function
?>