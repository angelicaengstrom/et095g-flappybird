#include "mbed.h"
#include "C12832.h"
#include "bird.h"
#include "pipe.h"
#include <stdio.h>

LocalFileSystem local("local");
C12832 lcd(p5, p7, p6, p8, p11); // Initialize lcd
DigitalIn joystickup(p15);

AnalogIn pot(p19); //potentiometer 1
DigitalOut blinker1(LED1);
DigitalOut blinker2(LED2);
DigitalOut blinker3(LED3);
DigitalOut blinker4(LED4);

InterruptIn center(p14);
Timer t;

Bitmap curr_bitmaptop;
Bitmap curr_bitmapbot;

int bird_ypos = 10;
int bird_xpos = 5;
int pipe_xpos = 50;
int toppipe_edgepos;
int bottompipe_edgepos;
int speed = 0;
int highscore;

void new_highscore(){ //If the player gets a new highscore
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Congratulations!!!\nNEW HIGHSCORE");
    
    for(int i = 0; i < 7; i++){
        blinker1 = blinker2 = blinker3 = blinker4 = 1;
        wait(0.1);
        blinker1 = blinker2 = blinker3 = blinker4 = 0;
        wait(0.1);
    }
    wait(1);
}

void start_screen(int highscore){ //Displays definition of the start_screen
    lcd.cls();
    int potInput = pot * 3;
    lcd.locate(0,0);
    lcd.printf("Press joystick to play...");
    lcd.printf("\nHighscore: %d", highscore);
    
    if(potInput == 0){ //EASY
        speed = 10;
        lcd.printf("\nEasy: %d km/h", speed);
    }
    else if(potInput == 1){ //MEDIUM
        speed = 30;
        lcd.printf("\nMedium: %d km/h", speed);
    }
    else if(potInput == 2){ //HARD
        speed = 60;
        lcd.printf("\nHard: %d km/h", speed);
        }
    else{ //EXPERT
        speed = 100;
        lcd.printf("\nExpert: %d km/h", speed);
        }
    wait(1);
}


void new_pipes(){ //Updates bitmap pipes to a random bitmap
    int rand_pipes = rand() % 4;
    
    if(rand_pipes == 0){
        curr_bitmaptop = bitmaptoppipe1;
        toppipe_edgepos = 5;
        curr_bitmapbot = bitmapbotpipe1;
        bottompipe_edgepos = 24;
    }
    else if(rand_pipes == 1){
        curr_bitmaptop = bitmaptoppipe2;
        toppipe_edgepos = 2;
        curr_bitmapbot = bitmapbotpipe2;
        bottompipe_edgepos = 21;
    }
    else if(rand_pipes == 2){
        curr_bitmaptop = bitmaptoppipe3;
        toppipe_edgepos = 10;
        curr_bitmapbot = bitmapbotpipe3;
        bottompipe_edgepos = 29;
    }
    else if(rand_pipes == 3){
        curr_bitmaptop = bitmaptoppipe4;
        toppipe_edgepos = 7;
        curr_bitmapbot = bitmapbotpipe4;
        bottompipe_edgepos = 26;
    }
}

int highest_score(){ //Checking what the highest score in the file is
    int i = 0;
    int j = 0;
    
    if(speed == 10){ //EASY
        FILE* File1 = fopen("/local/easy.txt","r"); 
        while(!feof(File1)){
            fscanf(File1, "%d", &j);
            if(j > i){
            i = j;
            }
        }
        fclose(File1);
    }
    else if(speed == 30){ //MEDIUM
        FILE* File1 = fopen("/local/medium.txt","r"); 
        while(!feof(File1)){
            fscanf(File1, "%d", &j);
            if(j > i){
            i = j;
            }
        }
        fclose(File1);
    }
    else if(speed == 60){ //HARD
        FILE* File1 = fopen("/local/hard.txt","r"); 
        while(!feof(File1)){
            fscanf(File1, "%d", &j);
            if(j > i){
            i = j;
            }
        }
        fclose(File1);
    }
    else{ //EXPERT
        FILE* File1 = fopen("/local/expert.txt","r"); 
        while(!feof(File1)){
            fscanf(File1, "%d", &j);
            if(j > i){
            i = j;
            }
        }
        fclose(File1);
    }
    return i;
}

