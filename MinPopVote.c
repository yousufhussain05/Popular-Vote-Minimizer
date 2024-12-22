/*-------------------------------------------
Project 4: Popular Vote Minimizer - Which answers the central question:
What was the fewest number of popular votes you could get and still be elected President of the United States of America?
Course: CS 211, Fall 2024, UIC
Author: Yousuf Hussain
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

/* 
 * Function: totalEVs
 * Purpose: Computes the total number of electoral votes for all states in the array.
 * Parameters: 
 *    - states: Pointer to the array of State structures.
 *    - szStates: The size of the array (number of states).
 * Returns: The sum of all electoral votes.
*/
int totalEVs(State* states, int szStates) {
    int count = 0;
    // Loop through each state and sum up their electoral votes
    for (int i = 0; i < szStates; i++) {
        int electoralCt = states[i].electoralVotes;
        count += electoralCt;
    }
    return count;
}

/* 
 * Function: totalPVs
 * Purpose: Computes the total number of popular votes for all states in the array.
 * Parameters: 
 *    - states: Pointer to the array of State structures.
 *    - szStates: The size of the array (number of states).
 * Returns: The sum of all popular votes.
*/
int totalPVs(State* states, int szStates) {
    int count = 0;
    // Loop through each state and sum up their popular votes
    for (int i = 0; i < szStates; i++) {
        int popularCt = states[i].popularVotes;
        count += popularCt;
    }
    return count; 
}

/* 
 * Function: setSettings
 * Purpose: Configures settings based on command-line arguments, including election year, fast mode, and quiet mode.
 * Parameters: 
 *    - argc: The number of command-line arguments.
 *    - argv: Array of command-line arguments.
 *    - year: Pointer to an integer to store the election year.
 *    - fastMode: Pointer to a boolean to store the fast mode setting.
 *    - quietMode: Pointer to a boolean to store the quiet mode setting.
 * Returns: true if settings were successfully set, false otherwise.
*/
bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    *year = 0; 
    *quietMode = false; 
    *fastMode = false; 
    // Iterate through command-line arguments to find the appropriate flags
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-y") == 0) {
            int yr = atoi(argv[i + 1]);
            if (yr >= 1828 && yr <= 2020 && yr % 4 == 0) {
                *year = yr;
            }
            else {
                *year = 0;
            }
            i++;
        }
        else if (strcmp(argv[i], "-q") == 0) {
            *quietMode = true;
        }
        else if (strcmp(argv[i], "-f") == 0) {
            *fastMode = true;
        }
        else {
            *year = 0; 
            *quietMode = false; 
            *fastMode = false; 
            return false;
            
        }
    }
        
    return true; 
}

/* 
 * Function: inFilename
 * Purpose: Constructs the input filename based on the given election year.
 * Parameters: 
 *    - filename: Character array to store the filename.
 *    - year: The election year.
*/
void inFilename(char* filename, int year) {
    sprintf(filename, "data/%d.csv", year);
    
    return;
}

/* 
 * Function: outFilename
 * Purpose: Constructs the output filename based on the given election year.
 * Parameters: 
 *    - filename: Character array to store the filename.
 *    - year: The election year.
*/
void outFilename(char* filename, int year) {
    sprintf(filename, "toWin/%d_win.csv", year);   
    return;
}

/* 
 * Function: parseLine
 * Purpose: Parses a line of CSV data and fills in the details of a State structure.
 * Parameters: 
 *    - line: Character array containing the line of CSV data.
 *    - myState: Pointer to a State structure to store the parsed information.
 * Returns: true if parsing was successful, false otherwise.
*/
bool parseLine(char* line, State* myState) {
    int count = 0;
    // Count the number of commas in the line to verify the format
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ',') {
            count++;
        }
    }

    if (count != 3) {
        return false;
    }

    // Parse each token from the line
    char *token;
    token = strtok(line, ",");
    if (token == NULL) {
        return false;
    }
    strcpy(myState->name, token);


    token = strtok(NULL, ",");
    if (token == NULL) {
        return false;
    }
    strcpy(myState->postalCode, token);
    

    token = strtok(NULL, ",");
    if (token == NULL) {
        return false;
    }
    myState->electoralVotes = atoi(token);


    token = strtok(NULL, ",");
    if (token == NULL) {
        return false;
    }
    myState->popularVotes = atoi(token);

    return true;
}
/* 
 * Function: readElectionData
 * Purpose: Reads election data from a file and stores it in an array of State structures.
 * Parameters: 
 *    - filename: Character array containing the name of the file to read.
 *    - allStates: Array of State structures to store the read data.
 *    - nStates: Pointer to an integer to store the number of states read.
 * Returns: true if the file was successfully read, false otherwise.
*/
bool readElectionData(char* filename, State* allStates, int* nStates) {
    *nStates = 0; // required initialization prior to incrementing. 
    FILE *myFile = fopen(filename, "r");
    if (myFile == NULL) {
        return false;
    }
    // Read each line from the file and parse it into a State structure
    char line[51];
    while (fgets(line, sizeof(line), myFile) != NULL) {
        if (parseLine(line, &allStates[*nStates])) {
            (*nStates)++;
        }
        
    }
    fclose(myFile);
    return true; 
}

