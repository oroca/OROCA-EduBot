#include "tabterm.h"
#include "ui_tabterm.h"
#include "console.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDir>
#include <QProcess>
#include <QFileInfoList>

TabTerm::TabTerm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TabTerm)
{
  ui->setupUi(this);


  console = new Console;
  console->setEnabled(false);
  QVBoxLayout *console_layout  =new QVBoxLayout();
  console_layout->addWidget(console);
  ui->verticalLayout->addWidget(console);
  connect(console, &Console::getData, this, &TabTerm::writeData);


  refreshPortList();

  dir_depth = 0;
  refreshFileList("bin");

  tool_dir = QDir::currentPath() + "/esptool_py/";
  bin_dir =  QDir::currentPath() + "/bin/";

  dir = bin_dir;

  process = new QProcess();
  QObject::connect( process, SIGNAL( started() ), this, SLOT( processStarted() ) );
  QObject::connect( process, SIGNAL( readyReadStandardOutput() ), this, SLOT( processOut() ) );
  QObject::connect( process, SIGNAL( readyReadStandardError() ), this, SLOT( processOutError() ) );
  QObject::connect( process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
      [=](int exitCode, QProcess::ExitStatus exitStatus){ processFinished(exitCode, exitStatus);});

}

TabTerm::~TabTerm()
{
  delete ui;
}

void TabTerm::writeData(const QByteArray &data)
{
    //serial->write(data);
}

void TabTerm::readData()
{
  QByteArray data = "a";//serial->readAll();
  console->putData(data);
}

void TabTerm::refreshPortList()
{
  ui->comboBox_PortList->clear();
  foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
  {
    QString port_name;

    port_name = serialPortInfo.portName() + "     ";
    port_name += serialPortInfo.description();

    ui->comboBox_PortList->addItem(port_name);
  }
}

void TabTerm::refreshFileList(QString dir_name)
{
  if (dir_depth == 0)
  {
    ui->comboBox_FileList->clear();
    ver = " ";
  }

  dir.cd(dir_name);

  dir.setSorting(QDir::Type | QDir::Reversed);

  QFileInfoList list = dir.entryInfoList();



  for (int i = 0; i < list.size(); ++i)
  {
    QFileInfo fileInfo = list.at(i);

    if (fileInfo.isDir())
    {
    }
    else
    {
      if (fileInfo.suffix() == "txt")
      {
        QStringList name_tbl = fileInfo.fileName().split(" ");

        ver += name_tbl[1];
      }
    }
  }


  for (int i = 0; i < list.size(); ++i)
  {
    QFileInfo fileInfo = list.at(i);

    if (fileInfo.isFile())
    {
      if (fileInfo.suffix() == "bin")
      {
        if (dir_depth > 0)
        {
          ui->comboBox_FileList->addItem(dir_name + "/" + fileInfo.fileName() + "  " + ver);
        }
        else
        {
          ui->comboBox_FileList->addItem(fileInfo.fileName() + "  " + ver);
        }
      }
    }

    if (fileInfo.isDir() && fileInfo.fileName() != "." && fileInfo.fileName() != "..")
    {
      dir_depth++;

      if (dir_depth == 1)
      {
        refreshFileList(fileInfo.fileName());
        dir.cd("..");
      }
      else
      {
        dir.cd("..");
        refreshFileList(dir_name + "/" + fileInfo.fileName());
        dir.cd("..");
      }
      dir_depth--;
    }
  }
}

void TabTerm::processStarted(void)
{
  console->putData("Started\n");
  ui->pushButton_Update->setEnabled(false);
  ui->Load->setEnabled(false);
  ui->comboBox_PortList->setEnabled(false);
  ui->comboBox_FileList->setEnabled(false);
}

void TabTerm::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
  console->putData("Finished\n");
  ui->pushButton_Update->setEnabled(true);
  ui->Load->setEnabled(true);
  ui->comboBox_PortList->setEnabled(true);
  ui->comboBox_FileList->setEnabled(true);
}

void TabTerm::processOut(void)
{
  console->putData(process->readAllStandardOutput());
}

void TabTerm::processOutError(void)
{
  console->putData(process->readAll());
}

void TabTerm::on_pushButton_Update_clicked()
{
  dir_depth = 0;
  refreshPortList();
  refreshFileList("bin");
  console->putData("Refresh Port List\n");
  console->putData("Refresh File List\n");
}

void TabTerm::on_Load_clicked()
{
  QString tool_file;
  QStringList com_name= ui->comboBox_PortList->currentText().split(" ");
  QStringList file_name= ui->comboBox_FileList->currentText().split(" ");

  tool_file  = tool_dir + "esptool.exe --chip esp32 --port ";
  tool_file += com_name[0] + " ";
  tool_file += "--baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 ";
  tool_file += tool_dir + "boot_app0.bin 0x1000 ";
  tool_file += tool_dir + "bootloader_dio_80m.bin 0x10000 ";
  tool_file += bin_dir + file_name[0] + " ";
  tool_file += "0x8000 " + tool_dir + "partitions.bin";


  console->putData(tool_file.toUtf8());

  process->setProcessChannelMode(QProcess::MergedChannels);
  process->start(tool_file);
}

