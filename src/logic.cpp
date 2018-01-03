#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>
#include <fstream>

struct Figure
{
  int type;
  int x;
  int y;
};

struct Prev{
    string index = "";
    string fromX = "";
    string fromY = "";
    string indexNext = "";
    string typeNext = "";
    string toX = "";
    string toY = "";
};

struct Logic::Impl
{
  QList<Figure> figure;
  QList<Figure> figures;

  int findByPosition(int x, int y);
};

int Logic::Impl::findByPosition(int x, int y) {
  for (int i(0); i<figures.size(); ++i) {
    if (figures[i].x != x || figures[i].y != y ) {
      continue; 
    }
    return i;    
  }
  return -1;
}



Logic::Logic(QObject *parent)
  : QAbstractListModel(parent)
  , impl(new Impl()) 
{
  impl->figure << Figure { 4, 0, 0 };
  impl->figure << Figure { 3, 1, 0 };
  impl->figure << Figure { 2, 2, 0 };
  impl->figure << Figure { 1, 3, 0 };
  impl->figure << Figure { 0, 4, 0 };
  impl->figure << Figure { 2, 5, 0 };
  impl->figure << Figure { 3, 6, 0 };
  impl->figure << Figure { 4, 7, 0 };
  impl->figure << Figure { 5, 0, 1 };
  impl->figure << Figure { 5, 1, 1 };
  impl->figure << Figure { 5, 2, 1 };
  impl->figure << Figure { 5, 3, 1 };
  impl->figure << Figure { 5, 4, 1 };
  impl->figure << Figure { 5, 5, 1 };
  impl->figure << Figure { 5, 6, 1 };
  impl->figure << Figure { 5, 7, 1 };

  impl->figure << Figure { 10, 0, 7 };
  impl->figure << Figure { 9, 1, 7 };
  impl->figure << Figure { 8, 2, 7 };
  impl->figure << Figure { 7, 3, 7 };
  impl->figure << Figure { 6, 4, 7 };
  impl->figure << Figure { 8, 5, 7 };
  impl->figure << Figure { 9, 6, 7 };
  impl->figure << Figure { 10, 7, 7 };
  impl->figure << Figure { 11, 0, 6 };
  impl->figure << Figure { 11, 1, 6 };
  impl->figure << Figure { 11, 2, 6 };
  impl->figure << Figure { 11, 3, 6 };
  impl->figure << Figure { 11, 4, 6 };
  impl->figure << Figure { 11, 5, 6 };
  impl->figure << Figure { 11, 6, 6 };
  impl->figure << Figure { 11, 7, 6 };
}

Logic::~Logic() {
}

int Logic::boardSize() const {
  return BOARD_SIZE;
}

int Logic::rowCount(const QModelIndex & ) const {
  return impl->figures.size(); 
}

QHash<int, QByteArray> Logic::roleNames() const { 
  QHash<int, QByteArray> names;
  names.insert(Roles::Type      , "type");
  names.insert(Roles::PositionX , "positionX");
  names.insert(Roles::PositionY , "positionY");
  return names;
}

QVariant Logic::data(const QModelIndex & modelIndex, int role) const { 
  if (!modelIndex.isValid()) {
    return QVariant();
  }
    
  int index = static_cast<int>(modelIndex.row());
  
  if (index >= impl->figures.size() || index < 0) {
    return QVariant();
  }

  Figure & figur = impl->figures[index];
    
  switch (role) {
    case Roles::Type     : return figur.type;
    case Roles::PositionX: return figur.x;
    case Roles::PositionY: return figur.y;
  }
  return QVariant(); 
}

void Logic::deliting(int x){
    beginResetModel();
    impl->figures.removeAt(x);
    endResetModel();
}

void Logic::save(){
    ofstream his("history.txt");
    his << history;
    his.close();
}

void Logic::next_step(){
     ifstream his("history.txt");
     string line = "";
     step++;
     Prev buf;
     int space_count=0;
     int count = 0;
     while(getline(his, line)){
         if(count == step) break;
         count++;
     }
     if(line != ""){
         for(int i = 0; i < line.size(); i++){
             if(line[i] == ' ') space_count++;
             else if(space_count == 0) buf.index += line[i];
             else if(space_count == 2){
                 buf.toX = line[i];
                 i+= 2;
                 buf.toY = line[i];
             }
             else if(space_count == 3) buf.indexNext+= line[i];
             else if (line[i] == '\n') break;
         }
         beginResetModel();
         impl-> figures[atoi(buf.index.c_str())].x = atoi(buf.toX.c_str());
         impl-> figures[atoi(buf.index.c_str())].y = atoi(buf.toY.c_str());
         endResetModel();
         if(buf.indexNext != "") deliting(atoi(buf.indexNext.c_str()));
     }
}

