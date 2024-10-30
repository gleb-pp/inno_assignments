#include <stdio.h>
#include <string.h>
#define TEAM_SIZE 100

// defining an enumeration Position
typedef enum {Goalkeeper, Defender, Midfielder, Forward} Positions;

// defining a structure Player
struct Player {
    int id;
    char name[16];
    Positions pos;
    int age;
    int goals;
};


int add_players(struct Player team[TEAM_SIZE], int id, char name[16], char pos[11], int age, int goals) {
    
    // check if new player is not yet in the team
    for (int i=0; i<TEAM_SIZE; i++) {
        if (team[i].id == id) {
            return 2;
        }
    }

    // check if name start with capital letter
    char alphabet_high[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int found = 0;
    for (int i=0; i<26; i++) {
        if (name[0] == alphabet_high[i]) {
            found = 1;
            break;
        }
    }
    if (found == 0) {
        return 2;
    }

    // check if name has no lenth less than 2 or more than 15
    if (strlen(name) < 2 || strlen(name) > 15) {
        return 2;
    }

    // check that name consists only from English letters
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i=0; i<strlen(name); i++) {
        int found = 0;
        for (int j=0; j<52; j++) {
            if (name[i] == alphabet[j]) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            return 2;
        }
    }

    // check the age
    if (age < 18 || age > 100) {
        return 2;
    }

    // check the number of goals
    if (goals < 0 || goals > 1000) {
        return 2;
    }

    // check the position
    if (strcmp(pos, "Goalkeeper") != 0 && strcmp(pos, "Defender") != 0 && strcmp(pos, "Midfielder") != 0 && strcmp(pos, "Forward") != 0) {
        return 2;
    }
    
    // seraching in the team player with 0 id (empty player) and put new player on its place
    for (int i=0; i<TEAM_SIZE; i++) {
        if (team[i].id == 0) {

            team[i].id = id;
            strcpy(team[i].name, name);
            team[i].age = age;
            team[i].goals = goals;

            if (strcmp(pos, "Goalkeeper") == 0) {
                team[i].pos = Goalkeeper;
            } else if (strcmp(pos, "Defender") == 0) {
                team[i].pos = Defender;
            } else if (strcmp(pos, "Midfielder") == 0) {
                team[i].pos = Midfielder;
            } else if (strcmp(pos, "Forward") == 0) {
                team[i].pos = Forward;
            }

            break;
        }
    }

    return 0;
}


int delete_player(struct Player team[TEAM_SIZE], int id) {

    // searching for player
    for (int i=0; i<TEAM_SIZE; i++) {
        if (team[i].id == id) {
            team[i].id = 1; // we don't need to change all the info about the player, we can just change his id 
            return 0;
        }
    }
    return 1;
}


int display(struct Player team[TEAM_SIZE], FILE *fw) {

    // searching for players with non zero id (not empty players)
    int found = 0;
    for (int i=0; i<TEAM_SIZE; i++) {
        if (team[i].id != 0 && team[i].id != 1) {
            const char* Pos_names[] = {"Goalkeeper", "Defender", "Midfielder", "Forward"}; // since Position is just number, we need to convert it to text
            fprintf (fw, "ID: %d, Name: %s, Position: %s, Age: %d, Goals: %d\n", team[i].id - 2, team[i].name, Pos_names[team[i].pos], team[i].age, team[i].goals);
            found = 1;
        }
    }
    if (found == 0) { // if we didn't find any player, it is Invalis input
        return 2;
    } else {
        return 0;
    }

}


int search(struct Player team[TEAM_SIZE], int id) {

    // searching for player
    for (int i=0; i<TEAM_SIZE; i++) {
        if (team[i].id == id) {
            return 0;
        }
    }
    return 1;
}


int update_player(struct Player team[TEAM_SIZE], int id, char name[16], char pos[11], int age, int goals) { // this function works almost the same as add_player func
    
    // check if our player is in the team
    int found = 0;
    for (int i=0; i<TEAM_SIZE; i++) {
        if (team[i].id == id) {
            found = 1;
            break;
        }
    }
    if (found == 0) {
        return 2;
    }

    // check if name start with capital letter
    char alphabet_high[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    found = 0;
    for (int i=0; i<26; i++) {
        if (name[0] == alphabet_high[i]) {
            found = 1;
            break;
        }
    }
    if (found == 0) {
        return 2;
    }

    // check if name has no lenth less than 2 or more than 15
    if (strlen(name) < 2 || strlen(name) > 15) {
        return 2;
    }

    // check that name consists only from English letters
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i=0; i<strlen(name); i++) {
        int found = 0;
        for (int j=0; j<52; j++) {
            if (name[i] == alphabet[j]) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            return 2;
        }
    }

    // check the age
    if (age < 18 || age > 100) {
        return 2;
    }

    // check the number of goals
    if (goals < 0 || goals > 1000) {
        return 2;
    }

    // check the position
    if (strcmp(pos, "Goalkeeper") != 0 && strcmp(pos, "Defender") != 0 && strcmp(pos, "Midfielder") != 0 && strcmp(pos, "Forward") != 0) {
        return 2;
    }
    
    // seraching for player in the team and changing his information
    for (int i=0; i<TEAM_SIZE; i++) {
        if (team[i].id == id) {

            strcpy(team[i].name, name);
            team[i].age = age;
            team[i].goals = goals;

            if (strcmp(pos, "Goalkeeper") == 0) {
                team[i].pos = Goalkeeper;
            } else if (strcmp(pos, "Defender") == 0) {
                team[i].pos = Defender;
            } else if (strcmp(pos, "Midfielder") == 0) {
                team[i].pos = Midfielder;
            } else if (strcmp(pos, "Forward") == 0) {
                team[i].pos = Forward;
            }

            break;
        }
    }

    return 0;
}


