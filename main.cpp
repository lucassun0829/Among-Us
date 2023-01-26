//  9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7
//  main.cpp
//  p4
//
//  Created by Lucas Sun on 11/22/22.
//

#include <iostream>
#include <getopt.h>
#include <stdio.h>
#include <math.h>
#include <istream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <limits>
#include "xcode_redirect.hpp"
#include <iomanip>
#include <numeric>
using namespace std;

class Vertex{
public:
    int x;
    int y;
    char type;
    
    Vertex(){};
    Vertex(int temp_x, int temp_y, char t): x(temp_x), y(temp_y), type(t){};
};


class Program{
public:
    char mode;
    vector<Vertex> coords;
    double get_distance(Vertex& v1, Vertex& v2);
    double get_dis(Vertex& v1, Vertex& v2);
    double get_cost(Vertex & v1,Vertex & v2,Vertex & v3);
    void run_a();
    void run_b();

    Program(vector<Vertex> t): coords(t){};
};

char get_type(int x, int y){
    if((x==0&&y<=0) || (y==0&&x<=0)){
        return('d');
    }
    else if(x<0 && y<0){
        return('l');
    }
    else{
        return('o');
    }
}

void Program::run_a(){
    vector<double> d(coords.size(), numeric_limits<double>::infinity());
    vector<bool> k(coords.size());
    vector<size_t> p(coords.size());
    d[0] = 0;
    for(size_t i =0; i < coords.size(); ++i){
        double cur_dis =numeric_limits<double>::infinity();
        size_t index=0;
        for(size_t j =0; j<d.size(); ++j){
            if(k[j]==false && d[j]<cur_dis){
                cur_dis = d[j];
                index = j;
            }
        }
        k[index] = true;
        Vertex cur_vertex = coords[index];
        for(size_t m =0; m<coords.size(); ++m){
            if(!k[m]){
                double distance = get_distance(coords[m], cur_vertex);
                if(distance<d[m]){
                    d[m] = distance;
                    p[m] = index;
                }
            }
        }
    }
    double sum = 0;
    for(size_t i =0; i<d.size(); ++i){
        sum+=d[i];
    }
    cout<<sum<<'\n';
    for(size_t i =1; i<p.size(); ++i){
        cout<<min(p[i],i)<<' '<<max(p[i],i)<<'\n';
    }

}

void Program::run_b(){
    vector<size_t> tour;
    tour.reserve(coords.size());
    tour.push_back(0);
    tour.push_back(1);
    tour.push_back(0);
    double length = 0;
    unsigned int insert_index;
    double cur_dis;
    double min_dis;
    Vertex cur_vertex;
    for(size_t i = 2; i < coords.size(); ++i){
        cur_vertex = coords[i];
        min_dis =numeric_limits<double>::infinity();
        insert_index=0;
        cur_dis=0;
        for(unsigned int j =0; j<tour.size()-1; ++j){
            cur_dis = get_cost(coords[tour[j]],cur_vertex,coords[tour[j+1]]);
            if(cur_dis<min_dis){
                min_dis = cur_dis;
                insert_index = j+1;
            }
        }
        tour.insert(tour.begin()+insert_index, i);
        
    }
    double len;
    for(size_t i =0; i<tour.size()-1;++i){
        len =get_dis(coords[tour[i]],coords[tour[i+1]]);
        length += len;
    }
    length+=get_dis(coords[tour[0]], coords[tour[tour.size()-1]]);
    cout<<length<<'\n';
    for(size_t i =0; i<tour.size()-1;++i){
        cout<<tour[i]<<' ';
    }

}
double Program::get_dis(Vertex & v1, Vertex & v2){
    double x_dif =static_cast<double>(v2.x-v1.x);
    double y_dif =static_cast<double>(v2.y-v1.y);
    double num1 = x_dif * x_dif;
    double num2 = y_dif * y_dif;
    return(sqrt(num1+num2));
}

