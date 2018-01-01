#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>

struct Figure
{
  int type;
  int x;
  int y;
};

struct Logic::Impl
{
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

void Logic::deliting(int x){
    impl->figures.swap(x,impl->figures.size()-1);
    beginResetModel();
    impl->figures.removeLast();
    endResetModel();
}

Logic::Logic(QObject *parent)
  : QAbstractListModel(parent)
  , impl(new Impl()) 
{
  impl->figures << Figure { 4, 0, 0 };
  impl->figures << Figure { 3, 1, 0 };
  impl->figures << Figure { 2, 2, 0 };
  impl->figures << Figure { 1, 3, 0 };
  impl->figures << Figure { 0, 4, 0 };
  impl->figures << Figure { 2, 5, 0 };
  impl->figures << Figure { 3, 6, 0 };
  impl->figures << Figure { 4, 7, 0 };
  impl->figures << Figure { 5, 0, 1 };
  impl->figures << Figure { 5, 1, 1 };
  impl->figures << Figure { 5, 2, 1 };
  impl->figures << Figure { 5, 3, 1 };
  impl->figures << Figure { 5, 4, 1 };
  impl->figures << Figure { 5, 5, 1 };
  impl->figures << Figure { 5, 6, 1 };
  impl->figures << Figure { 5, 7, 1 };

  impl->figures << Figure { 10, 0, 7 };
  impl->figures << Figure { 9, 1, 7 };
  impl->figures << Figure { 8, 2, 7 };
  impl->figures << Figure { 7, 3, 7 };
  impl->figures << Figure { 6, 4, 7 };
  impl->figures << Figure { 8, 5, 7 };
  impl->figures << Figure { 9, 6, 7 };
  impl->figures << Figure { 10, 7, 7 };
  impl->figures << Figure { 11, 0, 6 };
  impl->figures << Figure { 11, 1, 6 };
  impl->figures << Figure { 11, 2, 6 };
  impl->figures << Figure { 11, 3, 6 };
  impl->figures << Figure { 11, 4, 6 };
  impl->figures << Figure { 11, 5, 6 };
  impl->figures << Figure { 11, 6, 6 };
  impl->figures << Figure { 11, 7, 6 };
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

  Figure & figure = impl->figures[index];
    
  switch (role) {
    case Roles::Type     : return figure.type;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
  }
  return QVariant(); 
}

void Logic::clear() {
  beginResetModel();
  impl->figures.clear();
  endResetModel();
}

bool Logic::pathFind_for_rook(int currentIndex, int index, int i, int to){
    if(index != -1 && i == to){
        if (((currentIndex >= 0 && currentIndex <=15) && (index >=16 && index <= 31)) || ((currentIndex >= 16 && currentIndex <=31) && (index >=0 && index <= 15))){
            if (index == 4 || index == 20){
                clear();
                return false;
            }
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
        if (((currentIndex >= 0 && currentIndex <=15) && (index >=16 && index <= 31)) || ((currentIndex >= 16 && currentIndex <=31) && (index >=0 && index <= 15))){
            if (index == 4 || index == 20){
                clear();
                return false;
            }
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
            }
            return res;
        }
        else{
            for(int i=fromY+1; i <= toY; i++){
                int index= impl->findByPosition(fromX, i);
                res= pathFind_for_rook(currentIndex, index, i, toY);
            }
            return res;
        }
    }
    else if(fromY == toY){
        if(fromX > toX){
            for(int i = fromX-1 ; i >= toX; i--){
                int index= impl->findByPosition(i, fromY);
                res = pathFind_for_rook(currentIndex, index, i, toX);
            }
            return res;
        }
        else{
            for(int i= fromX+1; i <= toX; i++){
                int index= impl->findByPosition(i, fromY);
                res= pathFind_for_rook(currentIndex, index, i, toX);
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
            }
            return res;
        }
        else if(fromY > toY && fromX < toX){
            for(int i = fromY-1 ; i >= toY; i--){
                j++;
                int index= impl->findByPosition(j, i);
                res = pathFind_for_rook(currentIndex, index, i, toY);
            }
            return res;
        }
        else if(fromY < toY && fromX < toX){
            for(int i=fromY+1; i <= toY; i++){
                j++;
                int index= impl->findByPosition(j, i);
                res= pathFind_for_rook(currentIndex, index, i, toY);
            }
            return res;
        }
        else if(fromY < toY && fromX > toX){
            for(int i=fromY+1; i <= toY; i++){
                j--;
                int index= impl->findByPosition(j, i);
                res= pathFind_for_rook(currentIndex, index, i, toY);
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
        if((fromY-toY == 1 || fromY-toY == 2) && index == -1) return true;
        else return false;
    }
    else if((fromY-toY == 1) && index == -1) return true;
    else return bishop_move(fromX, fromY, toX, toY);
}
bool Logic::white_pawn_move(int fromX, int fromY, int toX, int toY){
    int index = impl->findByPosition(toX,toY);
    if(fromY == 1){
        if((toY-fromY == 1 || toY-fromY==2) && index == -1)return true;
        else return false;
    }
    else if((toY-fromY == 1) && index == -1) return true;
    else return bishop_move(fromX, fromY, toX, toY);
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
