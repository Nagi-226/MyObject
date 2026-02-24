#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onEvalClicked();
    void onSymbolicDiffClicked();
    void onNumericDiffClicked();
    void onIntegrateClicked();
    void onRootClicked();
    void onMinimizeClicked();
    void onOdeSolveClicked();
    void onExportCsvClicked();

private:
    Ui::MainWindow* ui;

    void appendOutput(const QString& text);
};
