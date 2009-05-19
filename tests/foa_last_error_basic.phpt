--TEST--
foa_last_error(), foa_reset_error() and foa_has_error() functions
--SKIPIF--
<?php if (!extension_loaded("foa")) print "skip"; ?>
--FILE--
<?php 
$result = '';
// This buffer should generate parse error:
$buffer = "([])";

foa_set_buffer($buffer);
$result .= foa_has_error() ? "0" : "1";

foa_decode();
$result .= foa_has_error() ? "1" : "0";

$error = foa_last_error();
if(isset($error) && strlen($error) != 0)  {
    $result .= "1";
} else {
    $result .= "0";
}

foa_reset_error();
$result .= foa_has_error() ? "0" : "1";
$result .= foa_last_error() != null ? "0" : "1";
var_dump($result);
?>
--EXPECT--
string(5) "11111"
