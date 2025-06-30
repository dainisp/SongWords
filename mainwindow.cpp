#include <iostream>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QMessageBox>
#include <QIcon>
#include <QSettings>
//#include <qdropbox.h>
//#include <qdropboxfile.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"


using std::cout,std::endl,std::flush;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
  //  ui->loadb->setIcon(QIcon("document-open",QIcon::fromTheme("document-save")));
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::timertimeout);
    songsmodel = new SongsModel(this);
    connect(songsmodel,&SongsModel::valueChanged,this,&MainWindow::onSongNameChanged);
    QSettings settings("DainisSoft","SongWords");
    if(settings.contains("songsdir"))
    {
        songsdir = settings.value("songsdir").toString();
        readSonglist(songsdir) ;
      //  QMessageBox::information(this,"SongWords","SongsLoaded from " + songsdir);
    }else{
        QMessageBox::information(this,"SongWords","Settings not contain songs path!");
        QString songsFileName =   QFileDialog::getOpenFileName(this,"Coose songfile");
        if(!songsFileName.isEmpty())
        {
            settings.setValue("songsdir",songsFileName);
            readSonglist(songsFileName);
        }
        //readSonglist("/u01/my_dir/csourc/SongWords/songs/songlist.json");

    }
    if(settings.contains("fontzize"))
    {
        QFont font = ui->contente->font();
        font.setPointSize(settings.value("fontzize").toInt());
        ui->contente->setFont(font);
    }

;


    ui->songlistv->setModel(songsmodel);

    ui->songlistv->addAction(ui->actionAdd);
    ui->songlistv->addAction(ui->actionDelete);
    ui->songlistv->addAction(ui->actionNew);
}

MainWindow::~MainWindow()
{
  saveContent();

        delete ui;
}

void MainWindow::readSonglist(QString fname)
{
    QFile listfile(fname);
    if(listfile.open(QIODevice::ReadOnly))
    {
        json_content = listfile.readAll();
        listfile.close();
        QJsonDocument doc = doc.fromJson(json_content);

        if(!doc.isNull())
        {
            QJsonArray sorted;
            QJsonArray unsorted = doc.array();

            QStringList songNames;
            foreach (QJsonValue jsonItem, unsorted) {
               songNames.append(jsonItem.toObject()["name"].toString());
                sorted.append(QJsonValue());
            }
            std::sort(songNames.begin(),songNames.end());

            foreach (QJsonValue jsonItem, unsorted) {
                int sortedIndex = songNames.indexOf( jsonItem.toObject()["name"].toString());
                sorted[sortedIndex] = jsonItem;

            }

            songsmodel->songlist = sorted;

            // songsmodel->correctDate();
            songsdir = fname;
            ui->songlistv->reset();
        }else json_content.clear();

    }


}

void MainWindow::saveContent()
{

    /*
    QSettings settings("DainisSoft","SongWords");
    if(settings.contains("songsdir"))
    {
        songsdir = settings.value("songsdir");
        readSonglist(songsdir) ;
    }else{
        readSonglist("/u01/my_dir/csourc/SongWords/songs/songlist.json");

    }
    */
    QJsonDocument doc(songsmodel->songlist);
    QByteArray new_content = doc.toJson();



    if(new_content != json_content)
       {
        if(songsdir.isEmpty())
        {
        QString newName =   QFileDialog::getSaveFileName(this,"SongWords");
            if(!newName.isEmpty())
        {
              songsdir = newName;
            QSettings settings("DainisSoft","SongWords");
                settings.setValue("songsdir",songsdir);


            }
            else songsdir = "/home/dainis/csourc/SongWords/songs/songlist.json";

        }



        QFile listfile(songsdir);
   if(listfile.open(QIODevice::WriteOnly))
   {
   listfile.write(new_content);
   listfile.close();
   }
    }
}
/*
void MainWindow::saveContentToDropBox()
{
    QDropbox dropbox("927ame3d8ev6zqx","r4aovlo479n9ms2",QDropbox::Plaintext,"api.dropbox.com",this);

if( !dropbox.requestTokenAndWait())
   {

    cout <<  "Dropbox error:" << dropbox.error() << " " << dropbox.errorString().toStdString() << endl << flush;
  QMessageBox::critical(this,"SongWords", "Dropbox error:" + QString::number( dropbox.error()) +  " " + dropbox.errorString());
    return;
}


    QDropboxFile dropboxfile("/SongWords/songlist.json",&dropbox,this);
  if(!dropboxfile.open(QIODevice::WriteOnly))
    {
   QMessageBox::critical(this,"SongWords", "Can't open file on Dropbox!" );
      return;
  }
QByteArray content = dropboxfile.readAll();
dropboxfile.close();

printf("asdf");
}

*/


