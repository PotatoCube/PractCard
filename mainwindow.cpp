#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // ВЫПОЛНЕНИЕ ТЗ: Окно маленькое и всегда поверх других окон
    setWindowFlags(Qt::WindowStaysOnTopHint | QWidget().windowFlags());
    resize(320, 450);
    setWindowTitle("Карточки");

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(createMainMenu()); // Индекс 0
    stackedWidget->addWidget(createAddMenu());  // Индекс 1
    stackedWidget->addWidget(createEditMenu()); // Индекс 2
    stackedWidget->addWidget(createTrainMenu());// Индекс 3

    setCentralWidget(stackedWidget);
}

// ---------------- ГЛАВНОЕ МЕНЮ ----------------
QWidget* MainWindow::createMainMenu() {
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    QPushButton *btnAdd = new QPushButton("1. Добавление слов", widget);
    QPushButton *btnEdit = new QPushButton("2. Редактирование слов", widget);
    QPushButton *btnTrain = new QPushButton("3. Тренировка", widget);
    QPushButton *btnExit = new QPushButton("4. Выход", widget);

    layout->addWidget(new QLabel("=== ГЛАВНОЕ МЕНЮ ==="));
    layout->addWidget(btnAdd);
    layout->addWidget(btnEdit);
    layout->addWidget(btnTrain);
    layout->addWidget(btnExit);

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::showAddMenu);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::showEditMenu);
    connect(btnTrain, &QPushButton::clicked, this, &MainWindow::showTrainMenu);
    connect(btnExit, &QPushButton::clicked, this, &MainWindow::close);

    return widget;
}

// ---------------- МЕНЮ ДОБАВЛЕНИЯ ----------------
QWidget* MainWindow::createAddMenu() {
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    addUnknownEdit = new QLineEdit(widget);
    addTranslationEdit = new QLineEdit(widget);
    QPushButton *btnConfirm = new QPushButton("Подтвердить", widget);
    QPushButton *btnDone = new QPushButton("Готово", widget);

    layout->addWidget(new QLabel("Unknown (Слово):"));
    layout->addWidget(addUnknownEdit);
    layout->addWidget(new QLabel("Translation (Перевод):"));
    layout->addWidget(addTranslationEdit);
    layout->addWidget(btnConfirm);
    layout->addWidget(btnDone);

    connect(btnConfirm, &QPushButton::clicked, this, &MainWindow::onAddConfirm);
    connect(btnDone, &QPushButton::clicked, this, &MainWindow::showMainMenu);

    return widget;
}

void MainWindow::onAddConfirm() {
    if(addUnknownEdit->text().isEmpty() || addTranslationEdit->text().isEmpty()) return;
    manager.addCard(addUnknownEdit->text().toStdString(), addTranslationEdit->text().toStdString());
    addUnknownEdit->clear();
    addTranslationEdit->clear();
    QMessageBox::information(this, "Успех", "Пара слов добавлена!");
}

// ---------------- МЕНЮ РЕДАКТИРОВАНИЯ ----------------
QWidget* MainWindow::createEditMenu() {
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    editListWidget = new QListWidget(widget);
    editUnknownEdit = new QLineEdit(widget);
    editTranslationEdit = new QLineEdit(widget);
    
    QPushButton *btnConfirm = new QPushButton("Подтвердить изменения", widget);
    QPushButton *btnDelete = new QPushButton("Удалить", widget);
    QPushButton *btnDone = new QPushButton("Готово", widget);

    layout->addWidget(editListWidget);
    layout->addWidget(new QLabel("Редактировать Слово:"));
    layout->addWidget(editUnknownEdit);
    layout->addWidget(new QLabel("Редактировать Перевод:"));
    layout->addWidget(editTranslationEdit);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnConfirm);
    btnLayout->addWidget(btnDelete);
    layout->addLayout(btnLayout);
    layout->addWidget(btnDone);

    connect(editListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::onEditSelect);
    connect(btnConfirm, &QPushButton::clicked, this, &MainWindow::onEditConfirm);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onEditDelete);
    connect(btnDone, &QPushButton::clicked, this, &MainWindow::showMainMenu);

    return widget;
}

void MainWindow::refreshEditList() {
    editListWidget->clear();
    for (const auto& card : manager.getCards()) {
        editListWidget->addItem(QString::fromStdString(card.getUnknown() + " -> " + card.getTranslation()));
    }
    editUnknownEdit->clear();
    editTranslationEdit->clear();
}

void MainWindow::onEditSelect() {
    int idx = editListWidget->currentRow();
    if (idx < 0 || idx >= (int)manager.getCards().size()) return;
    editUnknownEdit->setText(QString::fromStdString(manager.getCards()[idx].getUnknown()));
    editTranslationEdit->setText(QString::fromStdString(manager.getCards()[idx].getTranslation()));
}

void MainWindow::onEditConfirm() {
    int idx = editListWidget->currentRow();
    if (idx < 0) return;
    manager.updateCard(idx, editUnknownEdit->text().toStdString(), editTranslationEdit->text().toStdString());
    refreshEditList();
}

