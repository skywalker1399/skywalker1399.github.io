import sqlite3


DATABASE_FILE = "database/game.db"


def get_connection():
    #Creates and returns connection to the SQLite database.
    return sqlite3.connect(DATABASE_FILE)

def get_connected_room(current_room, direction):
    #Finds the connected rooms to current room

    connection = get_connection()

    result = connection.execute(
        """
        SELECT connected_rooms.room_name
        FROM room_connections
        JOIN rooms
            ON room_connections.room_id = rooms.room_id
        JOIN rooms AS connected_rooms
            ON room_connections.connected_room_id = connected_rooms.room_id
        WHERE rooms.room_name = ?
        AND room_connections.direction = ?
        """,
        (current_room, direction)
    ).fetchone()

    connection.close()

    if result is None:
        return None

    return result[0]

def create_player(player_name):
    #creates new player

    connection = get_connection()

    cursor = connection.execute(

        """
        INSERT INTO player_info (player_name) VALUES (?)""",
        (player_name,)
    )

    player_id = cursor.lastrowid

    connection.commit()
    connection.close()

    return player_id

def create_game_session(player_id):
    #Creates a new game session for player when game starts

    connection = get_connection()

    #find the start room
    starting_room = connection.execute(
        """
        SELECT room_id FROM rooms WHERE room_name = ?""",
        ("Bedroom", )
    ).fetchone()

    starting_room_id = starting_room[0]

    #create the game session
    cursor = connection.execute(
        """
        INSERT INTO game_sessions (player_id, current_room_id) VALUES (?, ?)""",
        (player_id, starting_room_id)
    )

    session_id = cursor.lastrowid

    #get desserts and their starting rooms
    items = connection.execute(
        """
        SELECT item_id, room_id FROM items"""
    ).fetchall()

    #create the item locations for each game session
    for item_id, room_id in items:
        connection.execute(
            """
            INSERT INTO game_session_items (session_id, item_id, room_id) VALUES (?, ?, ?)""",
            (session_id, item_id, room_id)
        )
    connection.commit()
    connection.close()
    return session_id

def get_saved_games():
    #Returns all saved games

    connection = get_connection()

    games = connection.execute(
        """
        SELECT game_sessions.session_id,
               player_info.player_name
        FROM game_sessions
        JOIN player_info
            ON game_sessions.player_id = player_info.player_id
        ORDER BY game_sessions.session_id
        """
    ).fetchall()

    connection.close()

    return games

def load_game(session_id):
    #Loads previous games

    connection = get_connection()

    # Get the player and current room.
    game = connection.execute(
        """
        SELECT player_info.player_id,
               player_info.player_name,
               rooms.room_name
        FROM game_sessions
        JOIN player_info
            ON game_sessions.player_id = player_info.player_id
        JOIN rooms
            ON game_sessions.current_room_id = rooms.room_id
        WHERE game_sessions.session_id = ?
        """,
        (session_id,)
    ).fetchone()

    if game is None:
        connection.close()
        return None

    player_id = game[0]
    player_name = game[1]
    current_room = game[2]

    # Get the player's inventory.
    inventory = connection.execute(
        """
        SELECT items.item_name
        FROM player_inventory
        JOIN items
            ON player_inventory.item_id = items.item_id
        WHERE player_inventory.player_id = ?
        """,
        (player_id,)
    ).fetchall()

    connection.close()

    inventory = [item[0] for item in inventory]

    return player_id, player_name, current_room, inventory

def update_game_location(session_id, room_name):
    #Updates the current room in a saved game session.

    connection = get_connection()

    room = connection.execute(
        """
        SELECT room_id
        FROM rooms
        WHERE room_name = ?
        """,
        (room_name,)
    ).fetchone()

    if room is None:
        connection.close()
        return

    room_id = room[0]

    connection.execute(
        """
        UPDATE game_sessions
        SET current_room_id = ?
        WHERE session_id = ?
        """,
        (room_id, session_id)
    )

    connection.commit()
    connection.close()

def get_session_room_item(session_id, room_name):
    #Return dessert in current room

    connection = get_connection()

    result = connection.execute(
        """
        SELECT items.item_name
        FROM game_session_items
        JOIN items ON game_session_items.item_id = items.item_id
        JOIN rooms on game_session_items.room_id = rooms.room_id
        WHERE game_session_items.session_id = ?
        AND rooms.room_name = ?""",
        (session_id, room_name)
    ).fetchone()

    connection.close()

    if result is None:
        return None

    return result[0]

def remove_item_from_session(session_id, item_name):
    #Removes item from room for the current session.

    connection = get_connection()

    connection.execute(
        """
        UPDATE game_session_items
        SET room_id = NULL
        WHERE session_id = ?
        AND item_id = (
            SELECT item_id
            FROM items
            WHERE item_name = ?
        )
        """,
        (session_id, item_name)
    )

    connection.commit()
    connection.close()


def add_item_to_inventory(player_id, item_name):
    #adds item to inventory

    connection = get_connection()

    item = connection.execute(
        """
        SELECT item_id FROM items WHERE item_name = ?""",
        (item_name,)
    ).fetchone()

    if item is None:
        connection.close()
        return

    item_id = item[0]

    connection.execute(
        """
        INSERT INTO player_inventory (player_id, item_id) VALUES (?, ?)""",
        (player_id, item_id)
    )
    connection.commit()
    connection.close()

def get_inventory(player_id):
    #Returns item from inventory

    connection = get_connection()

    inventory = connection.execute(
        """
        SELECT items.item_name 
        FROM player_inventory 
        JOIN items ON player_inventory.item_id = items.item_id
        WHERE player_inventory.player_id = ?""",
        (player_id,)
    ).fetchall()

    connection.close()

    return [item[0] for item in inventory]

def start_new_game(player_name):
    #Creates a new player and session

    player_id = create_player(player_name)

    session_id = create_game_session(player_id)

    return player_id, session_id

def create_database():
    #Creates database

    connection = get_connection()

    with open("schema.sql", "r") as schema_file:
        schema = schema_file.read()

    connection.executescript(schema)
    connection.commit()
    connection.close()
