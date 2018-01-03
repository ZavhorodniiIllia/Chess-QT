#pragma once
#include <cstdlib>
#include <memory>
#include <string>
#include <QAbstractListModel>

using namespace std;

class Logic: public QAbstractListModel
{
    Q_OBJECT
public:
    int turn_count = 1;
    int step = -1;
    int del_figure = -1;
    int del_type = -1;
    string history = "";
    enum GlobalConstants {
        BOARD_SIZE = 8
    };

    enum Roles {
        Type = Qt::UserRole,
        PositionX,
        PositionY,
    };
    
public:
    explicit Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT);
    int boardSize() const;

    Q_INVOKABLE void clear();
    Q_INVOKABLE bool move(int fromX, int fromY, int toX, int toY, int type);

    Q_INVOKABLE bool bishop_move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE bool king_move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE bool knight_move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE bool black_pawn_move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE bool queen_move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE bool rook_move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE bool white_pawn_move(int fromX, int fromY, int toX, int toY);

    Q_INVOKABLE bool pathFind_for_rook(int currentIndex, int index, int i, int to);
    Q_INVOKABLE bool pathFind_for_king(int currentIndex, int index);
    Q_INVOKABLE bool turn_check(int type);
    Q_INVOKABLE void save();
    Q_INVOKABLE void next_step();
    Q_INVOKABLE void prev_step();
    Q_INVOKABLE void set_figures();
    Q_INVOKABLE void deliting(int x);

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

