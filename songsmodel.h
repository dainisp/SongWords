#ifndef SONGSMODEL_H
#define SONGSMODEL_H

#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonObject>
class SongsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SongsModel(QObject *parent = nullptr);
    QJsonArray songlist;
    QString renameSong(QModelIndex & index, QString new_name);
    void addSong(QString content,QString name=QString(),double secsperbeat=1.2);
    void saveContent(QString name,QString content);
    void saveTempo(QString name,double tempo);
      void correctDate();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                           int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
      bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
signals:
   void   valueChanged(QString value);

private:
   QJsonObject searchSong(QString name);
   int searchSongIndex(QString name);
    void removeSong(QString name);
      void appendSong(QJsonObject song);
      QString searchName();

      bool internalInsert=false;
            QJsonObject current_song;
};

#endif // SONGSMODEL_H
