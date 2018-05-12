<?php

class Api 
{
	private $db;

	function __construct()
	{
		$hostname = 'db731374446.db.1and1.com';
		$username = 'dbo731374446';
		$password = 'FsLazyAr#24783';
		$database = 'db731374446';
		$this->db = new mysqli($hostname, $username, $password, $database);
	}

	function __destruct()
	{
		$this->db->close();
	}

	function run()
	{
		$player = $_GET['player'];
		if($player==0 || $player==2)
		{
			$this->db->query('UPDATE c4_mgmt SET nplayers=nplayers-1 WHERE row=1');
			$this->db->commit();
		}

		return;
	}
}


$api = new Api;
$api->run();

?>