void MainWindow::onEditDelete() {
    int idx = editListWidget->currentRow();
    if (idx < 0) return;
    manager.deleteCard(idx);
    refreshEditList();
}

// ---------------- МЕНЮ ТРЕНИРОВКИ ----------------
QWidget* MainWindow::createTrainMenu() {
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(widget);

    trainWordLabel = new QLabel("", widget);
    trainWordLabel->setAlignment(Qt::AlignCenter);
    QFont font = trainWordLabel->font(); font.setPointSize(16); font.setBold(true);
    trainWordLabel->setFont(font);

    trainResultLabel = new QLabel("", widget);
    trainResultLabel->setAlignment(Qt::AlignCenter);
    
    trainCardStatLabel = new QLabel("", widget);
    trainSessionStatLabel = new QLabel("Сессия: Вспомнили 0 из 0", widget);

    btnCheck = new QPushButton("Проверить", widget);
    btnCorrect = new QPushButton("Верно", widget);
    btnIncorrect = new QPushButton("Неверно", widget);
    QPushButton *btnFinish = new QPushButton("Финиш", widget);

    layout->addWidget(trainSessionStatLabel);
    layout->addWidget(trainWordLabel);
    layout->addWidget(trainResultLabel);
    layout->addWidget(trainCardStatLabel);
    layout->addWidget(btnCheck);
    
    QHBoxLayout *choiceLayout = new QHBoxLayout();
    choiceLayout->addWidget(btnCorrect);
    choiceLayout->addWidget(btnIncorrect);
    layout->addLayout(choiceLayout);
    layout->addWidget(btnFinish);

    connect(btnCheck, &QPushButton::clicked, this, &MainWindow::onTrainCheck);
    connect(btnCorrect, &QPushButton::clicked, this, &MainWindow::onTrainCorrect);
    connect(btnIncorrect, &QPushButton::clicked, this, &MainWindow::onTrainIncorrect);
    connect(btnFinish, &QPushButton::clicked, this, &MainWindow::showMainMenu);

    return widget;
}

void MainWindow::nextTrainingCard() {
    currentTrainingIndex = manager.getRandomCardIndex();
    if (currentTrainingIndex == -1) {
        trainWordLabel->setText("Словарь пуст!");
        btnCheck->setEnabled(false);
        return;
    }
    btnCheck->setEnabled(true);
    btnCorrect->hide();
    btnIncorrect->hide();
    
    trainWordLabel->setText(QString::fromStdString(manager.getCards()[currentTrainingIndex].getUnknown()));
    trainResultLabel->clear();
    trainCardStatLabel->clear();
}

void MainWindow::onTrainCheck() {
    if (currentTrainingIndex == -1) return;
    const auto& card = manager.getCards()[currentTrainingIndex];
    trainResultLabel->setText("Перевод: " + QString::fromStdString(card.getTranslation()));
    trainCardStatLabel->setText(QString("Общий счет карты: Верно: %1 | Неверно: %2")
                                .arg(card.getCorrectCount()).arg(card.getIncorrectCount()));
    btnCorrect->show();
    btnIncorrect->show();
}

void MainWindow::onTrainCorrect() {
    manager.loadFromFile(); // Синхронизируем на случай изменений
    auto cards = manager.getCards();
    cards[currentTrainingIndex].markCorrect();
    
    // Перезаписываем менеджер и файл
    manager.updateCard(currentTrainingIndex, cards[currentTrainingIndex].getUnknown(), cards[currentTrainingIndex].getTranslation());
    // Принудительно вызываем маркеры изменений счета внутренней логики
    std::string unk = cards[currentTrainingIndex].getUnknown();
    std::string trans = cards[currentTrainingIndex].getTranslation();
    manager.deleteCard(currentTrainingIndex);
    // Для простоты реализации без перегрузки менеджера напрямую внесем обновленные данные в файл
    manager.addCard(unk, trans); 
    manager.loadFromFile(); 

    sessionCorrect++;
    sessionTotal++;
    trainSessionStatLabel->setText(QString("Сессия: Вспомнили %1 из %2").arg(sessionCorrect).arg(sessionTotal));
    nextTrainingCard();
}

void MainWindow::onTrainIncorrect() {
    sessionTotal++;
    trainSessionStatLabel->setText(QString("Сессия: Вспомнили %1 из %2").arg(sessionCorrect).arg(sessionTotal));
    nextTrainingCard();
}

// СЛОТЫ НАВИГАЦИИ
void MainWindow::showMainMenu() { stackedWidget->setCurrentIndex(0); }
void MainWindow::showAddMenu() { stackedWidget->setCurrentIndex(1); }
void MainWindow::showEditMenu() { refreshEditList(); stackedWidget->setCurrentIndex(2); }
void MainWindow::showTrainMenu() { 
    sessionCorrect = 0; sessionTotal = 0; 
    trainSessionStatLabel->setText("Сессия: Вспомнили 0 из 0");
    nextTrainingCard(); 
    stackedWidget->setCurrentIndex(3); 
}
