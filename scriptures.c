#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

/*-------------------------
Steps:
- args input **IN PROG**
- terminal mode **NOT STARTED**
- ReadME **NOT STARTED**
- Making a Package **NOT STARTED**
-------------------------*/

/*-------------------------
Issues:
- inputing [book]:[larger number]-[smaller number] outputs until end of verse
- Unknown behavior at the end of document (articles of faith whole chapter)
- inputing verses that go beyond the bound into another book will produce issues if the current book is 1 chapter
    - add a field to struct that is the bound of book
- inputting a chapter that doesn't exist creates a non-closing loop
- odd capitalization needs to be accepted
    - manipulate input strings into all lowercase
- printing whole chaper (gen 2 specifically) produces incorrect malloc freeing
- D&C issues (probably &)
    - Potentially get rid of &
- Have not handled if book does not exist
    - return 0 if the book isn't found in bibleBooks
- Gen 1 doesn't print ever
- comment on finished parts
    -potentially make clearer variables
- Find new name for BibleBook and bibleBooks
- General outline?
-------------------------*/

/*-------------------------
Test:
- ./gamer gen 1:4 -5
    - **INCORRECT** prints gen 1 4
- ./gamer gen 1 :4-5
    - **INCORRECT** prints all of gen 1 (full chapter mode)
- ./gamer gen 1:4- 5
    - **INCORRECT** prints all of gen 1 (verse-by-verse mode)
-------------------------*/

typedef struct {
    char *name;
    char *abbr;
    char *alt;
    int line;
} BibleBook;

const BibleBook bibleBooks[] = {
    {"Genesis", "Gen", "Gen", 1},
    {"Exodus", "Ex", "Ex", 1534},
    {"Leviticus", "Lev", "Lev", 2747},
    {"Numbers", "Num", "Num", 3606},
    {"Deuteronomy", "Deut", "Deut", 4894},
    {"Joshua", "Josh", "Josh", 5853},
    {"Judges", "Judg", "Judg", 6511},
    {"Ruth", "Ruth", "Ruth", 7129},
    {"1Samuel", "1Sam", "1Sam", 7214},
    {"2Samuel", "2Sam", "2Sam", 8024},
    {"1Kings", "1Kgs", "1Kgs", 8719},
    {"2Kings", "2Kgs", "2Kgs", 9535},
    {"1Chronicles", "1Chr", "1Chr", 10254},
    {"2Chronicles", "2Chr", "2Chr", 11196},
    {"Ezra", "Ezra", "Ezra", 12018},
    {"Nehemiah", "Neh", "Neh", 12298},
    {"Esther", "Esth", "Est", 12704},
    {"Job", "Job", "Job", 12871},
    {"Psalms", "Ps", "Psl", 13941},
    {"Proverbs", "Prov", "Prov", 16402},
    {"Ecclesiastes", "Eccl", "Eccl", 17317},
    {"SongofSolomon", "Song", "Songofsolomon", 17539}, 
    {"Isaiah", "Isa", "Isa", 17656 },
    {"Jeremiah", "Jer", "Jer", 18948},
    {"Lamentations", "Lam", "Lam", 20312},
    {"Ezekiel", "Ezek", "Ezek", 20466},
    {"Daniel", "Dan", "Dan", 21739},
    {"Hosea", "Hosea", "Hos", 22096},
    {"Joel", "Joel", "Joel", 22293},
    {"Amos", "Amos", "Amos", 22366 },
    {"Obadiah", "Obad", "Obad", 22512 },
    {"Jonah", "Jonah", "Jon", 22533},
    {"Micah", "Micah", "Mic", 22581 },
    {"Nahum", "Nahum" , "Nah", 22586},
    {"Habakkuk", "Hab", "Hab", 22733},
    {"Zephaniah", "Zeph", "Zeph", 22789},
    {"Haggai", "Hag", "Hag", 22842},
    {"Zechariah", "Zech", "Zech", 22880},
    {"Malachi", "Mal", "Mal", 23091},
    {"Matthew", "Matt", "Matt", 23146},
    {"Mark", "Mark", "Mark", 24217},
    {"Luke", "Luke", "Luke", 24895},
    {"John", "John", "John", 26046},
    {"Acts", "Acts", "Acts", 26925},
    {"Romans", "Rom", "Rom", 27932},
    {"1Corinthians", "1Cor", "1Cor", 28365},
    {"2Corinthians", "2Cor", "2Cor", 28802},
    {"Galatians", "Gal", "Gal", 29059},
    {"Ephesians", "Eph", "Eph", 29208},
    {"Philippians", "Philip", "Phil", 29363},
    {"Colossians", "Col", "Col", 29467},
    {"1Thessalonians", "1Thes", "1Thess", 29562},
    {"2Thessalonians", "2Thes", "2Thess", 29651},
    {"1Timothy", "1Tim", "1Tim", 29698},
    {"2Timothy", "2Tim", "2Tim", 29811},
    {"Titus", "Titus", "Titus", 29894},
    {"Philemon", "Philem", "Phlm", 29940},
    {"Hebrews", "Heb", "Heb", 29965},
    {"James", "James", "Jas", 30268},
    {"1Peter", "1Pet", "1Pet", 30376},
    {"2Peter", "2Pet", "2Pet", 30481},
    {"1John", "1Jn", "1John", 30542},
    {"2John", "2Jn", "2John", 30647},
    {"3John", "3Jn","3John", 30660},
    {"Jude", "Jude", "Jude", 30674},
    {"Revelation", "Rev", "Rev", 30699},
    {"1Nephi", "1Ne", "1Ne", 31103},
    {"2Nephi", "2Ne", "2Ne", 31721},
    {"Jacob", "Jacob", "Jacob", 32500},
    {"Enos", "Enos", "Enos", 32703},
    {"Jarom", "Jarom", "Jarom", 32730},
    {"Omni", "Omni", "Omni", 32745},
    {"WordsofMormon", "Wofm", "Wom", 32775}, // doesn't encode for wordsofmormon, WOM
    {"Mosiah", "Mosiah", "Mosiah", 32793},
    {"Alma", "Alma", "Alma", 33578},
    {"Helaman", "Hel", "Hel", 35553},
    {"3Nephi", "3Ne", "3Ne", 36050},
    {"4Nephi", "4Ne", "4Ne", 36835},
    {"Mormon", "Morm", "Morm", 36903},
    {"Ether", "Ether", "Ether", 37111},
    {"Moroni", "Moro", "Moro", 37544},
    {"DoctrineAndcovenants", "D&c" ,"Dc", 37707}, //doesn't encode for doctrineandcovenants
    {"Moses", "Moses", "Moses", 41361},
    {"Abraham", "Abr", "Abr", 41717},
    {"Josephsmith—Matthew", "Jsm","Js-M", 41853}, //doesn't encode for js-m, Js-m, any iteration of JosephSmithMatthew, josephsmith-matthew, Josephsmith-matthew, josephsmith-Matthew
    {"Josephsmith—History", "Jsh","Js—H", 41908}, //doesn't encode for js-h, Js-h, any iteration of JosephSmithHistory, josephsmith-history, Josephsmith-history, josephsmith-History
    {"Articlesoffaith", "Aof", "Aoff", 41983}, //doesn't encode for articlesoffaith
    //{"Officialdeclarations", "Od", "Od"}, 
};

