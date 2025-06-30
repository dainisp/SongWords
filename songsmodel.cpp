#include <QDateTime>
#include "songsmodel.h"

SongsModel::SongsModel(QObject *parent):QAbstractListModel(parent)
{

}

  int SongsModel::rowCount(const QModelIndex &parent) const
  {
      return songlist.count();

  }


   QVariant SongsModel::data(const QModelIndex &index, int role) const
   {
       if(!index.isValid() || index.row() < 0 ||  index.row() >= songlist.count()   )
           return QVariant();
       QString name = songlist[index.row()].toObject()["name"].toString();
       if (role == Qt::DisplayRole || role == Qt::EditRole)
             return name;
         else
             return QVariant();
      // return QVariant(name);
   }
     Qt::ItemFlags SongsModel::flags(const QModelIndex &index) const
     {
     Qt::ItemFlags parentFlags = QAbstractListModel::flags(index) | Qt::ItemIsEditable  ;
     return parentFlags;

     }

     bool SongsModel::setData(const QModelIndex &index, const QVariant &value, int role )
     {
         if(!index.isValid() || index.row() < 0 ||  index.row() >= songlist.count() || value.type() != QMetaType::QString   )
             return false;

         if (role == Qt::DisplayRole || role == Qt::EditRole)
         {
             QString valueStr = value.toString();
            QJsonObject  songObject = songlist[index.row()].toObject();
            songObject["name"] = valueStr;
            songObject.insert("lastmodified",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
            songlist[index.row()] = songObject;
            emit valueChanged(valueStr);
            QList<int> roles;
            roles.append(role);
            emit dataChanged(index,index,roles);
               return true;
          }
               else
               return false;
     }

   int SongsModel::columnCount(const QModelIndex &parent ) const
   {
       return 1;

   }
   QVariant SongsModel::headerData(int section, Qt::Orientation orientation,
                          int role ) const
   {
       if (role != Qt::DisplayRole)
             return QVariant();

         if (orientation == Qt::Horizontal)
             return QStringLiteral("Column %1").arg(section);
         else
             return QStringLiteral("Row %1").arg(section);

   }


   bool SongsModel::insertRows(int position, int rows, const QModelIndex &index)
   {
       beginInsertRows(QModelIndex(), position, position+rows-1);

          for (int row = position; row < position + rows; row++) {
             // stringList.insert(position, "");
              if(internalInsert)
              {
                  appendSong(current_song);
              }
            else
                {
              QJsonObject song;
              song.insert("name",searchName());
              song.insert("tempo",1.2);
              song.insert("lastmodified",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
             songlist.insert(position,song);
              }

          }

          endInsertRows();
          return true;
   }


     bool SongsModel::removeRows(int position, int rows, const QModelIndex &index)
     {

         beginRemoveRows(QModelIndex(), position, position+rows-1);

             for (int row = position; row < position + rows; row++) {
                 songlist.removeAt(row);
             }

             endRemoveRows();
             return true;
     }



QString SongsModel::renameSong(QModelIndex & index, QString new_name)
{
    QJsonObject  songObject = songlist[index.row()].toObject();
    songObject["name"] = new_name;
    songlist[index.row()] = songObject;

}

    void SongsModel::addSong(QString content,QString name,double secsperbeat)
    {
    //    beginResetModel();
        QString songname=name;
      if(  songname.isEmpty() || (!songname.isEmpty() && !searchSong(songname).isEmpty() )  )
      {

              songname = searchName();
      }
   current_song = QJsonObject();
      current_song.insert("name",songname);
      current_song.insert("content",content);
      current_song.insert("tempo",secsperbeat);
        current_song.insert("lastmodified",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
      internalInsert = true;
     insertRow(songlist.count());
internalInsert = false;
     //   endResetModel();
    }

    void SongsModel::saveContent(QString name,QString content)
    {
        if(name.trimmed().isEmpty() || content.trimmed().isEmpty())

             return;
int songind = searchSongIndex(name);
 if(songind < 0)
     return;
 QJsonObject song = songlist[songind].toObject();

 song.insert( "content", content);
  song.insert("lastmodified",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
songlist[songind] = song;
 //remo
    }

    void SongsModel::saveTempo(QString name,double tempo)
    {
          if(name.trimmed().isEmpty() ||   tempo < 0.1 || tempo > 5.0 )
              return;
          int songind = searchSongIndex(name);
           if(songind < 0)
               return;

           QJsonObject song = songlist[songind].toObject();

           song.insert( "tempo", tempo);
            song.insert("lastmodified",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
          songlist[songind] = song;


    }
void SongsModel::correctDate()
{
   for(int  i=0;i<songlist.count();i++) {
        QJsonObject songObject = songlist[i].toObject();
        songObject.insert("lastmodified",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
        songlist[i] = songObject;
    }
}

QJsonObject SongsModel::searchSong(QString name)
{
   for(int  i=0;i<songlist.count();i++)
   {
       if(songlist[i].toObject()["name"].toString() == name )
       {
           return songlist[i].toObject();
       }
   }
   return QJsonObject();
}

   int SongsModel::searchSongIndex(QString name)
   {
       for(int  i=0;i<songlist.count();i++)
       {
           if(songlist[i].toObject()["name"].toString() == name )
           {
               return i;
           }
       }
       return -1;
   }

void SongsModel::removeSong(QString name)
{
    for(int  i=0;i<songlist.count();i++)
    {
        if(songlist[i].toObject()["name"].toString() == name  )
        {
            songlist.removeAt(i);
            return;
        }
    }
}


 void SongsModel::appendSong(QJsonObject song)
  {
      songlist.append(song);
  }
 QString SongsModel::searchName()
 {
     int sngnum = 1;
     for(int  i=0;i<songlist.count();i++)
     {
        QString name = songlist[i].toObject()["name"].toString();
         if(name.startsWith("Song"))
         {
           name = name.right(name.count()-4);
           bool ok;
           int num = name.toInt(&ok);
           if(ok && num >= sngnum )
               sngnum = num +1;

         }
     }
     return QString("Song") + QString::number(sngnum) ;
 }
