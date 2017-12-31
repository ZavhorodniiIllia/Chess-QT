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


bool Logic::rook_move(int fromX, int fromY, int toX, int toY){
    int type = impl->findByPosition(fromX, fromY);
    int a;
    int b;
    int count=0;
    int indexDel;
    if (fromX == toX){
        if(fromY > toY){
            a= toY;
            b= fromY-1;
        }
        else{
            a= fromY+1;
            b= toY;
        }
        for(int i=a; i <= b; i++){
            int index= impl->findByPosition(fromX, i);
            if (index == -1 && i == b){
                if(count == 0 ) return true;
                else if(count == 1){
                    deliting(indexDel);
                    return true;
                }
                else if(count > 1) return false;
            }
            else{
                if(((type>=0 && type<=15) && (index>=0 && index <=15)) || ((type>=16 && type<=31) && (index>=16 && index <=31)) ) return false;
                else if(((type>=0 && type<=15) && (index>=16 && index <=31)) || ((type>=16 && type<=31) && (index>=0 && index <=15)) ){
                    count ++;
                    indexDel=index;
                }
            }
        }
            }
    else if(fromY == toY){
        if(fromX > toX){
            a= toX+1;
            b= fromX-1;
        }
        else{
            a= fromX+1;
            b= toX-1;
        }
        for(int i=a; i <= b; i++){
            if (impl->findByPosition(i, fromY) == -1 && i == b) return true;
        }
    }
    else return false;
}
bool Logic::king_move(int fromX, int fromY, int toX, int toY){
    if(abs(fromX-toX) == 1 && abs(fromY-toY) == 1) return true;
    else if(fromY == toY && abs(fromX-toX) == 1) return true;
    else if(fromX == toX && abs(fromY-toY) == 1) return true;
    else return false;

}
bool Logic::knight_move(int fromX, int fromY, int toX, int toY){
    if(abs(fromX-toX) == 2 && abs(fromY-toY) == 1) return true;
    else if(abs(fromX-toX) == 1 && abs(fromY-toY) == 2) return true;
    else return false;

}
bool Logic::queen_move(int fromX, int fromY, int toX, int toY){
    if(abs(fromX-toX) == abs(fromY-toY)) return true;
    else if (fromX == toX){
        if(toY >= 0 || toY <= 7) return true;
    }
    else if(fromY == toY){
        if(toX >= 0 || toX <= 7) return true;
    }
    else return false;

}
bool Logic::bishop_move(int fromX, int fromY, int toX, int toY){
    if(abs(fromX-toX) == abs(fromY-toY)) return true;
    else return false;

}
bool Logic::black_pawn_move(int fromX, int fromY, int toX, int toY){
    for (int i= fromY-1; i > toY; i--){
        if (impl->findByPosition(fromX, i) != -1){
            return false;
        }
    }
    if(fromY == 6){
        if(fromY-toY == 1 || fromY-toY == 2) return true;
        else return false;
    }
    else{
        if(fromY-toY == 1) return true;
        else return false;
    }
}
bool Logic::white_pawn_move(int fromX, int fromY, int toX, int toY){
    for (int i= fromY+1; i < toY; i++){
        if (impl->findByPosition(fromX, i) != -1) return false;

    }
    if(fromY == 1){
        if(toY-fromY == 1 || toY-fromY==2)return true;
        else return false;
    }
    else{
        if(toY-fromY == 1) return true;
        else return false;
    }

}

bool Logic::move(int fromX, int fromY, int toX, int toY, int type) {
  if ((fromX == toX) && (fromY == toY)){
      return false;
  }
  else {
      int index = impl->findByPosition(fromX, fromY);
      int indexNext = impl->findByPosition(toX, toY);
      bool flag;

      if (index < 0) return false;

      if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE || indexNext >=0) {
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
          return true;
      }
      else return false;
  }
}