int atoi(const char* str){
    int num = 0;
    int i = 0;
    int isNegetive = 0;
    if(str[i] == '-'){
        isNegetive = 1;
        i++;
    }
    while (str[i] && (str[i] >= '0' && str[i] <= '9')){
        num = num * 10 + (str[i] - '0');
        i++;
    }
    if(isNegetive) num = -1 * num;
    return num;
}

int bookLookUp(char* book){
    for( int i = 0; i < sizeof(bibleBooks)/sizeof(BibleBook); i++){
        if (strcmp(book, bibleBooks[i].name) == 0 || strcmp(book, bibleBooks[i].abbr) == 0 || strcmp(book, bibleBooks[i].alt) == 0 ||
            strcmp(book, bibleBooks[i].name) == 32 || strcmp(book, bibleBooks[i].abbr) == 32 || strcmp(book, bibleBooks[i].alt) == 32) {
            return bibleBooks[i].line;
        }
    }
}

void versePrint(char* book, char* chapter, char* verseStart, char* verseEnd){
    char *pinput;
    char cmd_buff[2000];
    FILE* file = fopen("lds-scriptures.txt", "r");
    
    
    int line = bookLookUp(book);

    for(int i = 0; i<line; i++){
        pinput = fgets(cmd_buff, 2000, file);
    }
    printf("\n");
    if(strcmp(verseStart, "ALL")==0){
        int notDone = 1;
        while(notDone){
            pinput = fgets(cmd_buff, 2000, file);
            char* chapt = malloc(sizeof(char)*10);
            int i = 0;
            while(cmd_buff[i]!=':'){
                i++;
            }
            int j = 0;
            while(cmd_buff[i]!=' ' && cmd_buff[i]!='\t'){
                i--;
                j++;
            }
            i++;
            j--;
            strncpy(chapt, &cmd_buff[i], j);
            if(strcmp(chapt, chapter)==0){
                notDone=0;
            }
            
            free(chapt);

        }
        notDone = 1;
        while(notDone){
            printf("%s\n", cmd_buff);
            pinput = fgets(cmd_buff, 2000, file);
            char* chapt = malloc(sizeof(char)*10);
            int i = 0;
            while(cmd_buff[i]!=':'){
                i++;
            }
            int j = 0;
            while(cmd_buff[i]!=' ' && cmd_buff[i]!='\t'){
                i--;
                j++;
            }
            i++;
            j--;
            strncpy(chapt, &cmd_buff[i], j);
            if(strcmp(chapt, chapter)!=0){
                return;
            }
            
            //printf("chapt is: %s, chapter is %s, their comparison is %d",chapt, chapter, strcmp(chapt, chapter));
            //printf(" hey ");
            free(chapt);
        }
        
    }
    else{
        int notDone = 1;
        while(notDone){
            pinput = fgets(cmd_buff, 2000, file);
            char* chapt = malloc(sizeof(char)*10);
            int i = 0;
            while(cmd_buff[i]!=':'){
                i++;
            }
            int j = 0;
            while(cmd_buff[i]!=' ' && cmd_buff[i]!='\t'){
                i--;
                j++;
            }
            i++;
            j--;
            strncpy(chapt, &cmd_buff[i], j);
            if(strcmp(chapt, chapter)==0){
                notDone=0;
            }
            
            free(chapt);

        }
        //printf("%s\n", cmd_buff);
        notDone = 1;
        while(notDone){
            //printf("%s\n", cmd_buff);
            char* verse = malloc(sizeof(char)*10);
            int i = 0;
            while(cmd_buff[i]!=':'){
                i++;
            }
            int k = i;
            int j = 0;
            i++;
            while(cmd_buff[k]!=' ' && cmd_buff[k]!='\t'){
                k++;
                j++;
            }
            //i++;
            j--;
            strncpy(verse, &cmd_buff[i], j);
            if(strcmp(verse, verseStart)==0){
                notDone=0; 
            }
            else{
                pinput = fgets(cmd_buff, 2000, file);
            }
            
            //printf("verse is: %s, verseStart is %s, their comparison is %d",verse, verseStart, strcmp(verse, verseStart));
            //printf(" %c ", cmd_buff[i]);
            free(verse);

        }

        char* chapt = malloc(sizeof(char)*10);
        int i = 0;
        while(cmd_buff[i]!=':'){
            i++;
        }
        int j = 0;
        while(cmd_buff[i]!=' ' && cmd_buff[i]!='\t'){
            i--;
            j++;
        }
        i++;
        j--;
        strncpy(chapt, &cmd_buff[i], j);

        if(strcmp(verseStart, verseEnd)==0){
            printf("%s\n", cmd_buff);
            return;
        }
        else if(strcmp(chapt, chapter)!=0){
            printf("Type 'help' for usage.");
            exit(0);
        }
        free(chapt);

        notDone = 1;
        printf("\n");
        while(notDone){
            printf("%s\n", cmd_buff);




            char* verse = malloc(sizeof(char)*10);
            i = 0;
            while(cmd_buff[i]!=':'){
                i++;
            }
            int k = i;
            j = 0;
            i++;
            while(cmd_buff[k]!=' ' && cmd_buff[k]!='\t'){
                k++;
                j++;
            }
            //i++;
            j--;
            strncpy(verse, &cmd_buff[i], j);

            if(strcmp(verse, verseEnd)==0){
                notDone=0; 
                pinput = fgets(cmd_buff, 2000, file);
            }
            else{
                pinput = fgets(cmd_buff, 2000, file);
            }

            char* chapt = malloc(sizeof(char)*10);
            int i = 0;
            while(cmd_buff[i]!=':'){
                i++;
            }
            int j = 0;
            while(cmd_buff[i]!=' ' && cmd_buff[i]!='\t'){
                i--;
                j++;
            }
            i++;
            j--;
            strncpy(chapt, &cmd_buff[i], j);

            if(strcmp(chapt, chapter)!=0){
                free(verse);
                free(chapt);
                break;
            }
            
            //printf("verse is: %s, verseStart is %s, their comparison is %d",verse, verseStart, strcmp(verse, verseStart));
            //printf(" %c ", cmd_buff[i]);
            free(verse);
            free(chapt);

        }




        
        /*notDone = 1;
        while(notDone){
            printf("%s\n", cmd_buff);
            pinput = fgets(cmd_buff, 2000, file);
            char* chapt = malloc(sizeof(char)*10);
            int i = 0;
            while(cmd_buff[i]!=':'){
                i++;
            }
            int j = 0;
            while(cmd_buff[i]!=' ' && cmd_buff[i]!='\t'){
                i--;
                j++;
            }
            i++;
            j--;
            strncpy(chapt, &cmd_buff[i], j);
            if(strcmp(chapt, chapter)!=0){
                notDone=0;
            }
            
            //printf("chapt is: %s, chapter is %s, their comparison is %d",chapt, chapter, strcmp(chapt, chapter));
            //printf(" %c ", cmd_buff[i]);
            free(chapt);

        }*/
    }
    
    printf("\n\n%d", line);
    
    fclose(file);
}