/*
void MainWindow::on_songlistv_doubleClicked(const QModelIndex &index)
{
   if(timer->isActive())
       on_gob_clicked();
QJsonObject  song = songsmodel->songlist[index.row()].toObject();
internalUpdate = true;
ui->contente->setPlainText(song["content"].toString());
ui->tempoe->setText( QString::number(  song["tempo"].toDouble()));
ui->namee->setText(song["name"].toString());
internalUpdate = false;
}
*/


void MainWindow::timertimeout()
{
  if(barcounter<3)
    {
      switch (barcounter) {
      case 0:
          ui->statusbar->showMessage("One");
          break;
      case 1:
          ui->statusbar->showMessage("Two");
          break;
      case 2:
          ui->statusbar->showMessage("Three");
      default:
          break;


      }
  }
      else {
     ui->statusbar->showMessage(QString::number(barcounter-2));
     if(barcounter==3)
     {
       barindex = ui->contente->textCursor().position() ;
        }
     else
     {
     if(barindex<0)
      {
        if(!ui->loopc->isChecked())
       {
         on_gob_clicked();
     return;
        }
        else
            barindex = 0;
     }
     }
       barindex =  ui->contente->toPlainText().indexOf('|',barindex);
       if(barindex>=0)
       {
      QTextCursor cursor =  ui->contente->textCursor();
      cursor.setPosition(barindex);
      cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
      ui->contente->setTextCursor(cursor);
      //std::cout << "Pos:" << ui->contente->pos().x() << "," << ui->contente->pos().y() << "\n" << std::flush;
      ui->contente->centerCursor();
      barindex++;
       }
      }


   barcounter++;
}

double MainWindow::getTempo(double defval)
{
    if(ui->tempoe->text().trimmed().isEmpty())
        return defval;
   bool ok;
 double tempo =  ui->tempoe->text().trimmed().toDouble(&ok);
  if(ok && tempo > 0.1 && tempo < 5.0 )
      return tempo;
  return defval;
}

//--------------------------------------------------protected--------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event)
{

    if(songsdir.isEmpty())
    {
      QMainWindow::closeEvent(event);
        return;
    }
        QSettings settings("DainisSoft","SongWords");

    if(settings.contains("songsdir"))
    {
        if (  songsdir != settings.value("songsdir").toString())
        {

            settings.setValue("songsdir",songsdir);
        }

    }else
        settings.setValue("songsdir",songsdir);
    if(sizeChanged)
    {

        settings.setValue("fontzize",ui->contente->font().pointSize());

    }

     QMainWindow::closeEvent(event);

}

//----------------------------------------------------slots------------------------------------------------------

void MainWindow::on_actionAdd_triggered()
{

QString fname = QFileDialog::getOpenFileName(this,"Add song" );
if(fname.isEmpty())
    return;
QFile file(fname);
if(!file.open(QIODevice::ReadOnly))
    return;
QString content = QString::fromUtf8(file.readAll());
file.close();
songsmodel->addSong(content,ui->namee->text().trimmed(),getTempo(1.2));

}


void MainWindow::on_actionDelete_triggered()
{
  QModelIndexList selsongs =  ui->songlistv->selectionModel()->selectedIndexes();
if(selsongs.count()==1)
{
    songsmodel->removeRow(selsongs[0].row());
}
}


void MainWindow::on_saveb_clicked()
{
saveContent();
   //saveContentToDropBox();
}


void MainWindow::setFullScreen(bool full)
{
    setVisibleAllChilds(!full);
    if(full)
        showFullScreen();
    else
        showNormal();

}

  void MainWindow::setVisibleAllChilds(bool visibleAll)
{
/*
      for(QObject * childObject:ui->centralwidget->children())
    {
        if(childObject->isWidgetType())
          {
              QWidget * childWidget = qobject_cast<QWidget *>(childObject);
            if(childWidget != nullptr  )
              {
                  QString tst =  childWidget->objectName() ;
                  childWidget->setVisible(visibleAll);
            }
        }else if(childObject->inherits("QLayoutItem"))
        {
            QLayout * layoutItem = qobject_cast<QLayout *>(childObject);
            if(layoutItem != nullptr)
            {
                   QString tst =  layoutItem->objectName() ;
                setVisibleAll(visibleAll,layoutItem);
            }
        }
      }

      */



    ui->songlistv->setVisible(visibleAll);
    //ui->contente->setVisible(visibleAll);
    ui->newsongb->setVisible(visibleAll);
    ui->loadb->setVisible(visibleAll);
    ui->saveb->setVisible(visibleAll);
    ui->gob->setVisible(visibleAll);
    ui->loopc->setVisible(visibleAll);
    ui->label->setVisible(visibleAll);
    ui->tempoe->setVisible(visibleAll);
    ui->label_2->setVisible(visibleAll);
    ui->namee->setVisible(visibleAll);
    ui->zoomInB->setVisible(visibleAll);
    ui->zoomOutB->setVisible(visibleAll);

}

