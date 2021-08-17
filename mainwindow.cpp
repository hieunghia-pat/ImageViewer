#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QToolBar>
#include <QImage>
#include <QLabel>
#include <QScrollArea>
#include <QResizeEvent>
#include <QFileDialog>
#include <QDir>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QShortcut>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // establish window configuration
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    setWindowTitle("Image Viewer");
    createFileMenu();
    createEditMenu();
    createToolbar();
    createConnections();

    // establish image layout configuration
    imageLabel = new QLabel(this);

    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    // establish scrollable area
    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);

    setCentralWidget(scrollArea);
}

void MainWindow::createFileMenu()
{
    // establish File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");
    // add actions to File menu
    openAction = new QAction(QIcon(QString(":/icons/open-icon.png")), QString("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAction);
    saveAction = new QAction(QIcon(":/icons/save-icon.png"), QString("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setEnabled(false);
    fileMenu->addAction(saveAction);
    saveAsAction = new QAction(QIcon(":/icons/saveas-icon.png"), "Save As...", this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setEnabled(false);
    fileMenu->addAction(saveAsAction);
    quitAction = new QAction(QIcon(":/icons/quit-icon.png"), "Quit", this);
    quitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);
}

MainWindow::~MainWindow()
{

}

void MainWindow::createEditMenu()
{
    // etstablish Edit menu
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    // add actions to Edit menu
    copyAction = new QAction(QIcon(":/icons/copy-icon.png"), "Copy", this);
    copyAction->setShortcut(QKeySequence::Copy);
    editMenu->addAction(copyAction);
    pasteAction = new QAction(QIcon(":/icons/paste-icon.png"), "Paste", this);
    pasteAction->setShortcut(QKeySequence::Paste);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    zoomInAction = new QAction(QIcon(":/icons/zoomin-icon.png"), "Zoom in", this);
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    editMenu->addAction(zoomInAction);
    zoomOutAction = new QAction(QIcon(":/icons/zoomout-icon.png"), "Zoom out", this);
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    editMenu->addAction(zoomOutAction);
    editMenu->addSeparator();
    normalSizeAction = new QAction(QIcon(":/icons/normalsize-icon.png"), "Normal Size", this);
    editMenu->addAction(normalSizeAction);
    fitToWindowAction = new QAction(QIcon(":/icons/fittowindow-icon.png"), "Fit to window", this);
    editMenu->addAction(fitToWindowAction);

    updateMenuBar(false);
}

void MainWindow::createToolbar()
{
    QToolBar* toolbar = addToolBar("MainToolbar");
    toolbar->setMovable(false);
    openToolbarAction = toolbar->addAction(QIcon(":/icons/open-icon.png"), QString("Open"));
    saveToolbarAction = toolbar->addAction(QIcon(":/icons/save-icon.png"), QString("Save"));
    saveAsToolbarAction = toolbar->addAction(QIcon(":/icons/saveas-icon.png"), QString("Save As"));

    toolbar->addSeparator();

    copyToolbarAction = toolbar->addAction(QIcon(":/icons/copy-icon.png"), QString("Copy"));
    pasteToolbarAction = toolbar->addAction(QIcon(":/icons/paste-icon.png"), QString("Paste"));
    zoomInToolbarAction = toolbar->addAction(QIcon(":/icons/zoomin-icon.png"), QString("Zoom In"));
    zoomOutToolbarAction = toolbar->addAction(QIcon(":/icons/zoomout-icon.png"), QString("Zoom Out"));
    normalSizeToolbarAction = toolbar->addAction(QIcon(":/icons/normalsize-icon.png"), QString("Normal Size"));
    fitToWindowToolbarAction = toolbar->addAction(QIcon(":/icons/fittowindow-icon.png"), QString("Fit To Window"));

    toolbar->addSeparator();

    quitToolbarAction = toolbar->addAction(QIcon(":/icons/quit-icon.png"), QString("Quit"));

    updateToolbar(false);
}

void MainWindow::createConnections()
{
    QObject::connect(openAction, &QAction::triggered, this, &MainWindow::open);
    QObject::connect(openToolbarAction, &QAction::triggered, this, &MainWindow::open);

    QObject::connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    QObject::connect(saveToolbarAction, &QAction::triggered, this, &MainWindow::save);

    QObject::connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
    QObject::connect(saveAsToolbarAction, &QAction::triggered, this, &MainWindow::saveAs);

    QObject::connect(copyAction, &QAction::triggered, this, &MainWindow::copy);
    QObject::connect(copyToolbarAction, &QAction::triggered, this, &MainWindow::copy);

    QObject::connect(pasteAction, &QAction::triggered, this, &MainWindow::paste);
    QObject::connect(pasteToolbarAction, &QAction::triggered, this, &MainWindow::paste);

    QObject::connect(zoomInAction, &QAction::triggered, this, &MainWindow::zoomIn);
    QObject::connect(zoomInToolbarAction, &QAction::triggered, this, &MainWindow::zoomIn);

    QObject::connect(zoomOutAction, &QAction::triggered, this, &MainWindow::zoomOut);
    QObject::connect(zoomOutToolbarAction, &QAction::triggered, this, &MainWindow::zoomOut);

    QObject::connect(normalSizeAction, &QAction::triggered, this, &MainWindow::normalSize);
    QObject::connect(normalSizeToolbarAction, &QAction::triggered, this, &MainWindow::normalSize);

    QObject::connect(fitToWindowAction, &QAction::triggered, this, &MainWindow::fitToWindow);
    QObject::connect(fitToWindowToolbarAction, &QAction::triggered, this, &MainWindow::fitToWindow);

    QObject::connect(quitAction, &QAction::triggered, this, &MainWindow::quit);
    QObject::connect(quitToolbarAction, &QAction::triggered, this, &MainWindow::quit);

}

void MainWindow::updateMenuBar(bool enabled)
{
    saveAction->setEnabled(enabled);
    saveAsAction->setEnabled(enabled);

    copyAction->setEnabled(enabled);
    pasteAction->setEnabled(enabled);

    zoomInAction->setEnabled(enabled);
    zoomOutAction->setEnabled(enabled);

    normalSizeAction->setEnabled(enabled);
    fitToWindowAction->setEnabled(enabled);
}

void MainWindow::updateToolbar(bool enabled)
{
    saveToolbarAction->setEnabled(enabled);
    saveAsToolbarAction->setEnabled(enabled);

    copyToolbarAction->setEnabled(enabled);
    pasteToolbarAction->setEnabled(enabled);

    zoomInToolbarAction->setEnabled(enabled);
    zoomOutToolbarAction->setEnabled(enabled);

    normalSizeToolbarAction->setEnabled(enabled);
    fitToWindowToolbarAction->setEnabled(enabled);
}

void MainWindow::updateZoomFunctions(bool enabled)
{
    zoomInAction->setEnabled(enabled);
    zoomInToolbarAction->setEnabled(enabled);
    zoomOutAction->setEnabled(enabled);
    zoomOutToolbarAction->setEnabled(enabled);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (imageLabel->isVisible())
        fitToWindow();
}

void MainWindow::initializeImageDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (QByteArray const& mimeTypeName : supportedMimeTypes)
    {
        mimeTypeFilters.append(mimeTypeName);
    }
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void MainWindow::scaleImage(qfloat16 factor)
{
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap().size());

    zoomInAction->setEnabled(scaleFactor <= 3.0);
    zoomOutAction->setEnabled(scaleFactor >= 0.3);

    zoomInToolbarAction->setEnabled(scaleFactor <= 3.0);
    zoomOutToolbarAction->setEnabled(scaleFactor >= 0.3);
}

void MainWindow::open()
{
    QFileDialog openDialog(this, QString("Open File"));
    initializeImageDialog(openDialog, QFileDialog::AcceptOpen);

    while (openDialog.exec() == QDialog::Accepted && !loadFile(openDialog.selectedFiles().constFirst()))
    {
        // this loop keep the program hang on while the open window is opening
    }
}

bool MainWindow::loadFile(QString const& fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    image = reader.read();
    if (image.isNull())
    {
        QMessageBox::information(this, QApplication::applicationDisplayName(), QString("Can not load %1: %2").arg(fileName, reader.errorString()));
        return false;
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));
    imageLabel->adjustSize(); // resize the label to fit its content
    scrollArea->setVisible(true);

    updateMenuBar(true);
    updateToolbar(true);

    return true;
}

void MainWindow::save()
{

}

void MainWindow::saveAs()
{

}


void MainWindow::copy()
{

}

void MainWindow::paste()
{

}

void MainWindow::quit()
{
    QApplication::quit();
}

void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::normalSize()
{
    scrollArea->setWidgetResizable(false);
    imageLabel->adjustSize();
    scaleFactor = 1;
    updateZoomFunctions(true);
}

void MainWindow::fitToWindow()
{
    scrollArea->setWidgetResizable(true);
    updateZoomFunctions(false);
}
