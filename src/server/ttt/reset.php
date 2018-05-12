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
		$this->db->query('UPDATE ttt_mgmt SET nplayers=0, turn=1 WHERE row=1');
		$this->db->commit();

		for($row = 0; $row < 9; $row++) 
		{
			$query = 'UPDATE ttt_board SET val=0 WHERE row=?';
			$stmt = $this->db->prepare($query);
			$stmt->bind_param('i', $row);
			$stmt->execute();
			$stmt->close();
		}

		return;
	}
}


$api = new Api;
$api->run();

?>