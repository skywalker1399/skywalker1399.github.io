import sqlite3


DATABASE_FILE = "database/game.db"


def test_database():
    connection = sqlite3.connect(DATABASE_FILE)

    print("\nROOMS")
    print("-" * 30)

    rooms = connection.execute(
        "SELECT room_id, room_name FROM rooms"
    ).fetchall()

    for room in rooms:
        print(room)

    print("\nROOM CONNECTIONS")
    print("-" * 30)

    connections = connection.execute(
        """
        SELECT
            rooms.room_name,
            room_connections.direction,
            connected_rooms.room_name
        FROM room_connections
        JOIN rooms
            ON room_connections.room_id = rooms.room_id
        JOIN rooms AS connected_rooms
            ON room_connections.connected_room_id = connected_rooms.room_id
        ORDER BY rooms.room_id
        """
    ).fetchall()

    for connection_data in connections:
        print(connection_data)

    print("\nITEMS")
    print("-" * 30)

    items = connection.execute(
        """
        SELECT
            items.item_name,
            rooms.room_name
        FROM items
        JOIN rooms
            ON items.room_id = rooms.room_id
        """
    ).fetchall()

    for item in items:
        print(item)

    print("\nGAME SESSION ITEMS")
    print("-" * 30)

    session_items = connection.execute(
        """
        SELECT * FROM game_session_items"""
    ).fetchall()

    for item in session_items:
        print(item)

    connection.close()

if __name__ == "__main__":
    test_database()