int main() {

    FILE *fp; // opening file with input data
    fp = fopen("input.txt", "r");

    FILE *fw;  // opening file for output data
    fw = fopen("output.txt", "w");

    struct Player team[TEAM_SIZE]; // creating a team
    for (int i=0; i<TEAM_SIZE; i++) {
        team[i].id = 0; // setting id of each "empty" player equal to 0
    }

    char command[8]; 
    while (fgets(command, 8, fp) != NULL) { // start reading file till the end
        command[strcspn(command, "\n")] = 0; // deleting \n

        // consider the case with each command separately
        if (strcmp(command, "Add") == 0) {
            int pot_id;
            char pot_name[16];
            char pot_pos[11];
            int pot_age;
            int pot_goals;

            if (fscanf(fp, "%d %s %s %d %d", &pot_id, pot_name, pot_pos, &pot_age, &pot_goals) == 5) { // check if all data given correctly
                int res = add_players(team, pot_id + 2, pot_name, pot_pos, pot_age, pot_goals); // getting functions result
                fgets(command, 8, fp); // reading an empty string that we don't need
                if (strcmp(command, "\n") != 0) {
                    fclose(fp);
                    fprintf (fw, "Invalid inputs\n");
                    fclose(fw);
                    return 0;
                }
                if (res == 2) {
                    fclose(fp);
                    fprintf (fw, "Invalid inputs\n");
                    fclose(fw);
                    return 0;
                }
            } else { // case if some data given incorrectly
                fclose(fp);
                fprintf (fw, "Invalid inputs\n");
                fclose(fw);
                return 0;
            } 

        } else if (strcmp(command, "Update") == 0) {
            int pot_id;
            char pot_name[16];
            char pot_pos[11];
            int pot_age;
            int pot_goals;

            if (fscanf(fp, "%d %s %s %d %d", &pot_id, pot_name, pot_pos, &pot_age, &pot_goals) == 5) { // check if all data given correctly
                int res = update_player(team, pot_id + 2, pot_name, pot_pos, pot_age, pot_goals);
                fgets(command, 8, fp); // reading an empty string that we don't need
                if (strcmp(command, "\n") != 0) { // if there is some extra symbols, we stop working
                    fclose(fp);
                    fprintf (fw, "Invalid inputs\n");
                    fclose(fw);
                    return 0;
                }
                if (res == 2) {
                    fclose(fp);
                    fprintf (fw, "Invalid inputs\n");
                    fclose(fw);
                    return 0;
                }
            } else { // case if some data given incorrectly
                fclose(fp);
                fprintf (fw, "Invalid inputs\n");
                fclose(fw);
                return 0;
            } 

        } else if (strcmp(command, "Delete") == 0) {
            int pot_id;
            if (fscanf(fp, "%d", &pot_id) == 1) { // check if all data given correctly
                int res = delete_player(team, pot_id + 2);
                fgets(command, 8, fp); // reading an empty string that we don't need
                if (strcmp(command, "\n") != 0) { // if there is some extra symbols, we stop working
                    fclose(fp);
                    fprintf (fw, "Invalid inputs\n");
                    fclose(fw);
                    return 0;
                }
                if (res == 1) {
                    fprintf (fw, "Impossible to delete\n");
                }
            } else { // case if some data given incorrectly
                fclose(fp);
                fprintf (fw, "Invalid inputs\n");
                fclose(fw);
                return 0;
            } 

        } else if (strcmp(command, "Search") == 0) {
            int pot_id;
            if (fscanf(fp, "%d", &pot_id) == 1) { // check if all data given correctly
                int res = search(team, pot_id + 2);
                fgets(command, 8, fp); // reading an empty string that we don't need
                if (strcmp(command, "\n") != 0) { // if there is some extra symbols, we stop working
                    fclose(fp);
                    fprintf (fw, "Invalid inputs\n");
                    fclose(fw);
                    return 0;
                }
                if (res == 0) {
                    fprintf (fw, "Found\n");
                } else {
                    fprintf (fw, "Not found\n");
                }
            } else { // case if some data given incorrectly
                fclose(fp);
                fprintf (fw, "Invalid inputs\n");
                fclose(fw);
                return 0;
            } 

        } else if (strcmp(command, "Display") == 0) {
            fgets(command, 8, fp); // reading an empty string that we don't need
            if (strcmp(command, "\n") != 0 && strcmp(command, "") != 0) { // if there is some extra symbols, we stop working
                fclose(fp);
                fprintf (fw, "Invalid inputs\n");
                fclose(fw);
                return 0;
            }
            int res = display(team, fw);
            if (res == 2) {
                fclose(fp);
                fprintf (fw, "Invalid inputs\n");
                fclose(fw);
                return 0;
            }

        } else { // case when command is incorrect
            fclose(fp);
            fprintf (fw, "Invalid inputs\n");
            fclose(fw);
            return 0;
        }
    }

    fclose(fp);
    fclose(fw);
    return 0;
};