void Logic::prev_step(){
     ifstream his("history.txt");
     string line;
     Prev buf;
     int space_count=0;
     int count = 0;

     if(step >= 0){
        while(getline(his, line)){
            if(count == step) break;
            count++;
        }
        for(int i=0; i < line.size(); i++){
            if(line[i] == ' ') space_count++;
            else if(space_count == 0) buf.index+= line[i];
            else if(space_count == 1){
                buf.fromX = line[i];
                i+= 2;
                buf.fromY = line[i];
            }
            else if(space_count == 2) {
                buf.toX = line[i];
                i+= 2;
                buf.toY = line[i];
            }
            else if(space_count == 3) buf.indexNext+= line[i];
            else if(space_count == 4) buf.typeNext+= line[i];
            else if(line [i] == '\n') break;
        }
        beginResetModel();
        if(buf.indexNext != ""){
            impl->figures.insert(atoi(buf.indexNext.c_str()), Figure {atoi(buf.typeNext.c_str()), atoi(buf.toX.c_str()), atoi(buf.toY.c_str())});
        }
        impl->figures[atoi(buf.index.c_str())].x = atoi(buf.fromX.c_str());
        impl->figures[atoi(buf.index.c_str())].y = atoi(buf.fromY.c_str());
        endResetModel();
        step--;
     }
}

void Logic::clear() {
  beginResetModel();
  impl->figures.clear();
  endResetModel();
}

bool Logic::pathFind_for_rook(int currentIndex, int index, int i, int to){
    if(index != -1 && i == to){
        if (((impl->figures[currentIndex].type >= 0 && impl->figures[currentIndex].type <= 5) && (impl->figures[index].type >=6 && impl->figures[index].type <= 11)) || ((impl->figures[currentIndex].type >= 6 && impl->figures[currentIndex].type <=11) && (impl->figures[index].type >=0 && impl->figures[index].type <= 5))){
            if (impl->figures[index].type == 0 || impl->figures[index].type == 6){
                clear();
                return false;
            }
            del_figure = index;
            del_type = impl->figures[index].type;
            deliting(index);
            return true;
        }
        else return false;
    }
    else if(index != -1) return false;
    else if(index == -1 && i == to) return true;
    return false;
}

bool Logic::pathFind_for_king(int currentIndex, int index){
    if(index == -1)return true;
    else if(index != -1)
        if (((impl->figures[currentIndex].type >= 0 && impl->figures[currentIndex].type <= 5) && (impl->figures[index].type >=6 && impl->figures[index].type <= 11)) || ((impl->figures[currentIndex].type >= 6 && impl->figures[currentIndex].type <=11) && (impl->figures[index].type >=0 && impl->figures[index].type <= 5))){
            if (impl->figures[index].type == 0 || impl->figures[index].type == 6){
                clear();
                return false;
            }
            del_figure = index;
            del_type = impl->figures[index].type;
            deliting(index);
            return true;
        }
        else return false;
    return false;
}

