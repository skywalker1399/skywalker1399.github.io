--DATABASE
--create relational tables


--stores rooms
CREATE TABLE IF NOT EXISTS rooms (
    room_id INTEGER PRIMARY KEY AUTOINCREMENT,
    room_name TEXT NOT NULL UNIQUE
);

--Stores connection between rooms
CREATE TABLE IF NOT EXISTS room_connections (
    connection_id INTEGER PRIMARY KEY AUTOINCREMENT,
    room_id INTEGER NOT NULL,
    direction TEXT NOT NULL,
    connected_room_id INTEGER NOT NULL,

    FOREIGN KEY (room_id) REFERENCES rooms(room_id),
    FOREIGN KEY (connected_room_id) REFERENCES rooms(room_id),

    UNIQUE(room_id, direction)
);

--Store item info
CREATE TABLE IF NOT EXISTS items(
    item_id INTEGER PRIMARY KEY AUTOINCREMENT,
    item_name TEXT NOT NULL UNIQUE,
    description TEXT,
    room_id INTEGER,

    FOREIGN KEY (room_id) REFERENCES rooms(room_id)
);

--Store player info
CREATE TABLE IF NOT EXISTS player_info(
  player_id INTEGER PRIMARY KEY AUTOINCREMENT,
  player_name TEXT NOT NULL
);


--stores saved game
CREATE TABLE IF NOT EXISTS game_sessions (
    session_id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_id INTEGER NOT NULL,
    current_room_id INTEGER NOT NULL,

    FOREIGN KEY (player_id) REFERENCES player_info(player_id),
    FOREIGN KEY (current_room_id) REFERENCES rooms(room_id)
);

--Stores game session
CREATE TABLE IF NOT EXISTS game_session_items(
    session_id INTEGER NOT NULL,
    item_id INTEGER NOT NULL,
    room_id INTEGER,

    PRIMARY KEY (session_id, item_id),

    FOREIGN KEY (session_id) REFERENCES game_sessions(session_id),
    FOREIGN KEY (item_id) REFERENCES items(item_id),
    FOREIGN KEY (room_id) REFERENCES rooms(room_id)
);

--Stores collected items
CREATE TABLE IF NOT EXISTS player_inventory(
    player_id INTEGER NOT NULl,
    item_id INTEGER NOT NULL,

    PRIMARY KEY (player_id, item_id)

    FOREIGN KEY (player_id) REFERENCES player_info(player_id),
    FOREIGN KEY (item_id) REFERENCES items(item_id)
);


