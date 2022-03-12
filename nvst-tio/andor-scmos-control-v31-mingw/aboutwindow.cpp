#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QSettings>
#include <windows.h>
#include <QFileInfo>

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);
    setWindowTitle("About");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->verticalLayout->setAlignment(Qt::AlignCenter);
    ui->horizontalLayout->setAlignment(Qt::AlignHCenter);
    QPixmap ico = QPixmap(":/assets/fso-01.png");
    //ico.scaled(ui->icon->width(), ui->icon->height(), Qt::KeepAspectRatio);
    //ui->icon->setPixmap(ico);
    //ui->icon->setScaledContents(true);
    //ui->icon->setMargin(16);
    ico=ico.scaled(300,160,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->icon->setPixmap(ico);
    ui->appName->setAlignment(Qt::AlignHCenter);
    ui->appName->setText("<h1>Andor sCMOS Control</h1>");
    ui->version->setAlignment(Qt::AlignHCenter);
    QString proFullPath=QCoreApplication::applicationFilePath();
    QFileInfo tmpinfo(proFullPath);
    QString proName=tmpinfo.fileName();
    QStringList tmplist=proName.split(".");
    ui->version->setText("<h2>"+tmplist[0]+"</h2>");
    //ui->description->setAlignment(Qt::AlignHCenter);
    //ui->description->setWordWrap(true);
    //ui->description->setOpenExternalLinks(true);
    ui->description->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    ui->description->setAlignment(Qt::AlignCenter);
    ui->description->setReadOnly(true);
    ui->description->append("A GUI application for Andor sCMOS, based on Qt6.20+OpenCV 4.52(MinGW64)");
    ui->description->setAlignment(Qt::AlignLeft);
    ui->description->append("Origianl Information can be found here:");
    ui->description->append("  https://github.com/xinhangliu/andor-scmos-control");
    ui->description->append(" ");
    ui->description->append("Modified by Chen Dong @fso for high speed solar observation.");
    ui->description->append("Release Date : 2021.07.01");
    ui->description->append(" ");
    ui->description->append("System Info : ");
    ui->description->setAlignment(Qt::AlignLeft);
    //get cpu info
    QSettings *CPU = new QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",QSettings::NativeFormat);
    QString m_cpuDescribe = " CPU : "+CPU->value("ProcessorNameString").toString();
    delete CPU;

    //get os info
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL bIsWow64 = FALSE;
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    if (NULL != fnIsWow64Process)
    {
        fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
    }
    QString sysBit = "unknown";
    if(!bIsWow64)
        sysBit = "64bits";
    else
        sysBit = "32bits";
    QString m_osDescribe = "   OS : "+QSysInfo::prettyProductName() + " " + sysBit;

    //get mem info
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);
    float m_totalMem = statex.ullTotalPhys/1000000000;
    float m_freeMem = statex.ullAvailPhys/1000000000;
    QString m_memDescribe = QString("Mem : %1 GB/%2 GB" ).arg(QString::asprintf("%.2f", m_freeMem)).arg(QString::asprintf("%.2f", m_totalMem));
    //display info
    ui->description->append(m_osDescribe);
    ui->description->append(m_cpuDescribe);
    ui->description->append(m_memDescribe);
    //ui->description->setAlignment(Qt::AlignLeft);
    ui->description->append(" ");
}

AboutWindow::~AboutWindow() {
    delete ui;
}
