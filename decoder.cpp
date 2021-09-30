#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

      vector<bool> yVisited(mapImg.height(), false);
      vector<vector<bool>> visited(mapImg.width(), yVisited);
      vector<int> yDistance(mapImg.height(), -1);
      vector<vector<int>> distance(mapImg.width(), yDistance);
      pair<int,int> startP = make_pair(-1,-1);
      vector<pair<int,int>> yPath(mapImg.height(), startP);
      vector<vector<pair<int,int>>> path(mapImg.width(), yPath);

      visited[start.first][start.second] = true;
      distance[start.first][start.second] = 0;

      Queue<pair<int,int>> q;
      q.enqueue(start);
      pair<int,int> parent;
      while(!q.isEmpty()) {
         parent = q.dequeue();
         vector<pair<int,int>> neighbour = neighbors(parent);
         for (pair<int,int> p: neighbour) {
            if (good(visited, distance, parent, p)) {
               visited[p.first][p.second] = true;
               distance[p.first][p.second] = distance[parent.first][parent.second] + 1;
               path[p.first][p.second] = parent;
               q.enqueue(p);
            }
         }
      }
      Stack<pair<int,int>> stack;
      stack.push(parent);
      while (stack.peek() != start) {
         stack.push(path[parent.first][parent.second]);
         parent = path[parent.first][parent.second];
      }
     // stack.push(start);
      while (!stack.isEmpty()) {
         pathPts.push_back(stack.pop());
      }
}

PNG decoder::renderSolution(){

   PNG map = PNG(mapImg);

   for (int i=0; i < pathLength(); i++) {

      pair<int,int> pair = pathPts[i];
      RGBAPixel *pixel = map.getPixel(pair.first, pair.second);
      pixel->r = 255;
      pixel->g = 0;
      pixel->b = 0;
}

return map;
}

PNG decoder::renderMaze(){

 PNG map = PNG(mapImg);
    vector<bool> visited(map.height(), false);
    vector<vector<bool>> v(map.width(), visited);
    vector<int> yDistance(mapImg.height(), -1);
    vector<vector<int>> distance(mapImg.width(), yDistance);
    Queue<pair<int,int>> q;

    v[start.first][start.second] = true;
    distance[start.first][start.second] = 0;
    setGrey(map,start);
    q.enqueue(start);
    pair<int,int> curr; 

    while(!q.isEmpty()) {
        curr = q.dequeue();
        vector<pair<int,int>> neightbour = neighbors(curr);
        for(pair<int,int>pair: neightbour) {
           if (good(v, distance, curr, pair)) {
               v[pair.first][pair.second] = true;
               distance[pair.first][pair.second] = distance[curr.first][curr.second] + 1;
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

void decoder::setGrey(PNG & im, pair<int,int> loc){

RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);

}

pair<int,int> decoder::findSpot(){

return pathPts[pathPts.size()-1];        
}

int decoder::pathLength(){

return pathPts.size();

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

   int x = next.first;
   int y = next.second;

   if (x < 0 || x >= mapImg.width()) {
      return false;
   }
   if (y < 0 || y >= mapImg.height()) {
      return false;
   }
   if (v[x][y]) {
      return false;
   }

   RGBAPixel *nextPixel = mapImg.getPixel(next.first, next.second);
   return compare(*nextPixel, d[curr.first][curr.second]);

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

vector<pair<int,int>> neightbour;
    int xCoord = curr.first;
    int yCoord = curr.second;
    neightbour.push_back(make_pair(xCoord-1,yCoord)); //left
    neightbour.push_back(make_pair(xCoord,yCoord+1)); //below
    neightbour.push_back(make_pair(xCoord+1,yCoord)); //right
    neightbour.push_back(make_pair(xCoord,yCoord-1)); //above

    return neightbour;

}


bool decoder::compare(RGBAPixel p, int d){
   int val = (p.r % 4) << 4;
   val += (p.g % 4) << 2;
   val += p.b % 4;
   return val == (d + 1) % 64;
}
