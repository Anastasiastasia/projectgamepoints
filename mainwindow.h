#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
class GameWidget : public QWidget
{
    Q_OBJECT

public:
    GameWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        setFixedSize(400, 400);
        currentPlayer = 1;
        boardSize = 10;
        cellSize = 40;
        board.resize(boardSize * boardSize, 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter painter(this);

        // Отрисовка клеточного игрового поля
        painter.setPen(Qt::black);
        for (int i = 0; i <= boardSize; ++i)
        {
            painter.drawLine(i * cellSize, 0, i * cellSize, boardSize * cellSize);
            painter.drawLine(0, i * cellSize, boardSize * cellSize, i * cellSize);
        }

        // Отрисовка точек игроков
        for (int i = 0; i < boardSize; ++i)
        {
            for (int j = 0; j < boardSize; ++j)
            {
                if (board[i * boardSize + j] == 1)
                {
                    painter.setBrush(Qt::red);
                    painter.drawEllipse(i * cellSize + cellSize / 4, j * cellSize + cellSize / 4, cellSize / 2, cellSize / 2);
                }
                else if (board[i * boardSize + j] == 2)
                {
                    painter.setBrush(Qt::blue);
                    painter.drawEllipse(i * cellSize + cellSize / 4, j * cellSize + cellSize / 4, cellSize / 2, cellSize / 2);
                }
            }
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        int x = event->x() / cellSize;
        int y = event->y() / cellSize;

        if (board[x * boardSize + y] == 0)
        {
            board[x * boardSize + y] = currentPlayer;
            currentPlayer = (currentPlayer == 1) ? 2 : 1;
            update();
        }
    }
    void mousePressEvent(QMouseEvent *event) override
    {
        int row = event->y() / cellSize;
        int col = event->x() / cellSize;

        // Проверка, что точка находится в пределах поля
        if (row < 0 || col < 0 || row >= boardSize || col >= boardSize) {
            return;
        }

        int clickedCell = board[row * boardSize + col];

        // Проверка, что точка принадлежит текущему игроку
        if (clickedCell != currentPlayer) {
            return;
        }

        // Соединить соседние точки одинакового цвета
        connectNeighbours(row, col, clickedCell);
    }

    void connectNeighbours(int row, int col, int color)
    {
        if (row < 0 || col < 0 || row >= boardSize || col >= boardSize) {
            return;
        }

        if (board[row * boardSize + col] != color) {
            return;
        }

        board[row * boardSize + col] = 0;

        connectNeighbours(row - 1, col, color);  // Сверху
        connectNeighbours(row + 1, col, color);  // Снизу
        connectNeighbours(row, col - 1, color);  // Слева
        connectNeighbours(row, col + 1, color);  // Справа

        update();
    }
private:
    QVector<int> board;
    int boardSize;
    int cellSize;
    int currentPlayer;
};

