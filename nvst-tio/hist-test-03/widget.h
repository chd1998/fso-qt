#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = nullptr);
	~Widget();

private slots:
	void on_mPbOpen_clicked();

protected:
	void paintEvent(QPaintEvent*);

private:
	int color[256] = {0};
	float max;
	bool isUpdate;

private:
	Ui::Widget *ui;
};

#endif // WIDGET_H
