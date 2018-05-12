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

		if($turn==1)
		{
			$this->db->query('UPDATE battleship_mgmt SET turn=2 WHERE row=1');
		}
		else
		{
			$this->db->query('UPDATE battleship_mgmt SET turn=1 WHERE row=1');
		}
		$this->db->commit();

		$board = $_GET['board'];
		$query = 'UPDATE battleship_board SET val=? WHERE row=?';
		$stmt = $this->db->prepare($query);
		$stmt->bind_param('ii', $val, $row);
		for($row = 0; $row < 102; $row++) 
		{
			$val = $board[$row];
			$stmt->execute();
		} 
		$stmt->close();

		return;
	}
}


$api = new Api;
$api->run();

?>