double Program::get_cost(Vertex & v1,Vertex & v2,Vertex & v3){
    return(get_dis(v1,v2) + get_dis(v2,v3) - get_dis(v1,v3));
}


double Program::get_distance(Vertex & v1, Vertex & v2){
    if((v1.type=='l'&&v2.type=='o') || (v2.type=='l'&&v1.type=='o')){
        return(numeric_limits<double>::infinity());
    }
    double x_dif =static_cast<double>(v2.x-v1.x);
    double y_dif =static_cast<double>(v2.y-v1.y);
    double num1 = x_dif * x_dif;
    double num2 = y_dif * y_dif;
    return(sqrt(num1+num2));
}


char get_mode(int argc, char * argv[]){
    char mode = 'e';
    int option_index = 0, option =0;
    struct option long_options[] = {
        {"help"     ,   no_argument,       nullptr, 'h'},
        {"mode"   ,   required_argument,       nullptr, 'm'},
        { nullptr, 0,                      nullptr, '\0' }
    };
    while ((option = getopt_long(argc, argv, "hm:", long_options, &option_index)) != -1) {
        switch (option) {
            case 'h':
                cout<<"This is a help message";
                exit(0);
                break;
            case 'm':
                if(strcmp(optarg,"MST")==0){
                    mode = 'm';
                }
                else if(strcmp(optarg,"FASTTSP")==0){
                    mode = 'f';
                }
                else if(strcmp(optarg,"OPTTSP")==0){
                    mode = 'o';
                }
                else{
                    cerr<<"Invalid mode";
                    exit(1);
                }
                break;
        }
    }
    return mode;
}


class Part_c{
public:
    size_t i =0;
    vector<Vertex> coords;
    vector<size_t> best_path;
    vector<size_t> path;
    double bestCost = 0;
    double curCost = 0;
    double running_total = 0;
    double armCost = 0;
    void run_c();
    double part_a(size_t permLength);
    void part_b();
    bool promising(size_t permLength);
    void genPerms(size_t permLength);
    double get_dis(Vertex& v1, Vertex& v2);
    double get_cost(Vertex & v1,Vertex & v2,Vertex & v3);
    
    Part_c(vector<Vertex> t): coords(t){};
};
double Part_c::get_dis(Vertex & v1, Vertex & v2){
    double x_dif =static_cast<double>(v2.x-v1.x);
    double y_dif =static_cast<double>(v2.y-v1.y);
    double num1 = x_dif * x_dif;
    double num2 = y_dif * y_dif;
    return(sqrt(num1+num2));
}

double Part_c::get_cost(Vertex & v1,Vertex & v2,Vertex & v3){
    return(get_dis(v1,v2) + get_dis(v2,v3) - get_dis(v1,v3));
}

void Part_c::part_b(){

    best_path.push_back(0);
    best_path.push_back(1);
    best_path.push_back(0);
    Vertex cur_vertex;
    unsigned int insert_index;
    double cur_dis;
    double min_dis;
    for(size_t i = 2; i < coords.size(); ++i){
        cur_vertex = coords[i];
        min_dis =numeric_limits<double>::infinity();
        insert_index=0;
        cur_dis=0;
        for(unsigned int j =0; j<best_path.size()-1; ++j){
            cur_dis = get_cost(coords[best_path[j]],cur_vertex,coords[best_path[j+1]]);
            if(cur_dis<min_dis){
                min_dis = cur_dis;
                insert_index = j+1;
            }
        }
        best_path.insert(best_path.begin()+insert_index, i);
        
    }
    best_path.pop_back();
    for(size_t i =0; i<best_path.size()-1;++i){
        double len =get_dis(coords[best_path[i]],coords[best_path[i+1]]);
        bestCost += len;
    }
    bestCost+=get_dis(coords[best_path[0]], coords[best_path[best_path.size()-1]]);
}

