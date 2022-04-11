#include <iostream>
#include<vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include<math.h>


using namespace std;

static int n_entity = 0;//number of static variable
static int n_normal = 0;
static int n_infected = 0;
static int n_dead = 0;
static int n_recovered = 0;



class Entity {
public:
    int location_x;
    int location_y;
    string gens;
    char situation;//n=normal , i=infected; d=dead, r=recovered p=protected w=weakness g=get immune e=not exist.
    int weakness = 0;
    int infturn = 3;// 
    int deadturn14 = 14;
    int immuneturn = 30;
    int deadturn5 =5;

};

int findsize(string a) {
    int tsize;
    int tsize1;
    int tsize2;
    int size1 = 0;
    if (a.length() == 6) {
        tsize = a[5];
        size1 = tsize - 48;
    }
    else if (a.length() == 7) {

        tsize = a[5];
        tsize1 = a[6];
        size1 = 10 * (tsize - 48) + (tsize1 - 48);
    }
    else
    {
        tsize = a[5];
        tsize1 = a[6];
        tsize2 = a[7];
        size1 = 100 * (tsize - 48) + 10 * (tsize1 - 48) + (tsize2 - 48);
    }
    return size1;
}

int findturn_count(string a) {
    int turncount = 0;
    int t_turncount;
    int t_turncount1;
    int t_turncount2;
    if (a.length() == 12) {
        t_turncount = a[11];
        turncount = (t_turncount - 48);
    }
    else if (a.length() == 13) {
        t_turncount = a[11];
        t_turncount1 = a[12];
        turncount = 10 * (t_turncount - 48) + (t_turncount1 - 48);
    }
    else {
        t_turncount = a[11];
        t_turncount1 = a[12];
        t_turncount2 = a[13];
        turncount = 100 * (t_turncount - 48) + 10 * (t_turncount1 - 48) + (t_turncount2 - 48);
    }

    return turncount;
}
void printvector(vector<string>& a) {//for control the reading 
    vector<string>::iterator i = a.begin();
    while (i != a.end()) {
        cout << *i << endl;
        i++;
    }
}
vector<string>dosplit(string line, char delimiter)//split vector string as a line word
{
    vector<string> result;
    string word = "";
    for (char x : line)
    {
        if (x == delimiter)
        {
            result.push_back(word);
            word = "";
        }
        else if (x != '\n')
        {
            word = word + x;
        }
    }
    result.push_back(word);
    return result;

}

vector<int>distancecalculate(vector<Entity>a) {//distance calculate with abs and vector 
    vector<int>tresult;
    vector<int>result;
    int temp1, temp2;
    for (int i = 0; i < n_entity - 1; i++) {
        for (int j = 0; j < n_entity; j++) {
            if (i != j) {
                temp1 = abs(a[i].location_x - a[j].location_x);
                temp2 = abs(a[i].location_y - a[j].location_y);
                if (temp1 >= temp2) { tresult.push_back(temp1); }
                else { tresult.push_back(temp2); }
            }
            else { continue; }
        }
    }

    int c = n_entity - 2;



    for (int i = 0; i < n_entity - 2; i++) {

        tresult.erase(tresult.begin() + (c * (n_entity - 1)), tresult.begin() + (c * (n_entity)));
        c--;
    }


    return tresult;
}







