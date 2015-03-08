$(document).ready(function(){
	
	$( "form#login input" ).focus(function() {
		loginEnterField($(this).attr('name'));
	});

	$( "form#login input" ).focusout(function() {
		loginLeaveField($(this).attr('name'));
	});

	$( "form#login" ).submit(function() {
	  $( "form#login input#hashedpw" ).val( hex_sha512($( "form#login input#pw" ).val()) );
	  $( "form#login input#pw" ).val("");
	});

});

function loginEnterField(field) {
	if(field != 'email' && field != 'pw') return;
	
	if ( (field == 'email' && $( "form#login input#" + field ).val() == 'E-Mail') || (field == 'pw' && $( "form#login input#" + field ).val() == 'password') )
		$( "form#login input#" + field ).val("");

}

function loginLeaveField(field) {
	if(field != 'email' && field != 'pw') return;
	
	if(!$( "form#login input#" + field ).is(":focus"))
		if($( "form#login input#" + field ).val() == '')
			$( "form#login input#" + field ).val((field == 'pw') ? "password" : "E-Mail");
}