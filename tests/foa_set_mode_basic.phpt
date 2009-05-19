--TEST--
foa_set_mode() function
--SKIPIF--
<?php if (!extension_loaded("foa")) print "skip"; ?>
--FILE--
<?php 
foa_set_mode(FOA_MODE_ESCAPE, true);
foa_set_mode(FOA_MODE_ESCAPE, false);
foa_set_mode(FOA_MODE_HASHES, true);
foa_set_mode(FOA_MODE_HASHES, false);
?>
--EXPECT--
