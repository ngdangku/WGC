
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "basic.h"

#define VERBOSE

using namespace std;

string tostr(uint n) {
    stringstream ss;
    ss << n;
    return string(ss.str());
}


int main(int argc, char ** argv) {
    if(argc!=4) {
        cout << "usage: " << argv[0] << " <input> <max length> <output prefix>" << endl;
        return 0;
    }

    uint max_len = 0;
    {
        stringstream ss(argv[2]);
        ss >> max_len;
    }

    ifstream input(argv[1]);

    uint nodes;
    input.read((char*)&nodes,sizeof(uint));
    uint64 edges;
    input.read((char*)&edges,sizeof(edges));

    cout << " Nodes: " << nodes << endl;
    cout << " Edges: " << edges << endl;
    cout << " MaxL : " << max_len << endl;

    uint last = 0;
    vector<uint> list;
    uint64 added = 0;
    uint local_nodes=0;
    uint64 local_edges=0;

    string fname = string(argv[3])+string(".offset");
    ofstream offset(fname.c_str(),ios::binary|ios::trunc);
    uint real_node=1;
    offset.write((char*)&real_node,sizeof(uint));
    real_node = 0;

    uint file = 1;
    fname = string(argv[3])+tostr(file);
    ofstream output(fname.c_str(),ios::binary|ios::trunc);
    output.write((char*)&local_nodes,sizeof(uint));
    output.write((char*)&local_edges,sizeof(local_edges));

    uint64 pos;
    for(pos = 0; pos < edges+(ulong)nodes; pos++) {
        uint c;
        input.read((char*)&c,sizeof(uint));
        if((int)c<0) {
            if(last > 0) {
                if(added+list.size()>max_len) {
                    output.seekp(0);
                    local_nodes--;
                    output.write((char*)&local_nodes,sizeof(uint));
                    output.write((char*)&added,sizeof(added));
                    output.close();
                    #ifdef VERBOSE
                    cout << " Closing file  : " << fname << endl;
                    cout << "  nodes written: " << local_nodes << endl;
                    cout << "  edges written: " << added << endl;
                    cout << "  real node    : " << real_node << endl;
                    #endif
                    local_nodes++;
                    offset.write((char*)&real_node,sizeof(uint));
                    fname = string(argv[3]) + tostr(++file);
                    output.open(fname.c_str(),ios::binary|ios::trunc);
                    local_nodes = 1;
                    local_edges = 0;
                    added = 0;
                    output.write((char*)&local_nodes,sizeof(uint));
                    output.write((char*)&local_edges,sizeof(local_edges));
                    int act_node = -(int)local_nodes;
                    output.write((char*)&act_node,sizeof(uint));
                    for(uint i=0;i<list.size();i++) {
                        uint wr = list[i];
                        output.write((char*)&wr,sizeof(uint));
                    }
                    added += list.size();
                }
                else {
                    int act_node = -(int)local_nodes;
                    output.write((char*)&act_node,sizeof(uint));
                    for(uint i=0;i<list.size();i++) {
                        uint wr = list[i];
                        output.write((char*)&wr,sizeof(uint));
                    }
                    added += list.size();
                }
            }
            local_nodes++;
            real_node++;
            last = (uint)(-(int)c);
            list.clear();
        }
        else {
            list.push_back(c);
            local_edges++;
        }
    }
    // Add last node
    {
        int act_node = -(int)local_nodes;
        output.write((char*)&act_node,sizeof(uint));
        for(uint i=0;i<list.size();i++) {
            uint wr = list[i];
            output.write((char*)&wr,sizeof(uint));
        }
        added += list.size();
        output.seekp(0);
        //local_nodes--;
        output.write((char*)&local_nodes,sizeof(uint));
        output.write((char*)&added,sizeof(added));
        output.close();
        #ifdef VERBOSE
        cout << " Closing file  : " << fname << endl;
        cout << "  nodes written: " << local_nodes << endl;
        cout << "  edges written: " << added << endl;
        cout << "  read_node    : " << real_node << endl;
        #endif
    }
    nodes++;
    //cout << " last offset: " << nodes << endl;
    offset.write((char*)&nodes,sizeof(uint));
    offset.close();
    input.close();
    return 0;
}