void MainWindow::setVisibleAll(bool visibleAll,QLayoutItem * item)
{
    QWidget * widgetItem = item->widget();
    if(widgetItem != nullptr)
    {
          QString tst =  widgetItem->objectName() ;
        if(widgetItem->objectName() != "contente" )
        widgetItem->setVisible(visibleAll);
    }
    QLayout * layoutItem = item->layout();
    if(layoutItem != nullptr)
    {
           QString tst =  layoutItem->objectName() ;
        for(int itemIndex=0;itemIndex<layoutItem->count();itemIndex++ )
        {
            setVisibleAll(visibleAll,layoutItem->itemAt(itemIndex));
        }
    }

}
 void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
     if(event->key() == Qt::Key_Escape && timer->isActive() )
        on_gob_clicked();
     QMainWindow::keyReleaseEvent(event);
}

void MainWindow::on_gob_clicked()
{
    setFullScreen(!timer->isActive());
    if(!timer->isActive())
    {
        double tempo = getTempo( 1.2);
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-stop")));
        ui->gob->setIcon(icon);
      barcounter = 0;
      timer->start(  (int)(2400.0/tempo) );
    }else
    {
        QIcon icon(QIcon::fromTheme(QString::fromUtf8("media-playback-start")));
        ui->gob->setIcon(icon);
        timer->stop();
        ui->statusbar->showMessage("Stopped");
    }
}

void MainWindow::on_contente_textChanged()
{
    if(!internalUpdate)
       songsmodel->saveContent(ui->namee->text().trimmed(),ui->contente->toPlainText());
}

void MainWindow::on_namee_textChanged(const QString &arg1)
{
   // songsmodel->renameSong()
}

void MainWindow::onSongNameChanged(QString value)
{
    ui->namee->setText(value);

}

void MainWindow::on_tempoe_textChanged(const QString &arg1)
{
    if(internalUpdate)
        return;
  bool ok;
QString tempostr = arg1.trimmed();
double tempo = tempostr.toDouble(&ok);
if(ok)
   songsmodel->saveTempo(ui->namee->text().trimmed(),tempo);
}

void MainWindow::on_actionNew_triggered()
{
    songsmodel->addSong(ui->contente->toPlainText(),ui->namee->text().trimmed(),getTempo(1.2));
    //songsmodel->insertRow(songsmodel->rowCount());
}

void MainWindow::on_songlistv_pressed(const QModelIndex &index)
{
    if(timer->isActive())
        on_gob_clicked();
 QJsonObject  song = songsmodel->songlist[index.row()].toObject();
 internalUpdate = true;
 ui->contente->setPlainText(song["content"].toString());
 ui->tempoe->setText( QString::number(  song["tempo"].toDouble()));
 ui->namee->setText(song["name"].toString());
 internalUpdate = false;
}




void MainWindow::on_loadb_clicked()
{

    QString fname = QFileDialog::getOpenFileName(this,"Load songs" );

    if(fname.isEmpty())
        return;
 readSonglist(fname);

        songsdir = fname;


        QSettings settings("DainisSoft","SongWords");

    settings.setValue("songsdir",songsdir);
            QMessageBox::information(this,"SongWords","SongsLoaded from " + songsdir);
            ui->songlistv->setModel(songsmodel);


  /*
    QFile file(fname);
    if(!file.open(QIODevice::ReadOnly))
        return;
    QString content = QString::fromUtf8(file.readAll());
    file.close();
    songsmodel->addSong(content,ui->namee->text().trimmed(),getTempo(1.2));
*/
}


void MainWindow::on_zoomInB_clicked()
{

    ui->contente->zoomIn();
   // ui->statusbar->showMessage( QString::number( ui->contente->font().pointSize()));
    sizeChanged = true;
}


void MainWindow::on_zoomOutB_clicked()
{
    ui->contente->zoomOut();
    sizeChanged = true;
}