bool Logic::rook_move(int fromX, int fromY, int toX, int toY){
    int currentIndex = impl->findByPosition(fromX, fromY);
    bool res;
    if (fromX == toX){
        if(fromY > toY){
            for(int i = fromY-1 ; i >= toY; i--){
                int index= impl->findByPosition(fromX, i);
                res = pathFind_for_rook(currentIndex, index, i, toY);
                if(res == false) break;
            }
            return res;
        }
        else{
            for(int i=fromY+1; i <= toY; i++){
                int index= impl->findByPosition(fromX, i);
                res= pathFind_for_rook(currentIndex, index, i, toY);
                if(res == false) break;
            }
            return res;
        }
    }
    else if(fromY == toY){
        if(fromX > toX){
            for(int i = fromX-1 ; i >= toX; i--){
                int index= impl->findByPosition(i, fromY);
                res = pathFind_for_rook(currentIndex, index, i, toX);
                if(res == false) break;
            }
            return res;
        }
        else{
            for(int i= fromX+1; i <= toX; i++){
                int index= impl->findByPosition(i, fromY);
                res= pathFind_for_rook(currentIndex, index, i, toX);
                if(res == false) break;
            }
            return res;
        }
    }
    else return false;
}
bool Logic::king_move(int fromX, int fromY, int toX, int toY){
    int currentIndex = impl->findByPosition(fromX, fromY);
    int index = impl->findByPosition(toX,toY);
    if(abs(fromX-toX) == 1 && abs(fromY-toY) == 1) return pathFind_for_king(currentIndex, index);
    else if(fromY == toY && abs(fromX-toX) == 1) return pathFind_for_king(currentIndex, index);
    else if(fromX == toX && abs(fromY-toY) == 1) return pathFind_for_king(currentIndex, index);
    else return false;

}
bool Logic::knight_move(int fromX, int fromY, int toX, int toY){
    int currentIndex = impl->findByPosition(fromX, fromY);
    int index = impl->findByPosition(toX,toY);
    if(abs(fromX-toX) == 2 && abs(fromY-toY) == 1) return pathFind_for_king(currentIndex, index);
    else if(abs(fromX-toX) == 1 && abs(fromY-toY) == 2) return pathFind_for_king(currentIndex, index);
    else return false;

}
bool Logic::queen_move(int fromX, int fromY, int toX, int toY){
    if(bishop_move(fromX, fromY, toX, toY) == true) return true;
    else if(rook_move(fromX, fromY, toX, toY) == true) return true;
    else return false;
}
bool Logic::bishop_move(int fromX, int fromY, int toX, int toY){
    int currentIndex = impl->findByPosition(fromX, fromY);
    int j=fromX;
    bool res;
    if(abs(fromX-toX) == abs(fromY-toY)){
        if(fromY > toY && fromX > toX){
            for(int i = fromY-1 ; i >= toY; i--){
                j--;
                int index= impl->findByPosition(j, i);
                res = pathFind_for_rook(currentIndex, index, i, toY);
                if(res == false) break;
            }
            return res;
        }
        else if(fromY > toY && fromX < toX){
            for(int i = fromY-1 ; i >= toY; i--){
                j++;
                int index= impl->findByPosition(j, i);
                res = pathFind_for_rook(currentIndex, index, i, toY);
                if(res == false) break;
            }
            return res;
        }
        else if(fromY < toY && fromX < toX){
            for(int i=fromY+1; i <= toY; i++){
                j++;
                int index= impl->findByPosition(j, i);
                res= pathFind_for_rook(currentIndex, index, i, toY);
                if(res == false) break;
            }
            return res;
        }
        else if(fromY < toY && fromX > toX){
            for(int i=fromY+1; i <= toY; i++){
                j--;
                int index= impl->findByPosition(j, i);
                res= pathFind_for_rook(currentIndex, index, i, toY);
                if(res == false) break;
            }
            return res;
        }
    }
    else return false;
    return false;
}
bool Logic::black_pawn_move(int fromX, int fromY, int toX, int toY){
    int index = impl->findByPosition(toX,toY);
    if(fromY == 6){
        if(((fromY-toY == 1 && (fromX == toX)) || ((fromY-toY == 2) && (fromX == toX))) && index == -1) return true;
        else return false;
    }
    else if(((fromY-toY == 1) && (fromY-toY == 1)) && index == -1) return true;
    else if(((abs(fromX-toX) == 1) && (abs(fromY-toY)== 1)) && index != -1) return bishop_move(fromX, fromY, toX, toY);
    return false;
}
bool Logic::white_pawn_move(int fromX, int fromY, int toX, int toY){
    int index = impl->findByPosition(toX,toY);
    if(fromY == 1){
        if(((toY-fromY == 1 && (fromX == toX)) || ((toY-fromY == 2) && (fromX == toX))) && index == -1)return true;
        else return false;
    }
    else if(((toY-fromY == 1) && (fromX == toX)) && index == -1) return true;
    else if(((abs(fromX-toX) == 1) && (abs(fromY-toY)== 1)) && index != -1) return bishop_move(fromX, fromY, toX, toY);
    return false;
}

bool Logic::turn_check(int type){
    if(turn_count%2 == 1){
        if (type >= 0 && type <= 5) return true;
        else return false;
    }
    else if(turn_count%2 == 0){
        if(type >= 6 && type <= 11) return true;
        else return false;
    }
    return false;
}



bool Logic::move(int fromX, int fromY, int toX, int toY, int type) {
  if ((fromX == toX) && (fromY == toY)) return false;
  if (turn_check(type) == false) return false;
  else {
      int index = impl->findByPosition(fromX, fromY);
      bool flag;

      if (index < 0) return false;

      if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE ) {
        return false;
      }

      if (type == 0 || type == 6) flag = king_move(fromX, fromY, toX, toY);
      else if(type == 1 || type == 7) flag = queen_move(fromX, fromY, toX, toY);
      else if(type == 2 || type == 8) flag = bishop_move(fromX, fromY, toX, toY);
      else if(type == 3 || type == 9) flag = knight_move(fromX, fromY, toX, toY);
      else if(type == 4 || type == 10) flag = rook_move(fromX, fromY, toX, toY);
      else if (type == 11) flag = black_pawn_move(fromX, fromY, toX, toY);
      else if (type == 5) flag = white_pawn_move(fromX, fromY, toX, toY);
      if(flag == true){
          beginResetModel();
          endResetModel();
          history += to_string(index) + " " + to_string(fromX) + ':' + to_string(fromY) + " " + to_string(toX) + ':' + to_string(toY);
          if (del_figure != -1){
              history += " " + to_string(del_figure) + " " + to_string(del_type) + '\n';
              del_figure = -1;
          }
          else history += '\n';
          index = impl->findByPosition(fromX, fromY);
          impl->figures[index].x = toX;
          impl->figures[index].y = toY;
          QModelIndex topLeft = createIndex(index, 0);
          QModelIndex bottomRight = createIndex(index, 0);
          emit dataChanged(topLeft, bottomRight);
          turn_count++;
          return true;
      }
      else return false;
  }
}
void Logic::set_figures(){
    beginResetModel();
    impl->figures.clear();
    for(int i = 0; i < impl->figure.size(); i++){
        impl->figures << impl->figure[i];

    }
    endResetModel();
}
