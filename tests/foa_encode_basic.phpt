--TEST--
foa_encode() function
--SKIPIF--
<?php if (!extension_loaded("foa")) print "skip"; ?>
--FILE--
<?php 
$result = '';
$result .= foa_encode(FOA_TYPE_START_OBJECT, "persons");
$result .= foa_encode(FOA_TYPE_START_ARRAY);
$result .= foa_encode(FOA_TYPE_DATA_ENTITY, "name", "Adam");
$result .= foa_encode(FOA_TYPE_DATA_ENTITY, "name", "Bertil");
$result .= foa_encode(FOA_TYPE_END_ARRAY);
$result .= foa_encode(FOA_TYPE_END_OBJECT);
var_dump($result);
?>
--EXPECT--
string(44) "persons = (
[
name = Adam
name = Bertil
]
)
"