void new_frame(int score){ //Update frame of pipes and bird
    bird_ypos = bird_ypos + 2;
    pipe_xpos = pipe_xpos - 4;
    lcd.cls();
    lcd.locate(80,0);
    lcd.printf("Score: %d", score);
    lcd.print_bm(bitmapbird, bird_xpos, bird_ypos);
    lcd.print_bm(curr_bitmaptop, pipe_xpos, 0);
    lcd.print_bm(curr_bitmapbot, pipe_xpos, bottompipe_edgepos);
    lcd.copy_to_lcd();
    t.reset();
}


void play(){ //When player press start, game starts
    //Resets the whole game first
    bird_ypos = 10;
    pipe_xpos = 50;
    toppipe_edgepos = 5;
    bottompipe_edgepos = 24;
    lcd.cls();
    lcd.locate(80,0);
    int score = 0;
    
    curr_bitmaptop = bitmaptoppipe1;
    curr_bitmapbot = bitmapbotpipe1;
    
    lcd.printf("Score: 0");
    lcd.print_bm(bitmapbird, bird_xpos, bird_ypos);
    lcd.print_bm(curr_bitmaptop, pipe_xpos, 0);
    lcd.print_bm(curr_bitmapbot, pipe_xpos, bottompipe_edgepos);
    lcd.copy_to_lcd();
    
    while(1){ //Wait for user interaction before game starts
        if(joystickup){
            break;
        }
    }
    
    t.start();
    while(1){
        if(joystickup & bird_ypos > 0){ //If joystick is pressed upwards, bird flies
            lcd.cls();
            bird_ypos = bird_ypos - 4;
            
            lcd.locate(80,0);
            lcd.printf("Score: %d", score);
            lcd.print_bm(bitmapbird, bird_xpos, bird_ypos);
            lcd.print_bm(curr_bitmaptop, pipe_xpos, 0);
            lcd.print_bm(curr_bitmapbot, pipe_xpos, bottompipe_edgepos);
            lcd.copy_to_lcd();
            
            if(speed < 60){
                wait(0.2);
            }
            else{
                wait(0.05);
            }
        }
        if((bird_ypos >= 20) || ((pipe_xpos <= 17) && ((bird_ypos <= toppipe_edgepos) || bird_ypos >= bottompipe_edgepos - 10))){ //When bird is on floor or the same position as pipes
            lcd.cls();
            lcd.locate(0,0);
            lcd.printf("You lost! Score: %d", score);
            wait(1);
            lcd.cls();
            break;
            }
            
        if(speed == 10 & t > 0.5){ //EASY
            new_frame(score);
        }
        else if(speed == 30 & t > 0.3){ //MEDIUM
            new_frame(score);
        }
        else if(speed == 60 & t > 0.1){ //HARD
            new_frame(score);
        }
        else if(speed == 100 & t > 0.05){ //EXPERT
            new_frame(score);
        }
        
        if(pipe_xpos <= bird_xpos){ //If pipes have passed bird x-position
            score = score + 10;
            pipe_xpos = 50;
            
            new_pipes();
        }
    }
    
    //GAMEOVER
    if(speed == 10){ //EASY
        FILE* File1 = fopen("/local/easy.txt","a");
        fprintf(File1, "\n%d", score);
        fclose(File1);
    }
    else if(speed == 30){ //MEDIUM
        FILE* File1 = fopen("/local/medium.txt","a");
        fprintf(File1, "\n%d", score);
        fclose(File1);
    }
    else if(speed == 60){ //HARD
        FILE* File1 = fopen("/local/hard.txt","a");
        fprintf(File1, "\n%d", score);
        fclose(File1);
    }
    else{ //EXPERT
        FILE* File1 = fopen("/local/expert.txt","a");
        fprintf(File1, "\n%d", score);
        fclose(File1);
    }
    
    highscore = highest_score(); //Update highscore
    
    if(score == highscore){ //If the final score is the same as high score
        new_highscore();
    }
}

int main(){
    FILE* File1 = fopen("/local/expert.txt","a"); //creating files if they doesnt exist on mbed
    fclose(File1);
    File1 = fopen("/local/hard.txt","a");
    fclose(File1);
    File1 = fopen("/local/medium.txt","a");
    fclose(File1);
    File1 = fopen("/local/easy.txt","a");
    fclose(File1);
    
    center.rise(&play);
    highscore = highest_score();
    int old_speed = speed;
    
    while(1){
        start_screen(highscore);
        if(old_speed != speed){ //If potentiometer have changed value
            old_speed = speed;
            highscore = highest_score();
        }
    }
}