#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

bool test_totalEVs() {
    State aStates[10];
    int res;
    
    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    printf(" Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates,5);
    if (res != 38) {
        printf("  individual state EVs are 5, 8, 13, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return 0;
    }

    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 15;
    aStates[6].electoralVotes = 12;
    aStates[7].electoralVotes = 8;
    aStates[8].electoralVotes = 14;
    aStates[9].electoralVotes = 13;
    
    printf(" Checking totalEVs() for 10 States:\n");
    res = totalEVs(aStates,10);
    if (res != 100) {
        printf("  expected total EVs = 100, actual total EVs = %d\n",res);
        return false;
    }

    return true;
}

bool test_totalPVs() {
    State aStates[10];
    int result;
    
    aStates[0].popularVotes = 5;
    aStates[1].popularVotes = 8;
    aStates[2].popularVotes = 12;
    aStates[3].popularVotes = 6;
    aStates[4].popularVotes = 7;
    aStates[5].popularVotes = 10;

    printf(" Checking totalPVs() for 3 States:\n");
    result = totalPVs(aStates, 3);
    if (result != 25) {
        printf("  individual state PVs are 5, 8, 12\n");
        printf("  expected total PVs = 25, actual total PVs = %d\n", result);
        return 0;
    }

    aStates[0].popularVotes = 5;
    aStates[1].popularVotes = 3;
    aStates[2].popularVotes = 2;
    aStates[3].popularVotes = 16;
    aStates[4].popularVotes = 7;
    aStates[5].popularVotes = 5;
    aStates[6].popularVotes = 2;
    aStates[7].popularVotes = 8;
    aStates[8].popularVotes = 4;
    aStates[9].popularVotes = 3;
    
    printf(" Checking totalPVs() for 10 States:\n");
    result = totalPVs(aStates, 10);
    if (result != 55) {
        printf("  expected total PVs = 55, actual total PVs = %d\n", result);
        return false;
    }

    return true;
}

bool test_setSettings() {
    int year;
    bool fastMode = false;
    bool quietMode = false;

    char* args1[] = {"test1", "-f", "-q", "-y", "2020"};
    char* args2[] = {"test3", "-f", "-q"};
    char* args3[] = {"test4", "-f"};
    char* args4[] = {"test5"};

    printf(" Checking 5 valid Arguments\n");
    if (setSettings(5, args1, &year, &fastMode, &quietMode)) {
        if (year == 2020 && fastMode && quietMode) {
            printf("  Test 1 PASSED!\n");
        }
        else {
            printf("  Test 1 FAILED. Settings not updated correctly.\n");
        }
    }
    else {
        printf("  Test 1 FAILED. setSettings() returned false.\n");
    }
    
    printf(" Checking with no year argument\n");
    if (setSettings(3, args2, &year, &fastMode, &quietMode)) {
        if (year == 0 && fastMode && quietMode) {
            printf("  Test 2 PASSED!\n");
        }
        else {
            printf("  Test 2 FAILED. Settings not updated correctly.\n");
        }
    }
    else {
        printf("  Test 2 FAILED. setSettings() returned false.\n");
    }

    printf(" Checking with no year or quiet mode arguments\n");
    if (setSettings(2, args3, &year, &fastMode, &quietMode)) {
        if (year == 0 && fastMode && !quietMode) {
            printf("  Test 3 PASSED!\n");
        }
        else {
            printf("  Test 3 FAILED. Settings not updated correctly.\n");
        }
    }
    else {
        printf("  Test 3 FAILED. setSettings() returned false.\n");
    }

    printf(" Checking with no arguments\n");
    if (setSettings(1, args4, &year, &fastMode, &quietMode)) {
        if (year == 0 && !fastMode && !quietMode) {
            printf("  Test 4 PASSED!\n");
        }
        else {
            printf("  Test 4 FAILED. Settings not updated correctly.\n");
        }
    }
    else {
        printf("  Test 4 FAILED. setSettings() returned false.\n");
    }

    return true;
}

bool test_inFilename() {

    char filename[100];

    inFilename(filename, 2023);
    if (strcmp(filename, "data/2023.csv") != 0) {
        printf("  Test 1 Failed\n");
        return false;
    }

    inFilename(filename, 2020);
    if (strcmp(filename, "data/2020.csv") != 0) {
        printf("  Test 2 Failed\n");
        return false;
    }

    inFilename(filename, 1828);
    if (strcmp(filename, "data/1828.csv") != 0) {
        printf("  Test 3 Failed\n");
        return false;
    }

    return true;
    
}

bool test_outFilename() {
    char filename[100];

    outFilename(filename, 2023);
    if (strcmp(filename, "toWin/2023_win.csv") != 0) {
        printf("  Test 1 Failed\n");
        return false;
    }

    outFilename(filename, 2020);
    if (strcmp(filename, "toWin/2020_win.csv") != 0) {
        printf("  Test 2 Failed\n");
        return false;
    }

    outFilename(filename, 1828);
    if (strcmp(filename, "toWin/1828_win.csv") != 0) {
        printf("  Test 3 Failed\n");
        return false;
    }

    return true;
}

bool test_parseLine() {
    char test1[] = "California,CA,17,885390";
    char test2[] = "Texas,TX,101,17790";
    char test3[] = "New York,NY,190,293590";
    char test4[] = "Illinois,IL";

    State state1;
    State state2;
    State state3;
    State state4;

    bool res1 = parseLine(test1, &state1);
    bool res2 = parseLine(test2, &state2);
    bool res3 = parseLine(test3, &state3);
    bool res4 = parseLine(test4, &state4);

    if (strcmp(state1.name, "California") == 0 && strcmp(state1.postalCode, "CA") == 0 && state1.electoralVotes == 17 && state1.popularVotes == 885390) {
        printf("Test 1 Passed\n");
    }
    else {
        printf("Test 1 Failed\n");
    }

    if (strcmp(state2.name, "Texas") == 0 && strcmp(state2.postalCode, "TX") == 0 && state2.electoralVotes == 101 && state2.popularVotes == 17790) {
        printf("Test 2 Passed\n");
    }
    else {
        printf("Test 2 Failed\n");
    }

    if (strcmp(state3.name, "New York") == 0 && strcmp(state3.postalCode, "NY") == 0 && state3.electoralVotes == 190 && state3.popularVotes == 293590) {
        printf("Test 3 Passed\n");
    }
    else {
        printf("Test 3 Failed\n");
    }

    if (res4 == false) {
        printf("Test 4 Passed\n");
    }
    else {
        printf("Test 4 Failed\n");
    }

    if (res1 && res2 && res3 && !res4) {
        return true;
    }
    return false;
}

bool test_readElectionData() {
    
    FILE* fileOpen = fopen("test_data.csv", "w");

    fprintf(fileOpen, "Illinois,IL,15,18616\n");
    fprintf(fileOpen, "Alabama,AL,5,1916\n");
    fprintf(fileOpen, "California,CA,150,117213\n");
    fprintf(fileOpen, "Texas,TX,143,75827\n");
    fprintf(fileOpen, "Indiana,IN,11,1116\n");

    fclose(fileOpen);

    State allStates[51];
    int nStates = 0;

    bool result = readElectionData("test_data.csv", allStates, &nStates);
    int expectedStateCount = 5;
    State expectedStates[51] = {
        {"Illinois", "IL", 15, 18616},
        {"Alabama", "AL", 5, 1916},
        {"California", "CA", 150, 117213},
        {"Texas", "TX", 143, 75827},
        {"Indiana", "IN", 11, 1116}
    };

    if (result) {
        if (nStates != expectedStateCount) {
            printf("Test Failed: nStates is %d, expected %d\n", nStates, expectedStateCount);
            return false;
        }

        for (int i = 0; i < nStates; i++) {
            if (strcmp(allStates[i].name, expectedStates[i].name) != 0 
            || strcmp(allStates[i].postalCode, expectedStates[i].postalCode) != 0 
            || allStates[i].electoralVotes != expectedStates[i].electoralVotes
            || allStates[i].popularVotes != expectedStates[i].popularVotes) {
                printf("Test Failed: State data doesn't match the expected data\n");
                return false;
            } 
        }
    }
    else {
        printf("Test Failed: readElectionData returned false\n");
        return false;
    }
    
    printf("Test Passed\n");
    return true; 
}

bool test_minPVsSlow() {
    State states[4];

    strcpy(states[0].name, "StateA");
    strcpy(states[0].postalCode, "AA");
    states[0].electoralVotes = 5;
    states[0].popularVotes = 60;

    strcpy(states[1].name, "StateB");
    strcpy(states[1].postalCode, "BB");
    states[1].electoralVotes = 2;
    states[1].popularVotes = 20;

    strcpy(states[2].name, "StateC");
    strcpy(states[2].postalCode, "CC");
    states[2].electoralVotes = 8;
    states[2].popularVotes = 70;

    strcpy(states[3].name, "StateD");
    strcpy(states[3].postalCode, "DD");
    states[3].electoralVotes = 3;
    states[3].popularVotes = 30;

    MinInfo result = minPopVoteToWin(states, 4);

    int expectedSubsetPVs = 47;

    printf("sufficientEVs: %d, subsetPVs: %d\n", result.sufficientEVs, result.subsetPVs);

    if (result.sufficientEVs && result.subsetPVs == expectedSubsetPVs) {
        printf("Test Passed\n");
        return true;
    }
    else {
        printf("Test failed. Got %d instead of 47\n", result.subsetPVs);
        return false;
    }
}

bool test_minPVsFast() {
    State states[4];

    strcpy(states[0].name, "StateA");
    strcpy(states[0].postalCode, "AA");
    states[0].electoralVotes = 5;
    states[0].popularVotes = 60;

    strcpy(states[1].name, "StateB");
    strcpy(states[1].postalCode, "BB");
    states[1].electoralVotes = 2;
    states[1].popularVotes = 20;

    strcpy(states[2].name, "StateC");
    strcpy(states[2].postalCode, "CC");
    states[2].electoralVotes = 8;
    states[2].popularVotes = 70;

    strcpy(states[3].name, "StateD");
    strcpy(states[3].postalCode, "DD");
    states[3].electoralVotes = 3;
    states[3].popularVotes = 30;

    MinInfo result = minPopVoteToWin(states, 4);

    int expectedSubsetPVs = 47;

    printf("sufficientEVs: %d, subsetPVs: %d\n", result.sufficientEVs, result.subsetPVs);

    if (result.sufficientEVs && result.subsetPVs == expectedSubsetPVs) {
        printf("Test Passed\n");
        return true;
    }
    else {
        printf("Test failed. Got %d instead of 47\n", result.subsetPVs);
        return false;
    }
}

int main() {
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");
    
    printf("Testing totalEVs()...\n"); 
    if (test_totalEVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n"); 
    if (test_totalPVs()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n"); 
    if (test_setSettings()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing inFilename()...\n"); 
    if (test_inFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n"); 
    if (test_outFilename()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n"); 
    if (test_parseLine()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n"); 
    if (test_readElectionData()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }
    
    printf("Testing minPopVoteToWin()...\n"); 
    if (test_minPVsSlow()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n"); 
    if (test_minPVsFast()) {
        printf("  All tests PASSED!\n");
    } else {
        printf("  test FAILED.\n");
    }

    return 0;
}
