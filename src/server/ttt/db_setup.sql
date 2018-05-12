DROP TABLE IF EXISTS db731374446.ttt_mgmt;

CREATE TABLE db731374446.ttt_mgmt (
	row int NOT NULL PRIMARY KEY,
	nplayers int NOT NULL,
	turn int NOT NULL
);

INSERT INTO db731374446.ttt_mgmt 
	(row, nplayers, turn)
VALUES
	(1, 0, 1);


DROP TABLE IF EXISTS db731374446.ttt_board;

CREATE TABLE db731374446.ttt_board (
	row int NOT NULL PRIMARY KEY,
	val int NOT NULL
);

INSERT INTO db731374446.ttt_board
    (row, val)
VALUES
	(0,0),
    (1,0),
    (2,0),
    (3,0),
    (4,0),
    (5,0),
    (6,0),
    (7,0),
    (8,0);
    