int main() {
    int size = 0;
    int turn_count = 0;

    string tread;
    vector<string>read;//recorded the text in here
    vector<string>readonlyentity;//recoded only entity info
    vector<string>readonlygen;//recorded only gen string like ABCDE
    vector<string>readonlycoordinate;//recorded only coordinate string like 3x4
    vector<string>readonlyturn;//recorded only turn text





    ifstream myfile("INPUT.TXT");
    if (myfile.is_open())
    {
        while (getline(myfile, tread))
        {
            read.push_back(tread);


            if (tread[0] == 's' || tread[0] == 'S') {
                size = findsize(tread);
            }
            else if (tread[4] == '_') {
                turn_count = findturn_count(tread);
            }
            else if (tread[0] == 'e' || tread[0] == 'E') {
                n_entity++;
            }

        }

        myfile.close();
    }
    else
    {
        cout << "File error....";
    }


    ofstream write;
    write.open("TURNS.TXT", ios::out);

    //split string by using function and loop
    for (int i = 2; i < (read.size()) - turn_count; i++) {//recorded entities string line
        readonlyentity.push_back(read[i]);

    }
    for (int i = 2 + readonlyentity.size(); i < 2 + readonlyentity.size() + turn_count; i++) {//recorded turn string line
        readonlyturn.push_back(read[i]);

    }

    for (int i = 0; i < readonlyentity.size(); i++) {//recoreded only gen string
        vector<string>tempgen = dosplit(readonlyentity[i], ' ');
        readonlygen.push_back(tempgen[2]);
    }
    for (int i = 0; i < readonlyentity.size(); i++) {//recorded only location string
        vector<string>tempcoordinate = dosplit(readonlyentity[i], ' ');
        readonlycoordinate.push_back(tempcoordinate[3]);
    }



    //create entity object

    vector<Entity>entities(readonlyentity.size());


    for (int i = 0; i < readonlyentity.size(); i++) {//situation conditions 

        entities[i].gens = readonlygen[i];//step 1
        if (readonlyentity[i].back() == 'd') {//if last char is d  , infected
            entities[i].situation = 'i';//n=normal , i=infected; d=dead, r=recovered p=protected.
        }
        else {
            if (readonlygen[i].at(0) == 'A' || readonlygen[i].at(0) == 'B') {
                entities[i].situation = 'n';
            }
            else {
                if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B') {
                    entities[i].situation = 'n';
                }
                else {
                    if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B') {
                        entities[i].situation = 'n';
                    }
                    else {
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B') {
                            entities[i].situation = 'n';

                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B') {
                                entities[i].situation = 'n';
                            }
                            else
                            {
                                entities[i].situation = 'p';
                            }
                        }
                    }
                }
            }


        }

    }



    vector<string>strx;//string only x
    vector<string>stry;//string only y

    for (int i = 0; i < readonlycoordinate.size(); i++) {
        vector<string>temp = dosplit(readonlycoordinate[i], 'x');
        strx.push_back(temp[0]);//x[0] is first integer(typestring) of readonlycoordinate[0] 
        stry.push_back(temp[1]);//y[0] is first integer(typestring) of readonlycoordinate[0] 
    }
    vector<int>x;//temp x
    vector<int>y;//temp y
    for (int i = 0; i < readonlycoordinate.size(); i++) {//x and y integer coordinate use ascýý table number char to integer ...
        if (strx[i].length() == 1) {
            x.push_back(strx[i].at(0) - 48);
        }
        else if (strx[i].length() == 2) {
            x.push_back(10 * (strx[i].at(0) - 48) + (strx[i].at(1) - 48));
        }
        else if (strx[i].length() == 3) {
            x.push_back(100 * (strx[i].at(0) - 48) + (10 * (strx[i].at(1) - 48)) + (strx[i].at(2) - 48));
        }
        else
            x.push_back((1000 * (strx[i].at(0) - 48)) + (100 * (strx[i].at(1) - 48)) + (10 * (strx[i].at(2) - 48)) + (strx[i].at(3) - 48));

        if (stry[i].length() == 1) {
            y.push_back(stry[i].at(0) - 48);
        }
        else if (stry[i].length() == 2) {
            y.push_back(10 * (stry[i].at(0) - 48) + (stry[i].at(1) - 48));
        }
        else if (stry[i].length() == 3) {
            y.push_back(100 * (stry[i].at(0) - 48) + (10 * (stry[i].at(1) - 48)) + (stry[i].at(2) - 48));
        }
        else
            y.push_back((1000 * (stry[i].at(0) - 48)) + (100 * (stry[i].at(1) - 48)) + (10 * (stry[i].at(2) - 48)) + (stry[i].at(3) - 48));
    }


    for (int i = 0; i < readonlyentity.size(); i++) {
        entities[i].location_x = x[i];
        entities[i].location_y = y[i];
    }
    for (int i = 0; i < turn_count; i++) {
        if (i < 9)
        {
            readonlyturn[i].erase(readonlyturn[i].begin(), readonlyturn[i].begin() + 7);
        }
        else if (i < 99 && i >= 9) {
            readonlyturn[i].erase(readonlyturn[i].begin(), readonlyturn[i].begin() + 8);
        }
        else
            readonlyturn[i].erase(readonlyturn[i].begin(), readonlyturn[i].begin() + 9);
    }

    vector<string>tempturn;

    for (int i = 0; i < turn_count; i++) {
        vector<string>temp = dosplit(readonlyturn[i], ' ');

        for (int j = 0; j < readonlyentity.size(); j++)
        {
            tempturn.push_back(temp[j]);

        }
    }
    vector<string>strturnx;//temp x string
    vector<string>strturny;//temp y string


    for (int i = 0; i < turn_count; i++) {
        vector<string>temp = dosplit(readonlyturn[i], ' ');

        for (int j = 0; j < readonlyentity.size(); j++) {
            vector<string>temp1 = dosplit(temp[j], 'x');
            strturnx.push_back(temp1[0]);
            strturny.push_back(temp1[1]);
        }

    }


    vector<int>turnx;
    vector<int>turny;
    for (int i = 0; i < turn_count * readonlyentity.size(); i++) {//string to integer for x and y using ascýý number
        if (strturnx[i].length() == 1) {
            turnx.push_back(strturnx[i].at(0) - 48);
        }
        else if (strturnx[i].length() == 2) {
            turnx.push_back(10 * (strturnx[i].at(0) - 48) + (strturnx[i].at(1) - 48));
        }
        else if (strturnx[i].length() == 3) {
            turnx.push_back((100 * (strturnx[i].at(0) - 48)) + 10 * (strturnx[i].at(1) - 48) + (strturnx[i].at(2) - 48));

        }
        else
            turnx.push_back((1000 * (strturnx[i].at(0) - 48)) + 100 * (strturnx[i].at(1) - 48) + 10 * (strturnx[i].at(2) - 48) + (strturnx[i].at(3) - 48));

        //--------------------------------------------------//
        if (strturny[i].length() == 1) {
            turny.push_back(strturny[i].at(0) - 48);
        }
        else if (strturny[i].length() == 2) {
            turny.push_back(10 * (strturny[i].at(0) - 48) + (strturny[i].at(1) - 48));
        }
        else if (strturny[i].length() == 3) {
            turny.push_back((100 * (strturny[i].at(0) - 48)) + 10 * (strturny[i].at(1) - 48) + (strturny[i].at(2) - 48));

        }
        else
            turny.push_back((1000 * (strturny[i].at(0) - 48)) + 100 * (strturny[i].at(1) - 48) + 10 * (strturny[i].at(2) - 48) + (strturny[i].at(3) - 48));
    }



    //-----------------------------------------------------------------------------//
    //start 
    for (int i = 0; i < readonlyentity.size(); i++) {
        if (entities[i].situation == 'i') {

            if (readonlygen[i].at(0) == 'A' || readonlygen[i].at(0) == 'B' || readonlygen[i].at(0) == 'C' || readonlygen[i].at(0) == 'D') {//1
                entities[i].weakness++;
                if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B' || readonlygen[i].at(1) == 'C' || readonlygen[i].at(1) == 'D') {//1-2
                    entities[i].weakness++;
                    if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {//1-2-3
                        entities[i].weakness++;
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//1-2-3-4
                            entities[i].weakness++;
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-2-3-4-5
                                entities[i].weakness++;
                            }
                        }
                    }
                    else if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//1-2-4
                        entities[i].weakness++;
                        if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-2-4-5
                            entities[i].weakness++;
                        }
                    }
                    else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-2-5
                        entities[i].weakness++;
                    }
                }

                else if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {//1-3
                    entities[i].weakness++;
                    if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//1-3-4
                        entities[i].weakness++;
                        if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-3-4-5
                            entities[i].weakness++;
                        }
                    }
                    else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-3-5
                        entities[i].weakness++;
                    }
                }


                else if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//1-4
                    entities[i].weakness++;
                    if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-4-5
                        entities[i].weakness++;
                    }

                }

                else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-5
                    entities[i].weakness++;
                }
            }


            else if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B' || readonlygen[i].at(1) == 'C' || readonlygen[i].at(1) == 'D') {//2
                entities[i].weakness++;

                if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {//2-3
                    entities[i].weakness++;
                    if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//2-3-4
                        entities[i].weakness++;
                        if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//2-3-4-5
                            entities[i].weakness++;
                        }
                    }
                    else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//2-3-5
                        entities[i].weakness++;
                    }
                }
                else if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//2-4
                    entities[i].weakness++;

                    if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//2-4-5
                        entities[i].weakness++;
                    }
                }
                else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//2-5
                    entities[i].weakness++;
                }

            }


            else if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {//3
                entities[i].weakness++;

                if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//3-4
                    entities[i].weakness++;
                    if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//3-4-5
                        entities[i].weakness++;
                    }
                }
                else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//3-5
                    entities[i].weakness++;
                }
            }


            else if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//4
                entities[i].weakness++;
                if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//4-5
                    entities[i].weakness++;
                }

            }

            else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//5
                entities[i].weakness++;
            }
        }
           