/* 
 * Function: minPopVoteAtLeast
 * Purpose: Recursively determines the minimum number of popular votes needed to achieve a given number of electoral votes.
 * Parameters: 
 *    - states: Pointer to the array of State structures.
 *    - szStates: The size of the array (number of states).
 *    - start: The current index in the array.
 *    - EVs: The number of electoral votes required to win.
 * Returns: A MinInfo structure containing the results.
*/
MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
    // Base case: if all states are considered or the required EVs are reached
    if (start == szStates || EVs <= 0) {
        MinInfo res;
        res.subsetPVs = 0;
        if (EVs <= 0) {
            res.sufficientEVs = true;
        }
        else {
            res.sufficientEVs = false;
        }
        res.szSomeStates = 0;
        return res;
    }

    // Calculate results excluding and including the current state
    MinInfo exclude = minPopVoteAtLeast(states, szStates, start + 1, EVs);
    MinInfo include = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes); 
    include.subsetPVs += (states[start].popularVotes/2) + 1;
    include.someStates[include.szSomeStates] = states[start];
    include.szSomeStates++;

    // Return the minimum result that meets the required EVs
    if (include.sufficientEVs && (!exclude.sufficientEVs || include.subsetPVs <= exclude.subsetPVs)) {
        return include;
    }
    else {
        return exclude;
    }

}

MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

/* 
 * Function: minPopVoteAtLeastFast
 * Purpose: Uses memoization to efficiently determine the minimum number of popular votes needed to achieve a given number of electoral votes.
 * Parameters: 
 *    - states: Pointer to the array of State structures.
 *    - szStates: The size of the array (number of states).
 *    - start: The current index in the array.
 *    - EVs: The number of electoral votes required to win.
 *    - memo: 2D array for storing previously computed results.
 * Returns: A MinInfo structure containing the results.
*/
MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    // Base case: if all states are considered or the required EVs are reached
    if (start == szStates || EVs <= 0) {
        MinInfo res;
        res.subsetPVs = 0;
        if (EVs <= 0) {
            res.sufficientEVs = true;
        }
        else {
            res.sufficientEVs = false;
        }
        res.szSomeStates = 0;
        return res;
    }

    // Check if the result is already memoized
    if (memo[start][EVs].subsetPVs != -1) {
        return memo[start][EVs];
    }

    // Calculate results excluding and including the current state
    MinInfo exclude = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);
    MinInfo include = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo); 
    include.subsetPVs += (states[start].popularVotes/2) + 1;;
    include.someStates[include.szSomeStates] = states[start];
    include.szSomeStates++;

    // Store the minimum result that meets the required EVs in the memoization table
    if (include.sufficientEVs && (!exclude.sufficientEVs || include.subsetPVs < exclude.subsetPVs)) {
        memo[start][EVs] = include;
    }
    else {
        memo[start][EVs] = exclude;
    }
    return memo[start][EVs];
}

MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election

    MinInfo** memo = (MinInfo**)malloc((szStates+1)*sizeof(MinInfo*));
    for (int i = 0; i < szStates+1; ++i) {
        memo[i] = (MinInfo*)malloc((reqEVs+1)*sizeof(MinInfo));
        for (int j = 0; j < reqEVs+1; ++j) {
            memo[i][j].subsetPVs = -1;
        }
    }
    // Calculate the minimum popular votes to win and free the memoization table
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);
    for (int i = 0; i < szStates + 1; i++) {
        free(memo[i]);
    }
    free(memo);

    return res;
}



/* 
 * Function: writeSubsetData
 * Purpose: Writes the election results to a file, including total and won electoral votes, total and won popular votes.
 * Parameters: 
 *    - filenameW: Character array containing the name of the file to write.
 *    - totEVs: The total number of electoral votes.
 *    - totPVs: The total number of popular votes.
 *    - wonEVs: The number of electoral votes won.
 *    - toWin: A MinInfo structure containing the results of the minimum votes required to win.
 * Returns: true if the file was successfully written, false otherwise.
*/
bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    FILE *myFile = fopen(filenameW, "w");
    if (myFile == NULL) {
        return false;
    }
    // Write the summary line to the file
    fprintf(myFile, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);

    // Write each state's details in the subset to the file
    for (int i = toWin.szSomeStates - 1; i >= 0; i--) {
        fprintf(myFile, "%s,%s,%d,%d\n", toWin.someStates[i].name, toWin.someStates[i].postalCode, toWin.someStates[i].electoralVotes, (toWin.someStates[i].popularVotes / 2) + 1);
    }
    
    fclose(myFile);
    return true; 
}
