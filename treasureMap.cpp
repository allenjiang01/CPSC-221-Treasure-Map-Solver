#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

start = s;
base = baseim;
maze = mazeim;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);

}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

    int number = d%64;
    char rNum = number >> 4;
    char gNum = (d%16) >> 2;
    char bNum = (d%4);
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

    rNum += ((pixel->r >> 2) << 2);
    gNum += ((pixel->g >> 2) << 2);
    bNum += ((pixel->b >> 2) << 2);

    pixel->r = rNum;
    pixel->g = gNum;
    pixel->b = bNum;
}

PNG treasureMap::renderMap(){

    PNG map = PNG(base);
    vector<bool> visited(map.height(), false);
    vector<vector<bool>> v(map.width(), visited);
    //visited[map.size()-1][map.size()-1];
    vector<int> distance(map.height(), -1);
    vector<vector<int>> path(map.width(), distance);
    Queue<pair<int,int>> q;

    v[start.first][start.second] = true;
    path[start.first][start.second] = 0;
    setLOB(map,start,0);
    q.enqueue(start);
    pair<int,int> curr;

    while(!q.isEmpty()) {
        curr = q.dequeue();
        vector<pair<int,int>> neightbour = neighbors(curr);
        for(pair<int,int>pair: neightbour) {
           if (good(v,curr, pair)) {
               v[pair.first][pair.second] = true;
               path[pair.first][pair.second] = path[curr.first][curr.second] + 1;
               setLOB(map, pair, path[pair.first][pair.second]);
               q.enqueue(pair);
           }
        }
    }

    return map;
}


PNG treasureMap::renderMaze(){

    PNG map = PNG(base);
    vector<bool> visited(map.height(), false);
    vector<vector<bool>> v(map.width(), visited);
    //visited[map.size()-1][map.size()-1];
    //vector<int> distance(map.height(), -1);
    //vector<vector<int>> path(map.width(), distance);
    Queue<pair<int,int>> q;

    v[start.first][start.second] = true;
    //path[start.first][start.second] = 0;
    setGrey(map,start);
    q.enqueue(start);
    pair<int,int> curr;

    // int x = 0;
    // int y = 0;

    // if(start.first < 7) {
    //     if (start.first + 7 >= map.width()) {
    //         x = map.width() - start.first-1;
    //     } else {
    //         x = start.first;
    //     }
    // }
    //    if(start.second < 7) {
    //     if (start.second + 7 >= map.height()) {
    //         y = map.height() - start.second-1;
    //     } else {
    //         y = start.second;
    //     }
    // }

    

    while(!q.isEmpty()) {
        curr = q.dequeue();
        vector<pair<int,int>> neightbour = neighbors(curr);
        for(pair<int,int>pair: neightbour) {
           if (good(v,curr, pair)) {
               v[pair.first][pair.second] = true;
              // path[pair.first][pair.second] = path[curr.first][curr.second] + 1;
               setGrey(map, pair);
               q.enqueue(pair);
           }
        }
    }

    int x = start.first;
    int y = start.second;
    vector<pair<int, int>> pairs;
    for (int i = x-3; i <= x+3; i++) {
        for (int j = y-3; j <= y+3; j++) {
            if (!(i<0 || j<0 ||  i >  map.width()-1 || j > map.height()-1)) {
                pairs.push_back(make_pair(i, j));
            }
        }
    }

    for (pair<int, int> p : pairs) {
        RGBAPixel *pixel = map.getPixel(p.first, p.second);
        pixel->r = 255;
        pixel->g = 0;
        pixel->b = 0;
    }

    return map;

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

    int next_xCoord = next.first;
    int next_yCoord = next.second;

    if (next_xCoord >= maze.width() || next_xCoord < 0) {
        return false;
    }

    if (next_yCoord >= maze.height() || next_yCoord < 0) {
        return false;
    }

    if (v[next_xCoord][next_yCoord]) {
        return false;
    }

    RGBAPixel *curr_pixel = maze.getPixel(curr.first, curr.second);
    RGBAPixel *next_pixel = maze.getPixel(next_xCoord, next_yCoord);

    if (*curr_pixel != *next_pixel) {
        return false;
    }
    return true;
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

    vector<pair<int,int>> neightbour;
    int xCoord = curr.first;
    int yCoord = curr.second;
    neightbour.push_back(make_pair(xCoord-1,yCoord)); //left
    neightbour.push_back(make_pair(xCoord,yCoord+1)); //below
    neightbour.push_back(make_pair(xCoord+1,yCoord)); //right
    neightbour.push_back(make_pair(xCoord,yCoord-1)); //above

    return neightbour;
}