/*
            if (readonlygen[i].at(0) == 'A' || readonlygen[i].at(0) == 'B' || readonlygen[i].at(0) == 'C' || readonlygen[i].at(0) == 'D') {
                entities[i].weakness++;
                if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B' || readonlygen[i].at(1) == 'C' || readonlygen[i].at(1) == 'D') {
                    entities[i].weakness++;
                    if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {
                        entities[i].weakness++;
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {
                            entities[i].weakness++;
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                    }
                    else {
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {
                            entities[i].weakness++;
                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                    }
                }
                else {
                    if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {
                        entities[i].weakness++;

                    }
                    else {
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {
                            entities[i].weakness++;
                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                    }
                }
            }
            else {
                if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B' || readonlygen[i].at(1) == 'C' || readonlygen[i].at(1) == 'D') {
                    entities[i].weakness++;
                }
                else {
                    if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {
                        entities[i].weakness++;
                    }
                    else {
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {
                            entities[i].weakness++;
                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                    }
                }
            }
        } */
        else if (entities[i].situation == 'p') {
            entities[i].weakness = 0;
        }
        else if (entities[i].situation == 'n') {

        if (readonlygen[i].at(0) == 'A' || readonlygen[i].at(0) == 'B' || readonlygen[i].at(0) == 'C' || readonlygen[i].at(0) == 'D') {//1
            entities[i].weakness++;
            if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B' || readonlygen[i].at(1) == 'C' || readonlygen[i].at(1) == 'D') {//1-2
                entities[i].weakness++;
                if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {//1-2-3
                    entities[i].weakness++;
                    if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//1-2-3-4
                        entities[i].weakness++;
                        if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-2-3-4-5
                            entities[i].weakness++;
                        }
                    }
                }
                else if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//1-2-4
                    entities[i].weakness++;
                    if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-2-4-5
                        entities[i].weakness++;
                    }
                }
                else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-2-5
                    entities[i].weakness++;
                }
            }

            else if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {//1-3
                entities[i].weakness++;
                if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//1-3-4
                    entities[i].weakness++;
                    if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-3-4-5
                        entities[i].weakness++;
                    }
                }
                else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-3-5
                    entities[i].weakness++;
                }
            }


            else if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//1-4
                entities[i].weakness++;
                if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-4-5
                    entities[i].weakness++;
                }

            }

            else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//1-5
                entities[i].weakness++;
            }
        }


        else if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B' || readonlygen[i].at(1) == 'C' || readonlygen[i].at(1) == 'D') {//2
            entities[i].weakness++;

            if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {//2-3
                entities[i].weakness++;
                if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//2-3-4
                    entities[i].weakness++;
                    if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//2-3-4-5
                        entities[i].weakness++;
                    }
                }
                else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//2-3-5
                    entities[i].weakness++;
                }
            }
            else if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//2-4
                entities[i].weakness++;

                if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//2-4-5
                    entities[i].weakness++;
                }
            }
            else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//2-5
                entities[i].weakness++;
            }

        }


        else if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {//3
            entities[i].weakness++;

            if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//3-4
                entities[i].weakness++;
                if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//3-4-5
                    entities[i].weakness++;
                }
            }
            else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//3-5
                entities[i].weakness++;
            }
        }


        else if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {//4
            entities[i].weakness++;
            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//4-5
                entities[i].weakness++;
            }

        }

        else if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {//5
            entities[i].weakness++;
        }

        }

            /*if (readonlygen[i].at(0) == 'A' || readonlygen[i].at(0) == 'B' || readonlygen[i].at(0) == 'C' || readonlygen[i].at(0) == 'D') {
                entities[i].weakness++;
                if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B' || readonlygen[i].at(1) == 'C' || readonlygen[i].at(1) == 'D') {
                    entities[i].weakness++;
                    if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {
                        entities[i].weakness++;
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {
                            entities[i].weakness++;
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                    }
                    else {
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {
                            entities[i].weakness++;
                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                    }
                }
                else {
                    if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {
                        entities[i].weakness++;
                    }
                    else {
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {
                            entities[i].weakness++;
                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                    }
                }
            }
            else {
                if (readonlygen[i].at(1) == 'A' || readonlygen[i].at(1) == 'B' || readonlygen[i].at(1) == 'C' || readonlygen[i].at(1) == 'D') {
                    entities[i].weakness++;
                }
                else {
                    if (readonlygen[i].at(2) == 'A' || readonlygen[i].at(2) == 'B' || readonlygen[i].at(2) == 'C' || readonlygen[i].at(2) == 'D') {
                        entities[i].weakness++;
                    }
                    else {
                        if (readonlygen[i].at(3) == 'A' || readonlygen[i].at(3) == 'B' || readonlygen[i].at(3) == 'C' || readonlygen[i].at(3) == 'D') {
                            entities[i].weakness++;
                        }
                        else {
                            if (readonlygen[i].at(4) == 'A' || readonlygen[i].at(4) == 'B' || readonlygen[i].at(4) == 'C' || readonlygen[i].at(4) == 'D') {
                                entities[i].weakness++;
                            }
                            else {
                                entities[i].weakness = entities[i].weakness;
                            }
                        }
                    }
                }
            }*/
        

        if (entities[i].situation == 'n' && entities[i].weakness >= 3) {
            entities[i].situation = 'w';//weakness delimiter of entity for the virus
        }
    }
   
    //-----------------------------------------------------------------------------------//
    //-------------------------------------LAST-STEP-------------------------------------//
    //-----------------------------------------------------------------------------------//

 /*  for (int i = 0; i < n_entity; i++) {
        if (entities[i].situation == 'i') {
            entities[i].infturn++;
            
    }
    }
  */
   


    int turncalc = 0;

    for (int i = 0; i < turn_count * readonlyentity.size();) {

        for (int j = 0; j < readonlyentity.size(); j++) {
         
           if (entities[j].situation == 'd') {
            
                entities[j].location_x = entities[j].location_x;
                entities[j].location_y = entities[j].location_y;
            }
            else if(entities[j].situation =='e' ){
                entities[j].location_x = entities[j].location_x;
                entities[j].location_y = entities[j].location_y;
            }
            else{
            entities[j].location_x = turnx[i];
            entities[j].location_y = turny[i];
            }
             // cout << entities[j].location_x << "," << entities[j].location_y << endl;
            i++;

        }


        for (int k = 0; k < readonlyentity.size(); k++) {
            if (entities[k].situation == 'i') {//infected turns
                entities[k].infturn--;
            }

            if (entities[k].situation == 'i' && entities[k].weakness < 3) {
                entities[k].immuneturn--;
            }


            if (entities[k].immuneturn < 0 && entities[k].weakness < 3) {//30 turn rule
                entities[k].situation = 'g';//get immune
            }


            if (entities[k].situation == 'd') {
                entities[k].deadturn5--;
            }


            if (entities[k].deadturn5 == 0) {//after 5 turn ,clear the entity.
                entities[k].situation = 'e';//not exist
            }

            if (entities[k].situation == 'i' && entities[k].weakness >= 3) {
                entities[k].deadturn14--;
            }




            if (entities[k].infturn < 0 && entities[k].situation == 'i') {   //3 turn rule 

                for (int m = 0; m < readonlyentity.size(); m++) {
                    if (k != m) {
                        int distance, temp1, temp2;
                        temp1 = abs(entities[k].location_x - entities[m].location_x);
                        temp2 = abs(entities[k].location_y - entities[m].location_y);
                        if (temp1 >= temp2) { distance = temp1; }
                        else { distance = temp2; }
                        //distance = sqrt((temp1 * temp1) + (temp2 * temp2));

                        if (distance <= 3 && (entities[m].situation == 'n' || entities[m].situation == 'w')) {
                            entities[m].situation = 'i';
                        }
                    }
                }
            }






            if (entities[k].deadturn14 < 0 && entities[k].situation == 'i') {//14 turn rule
                entities[k].situation = 'd';//death
                for (int m = 0; m < readonlyentity.size(); m++) {
                    if (k != m) {
                        int distance, temp1, temp2;
                        temp1 = abs(entities[k].location_x - entities[m].location_x);
                        temp2 = abs(entities[k].location_y - entities[m].location_y);
                        if (temp1 >= temp2) { distance = temp1; }
                       else { distance = temp2; }
                       // distance = sqrt((temp1 * temp1) + (temp2 * temp2));
                        if (distance <= 3 && (entities[m].situation == 'w' || entities[m].situation == 'n')) {
                            entities[m].situation = 'i';
                        }
                    }
                }
            }
            

           













        }
        //----------------------------------------------------------turns.txt------------------------------------//
       //-------------------------------------------------------------------------------------------------------//
      //-------------------------------------------------------------------------------------------------------//
     //-------------------------------------------------------------------------------------------------------//

       

        turncalc++;
        

        write << "Turn" << turncalc << ":" << endl;

        int control = 0;



        for (int i = 0; i < size + 2; i++) {//  Y
            for (int j = 0; j < size + 2; j++) {//  X
                if (j > 0 && i > 0 && i < size + 1 && j < size + 1) {
                    for (int k = 0; k < n_entity; k++) {
                        if (entities[k].location_x == i && entities[k].location_y == j) {
                            if (entities[k].situation == 'i') { write << "X"; }//n=normal , i=infected; d=dead, r=recovered p=protected w=weakness g=get immune e=not exist.
                            else if (entities[k].situation == 'n') { write << "O"; }
                            else if (entities[k].situation == 'w') { write << "O"; }
                            else if (entities[k].situation == 'p') { write << "O"; }
                            else if (entities[k].situation == 'd') { write << "D"; }
                            else if (entities[k].situation == 'g') { write << "0"; }
                            else if (entities[k].situation == 'e') { write << " "; }
                            control = 1;
                        }
                    }
                    if (control == 0) { write << " "; }
                    if (control == 1) { control = 0; }

                }
                else {
                    write << "-";
                }

            }
            write << '\n';
        }

       


        //----------------------------------------------------------turns.txt------------------------------------//
       //-------------------------------------------------------------------------------------------------------//
      //-------------------------------------------------------------------------------------------------------//
     //-------------------------------------------------------------------------------------------------------//

    }
    for (int i = 0; i < n_entity; i++) {////n=normal , i=infected; d=dead, r=recovered p=protected w=weakness g=get immune e=not exist.
        if (entities[i].situation == 'n') { n_normal++; }
        else if (entities[i].situation == 'p') { n_normal++; }
        else if (entities[i].situation == 'w') { n_normal++; }
        else if (entities[i].situation == 'd') { n_dead++; }
        else if (entities[i].situation == 'e') { n_dead++; }
        else if (entities[i].situation == 'i') { n_infected++; }
        else if (entities[i].situation == 'g') { n_recovered++; }
    }

    write.close();
    
    /*
    static int n_entity = 0;//number of static variable
    static int n_normal = 0;
    static int n_infected = 0;
    static int n_dead = 0;
    static int n_recovered = 0;
    */


    ///---------------------------OUTPUT.TXT---------------------------------------/////
  
    ofstream write1;
    write1.open("OUTPUT.TXT", ios::out);

    write1 << "Normal     : " << n_normal << endl;
    write1 << "Infected   : " << n_infected << endl;
    write1 << "Dead       : " << n_dead << endl;
    write1 << "Recovered  : " << n_recovered << endl;
    for (int i = 0; i < n_entity; i++) {
        string lastsit;//last situation
        if (entities[i].situation == 'n') { lastsit = "normal"; }
        else if (entities[i].situation == 'e') { lastsit = "dead"; }
        else if (entities[i].situation == 'i') { lastsit = "infected"; }
        else if (entities[i].situation == 'g') { lastsit = "immune"; }
        else if (entities[i].situation == 'p') { lastsit = "normal"; }
        else if (entities[i].situation == 'w') { lastsit = "normal"; }
        else if (entities[i].situation == 'd') { lastsit = "dead"; }
        write1 << "entity " << i + 1 << " " << entities[i].location_x << "x" << entities[i].location_y << " " << lastsit << endl;
    }
    write1.close();



    ///------------------------------------------------------------------------------\\\
   ///-------------------------------------THE--END-----------------------------------\\\
  ///----------------------------------------------------------------------------------\\\



    return 0;
}