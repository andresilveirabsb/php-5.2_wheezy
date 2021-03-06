--TEST--
mb_send_mail() test 5 (lang=Simplified Chinese)
--SKIPIF--
<?php
if (!function_exists("mb_send_mail") || !mb_language("Simplified Chinese")) {
	die("skip mb_send_mail() not available");
}
if (!@mb_internal_encoding('GB2312')) {
	die("skip GB2312 encoding is not available on this platform");
}
?>
--INI--
sendmail_path=/bin/cat
--FILE--
<?php
$to = 'example@example.com';

/* default setting */
mb_send_mail($to, mb_language(), "test");

/* Simplified Chinese (HK-GB-2312) */
if (mb_language("simplified chinese")) {
	mb_internal_encoding('GB2312');
	mb_send_mail($to, "???? ".mb_language(), "????");
}
?>
--EXPECTF--
To: example@example.com
Subject: %s
Mime-Version: 1.0
Content-Type: text/plain; charset=%s
Content-Transfer-Encoding: %s

%s
To: example@example.com
Subject: =?HZ-GB-2312?B?fnsyYlFpfn0gU2ltcGxpZmllZCBD?=
 =?HZ-GB-2312?B?aGluZXNl?=
Mime-Version: 1.0
Content-Type: text/plain; charset=HZ-GB-2312
Content-Transfer-Encoding: 7bit

~{2bQi~}
