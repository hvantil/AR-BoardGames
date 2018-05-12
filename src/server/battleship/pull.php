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
		$stmt = $this->db->prepare('SELECT turn FROM battleship_mgmt');
		$stmt->execute();
		$stmt->bind_result($turn);
		$stmt->fetch();
		$stmt->close();
		echo $turn;

		$stmt = $this->db->prepare('SELECT val FROM battleship_board');
		$stmt->execute();
		$stmt->bind_result($val);
		while ($stmt->fetch())
		{
			echo $val;
		}
		$stmt->close();

		return;
	}
}


$api = new Api;
$api->run();

?>