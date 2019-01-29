--TEST--
foa_set_buffer() function
--SKIPIF--
<?php if (!extension_loaded("foa")) print "skip"; ?>
--FILE--
<?php
$buffer = "hello world";
foa_set_buffer($buffer);

?>
--EXPECT--
