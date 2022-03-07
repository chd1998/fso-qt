#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QDateTime>
#include <QPainter>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	max(0.0),
	isUpdate(true),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
//	ui->mLbImage->setStyleSheet("background-color:rgba(255,255,255,50)");
}

Widget::~Widget()
{
	delete ui;
}

void Widget::on_mPbOpen_clicked()
{
	isUpdate = true;
	max = 0;
	memset(color,0,256*sizeof(int));

	//定义文件对话框类
	QFileDialog *fileDialog = new QFileDialog(this);
	//定义文件对话框标题
	fileDialog->setWindowTitle(tr("打开图片"));
	//设置默认文件路径
	fileDialog->setDirectory(".");
	//设置文件过滤器
	fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
	//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFile
	fileDialog->setFileMode(QFileDialog::ExistingFile);
	//设置视图模式
	fileDialog->setViewMode(QFileDialog::Detail);
	//打印所有选择的文件的路径
	QStringList fileNames;
	if(fileDialog->exec())
	{
		fileNames = fileDialog->selectedFiles();
	}

	ui->mLePath->setText(fileNames.first());

	QImage img(fileNames.first());
	qDebug() << "format:" << img.format();

	if(img.format() == QImage::Format_RGB32)
	{
		for(int y = 0; y<img.height(); y++){
				QRgb * line = (QRgb *)img.scanLine(y);
				for(int x = 0; x<img.width(); x++){
					unsigned int average = 0.299*qRed(line[x]) + 0.587*qGreen(line[x]) + 0.114*qRed(line[x]);
					color[average]++;
				}
			}
	}
	else if(img.format() == QImage::Format_Grayscale8)
	{
		for(int y = 0; y<img.height(); y++){
				auto line = img.scanLine(y);
				for(int x = 0; x<img.width(); x++){
					unsigned int average = line[x];
					color[average]++;
				}
			}
	}

	for(int i=0; i<256; ++i)
	{
		if(max < color[i])
			max = color[i];
	}

	if(!img.isNull())
	{
		QPixmap pix = QPixmap::fromImage(img).scaled(ui->mLbImage->size(),Qt::AspectRatioMode::KeepAspectRatio);

		QPixmap temp(pix.size());
		temp.fill(Qt::transparent);

		QPainter p1(&temp);
		p1.setCompositionMode(QPainter::CompositionMode_Source);
		p1.drawPixmap(0, 0, pix);
		p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);

		//根据QColor中第四个参数设置透明度，此处position的取值范围是0～255
		p1.fillRect(temp.rect(), QColor(0, 0, 0, 60));
		p1.end();

		pix = temp;

		ui->mLbImage->setPixmap(pix);
	}

	update();
}

void Widget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	if(max < 0.0001f || max < 0)
		return;

	painter.drawText(QPoint(28,469),"0");

	for(int i=0; i<256; ++i)
	{
		painter.drawLine(QPoint(30+i*2,458),QPoint(30+i*2,458-color[i]/max*300));
	}

	painter.drawText(QPoint(24+255*2,469),"255");
}
