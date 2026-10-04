import sqlite3

DATABASE_FILE = "database/game.db"

def seed_rooms(connection):
    #Adds rooms to database
    rooms = [
        "Den",
        "Bedroom",
        "Bathroom",
        "Closet",
        "Basement",
        "Kitchen",
        "Pantry",
        "Dining room"
    ]

    for room in rooms:
        connection.execute(
            "INSERT OR IGNORE INTO rooms (room_name) VALUES (?)",
            (room,)
        )

def seed_connections(connection):
    #Adds the connections between rooms.

    connections = [
        ("Den", "south", "Bedroom"),
        ("Den", "west", "Dining room"),
        ("Den", "east", "Basement"),
        ("Den", "north", "Kitchen"),
        ("Bedroom", "north", "Den"),
        ("Bedroom", "east", "Closet"),
        ("Closet", "west", "Bedroom"),
        ("Basement", "west", "Den"),
        ("Basement", "north", "Bathroom"),
        ("Bathroom", "south", "Basement"),
        ("Kitchen", "south", "Den"),
        ("Kitchen", "east", "Pantry"),
        ("Pantry", "west", "Kitchen"),
        ("Dining room", "east", "Den")
    ]

    for room, direction, connected_room in connections:

        room_id = connection.execute(
            "SELECT room_id FROM rooms WHERE room_name = ?",
            (room,)
        ).fetchone()[0]

        connected_room_id = connection.execute(
            "SELECT room_id FROM rooms WHERE room_name = ?",
            (connected_room,)
        ).fetchone()[0]

        connection.execute(
            """
            INSERT INTO room_connections
            (room_id, direction, connected_room_id)
            VALUES (?, ?, ?)
            """,
            (room_id, direction, connected_room_id)
        )

def seed_items(connection):
    #Add all desserts

    items = [
        ("pie", "A delicious pie.", "Den"),
        ("ice-cream", "Cold and creamy ice cream.", "Closet"),
        ("cookie", "A freshly baked cookie.", "Basement"),
        ("pudding", "A bowl of pudding.", "Bathroom"),
        ("popsicle", "A frozen popsicle.", "Kitchen"),
        ("cake", "A delicious cake.", "Pantry")
    ]

    for item_name, description, room_name in items:

        room_id = connection.execute(
            "SELECT room_id FROM rooms WHERE room_name = ?",
            (room_name,)
        ).fetchone()[0]

        connection.execute(
            """
            INSERT INTO items
            (item_name, description, room_id) VALUES (?, ?, ?)""",
            (item_name, description, room_id)
        )
def seed_database():

    connection = sqlite3.connect(DATABASE_FILE)

    seed_rooms(connection)
    seed_connections(connection)
    seed_items(connection)

    connection.commit()
    connection.close()

    print("Game data successfully added to database.")


if __name__ == "__main__":
    seed_database()