#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QMainWindow>
#include <QAction>
#include <QImage>
#include <QLabel>
#include <QScrollArea>
#include <QResizeEvent>
#include <QFileDialog>
#include <QResizeEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // File menu slots
    void open();
    void save();
    void saveAs();
    void quit();

    // Edit menu slots
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();

private:
    // File menu actions
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* quitAction;

    // Edit menu actions
    QAction* copyAction;
    QAction* pasteAction;
    QAction* zoomInAction;
    QAction* zoomOutAction;
    QAction* normalSizeAction;
    QAction* fitToWindowAction;

    // Toolbar actions
    QAction* openToolbarAction;
    QAction* saveToolbarAction;
    QAction* saveAsToolbarAction;
    QAction* quitToolbarAction;
    QAction* copyToolbarAction;
    QAction* pasteToolbarAction;
    QAction* zoomInToolbarAction;
    QAction* zoomOutToolbarAction;
    QAction* normalSizeToolbarAction;
    QAction* fitToWindowToolbarAction;

    void createFileMenu();
    void createEditMenu();
    void createToolbar();
    void createConnections();

    // image attributes
    QImage image;
    QLabel* imageLabel;
    QScrollArea* scrollArea;
    qfloat16 scaleFactor = 1;

    bool loadFile(QString const& fileName);
    static void initializeImageDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode);
    void scaleImage(qfloat16 factor);
    void updateMenuBar(bool enabled);
    void updateToolbar(bool enabled);
    void updateZoomFunctions(bool enabled);

protected:
    void resizeEvent(QResizeEvent *event);
};
#endif // MAINWINDOW_H
