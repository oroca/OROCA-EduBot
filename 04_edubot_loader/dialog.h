#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "tabterm.h"


namespace Ui {
  class Dialog;
}

class Dialog : public QDialog
{
  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = nullptr);
  ~Dialog();

private:
  Ui::Dialog *ui;

  TabTerm *p_tab_term;

};

#endif // DIALOG_H
