#include "highscore.h"

#include <stdio.h>
#include <stdlib.h>

int readHighscore()
{
    int score;
    FILE *datei = fopen("highscore.txt", "r");
    if (datei == NULL) {
        return 0;
    }
    fscanf(datei, "%i", &score);
    fclose(datei);
    return score;
}

void saveScore(int score)
{
    FILE *datei = fopen("highscore.txt", "w");	
    fprintf(datei, "%i", score);
    fclose(datei);
}

void saveHighscore(int score)
{
    if (score > readHighscore()) {
        saveScore(score);
    }
}