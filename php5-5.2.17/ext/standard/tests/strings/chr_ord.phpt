--TEST--
Test chr() and ord() functions
--FILE--
<?php
/* Prototype: string chr ( int $ascii );
   Description: Returns a one-character string containing the character specified by ascii. 

   Prototype: int ord ( string $string );
   Description: Returns the ASCII value of the first character of string
*/
echo "*** Testing ord() & chr() basic operations ***\n";
for($i=0; $i<256; $i++) echo !ord(chr($i)) == $i;

/* miscelleous input */
echo "\n*** Testing chr() usage variations ***\n";
$arr_test = array( 
  "true", 
  "false",
  true,
  false,
  "",             
  " ",           
  "a",
  299,
  321,
  NULL,
  '\0',
  "0",
  -312, 
  12.999,
  -1.05009,
  1100011,
  "aaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddddddddddddd",
  "abcd\nabcd\tabcd\0abcd\rabcdNULLabcdefgh",
  "abcd\x00abcd\x00abcd\x00abcdefghij",
);
$counter=1;
foreach($arr_test as $var){
  echo "-- Iteration $counter --\n";
  var_dump( chr($var) );
  $counter++;
}

echo "\n*** Testing ord() usage variations ***\n";
$counter=1;
foreach($arr_test as $var){
  echo "-- Iteration $counter --\n";
  var_dump( ord($var) );
  $counter++;
}

/* Error conditions */
echo "\n*** Testing chr() error conditions ***\n";
//zero arguments
var_dump( chr() ); 
// more than expected no. of args
var_dump( chr($arr_test[0], $arr_test[1]) ); 


echo "\n*** Testing ord() error conditions ***\n";
// zero arguments
var_dump( ord() ); 
// more than expected no. of args
var_dump( ord($arr_test[0], $arr_test[1]) ); 

echo "Done\n";
?>
--EXPECTF--
*** Testing ord() & chr() basic operations ***

*** Testing chr() usage variations ***
-- Iteration 1 --
string(1) " "
-- Iteration 2 --
string(1) " "
-- Iteration 3 --
string(1) ""
-- Iteration 4 --
string(1) " "
-- Iteration 5 --
string(1) " "
-- Iteration 6 --
string(1) " "
-- Iteration 7 --
string(1) " "
-- Iteration 8 --
string(1) "+"
-- Iteration 9 --
string(1) "A"
-- Iteration 10 --
string(1) " "
-- Iteration 11 --
string(1) " "
-- Iteration 12 --
string(1) " "
-- Iteration 13 --
string(1) "?"
-- Iteration 14 --
string(1) ""
-- Iteration 15 --
string(1) "?"
-- Iteration 16 --
string(1) "?"
-- Iteration 17 --
string(1) " "
-- Iteration 18 --
string(1) " "
-- Iteration 19 --
string(1) " "

*** Testing ord() usage variations ***
-- Iteration 1 --
int(116)
-- Iteration 2 --
int(102)
-- Iteration 3 --
int(49)
-- Iteration 4 --
int(0)
-- Iteration 5 --
int(0)
-- Iteration 6 --
int(32)
-- Iteration 7 --
int(97)
-- Iteration 8 --
int(50)
-- Iteration 9 --
int(51)
-- Iteration 10 --
int(0)
-- Iteration 11 --
int(92)
-- Iteration 12 --
int(48)
-- Iteration 13 --
int(45)
-- Iteration 14 --
int(49)
-- Iteration 15 --
int(45)
-- Iteration 16 --
int(49)
-- Iteration 17 --
int(97)
-- Iteration 18 --
int(97)
-- Iteration 19 --
int(97)

*** Testing chr() error conditions ***

Warning: Wrong parameter count for chr() in %s on line %d
NULL

Warning: Wrong parameter count for chr() in %s on line %d
NULL

*** Testing ord() error conditions ***

Warning: Wrong parameter count for ord() in %s on line %d
NULL

Warning: Wrong parameter count for ord() in %s on line %d
NULL
Done
