#ifndef TABTERM_H
#define TABTERM_H

#include <QWidget>
#include <QProcess>
#include <QDir>


namespace Ui {
  class TabTerm;
}


class Console;

class TabTerm : public QWidget
{
  Q_OBJECT

public:
  explicit TabTerm(QWidget *parent = nullptr);
  ~TabTerm();


private slots:
  void on_pushButton_Update_clicked();

  void processStarted(void);
  void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void processOut(void);
  void processOutError(void);


  void on_Load_clicked();

private:
  Ui::TabTerm *ui;
  QProcess *process;
  QString tool_dir;
  QString bin_dir;
  QDir dir;
  int  dir_depth;
  QString ver;

  void writeData(const QByteArray &data);
  void readData(void);

  Console *console;

  void refreshPortList();
  void refreshFileList(QString dir_name);
};

#endif // TABTERM_H
