#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);

  p_tab_term = new TabTerm;

  ui->tabWidget->addTab(p_tab_term, "Loader");
}

Dialog::~Dialog()
{
  delete ui;
}
