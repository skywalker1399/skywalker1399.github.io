# Skylar Walker

import database

playing = True  # Used in our game loop to keep it running

# Different messages for different events
welcome = ('Feed Johnny!!\nYour friend Johnny is extremely hungry for dessert. '
           'And not just one dessert but ALL the dessert.\n'
           'You need to go around the house and find all six desserts.\n'
           'But you better find them all before you see Johnny again or he will leave.\nCan you find them all?')
directions = ['go north', 'go south', 'go east', 'go west']
add_item = 'To get an item type: "get (item)" being the item in the room'
valid_inputs = directions, add_item
invalid_input = 'That is not a correct direction. Valid inputs are: ' + str(valid_inputs) + '.'
wrong_way = 'Cannot go that way.'
game_over = 'The end.'
game_over_loss = ('You have approached Johnny without all the desserts and he is very upset!'
                  ' He decided to leave and never be your friend again.\nYou Lose!')
game_over_win = 'Congratulations!! You have brought all the desserts to Johnny!! He is fed and now very happy. You win!'


def navigation(location, direction, session_id):  # Moving between rooms function.

    new_location = database.get_connected_room(location, direction)

    if new_location is None:
        print(wrong_way)

        item = database.get_session_room_item(session_id, location)

        if item is not None:
            print("The dessert in here is : ", item)
        return location

    database.update_game_location(session_id, new_location)

    print("You are in the {}".format(new_location))

    item = database.get_session_room_item(session_id, new_location)

    if item is not None:
        print("The Dessert in here is : ", item)

    return new_location


def get_item(location, item, player_id, session_id):
    #Get an item from current room to inventory

    room_item = database.get_session_room_item(session_id, location)

    if room_item is None:
        print('There is no dessert in this room.')
        return

    if item != room_item:
        print('Item not available.')
        print('Dessert in here is: ', room_item)
        return

    database.add_item_to_inventory(player_id, item)
    database.remove_item_from_session(session_id, item)

    print('You acquired: ', item)


def rules():  # prints Story, rules, and inputs
    print(welcome)
    print('-' * 38)
    print('Commands')
    print('go north, go south, so east, go west')
    print('get (item)')
    print('save')
    print('quit')

def game_menu():
    #displays new and load game menu

    print('\nFeed Johnny')
    print('-' * 38)
    print('1. New Game')
    print('2. Load Game')
    print('3. Exit')

    return input('Choose an option: ')

def display_saved_games():
    #Displays saved games.

    saved_games = database.get_saved_games()

    if not saved_games:
        print('\nThere are no saved games.')
        return None

    print('\nSaved Games')
    print('-' * 30)

    for session_id, player_name in saved_games:
        print('Game {} - {}'.format(session_id, player_name))

    return saved_games


def main():
    rules()

    while True:

        choice = game_menu()

        if choice == '1':
            player_name = input('Enter your name: ')

            player_id, session_id = database.start_new_game(player_name)

            location = 'Bedroom'

        elif choice == '2':
            saved_games = display_saved_games()

            if saved_games is None:
                return

            try:
                session_id = int(input('Enter the game number to load: '))
            except ValueError:
                print('Enter a valid game number.')
                continue

            game = database.load_game(session_id)

            if game is None:
                print('Game could not be found.')
                continue

            game = database.load_game(session_id)

            if game is None:
                print('That game could not be found.')
                return

            player_id = game[0]
            player_name = game[1]
            location = game[2]

            print('\nWelcome back, {}!'.format(player_name))
            print('You are in the {}'.format(location))

        elif choice == '3':
            print('Goodbye!')
            return

        else:
            print('Invalid choice.')
            return

        while playing:  # continues to loop the game until broken

            current_room = location

            if current_room == 'Dining room':  # If you are in the dining room the game will end either 1 of 2 ways.
                print('Johnny is here')
                inventory = database.get_inventory(player_id)
                if len(inventory) == 6:  # If you have all six desserts then you win. If not then you lose
                    print(game_over_win)
                    # This makes sure the game doesn't close automatically when you enter the dining room
                    input('Press Enter to close game.')
                    break
                else:
                    print(game_over_loss)
                    input('Press Enter to close game.')
                    break

            print('You are in the {}'.format(current_room))  # Prints your current status
            inventory = database.get_inventory(player_id)
            print('Inventory', inventory)
            if len(inventory) == 6:
                print('You have found all the desserts for Johnny! Now go and find him to make him happy.')

            print('-' * 30)
            player_move = input('What do you want to do?')  # Asks for your move

            if player_move == 'save':
                print('Game saved')
                continue
            elif player_move == 'quit':
                print('Game saved')
                print('Returning to menu')
                break

            elif player_move.startswith('go'):  # Checks for keywords to decide how to handle input

                action = player_move.split()
                #print(action)
                if len(action) == 2 and action[0] == 'go':# If 'go' appears then the input is sent to navigation
                    direciton = action[1]

                    if direciton in ['north', 'south', 'east', 'west']:
                        location = navigation(location, direciton, session_id)

                    else:  # If 'go' is in wrong places then invalid
                        print(invalid_input)
                else:
                    print(invalid_input)

            elif player_move.startswith('get'):  # If get is in player input then it will try to get an item

                action = player_move.split()
                if len(action) == 2 and action[0] == 'get':
                    get_item(location, action[1], player_id, session_id)

                else:  # If get is not the first word then it is invalid
                    print(invalid_input)

            else:  # If no 'go' or 'get' then invalid
                print(invalid_input)
                continue


main()  # starts the game
