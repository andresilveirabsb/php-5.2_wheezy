--TEST--
Test stripslashes() function : usage variations - with magic_quotes_sybase directive ON
--FILE--
<?php
/* Prototype  : string stripslashes ( string $str )
 * Description: Returns an un-quoted string
 * Source code: ext/standard/string.c
*/

/*
 * Test stripslashes() with PHP directive magic_quotes_sybase set ON 
*/

echo "*** Testing stripslashes() : with php directive magic_quotes_sybase set ON ***\n";

// setting ON the php directive magic_quotes_sybase
ini_set("magic_quotes_sybase", "1");

// initialising a heredoc string
$heredoc_string = <<<EOT
This is line 1 of 'heredoc' string
This is line 2 of "heredoc" string
EOT;

$heredoc_null_string =<<<EOT
EOT;
 
// initialising the string array
$str_array = array( 
                    // string without any characters that can be backslashed
                    'Hello world',
 
                    // string with single quotes
                    "how're you doing?", 
                    "don't disturb u'r neighbours",
                    "don't disturb u'r neighbours''",
                    '',
                    '\'',
                    "'",
                    
                    // string with double quotes
                    'he said, "he will be on leave"',
                    'he said, ""he will be on leave"',
                    '"""PHP"""',
                    "",
                    "\"",
                    '"',
 		    "hello\"",
                         
                    // string with backslash characters
                    'Is your name Ram\Krishna?',
                    'c:\php\testcase\stripslashes',
                    '\\',

                    // string with nul characters
                    'hello'.chr(0).'world',
                    chr(0).'hello'.chr(0),
                    chr(0).chr(0).'hello',
                    chr(0),

                    // mixed strings
                    "\\\"'0.0.0.0'",
                    "\\\"'0.0.0.0'".chr(0),
                    chr(0)."'c:\php\'",
                    '"c:\php\"'.chr(0)."'",
                    '"hello"'."'world'".chr(0).'//',

		    // string with hexadecimal number
                    "0xABCDEF0123456789",
                    "\xabcdef0123456789",
                    '!@#$%&*@$%#&/;:,<>',
                    "hello\x00world",

                    // heredoc strings
                    $heredoc_string,
                    $heredoc_null_string
                  );

$count = 1;
// looping to test for all strings in $str_array
foreach( $str_array as $str )  {
  echo "\n-- Iteration $count --\n";
  $str_addslashes = addslashes($str);
  var_dump("The string after addslashes is:", $str_addslashes);
  $str_stripslashes = stripslashes($str_addslashes);
  var_dump("The string after stripslashes is:", $str_stripslashes);
  if( strcmp($str, $str_stripslashes) != 0 )
    echo "\nOriginal string and string from stripslashes() donot match\n";
  $count ++;
}

echo "Done\n";
?>
--EXPECTF--
*** Testing stripslashes() : with php directive magic_quotes_sybase set ON ***

-- Iteration 1 --
string(31) "The string after addslashes is:"
string(11) "Hello world"
string(33) "The string after stripslashes is:"
string(11) "Hello world"

-- Iteration 2 --
string(31) "The string after addslashes is:"
string(18) "how''re you doing?"
string(33) "The string after stripslashes is:"
string(17) "how're you doing?"

-- Iteration 3 --
string(31) "The string after addslashes is:"
string(30) "don''t disturb u''r neighbours"
string(33) "The string after stripslashes is:"
string(28) "don't disturb u'r neighbours"

-- Iteration 4 --
string(31) "The string after addslashes is:"
string(34) "don''t disturb u''r neighbours''''"
string(33) "The string after stripslashes is:"
string(30) "don't disturb u'r neighbours''"

-- Iteration 5 --
string(31) "The string after addslashes is:"
string(0) ""
string(33) "The string after stripslashes is:"
string(0) ""

-- Iteration 6 --
string(31) "The string after addslashes is:"
string(2) "''"
string(33) "The string after stripslashes is:"
string(1) "'"

-- Iteration 7 --
string(31) "The string after addslashes is:"
string(2) "''"
string(33) "The string after stripslashes is:"
string(1) "'"

-- Iteration 8 --
string(31) "The string after addslashes is:"
string(30) "he said, "he will be on leave""
string(33) "The string after stripslashes is:"
string(30) "he said, "he will be on leave""

