#pragma once
#include <memory>
#include <QAbstractListModel>

class Logic: public QAbstractListModel
{
    Q_OBJECT
public:

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

protected:
    int rowCount(const QModelIndex & parent) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

