--TEST--
foa_get_mode() function
--SKIPIF--
<?php if (!extension_loaded("foa")) print "skip"; ?>
--FILE--
<?php 
$summary = '';

foa_set_mode(FOA_MODE_ESCAPE, true);
$result = foa_get_mode(FOA_MODE_ESCAPE);
$summary .= $result ? 1 : 0;

foa_set_mode(FOA_MODE_ESCAPE, false);
$result = foa_get_mode(FOA_MODE_ESCAPE);
$summary .= $result ? 1 : 0;

foa_set_mode(FOA_MODE_HASHES, true);
$result = foa_get_mode(FOA_MODE_HASHES);
$summary .= $result ? 1 : 0;

foa_set_mode(FOA_MODE_HASHES, false);
$result = foa_get_mode(FOA_MODE_HASHES);
$summary .= $result ? 1 : 0;

var_dump($summary)
?>
--EXPECT--
string(4) "1010"