-- Iteration 9 --
string(31) "The string after addslashes is:"
string(31) "he said, ""he will be on leave""
string(33) "The string after stripslashes is:"
string(31) "he said, ""he will be on leave""

-- Iteration 10 --
string(31) "The string after addslashes is:"
string(9) """"PHP""""
string(33) "The string after stripslashes is:"
string(9) """"PHP""""

-- Iteration 11 --
string(31) "The string after addslashes is:"
string(0) ""
string(33) "The string after stripslashes is:"
string(0) ""

-- Iteration 12 --
string(31) "The string after addslashes is:"
string(1) """
string(33) "The string after stripslashes is:"
string(1) """

-- Iteration 13 --
string(31) "The string after addslashes is:"
string(1) """
string(33) "The string after stripslashes is:"
string(1) """

-- Iteration 14 --
string(31) "The string after addslashes is:"
string(6) "hello""
string(33) "The string after stripslashes is:"
string(6) "hello""

-- Iteration 15 --
string(31) "The string after addslashes is:"
string(25) "Is your name Ram\Krishna?"
string(33) "The string after stripslashes is:"
string(25) "Is your name Ram\Krishna?"

-- Iteration 16 --
string(31) "The string after addslashes is:"
string(28) "c:\php\testcase\stripslashes"
string(33) "The string after stripslashes is:"
string(28) "c:\php\testcase\stripslashes"

-- Iteration 17 --
string(31) "The string after addslashes is:"
string(1) "\"
string(33) "The string after stripslashes is:"
string(1) "\"

-- Iteration 18 --
string(31) "The string after addslashes is:"
string(12) "hello\0world"
string(33) "The string after stripslashes is:"
string(11) "hello world"

-- Iteration 19 --
string(31) "The string after addslashes is:"
string(9) "\0hello\0"
string(33) "The string after stripslashes is:"
string(7) " hello "

-- Iteration 20 --
string(31) "The string after addslashes is:"
string(9) "\0\0hello"
string(33) "The string after stripslashes is:"
string(7) "  hello"

-- Iteration 21 --
string(31) "The string after addslashes is:"
string(2) "\0"
string(33) "The string after stripslashes is:"
string(1) " "

-- Iteration 22 --
string(31) "The string after addslashes is:"
string(13) "\"''0.0.0.0''"
string(33) "The string after stripslashes is:"
string(11) "\"'0.0.0.0'"

-- Iteration 23 --
string(31) "The string after addslashes is:"
string(15) "\"''0.0.0.0''\0"
string(33) "The string after stripslashes is:"
string(12) "\"'0.0.0.0' "

-- Iteration 24 --
string(31) "The string after addslashes is:"
string(13) "\0''c:\php\''"
string(33) "The string after stripslashes is:"
string(10) " 'c:\php\'"

-- Iteration 25 --
string(31) "The string after addslashes is:"
string(13) ""c:\php\"\0''"
string(33) "The string after stripslashes is:"
string(11) ""c:\php\" '"

-- Iteration 26 --
string(31) "The string after addslashes is:"
string(20) ""hello"''world''\0//"
string(33) "The string after stripslashes is:"
string(17) ""hello"'world' //"

-- Iteration 27 --
string(31) "The string after addslashes is:"
string(18) "0xABCDEF0123456789"
string(33) "The string after stripslashes is:"
string(18) "0xABCDEF0123456789"

-- Iteration 28 --
string(31) "The string after addslashes is:"
string(15) "?cdef0123456789"
string(33) "The string after stripslashes is:"
string(15) "?cdef0123456789"

-- Iteration 29 --
string(31) "The string after addslashes is:"
string(18) "!@#$%&*@$%#&/;:,<>"
string(33) "The string after stripslashes is:"
string(18) "!@#$%&*@$%#&/;:,<>"

-- Iteration 30 --
string(31) "The string after addslashes is:"
string(12) "hello\0world"
string(33) "The string after stripslashes is:"
string(11) "hello world"

-- Iteration 31 --
string(31) "The string after addslashes is:"
string(71) "This is line 1 of ''heredoc'' string
This is line 2 of "heredoc" string"
string(33) "The string after stripslashes is:"
string(69) "This is line 1 of 'heredoc' string
This is line 2 of "heredoc" string"

-- Iteration 32 --
string(31) "The string after addslashes is:"
string(0) ""
string(33) "The string after stripslashes is:"
string(0) ""
Done
