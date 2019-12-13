#pragma once
#pragma execution_character_set("utf-8")

#include <QDialog>

#include "ui_DialogItem.h"

namespace Ui {
	class DialogItem;
}

class ItemData
{
public:
	ItemData();

	int nTeacherJobNumber; //教师编号，主键，非空

	QString szTeacherName; //教师姓名
	int nTeacherGender; //教师性别
	int nTeacherAge; //教师年龄
	QString szTeacherID; //教师身份证号

	int nTeahcerEducation; //教师受教育程度
	float fTeacherSalary; //教师工资
	QString szTeacherMobile; //教师手机号

	int nTeacherDepartment; //教师学院编号
	int nTeacherMajor; //教师专业编号
	int nTeacherTitle; //教师职称

	QString szMemo; //备注
};

class DialogItem : public QDialog
{
	Q_OBJECT

public:
	explicit DialogItem(ItemData* pItemData, QWidget* parent = nullptr);
	~DialogItem();

signals:
	void sigAddItem(ItemData*);

private slots:
	void on_btnAdd_clicked();
	void on_btnCancel_clicked();
	void on_comboBoxDepartment_currentIndexChanged(int Department);
	void on_lineEditJobNumber_TextChanged(const QString& JobNumber);

public:
	static QString GenderToString(int Gender); //性别转换字符串
	static QString EducationToString(int Education); //教育程度转换字符串

	static QString DepartmentToString(int Department); //学院转换字符串	
	static QString MajorToString(int Major); //专业转换字符串
	static int StringToMajor(QString StrMajor); //专业字符串转换编号
	static QString TitleToString(int Title); //职称转换字符串

private:
	Ui::DialogItem* ui;
	ItemData* mItem;
};
