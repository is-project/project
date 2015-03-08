<?php

function valid_int($int, $min = false, $max = false) {
	if (is_numeric($int) === TRUE && (int)$int == $int) {
		return TRUE;
	}
	return FALSE;
}

?>