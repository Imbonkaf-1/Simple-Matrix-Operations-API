#define _WIN32_WINNT 0xA00
#include<iostream> 
#include<format>
#include<vector> 
#include "library/json.hpp"
#include "library/httplib.h"
using namespace std;

using Matrix = vector<vector<int>>; 
using json = nlohmann::json; 

struct Matrices {
    Matrix A; 
    Matrix B;
    Matrix C; 
};

void matrixAddition(Matrices& matrices); 
void matrixMultiplication(Matrices& matrices);
void matrixSubtraction(Matrices& matrices);
void displayMatrix(Matrices &matrices);
void chooseTag(string& tag);

int main(int argc, char *argv[]) {

    Matrices matrices; 
    // API
    httplib::Server server;
    int port = 9000;
    if(argc <= 2){
        try{
            port = stoi(argv[1]);
        }catch(...){
            cout << "Invalid Port: " << port << endl;  
        }
    }

    string tag = "/subtract";
    chooseTag(tag);

    server.Post(tag, [&matrices, &tag](const httplib::Request& req, httplib::Response& res){
        string body = req.body;
        json input = json::parse (req.body);
        matrices.A = input["A"].get<Matrix>();
        matrices.B = input["B"].get<Matrix>();
        if(tag == "/add"){
            matrixAddition(matrices);
        } else if (tag == "/subtract"){
            matrixSubtraction(matrices);
        } else if (tag == "/multiply"){
            matrixMultiplication(matrices);
        }
        json result;
        result["Result"] = matrices.C;
        res.set_content(result.dump(), "application/json");
    });

    cout << "MAT Server starting on port: " << port << "\nFor " << tag <<  endl;
    server.listen("127.0.0.1", port);
    matrixAddition(matrices);

    return 0; 
} 

void matrixMultiplication(Matrices& matrices){

    int rows_A = matrices.A.size();
    int columns_A = matrices.A[0].size(); 
    int columns_B = matrices.B[0].size(); 
    int rows_B = matrices.B.size(); 

    // Sizes the Matrix C according to how many elements the resultant would have
    matrices.C.resize(rows_A, vector<int>(columns_B, 0)); 
    // Checks for wether the multiplication is valid or not
    if(rows_B == columns_A) {
    
        // Multiplies the matrices
        for(int i = 0; i < rows_A; i++) {
            for(int j = 0; j < columns_B; j++) {
                for(int k = 0; k < columns_A; k++) {
                    matrices.C[i][j] += (matrices.A[i][k] * matrices.B[k][j]);
                } 
            } 
        } 
    
    }
    else {
        cout << "Not valid\n"; 
    } 

}

void matrixAddition(Matrices& matrices) {
    

    int rows_A = matrices.A.size();
    int columns_A = matrices.A[0].size(); 
    int columns_B = matrices.B[0].size(); 
    int rows_B = matrices.B.size(); 
    matrices.C.resize(rows_A, vector<int>(columns_B, 0));

    if(rows_A == rows_B && columns_A == columns_B){

    for(int i = 0; i < rows_A; i++) {
        for(int j = 0; j < columns_A; j++) {
            matrices.C[i][j] = matrices.A[i][j] + matrices.B[i][j]; 
        } 
    }        

    }else{
        cout << "Invalid Matrices. Must be of same order.";
    }
}

void matrixSubtraction(Matrices& matrices){

    int rows_A = matrices.A.size();
    int columns_A = matrices.A[0].size(); 
    int columns_B = matrices.B[0].size(); 
    int rows_B = matrices.B.size(); 
    matrices.C.resize(rows_A, vector<int>(columns_B, 0));

    if(rows_A == rows_B && columns_A == columns_B){
        for(int i = 0; i < rows_A; i++){
            for(int j = 0; j < columns_A; j++){
                matrices.C[i][j] = matrices.A[i][j] - matrices.B[i][j];
            }
        }
    }else {
        cout << "Invalid Matrices. Must be of same order.";
    }
}

void displayMatrix(Matrices &matrices){

    int rows_C = matrices.C.size();
    int column_C = matrices.C[0].size();

    for(int i = 0; i < rows_C; i++){
        for(int j = 0; j < column_C; j++){
            cout << matrices.C[i][j];
            cout << " ";
        }
        cout << "\n";
    }
}   

void chooseTag(string&tag){
    int choice; 
    cout << "choose what you want to do: \n1. Add\n2. Subtract\n3. Multiply\n";
    cin >> choice;
    switch(choice){
        case 1:
            tag = "/add";
            break;
        case 2:
            tag = "/subtract";
            break;
        case 3:
            tag = "/multiply";
            break;
        default:
            cout << "Invalid";
    }

}