int main(int argc, char** argv) {

/*-------------------------
Outline:
./gamer **IN PROG**

1. No arguments or undetermined behavior on inputs **DONE**
2. One input
    2a. -t terminal mode **NOT STARTED**
    2b. Incorrect inputs **NOT STARTED**
3. Two, three, or four inputs **IN PROG**
    3a.
4. More inputs **IN PROG**
    4a. 
-------------------------*/

    if(argc==1 || argc==0){
    //------------------------------ **PROG** or Error
        printf("Type 'help' for usage.");
        exit(0);
    }
    else if (argc==2){
    //------------------------------ **PROG** -t
        printf("2");
    }
    else if (argc==3 || argc==4 || argc==5){
        //printf("3");
    //------------------------------ **PROG** [book][requiredSpace][chapter][optSpace][:][optSpace][verse-verse]
        //--------------- Variables
        int bookLine = 0;
        char* chapter = malloc(sizeof(char)*10);
        char* verseStart = malloc(sizeof(char)*10);
        char* verseEnd = malloc(sizeof(char)*10);
    
        if(argv[1]){//bookLine = bookLookUp(argv[1])

            //--------------- : is included in the second arg
            if (strstr(argv[2], ":") != NULL){
                //--------------- if the layout is [book]: *DONE*
                if(argv[2][strlen(argv[2])-1]==':'){
                    argv[2][strlen(argv[2])-1]='\0'; 
                    chapter = argv[2];
                    if(argc==4){
                        if(strstr(argv[3], "-") == NULL){ //**DONE**
                            verseStart = argv[3];
                            verseEnd = argv[3];
                            versePrint(argv[1], chapter, verseStart, verseEnd);
                        }
                        else{   //**DONE**
                            int counter = 0;
                            int end = 0;
                            while(argv[3][counter]!='-'){
                                counter++;
                                end++;
                            }
                            strncpy(verseStart, argv[3], end);
                            strncpy(verseEnd, argv[3]+end+1, strlen(argv[3])-end);
                            versePrint(argv[1], chapter, verseStart, verseEnd);

                        }
                    }
                }
                //--------------- if the layout is [book]:[verse]
                else{//**DONE**
                    int counter = 0;
                    int end = 0;
                    while(argv[2][counter]!=':'){
                        counter++;
                        end++;
                    }
                    strncpy(chapter, argv[2], end);
                    int start = counter + 1;
                    if (strstr(argv[2], "-") != NULL){//**DONE**
                        int newEnd=0;
                        while(argv[2][counter]!='-'){
                        counter++;
                        newEnd++;}
                        strncpy(verseStart, argv[2]+start, newEnd-1);
                        strncpy(verseEnd, argv[2]+start+newEnd, strlen(argv[2])-newEnd-start);
                        versePrint(argv[1], chapter, verseStart, verseEnd);

                        }
                    else{//**DONE**
                        strncpy(verseStart, argv[2]+start, strlen(argv[2])-end);
                        strncpy(verseEnd, argv[2]+start, strlen(argv[2])-end);
                        versePrint(argv[1], chapter, verseStart, verseEnd);

                    }
                }
            }
            //--------------- second arg is just chapter
            else{//**DONE**
                chapter = argv[2];
                verseStart = "ALL";
                verseEnd = "ALL";
                versePrint(argv[1], chapter, verseStart, verseEnd);

            }
            free(chapter);
            free(verseStart);
            free(verseEnd);
        }
        else{
            printf("Type 'help' for usage.");
            exit(0);
        }
    }
    else{
    //------------------------------ Error
        printf("Type 'help' for usage.");
        exit(0);
    }
    
}