double Part_c::part_a(size_t permLength){
    vector<double> d(coords.size(), numeric_limits<double>::infinity());
    vector<bool> k(coords.size());
    vector<size_t> p(coords.size());
    d[permLength] = 0;
    for(size_t i =permLength; i < coords.size(); ++i){
        double cur_dis =numeric_limits<double>::infinity();
        size_t coord_index = permLength;
        size_t index = permLength;
        for(size_t j =permLength; j<d.size(); ++j){
            if(k[j]==false && d[j]<cur_dis){
                cur_dis = d[j];
                coord_index = path[j];
                index = j;
            }
        }
        k[index] = true;
        Vertex cur_vertex = coords[coord_index];
        for(size_t m =permLength; m<coords.size(); ++m){
            if(!k[m]){
                double distance = get_dis(coords[path[m]], cur_vertex);
                if(distance<d[m]){
                    d[m] = distance;
                    p[m] = coord_index;
                }
            }
        }
    }
    double sum = 0;
    for(size_t i =permLength; i<d.size(); ++i){
        sum+=d[i];
    }
    return(sum);
}
void Part_c::run_c(){
    path.resize(coords.size());
    iota(begin(path), end(path), 0);
    genPerms(1);
    cout<<bestCost<<'\n';
    for(size_t i =0; i<best_path.size();++i){
        cout<<best_path[i]<<' ';
    }
    
}

bool Part_c::promising(size_t permLength){
    
    double mstCost = part_a(permLength);
    double arm1Len = numeric_limits<double>::infinity();;
    double arm2Len = numeric_limits<double>::infinity();;
    for(size_t i =permLength; i<path.size(); ++i){
        double dis1 = get_dis(coords[0], coords[path[i]]);
        double dis2 = get_dis(coords[path[permLength-1]], coords[path[i]]);
        if(dis1<arm1Len){
            arm1Len = dis1;
        }
        if(dis2<arm2Len){
            arm2Len = dis2;
        }
    }
    double totalEst = arm1Len+arm2Len+mstCost+curCost;
    bool promise = totalEst<bestCost;
//    string promisse;
//    if(promise){
//        promisse = "true";
//    }
//    else{
//        promisse = "false";
//    }
//    for (size_t i = 0; i < path.size(); ++i)
//        cerr << setw(2) << path[i] << ' ';
//    cerr << setw(4) << permLength << setw(10) << curCost;
//    cerr << setw(10) << arm1Len << setw(10) << arm2Len;
//    cerr << setw(10) << mstCost << setw(10) << totalEst << "  " << promisse << '\n';
    return promise;
}

void Part_c::genPerms(size_t permLength) {
    if (permLength == path.size()) {
        armCost = get_dis(coords[path[0]], coords[path[path.size()-1]]);
        curCost += armCost;
        if(curCost<bestCost){
            bestCost = curCost;
            best_path = path;
        }
        curCost -= armCost;
        return;
    }

    if (!promising(permLength)) {
        return;
    }

    for (size_t i = permLength; i < path.size(); ++i) {
        swap(path[permLength], path[i]);
        curCost += get_dis(coords[path[permLength-1]], coords[path[permLength]]);
        genPerms(permLength + 1);
        curCost -= get_dis(coords[path[permLength-1]], coords[path[permLength]]);
        swap(path[permLength], path[i]);
    }
}

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    cerr << fixed << showpoint << setprecision(2);
    cout << std::setprecision(2);
    cout << std::fixed;
    vector<Vertex> temp_coords;
    size_t num;
    int x;
    int y;
    cin>>num;
    temp_coords.reserve(num);
    for(size_t i = 0; i<num; ++i){
        cin>>x>>y;
        temp_coords.push_back(Vertex(x,y,get_type(x,y)));
    }
    Program program(temp_coords);
    char mode =get_mode(argc, argv);
    switch(mode){
        case 'm':{
            program.run_a();
            break;
        }
        case 'f':{
            program.run_b();
            break;
        }
        case 'o':{
            Part_c c(temp_coords);
            c.part_b();
            c.run_c();
            break;
        }
        case 'e':{
            cerr<<"Error";
            exit(1);
        }
    }

    return 0;
}
