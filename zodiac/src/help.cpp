#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "help.h"

HelpWidget :: HelpWidget(QWidget *parent) : QFrame(parent)
 {
  title  = new QLabel;
  slides = new SlideWidget;
  label1 = new QLabel(this);
  label2 = new QLabel(this);

  title->setObjectName("title");
  label1->setWordWrap(true);
  label2->setWordWrap(true);
  slides->addSlide(label1);
  slides->addSlide(label2);
  slides->setTransitionEffect(SlideWidget::Transition_Overlay);

  QVBoxLayout* layout = new QVBoxLayout(this);
   layout->setMargin(0);
   layout->setSpacing(0);
   layout->addWidget(title);
   layout->addWidget(slides);

  loadArticles(1024);
  clear();
 }

void HelpWidget :: loadArticles(quint16 maxArticleLength)
 {
  int loadedCount = 0;

  QDir dir("text");
  foreach(QString filename, dir.entryList(QDir::Files))
   {
    QFile f(dir.absolutePath() + '/' + filename);
    f.open(QIODevice::ReadOnly);

    Article article;
    QByteArray buf;

    while (!f.atEnd())
     {
      buf = f.readLine().trimmed();

      if (buf.startsWith('[') && buf.endsWith(']'))     // key
       {
        if (!article.key.isEmpty())            // append article to map
          articles[article.key] = article;

        article = Article();
        article.key = getTag(buf.mid(1, buf.length() - 2));
       }
      else if (article.title.isEmpty())                 // title
       {
        article.title = buf;
       }
      else if (article.content.isEmpty())              // content
       {
        if (buf.length() <= maxArticleLength)
         {
          article.content += buf + ' ';
          article.loaded = true;
          loadedCount++;
         }
        else                                    // skip content (big enougth for cache)
         {
          article.filePath = f.fileName();
          article.length = buf.trimmed().length();
          article.pos = f.pos() - buf.length();
          article.content = "...";
          article.loaded = false;
         }
       }
     }

    if (!article.key.isEmpty())            // append article to map
      articles[article.key] = article;
   }

  qDebug("HelpWidget: loaded %d/%d articles less than %d bytes", loadedCount, articles.count(), maxArticleLength);
 }

void HelpWidget :: setContent(Article& article)
 {
  if (!article.loaded)
   {
    QFile f(article.filePath);

    if (f.open(QIODevice::ReadOnly))
     {
      f.seek(article.pos);
      article.content = f.read(article.length);
      article.loaded = true;
      qDebug() << "Loaded topic" << article.title;
     }
   }

  setContent(article.title, article.content);
 }

void HelpWidget :: setContent(const QString& title, const QString& text)
 {
  setWindowTitle(title);
  this->title->setText(title);

  if (slides->currentSlide() == label1)     // switch slides
   {
    label2->setText(text);
    slides->setSlide(label2);
   }
  else
   {
    label1->setText(text);
    slides->setSlide(label1);
   }

  emit contentChanged();
 }

void HelpWidget :: clear()
 {
  setContent("", "");
 }

QString HelpWidget :: getTag(QString searchStr)
 {
  QStringList keys = searchStr.toUpper().split("+");
  keys.sort();
  return keys.join("+");
 }

void HelpWidget :: searchFor(QString key)
 {
  static QString usedKey;
  if (key == usedKey) return;
  usedKey = key;

  //qDebug() << "HelpWidget: search for" << key;

  if (key.isEmpty())
   {
    clear();
    return;
   }

  QString tag = getTag(key);

  if (articles.contains(tag))
   {
    setContent(articles[tag]);
   }
  else
   {
    QString content;
    if (key.contains('\n'))
     {
      int pos = key.indexOf('\n');
      content = key.mid(pos + 1);
      key.remove(pos, key.count() - pos);
     }

    setContent(key, content);
   }
 }

void HelpWidget :: setAlignment(Qt::Alignment a)
 {
  title->setAlignment(a);
  label1->setAlignment(a);
  label2->setAlignment(a);
 }
