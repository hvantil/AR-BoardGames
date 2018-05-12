DROP TABLE IF EXISTS db731374446.c4_mgmt;

CREATE TABLE db731374446.c4_mgmt (
	row int NOT NULL PRIMARY KEY,
	nplayers int NOT NULL,
	turn int NOT NULL
);

INSERT INTO db731374446.c4_mgmt 
	(row, nplayers, turn)
VALUES
	(1, 0, 1);


DROP TABLE IF EXISTS db731374446.c4_board;

CREATE TABLE db731374446.c4_board (
	row int NOT NULL PRIMARY KEY,
	val int NOT NULL
);

INSERT INTO db731374446.c4_board
    (row, val)
VALUES
	(0,1),
    (1,1),
    (2,1),
    (3,1),
    (4,1),
    (5,1),
    (6,1),
    (7,1),
    (8,1),
    (9,1),
    (10,1),
    (11,1),
    (12,1),
    (13,1),
    (14,1),
    (15,1),
    (16,1),
    (17,1),
    (18,1),
    (19,1),
    (20,1),
    (21,1),
    (22,1),
    (23,1),
    (24,1),
    (25,1),
    (26,1),
    (27,1),
    (28,1),
    (29,1),
    (30,1),
    (31,1),
    (32,1),
    (33,1),
    (34,1),
    (35,1),
    (36,1),
    (37,1),
    (38,1),
    (39,1),
    (40,1),
    (41,1);
    