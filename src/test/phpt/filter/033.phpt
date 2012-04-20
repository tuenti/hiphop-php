--TEST--
Test all filters returned by filter_list()
--SKIPIF--
<?php if (!extension_loaded("filter")) die("skip"); ?>
--INI--
precision=14
default_charset=UTF-8
--FILE--
<?php
if (function_exists('setlocale')) {
	setlocale(LC_ALL, 'C');
}

function test($data) {
  return strtoupper($data);
}

$data = array(
	"PHP",
	"1",
	"foo@bar.com",
	"http://a.b.c",
	"1.2.3.4",
	"123",
	"123abc<>()",
	"O'Henry",
	"하퍼"
);

foreach(filter_list() as $filter) {
	if($filter=="validate_regexp") {
		foreach($data as $k=>$d) $result[$k] = filter_var($d,filter_id($filter),array("options"=>array("regexp"=>'/^O.*/')));
	} else {
		foreach($data as $k=>$d) {
            $result[$k] = filter_var($d,filter_id($filter),array("options"=>"test"));
        }
	}
	printf("%-20s",$filter);
	printf("%-5s",$result[0]);
	printf("%-3s",$result[1]);
	printf("%-15s",$result[2]);
	printf("%-20s",$result[3]);
	printf("%-10s",$result[4]);
	printf("%-5s",$result[5]);
	printf("%-20s",$result[6]);
	printf("%-15s",$result[7]);
	printf("%-10s\n",$result[8]);
}
?>
--EXPECT--     
int                      1                                               123                                               
boolean                  1                                                                                                 
float                    1                                               123                                               
validate_regexp                                                                                   O'Henry                  
validate_url                               http://a.b.c                                                                    
validate_email              foo@bar.com                                                                                    
validate_ip                                                    1.2.3.4                                                     
string              PHP  1  foo@bar.com    http://a.b.c        1.2.3.4   123  123abc()            O&#39;Henry    하퍼    
stripped            PHP  1  foo@bar.com    http://a.b.c        1.2.3.4   123  123abc()            O&#39;Henry    하퍼    
encoded             PHP  1  foo%40bar.com  http%3A%2F%2Fa.b.c  1.2.3.4   123  123abc%3C%3E%28%29  O%27Henry      %ED%95%98%ED%8D%BC
special_chars       PHP  1  foo@bar.com    http://a.b.c        1.2.3.4   123  123abc&#60;&#62;()  O&#39;Henry    하퍼    
full_special_chars  PHP  1  foo@bar.com    http://a.b.c        1.2.3.4   123  123abc&lt;&gt;()    O&#039;Henry   하퍼    
unsafe_raw          PHP  1  foo@bar.com    http://a.b.c        1.2.3.4   123  123abc<>()          O'Henry        하퍼    
email               PHP  1  foo@bar.com    httpa.b.c           1.2.3.4   123  123abc              O'Henry                  
url                 PHP  1  foo@bar.com    http://a.b.c        1.2.3.4   123  123abc<>()          O'Henry                  
number_int               1                                     1234      123  123                                          
number_float             1                                     1234      123  123                                          
magic_quotes        PHP  1  foo@bar.com    http://a.b.c        1.2.3.4   123  123abc<>()          O\'Henry       하퍼    
