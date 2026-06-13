#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include "cardmanager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    CardManager manager;
    QStackedWidget *stackedWidget;
    
    // Статистика сессии тренировки
    int sessionCorrect = 0;
    int sessionTotal = 0;
    int currentTrainingIndex = -1;

    // Элементы UI Добавления
    QLineEdit *addUnknownEdit;
    QLineEdit *addTranslationEdit;

    // Элементы UI Редактирования
    QListWidget *editListWidget;
    QLineEdit *editUnknownEdit;
    QLineEdit *editTranslationEdit;

    // Элементы UI Тренировки
    QLabel *trainWordLabel;
    QLabel *trainResultLabel;
    QLabel *trainSessionStatLabel;
    QLabel *trainCardStatLabel;
    QPushButton *btnCheck;
    QPushButton *btnCorrect;
    QPushButton *btnIncorrect;

    // Методы создания интерфейсов меню
    QWidget* createMainMenu();
    QWidget* createAddMenu();
    QWidget* createEditMenu();
    QWidget* createTrainMenu();

    void refreshEditList();
    void nextTrainingCard();

private slots:
    // Слоты управления окнами
    void showMainMenu();
    void showAddMenu();
    void showEditMenu();
    void showTrainMenu();

    // Слоты действий
    void onAddConfirm();
    void onEditSelect();
    void onEditConfirm();
    void onEditDelete();
    void onTrainCheck();
    void onTrainCorrect();
    void onTrainIncorrect();

public:
    MainWindow(QWidget *parent = nullptr);
};

#endif
