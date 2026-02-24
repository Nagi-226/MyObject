#include "MainWindow.h"
#include "Expression.h"
#include "Symbolic.h"
#include "Numerics.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QString>

#include "ui_MainWindow.h"

using namespace std;

static double parseDoubleOrThrow(const QString& s, const QString& fieldName) {
    bool ok = false;
    double v = s.trimmed().toDouble(&ok);
    if (!ok) throw runtime_error((fieldName + " is not a number").toStdString());
    return v;
}

static VarEnv readEnvFromUI(Ui::MainWindow* ui) {
    VarEnv env;
    env["x"] = parseDoubleOrThrow(ui->xValEdit->text(), "x");
    env["y"] = parseDoubleOrThrow(ui->yValEdit->text(), "y");
    env["z"] = parseDoubleOrThrow(ui->zValEdit->text(), "z");
    return env;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->evalBtn, &QPushButton::clicked, this, &MainWindow::onEvalClicked);
    connect(ui->symDiffBtn, &QPushButton::clicked, this, &MainWindow::onSymbolicDiffClicked);
    connect(ui->numDiffBtn, &QPushButton::clicked, this, &MainWindow::onNumericDiffClicked);
    connect(ui->intBtn, &QPushButton::clicked, this, &MainWindow::onIntegrateClicked);
    connect(ui->rootBtn, &QPushButton::clicked, this, &MainWindow::onRootClicked);
    connect(ui->minBtn, &QPushButton::clicked, this, &MainWindow::onMinimizeClicked);
    connect(ui->odeBtn, &QPushButton::clicked, this, &MainWindow::onOdeSolveClicked);
    connect(ui->csvBtn, &QPushButton::clicked, this, &MainWindow::onExportCsvClicked);

    appendOutput("Ready. Supported functions: sin cos tan exp log ln sqrt abs. Constants: pi e.");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::appendOutput(const QString& text) {
    ui->outputText->append(text);
}

void MainWindow::onEvalClicked() {
    try {
        auto exprText = ui->exprEdit->text().trimmed();
        Expression f(exprText.toStdString());
        VarEnv env = readEnvFromUI(ui);
        double v = f.eval(env);
        appendOutput(QString("Eval: f(%1,%2,%3) = %4")
                         .arg(env["x"], 0, 'g', 12)
                         .arg(env["y"], 0, 'g', 12)
                         .arg(env["z"], 0, 'g', 12)
                         .arg(v, 0, 'g', 16));
    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
}

void MainWindow::onSymbolicDiffClicked() {
    try {
        auto exprText = ui->exprEdit->text().trimmed();
        Expression f(exprText.toStdString());

        // 默认对 x 求导（可在后续 UI 扩展成下拉框）
        auto d = Symbolic::simplify(Symbolic::derivative(f.ast(), "x"));
        appendOutput(QString("Symbolic d/dx: %1").arg(QString::fromStdString(Symbolic::toString(d))));

        // 同时给出在当前点的数值值（方便对照）
        VarEnv env = readEnvFromUI(ui);
        Expression df(Symbolic::toString(d));
        appendOutput(QString("At current point: d/dx = %1").arg(df.eval(env), 0, 'g', 16));

    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
}

void MainWindow::onNumericDiffClicked() {
    try {
        auto exprText = ui->exprEdit->text().trimmed();
        Expression f(exprText.toStdString());
        VarEnv env = readEnvFromUI(ui);

        // 默认对 x 做偏导
        double pd = Numerics::partialDerivative(f, env, "x");
        appendOutput(QString("Numeric partial d/dx at current point = %1").arg(pd, 0, 'g', 16));
    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
}

void MainWindow::onIntegrateClicked() {
    try {
        // 演示：对 x 在 [0, pi] 上积分（后续可做成 UI 参数）
        auto exprText = ui->exprEdit->text().trimmed();
        Expression f(exprText.toStdString());
        VarEnv env = readEnvFromUI(ui);

        double a = 0.0;
        double b = acos(-1.0);
        double res = Numerics::integrateSimpson(f, env, "x", a, b, 2000);
        appendOutput(QString("Integral over x in [0,pi] = %1").arg(res, 0, 'g', 16));
    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
}

void MainWindow::onRootClicked() {
    try {
        // 演示：用 Newton 对 x 求根，从当前 x 初值开始
        auto exprText = ui->exprEdit->text().trimmed();
        Expression f(exprText.toStdString());
        VarEnv env = readEnvFromUI(ui);

        double x0 = env["x"];
        auto r = Numerics::rootNewton(f, env, "x", x0);
        appendOutput(QString("Root (Newton) starting x0=%1 => x=%2 (iters=%3)")
                         .arg(x0, 0, 'g', 12)
                         .arg(r.x, 0, 'g', 16)
                         .arg(r.iterations));
    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
}

void MainWindow::onMinimizeClicked() {
    try {
        // 演示：黄金分割在 [x-5, x+5] 上找最小值
        auto exprText = ui->exprEdit->text().trimmed();
        Expression f(exprText.toStdString());
        VarEnv env = readEnvFromUI(ui);

        double x = env["x"];
        double lo = x - 5.0;
        double hi = x + 5.0;
        auto r = Numerics::minimizeGolden(f, env, "x", lo, hi);
        appendOutput(QString("Minimize on [%1,%2] => x=%3, f=%4 (iters=%5)")
                         .arg(lo, 0, 'g', 12)
                         .arg(hi, 0, 'g', 12)
                         .arg(r.x, 0, 'g', 16)
                         .arg(r.fx, 0, 'g', 16)
                         .arg(r.iterations));
    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
}

void MainWindow::onOdeSolveClicked() {
    try {
        // 将表达式视为 y' = f(x,y)，使用当前 x 作为 x0，y 使用 yValEdit
        auto exprText = ui->exprEdit->text().trimmed();
        Expression dydx(exprText.toStdString());

        double x0 = parseDoubleOrThrow(ui->xValEdit->text(), "x");
        double y0 = parseDoubleOrThrow(ui->yValEdit->text(), "y");
        double x1 = x0 + 5.0;
        int steps = 200;

        auto samples = Numerics::solveODE_RK4(dydx, x0, y0, x1, steps, "x", "y");
        appendOutput(QString("ODE solved with RK4: %1 samples (x from %2 to %3)")
                         .arg((int)samples.size())
                         .arg(x0, 0, 'g', 12)
                         .arg(x1, 0, 'g', 12));
        // 显示末值
        if (!samples.empty()) {
            appendOutput(QString("Last: x=%1, y=%2")
                             .arg(samples.back().x, 0, 'g', 16)
                             .arg(samples.back().y, 0, 'g', 16));
        }
    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
}

void MainWindow::onExportCsvClicked() {
    try {
        auto exprText = ui->exprEdit->text().trimmed();
        Expression f(exprText.toStdString());
        VarEnv env = readEnvFromUI(ui);

        QString filePath = QFileDialog::getSaveFileName(this, "Export CSV", "", "CSV Files (*.csv)");
        if (filePath.isEmpty()) return;

        // 对 x 在 [x-10, x+10] 采样 1001 点
        double x = env["x"];
        Numerics::exportCSV_1D(f, env, "x", x - 10.0, x + 10.0, 1001, filePath.toStdString());
        appendOutput(QString("CSV exported: %1").arg(filePath));

    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", ex.what());
    }
}




