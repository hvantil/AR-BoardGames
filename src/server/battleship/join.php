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
		$stmt = $this->db->prepare('SELECT nplayers FROM battleship_mgmt');	
		$stmt->execute();
		$stmt->bind_result($nplayers);
		$stmt->fetch();
		$stmt->close();

		if($nplayers<2)
		{
			$this->db->query('UPDATE battleship_mgmt SET nplayers=nplayers+1 WHERE row=1');
			$this->db->commit();
			echo $nplayers+1;			
		}
		else
		{
			echo '3';
		}

		return;
	}
}


$api = new Api;
$api->run();

?>