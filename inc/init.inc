<?php
if(!file_exists ('config.php')) {
	die('File "config.php" not found.');
}
require_once 'config.php';
require_once 'inc/bo_user.class.inc';

$db = mysql_connect(DB_HOST, DB_USER, DB_PASS) or die("Could not connect to mysql server.");
mysql_select_db(DB_NAME, $db) or die('Could not select database.');

// TODO: Session & Login und so
global $current_user;
secSessionStart();

if( isset($_POST['login']) && $_POST['login'] == 'login' )
	login();

if( isset($_POST['logout']) && $_POST['logout'] == 'logout' )
	logout();

if(checkLogin())
	$current_user = new bo_user($_SESSION['user']);
else
	$current_user = new bo_user();


function _die($msg, $error = false) {
	ob_clean();

	if($error !== FALSE) {
		// TODO write Error in log DB table or file or whatever
	}

	$html = '<div style="background: red;">'.$msg.'</div>';
	die($html);
}

/*
 * function to start a secure session
 */
function secSessionStart($ajax = false) {
        $session_name = 'wgSession'; // Set a custom session name
        $secure = false; // Set to true if using https.
        $httponly = true; // This stops javascript being able to access the session id. 
 
        ini_set('session.use_only_cookies', 1); // Forces sessions to only use cookies. 
        $cookieParams = session_get_cookie_params(); // Gets current cookies params.
        session_set_cookie_params($cookieParams["lifetime"], $cookieParams["path"], $cookieParams["domain"], $secure, $httponly); 
        session_name($session_name); // Sets the session name to the one set above.
        session_start(); // Start the php session
        if( !$ajax )
        	session_regenerate_id(true); // regenerated the session, delete the old one.     
}

/*
 * function to login a user
  * 
 * @return
 *		true / false (login OK or failed)
 */
function login() {

	// check if the login form is sent with all values
	if(isset($_POST['email'], $_POST['pw'], $_POST['hashedpw'])) {

		// pw must be emtpy AND hashedpw must be 128 char long
		if($_POST['pw'] != '' || strlen($_POST['hashedpw']) != 128) return false;

		// escape $_POST data
		$email = mysql_real_escape_string($_POST['email']);
		$pw = mysql_real_escape_string($_POST['hashedpw']);

		// get the user from DB
		$result = mysql_query('SELECT * FROM `users` WHERE `email` = \''.$email.'\';') or die(mysql_error());
		
		// check if user is available
		if(mysql_num_rows($result)) {
		 	$result = mysql_fetch_array($result);
		 	// hash already prehashed pw and salt together to match DB entry
		 	if($pw == $result['password']) {
		 		// password correct, do the login stuff
		 		$user_browser = $_SERVER['HTTP_USER_AGENT'];

		 		// set session uid and email
		 		$_SESSION['user'] = $result['user'];
		 		// set session loginString containing the hashed PW and the user Browser to prevent session hijacking
		 		$_SESSION['login_string'] = hash('sha512', $pw.$user_browser);
				// header ("LOCATION: home.php");
				global $global_toasts;
				$global_toasts['info'][] = 'Welcome &laquo; '.$result['name'].' &raquo;';
				return true;				
		 	} else {
		 		// password wrong, write attemp in DB
		 		#mysql_query('INSERT INTO login_attempts (user) VALUES (\''.$result['user'].'\')');
		 		global $global_toasts;
		 		$global_toasts['error'][] = '##Login Error. Wrong E-Mail / password?##';
		 		return false;
		 	}
		}
		// email not found
		return false;
			
	} else {
		// not all values from the login form are given
		return false;
	}
}

/*
 * function to check if the user is correctly logged in
 * 
 * @return
 *		true / false
 */
function checkLogin() {
	// check needed session variables
	if(isset($_SESSION['user'], $_SESSION['login_string'])) {
		// get the userdata from DB
		$result = mysql_query('SELECT * FROM `users` WHERE `user` = \''.$_SESSION['user'].'\';') or die(mysql_error());
		// check if the user was found
		if(mysql_num_rows($result)) {
			$result = mysql_fetch_array($result);
			$user_browser = $_SERVER['HTTP_USER_AGENT'];
			// build the session loginString containing the hashed PW and the user Browser and match it with the given loginString from the user session
			if( $_SESSION['login_string'] == hash('sha512',$result['password'].$user_browser) )
				return true;
		}

	}

	return false;
}

function logout() {
	session_unset();
	session_destroy();
	global $global_toasts;
	$global_toasts['info'][] = '##You were logged out.##';
	// header ("LOCATION: manageProjects.php");
}
?>