#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <map>

using namespace std;

#define EMPTY -2.0
#define ll long lon
#define MAT vector<vector<double> >

class CF {
private:
    MAT userToItem, itemToItem;
    vector<double> average;
    int itemSize, userSize;
public:
    CF(MAT userToItem) {
        this->userToItem = userToItem;
        
        userSize = (int)userToItem.size()-1;
        itemSize = (int)userToItem.back().size()-1;
        average = vector<double> (userSize+1, 0.0);
    }
    
    void run () {
        buildItemToItemC();
    }
    
    void getAverage() {
        for(int i=1;i<=userSize;i++) {
            int cnt = 0;
            for(int j=1;j<=itemSize;j++) {
                if(userToItem[i][j] == EMPTY) continue;
                cnt++;
                average[i] += userToItem[i][j];
            }
            average[i] /= (double)cnt;
        }
    }
    
    void buildItemToItemC() {
        itemToItem = vector<vector<double> > (itemSize+1, vector<double> (itemSize+1));
        for(int i=1; i<= itemSize; i++) {
            for(int j=1; j <= itemSize; j++) {
                double top = 0, bleft =0, bright = 0;
                
                int cnt = 0;
                for(int k=1; k<= userSize; k++) {
                    if(userToItem[k][i] == EMPTY) continue;
                    if(userToItem[k][j] == EMPTY) continue;
                    
                    cnt++;
                    top += userToItem[k][i]*userToItem[k][j];
                    
                    bleft += userToItem[k][i]*userToItem[k][i];
                    bright += userToItem[k][j]*userToItem[k][j];
                }
                
                if(cnt < 1) {
                    itemToItem[i][j] = 0;
                } else {
                    itemToItem[i][j] = top/(sqrt(bleft)*sqrt(bright));
                }
            }
        }
    }
    
    void buildItemToItemP() {
        getAverage();
        itemToItem = vector<vector<double> > (itemSize+1, vector<double> (itemSize+1));
        for(int i=1; i<= itemSize; i++) {
            for(int j=1; j <= itemSize; j++) {
                double top = 0, bleft =0, bright = 0;
                
                int cnt = 0;
                for(int k=1; k<= userSize; k++) {
                    if(userToItem[k][i] == EMPTY) continue;
                    if(userToItem[k][j] == EMPTY) continue;
                    
                    cnt++;
                    top += (userToItem[k][i]-average[k])*(userToItem[k][j]-average[k]);
                    
                    bleft += (userToItem[k][i]-average[k])*(userToItem[k][i]-average[k]);
                    bright += (userToItem[k][j]-average[k])*(userToItem[k][j]-average[k]);
                }
                
                if(cnt < 1) {
                    itemToItem[i][j] = 0;
                } else {
                    itemToItem[i][j] = top/(sqrt(bleft)*sqrt(bright));
                }
            }
        }
    }
    
    int predict(int user, int item) {
        double top = 0;
        double bottom = 0;
        
        // given untrained items
        if(item > itemSize) {
            return 3;
        }
        
        for(int i=1;i<=itemSize; i++) {
            if(userToItem[user][i] == EMPTY) continue;
            
            bottom += abs(itemToItem[item][i]);
            top += itemToItem[item][i]*userToItem[user][i];
        }
        
        if(bottom == 0) {
            return 3;
        }
        
        double rating = top/bottom;
        // return rating;
        return rating+0.5;
    }
};

struct inputTuple{
    int user, item, rating;
};

class InputReader {
private:
    ifstream fin;
    MAT userToItem;
    vector<inputTuple> input;
public:
    InputReader(string filename) {
        fin.open(filename);
        if(!fin) {
            cout << filename << " file could not be opened\n";
            exit(0);
        }
        parse();
    }
    
    void parse() {
        int maxUser = 0, maxItem = 0;
        
        int user, item, rating, timeStamp;
        while(!fin.eof()) {
            fin >> user >> item >> rating >> timeStamp;
            
            input.push_back({user, item, rating});
            
            maxUser = max(maxUser, user);
            maxItem = max(maxItem, item);
        }
        input.pop_back();
        
        userToItem = vector<vector<double> > (maxUser+1, vector<double> (maxItem+1, EMPTY));
        for(int i=0;i<input.size();i++) {
            user = input[i].user, item = input[i].item, rating = input[i].rating;
            userToItem[user][item]= rating;
        }
    }
    
    vector<inputTuple> getInput() {
        return input;
    }
    
    MAT getUserToItem() {
        return userToItem;
    }
};

class OutputPrinter {
private:
    ofstream fout;
    string filename;
public:
    OutputPrinter(string filename) {
        this->filename = filename;
        fout.open(filename);
        if(!fout) {
            cout << filename << " file could not be writed\n";
            exit(0);
        }
    }
    void addLine(int user, int item, double rating) {
        fout << user << "\t" << item << "\t" << rating << endl;
    }
};

int main(int argc, const char * argv[]) {
    if(argc!=3) {
        cout << "Please follow this format. recommender.exe [base file name] [test file name]";
        return 0;
    }
    
    string baseFileName(argv[1]);
    string testFileName(argv[2]);
    
    InputReader baseInputReader(baseFileName);
    
    MAT userToItem = baseInputReader.getUserToItem();
    CF cf(userToItem);
    cf.run();
    
    InputReader testInputReader(testFileName);
    vector<inputTuple> test = testInputReader.getInput();
    
    OutputPrinter outputPrinter(testFileName.substr(0, 2)+".base_prediction.txt");
    
    double RSME = 0.0;
    
    for(int i=0;i<test.size();i++) {
        int user = test[i].user;
        int item = test[i].item;
        int rating = test[i].rating;
        
        double predict = cf.predict(user, item);
        
        RSME += (predict-rating)*(predict-rating);
        outputPrinter.addLine(user, item, predict);
    }
    
    RSME /= (double)test.size();
    RSME = sqrt(RSME);
    cout << "RSME :" << RSME << endl;
    return 0;
}
