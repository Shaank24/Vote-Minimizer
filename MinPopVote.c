#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    
// initialize default values
    *fastMode = false;
    *quietMode = false;
    *year = 0;

    // loops through command line arguments
    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-y") == 0) {

            // if year is presented, it makes sure the year is valid
            if (i + 1 < argc) {
                int candYear = atoi(argv[i+1]);
                if (candYear >= 1828 && candYear <= 2020 && candYear % 4 == 0) {
                    *year = candYear;
                }  
                // sets to default value if year is not valid
                else {
                    *year = 0;
                }
                i++;
            }
            // sets to default value if year is not presented
            else {
                *year = 0;
            }
        }
        else if (strcmp(argv[i], "-f") == 0) {
            *fastMode = true;
        }
        // sets quiet mode if selected
        else if (strcmp(argv[i], "-q") == 0) {
            *quietMode = true;
        }
        else {
            return false;
        }
    }
    
    // all command line arguments are valid and processed
    return true;
}

void inFilename(char* filename, int year) {
    // formats input file based on year
    snprintf(filename, 100, "data/%d.csv", year);
}

void outFilename(char* filename, int year) {
    // formats output file based on year
    snprintf(filename, 100, "toWin/%d_win.csv", year);
}

bool parseLine(char* line, State* myState) {

    // gets comma count
    int commaCount = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ','){
            commaCount++;
        }
    }

    // returns false if number of commas are invalid
    if (commaCount != 3) {
        return false;
    }

    // gets string up until first comma and copies name into myState
    char* commas = strtok(line, ",");
    strcpy(myState->name, commas);

    // gets string after first comma and before second comma and copies postal code into myState
    commas = strtok(NULL, ",");
    strcpy(myState->postalCode, commas);

    // gets string after second comma and before third comma and sets electoral votes to integer
    commas = strtok(NULL, ",");
    myState->electoralVotes = atoi(commas);
    
    // gets string after third comma and before the end of line and sets popular votes to integer
    commas = strtok(NULL, ",");
    myState->popularVotes = atoi(commas);

    return true;
}

bool readElectionData(char* filename, State* allStates, int* nStates) {
    // opens file
    FILE* fileOpen = fopen(filename, "r");

    // checks if file exists
    if (fileOpen == NULL) {
        return false;
    }

    // initial variables
    int stateCount = 0;
    char line[1024];

    // goes line by line
    while (fgets(line, sizeof(line), fileOpen) != NULL) {

        // breaks out of loop if the end of file has been reached
        if (feof(fileOpen)) {
            break;
        }

        // fills the allStates array using our parseLine function
        if (parseLine(line, &allStates[stateCount])) {
            stateCount++;
        }
    }

    // closes file
    fclose(fileOpen);

    // updates the number of states
    *nStates = stateCount;

    // returns true when the file was read in successfully
    return true;
}

int totalEVs(State* states, int szStates) {
    // TODO: Task 6 - write the totalEVs() function;
    //                note test_totalEVs() is 
    //                provided in test.c to test
    //                the functionality of totalEVs()
    //                   >> make build_test
    //                   >> make run_test 
    //----------------------------------------------
    int totalElecVotes = 0;

    // loops through the size of states and adds up all the electoral votes
    for (int i = 0; i < szStates; i++) {
        totalElecVotes += states[i].electoralVotes;
    }  
    
    return totalElecVotes;
}

int totalPVs(State* states, int szStates) {
    //--------------------------------------------------   
    // TODO: Task 6 - write the totalPVs() function;
    //                then, write your own test function 
    //                test_totalPVs() in test.c
    //                   >> make build_test
    //                   >> make run_test 
    //--------------------------------------------------
    int totalPopVotes = 0;

    // loops through the size of states and adds up all the popular votes
    for (int i = 0; i < szStates; i++) {
        totalPopVotes += states[i].popularVotes;
    }
    return totalPopVotes; 
}

MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {

    MinInfo withCurr;
    MinInfo withoutCurr;

    // handles base case
    if (start == szStates || EVs < 0) {
        withCurr.subsetPVs = 0;
        withCurr.sufficientEVs = (EVs <= 0);
        withCurr.szSomeStates = 0;
        return withCurr;
    }

    // includes current state in subset and updates state members
    withCurr = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);
    withCurr.someStates[withCurr.szSomeStates] = states[start];
    withCurr.szSomeStates++;
    withCurr.subsetPVs += states[start].popularVotes / 2 + 1;
    
    // excludes current state in subset and EVs remain the same
    withoutCurr = minPopVoteAtLeast(states, szStates, start + 1, EVs);

    // returns lowest PV count
    if (withCurr.sufficientEVs) {
        if (!withoutCurr.sufficientEVs || (withCurr.subsetPVs < withoutCurr.subsetPVs)) {
            return withCurr;
        }
    }
    else if (withoutCurr.sufficientEVs) {
        return withoutCurr;
    }

    // if neither withCurr or withoutCurr have sufficientEVs as true, return one with lower subsetPVs
    if (withCurr.subsetPVs < withoutCurr.subsetPVs) {
        return withCurr;
    }
    else {
        return withoutCurr;
    }
}

MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states,szStates);
    int reqEVs = totEVs/2 + 1; // required EVs to win election
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
   
    MinInfo withCurr;
    MinInfo withoutCurr;

    // handles base case
    if (start == szStates || EVs < 0) {
        withCurr.subsetPVs = 0;
        withCurr.sufficientEVs = (EVs <= 0);
        withCurr.szSomeStates = 0;
        return withCurr;
    }

    // checks if already been through memoization
    if (memo[start][EVs].subsetPVs != -1) {
        return memo[start][EVs];
    }

    // includes current state in subset and updates state members
    withCurr = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
    withCurr.someStates[withCurr.szSomeStates] = states[start];
    withCurr.szSomeStates++;
    withCurr.subsetPVs += states[start].popularVotes / 2 + 1;
    
    // excludes current state in subset and EVs remain the same
    withoutCurr = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);

    // returns lowest PV count
    if (withCurr.sufficientEVs) {
        if (!withoutCurr.sufficientEVs || (withCurr.subsetPVs < withoutCurr.subsetPVs)) {
            memo[start][EVs] = withCurr;
            return memo[start][EVs];
        }
    }
    else if (withoutCurr.sufficientEVs) {
        memo[start][EVs] = withoutCurr;
        return memo[start][EVs];
    }

    // if neither withCurr or withoutCurr have sufficientEVs as true, return one with lower subsetPVs
    if (withCurr.subsetPVs < withoutCurr.subsetPVs) {
        memo[start][EVs] = withCurr;
        return memo[start][EVs];
    }
    else {
        memo[start][EVs] = withoutCurr;
        return memo[start][EVs];
    }
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
    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);
    
    // frees all the memory from the memoization
    for (int i = 0; i < szStates + 1; i++) {
        free(memo[i]);
    }
    free(memo);

    return res;
}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    
    // opens file and checks if it opens properly
    FILE* fileOpen = fopen(filenameW, "w");
    if (fileOpen == NULL) {
        return false;
    }

    // writes the first line to the file
    fprintf(fileOpen, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);

    // writes the rest of the lines with the state and its details
    for (int i = toWin.szSomeStates - 1; i >= 0; i--) {
        State state = toWin.someStates[i];
        fprintf(fileOpen, "%s,%s,%d,%d\n", state.name, state.postalCode, state.electoralVotes, state.popularVotes / 2 + 1);
    }

    // closes file
    fclose(fileOpen);

    return true;
}
