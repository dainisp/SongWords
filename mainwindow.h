#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QTimer>
#include <QKeyEvent>

#include "qlayoutitem.h"
#include "songsmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAdd_triggered();



    void on_actionDelete_triggered();

    void on_saveb_clicked();

  //  void on_songlistv_doubleClicked(const QModelIndex &index);

    void on_gob_clicked();
    void timertimeout();



    void on_contente_textChanged();

    void on_namee_textChanged(const QString &arg1);

    void on_tempoe_textChanged(const QString &arg1);

    void on_actionNew_triggered();

    void on_songlistv_pressed(const QModelIndex &index);

    void onSongNameChanged(QString value);



    void on_loadb_clicked();

    void on_zoomInB_clicked();

    void on_zoomOutB_clicked();

private:
    Ui::MainWindow *ui;
    bool sizeChanged = false;
    QString songsdir;
    SongsModel * songsmodel;
    QByteArray json_content;
    QTimer * timer;
    int barcounter;
    int barindex;
    bool internalUpdate=false;
    double getTempo(double defval);
    void saveContent();
    void readSonglist(QString fname);
    void setFullScreen(bool full);
    void setVisibleAll(bool visibleAll, QLayoutItem *item);
     void setVisibleAllChilds(bool visibleAll);
  //  void saveContentToDropBox();
protected:
  void closeEvent(QCloseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
