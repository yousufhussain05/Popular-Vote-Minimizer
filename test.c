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
        printf("  individual state EVs are 5, 8, 12, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n",res);
        return false;
    }

    printf(" Checking totalEVs() for 6 States:\n");
    res = totalEVs(aStates,6);
    if (res != 48) {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10\n");
        printf("  expected total EVs = 48, actual total EVs = %d\n",res);
        return false;
    }

    printf(" Checking totalEVs() for 3 States:\n");
    res = totalEVs(aStates, 3);
    if (res != 25) {
        return false;
    }
    return true;
}

bool test_totalPVs() {
    State aStates[3];
    int res;
    
    aStates[0].popularVotes = 5;
    aStates[1].popularVotes = 8;
    aStates[2].popularVotes = 12;

    res = totalPVs(aStates, 3);
    if (res != 25) {
        return false;
    }
    return true;
}

bool test_setSettings() {
    int year;
    bool fastMode, quietMode;

    char *args1[] = {"./app.exe", "-y", "2020", "-f"};
    if (!setSettings(4, args1, &year, &fastMode, &quietMode) || year != 2020 || !fastMode) {
        return false;
    }

    char *args2[] = {"./app.exe", "-q"};
    if (!setSettings(2, args2, &year, &fastMode, &quietMode) || !quietMode) {
        return false;
    }

    char *args3[] = {"./app.exe", "-x"};
    if (setSettings(2, args3, &year, &fastMode, &quietMode)) {
        return false;
    }
    
    char *args4[] = {"./app.exe", "-y", "1800", "-q", "-f"};
    if (!setSettings(5, args4, &year, &fastMode, &quietMode) || year != 0 || !quietMode || !fastMode) {
        return false;
    }

    char *args5[] = {"./app.exe", "-y"};
    if (setSettings(2, args5, &year, &fastMode, &quietMode)) {
        return false;
    }

    return true;
}

bool test_inFilename() {
    char fileName[20];
    inFilename(fileName, 2020);
    if (strcmp(fileName, "data/2020.csv") != 0) {
        return false;
    }
    return true;
}

bool test_outFilename() {
    char fileName[20];
    outFilename(fileName, 2020);
    if (strcmp(fileName, "toWin/2020_win.csv") != 0) {
        return false;
    }
    return true;
}

bool test_parseLine() {
    State aState;
    char line[] = "Illinois,IL,20,5000000";
    if (!parseLine(line, &aState)) {
        return false;
    }

    if (strcmp(aState.name, "Illinois") != 0 || strcmp(aState.postalCode, "IL") != 0 || aState.electoralVotes != 20 || aState.popularVotes != 5000000) {
        return false;
    }
    return true;
}

bool test_readElectionData() {
    State allStates[51];
    int numStates;
    char input1[] = "data/2016.csv";
    char input2[] = "data/2021.csv";
    
    bool test1 = readElectionData(input1, allStates, &numStates);

    if (!test1 || numStates != 51) {
        return false;
    }
    return true;
}

bool test_minPVsSlow() {
    State aState[4] = {
        {"StateA", "AA", 10, 1000},
        {"StateB", "BB", 20, 2000},
        {"StateC", "CC", 30, 3000},
        {"StateD", "DD", 40, 4000}
    };
    MinInfo result = minPopVoteToWin(aState, 4);
    if (result.subsetPVs == 3002 && result.szSomeStates == 2) {
        if ((strcmp(result.someStates[0].name, "StateD") == 0 && strcmp(result.someStates[1].name, "StateB") == 0)) {
            return true;
        } else {
            return false;
        }
    }
    return false;   
}

bool test_minPVsFast() {
    State aState[4] = {
        {"StateA", "AA", 10, 1000},
        {"StateB", "BB", 20, 2000},
        {"StateC", "CC", 30, 3000},
        {"StateD", "DD", 40, 4000}
    };
    MinInfo result = minPopVoteToWinFast(aState, 4);
    if (result.subsetPVs == 3002 && result.szSomeStates == 2) {
        if ((strcmp(result.someStates[0].name, "StateD") == 0 && strcmp(result.someStates[1].name, "StateB") == 0)) {
            return true;
        } else {
            return false;
        }
    }
    return false;   
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
