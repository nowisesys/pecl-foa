--TEST--
foa_decode() function
--SKIPIF--
<?php if (!extension_loaded("foa")) print "skip"; ?>
--FILE--
<?php 
$result = '';
$buffer = "object = (\npersons = [\nname = Adam\nname = Bertil\n]\n)\n";
foa_set_buffer($buffer);

$entity = foa_decode();
if($entity['type'] == FOA_TYPE_START_OBJECT && 
   $entity['name'] == "object" && 
   $entity['data'] == "(") {
    $result .= "1";
} else {
    $result .= "0";
}

$entity = foa_decode();
if($entity['type'] == FOA_TYPE_START_ARRAY && 
   $entity['name'] == "persons" && 
   $entity['data'] == "[") {
    $result .= "1";
} else {
    $result .= "0";
}

$entity = foa_decode();
if($entity['type'] == FOA_TYPE_DATA_ENTITY && 
   $entity['name'] == "name" && 
   $entity['data'] == "Adam") {
    $result .= "1";
} else {
    $result .= "0";
}

$entity = foa_decode();
if($entity['type'] == FOA_TYPE_DATA_ENTITY && 
   $entity['name'] == "name" && 
   $entity['data'] == "Bertil") {
    $result .= "1";
} else {
    $result .= "0";
}

$entity = foa_decode();
if($entity['type'] == FOA_TYPE_END_ARRAY && 
   $entity['data'] == "]" && 
   !isset($entity['name'])) { 
    $result .= "1";
} else {
    $result .= "0";
}

$entity = foa_decode();
if($entity['type'] == FOA_TYPE_END_OBJECT && 
   $entity['data'] == ")" && 
   !isset($entity['name'])) {
    $result .= "1";
} else {
    $result .= "0";
}
var_dump($result);
?>
--EXPECT--
string(6) "111111"
