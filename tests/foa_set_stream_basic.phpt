--TEST--
foa_set_stream() function
--SKIPIF--
<?php if (!extension_loaded("foa")) print "skip"; ?>
--FILE--
<?php 
$stream = tmpfile();
foa_set_stream($stream);
fclose($stream);
?>
--EXPECT--
