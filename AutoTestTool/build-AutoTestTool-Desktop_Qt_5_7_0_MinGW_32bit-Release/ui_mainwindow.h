/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *widget_2;
    QGroupBox *groupBox;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QLineEdit *lineEdit_mould;
    QLabel *label_11;
    QLineEdit *lineEdit_lot_code;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *lineEdit_serial_number;
    QLineEdit *lineEdit_uut_type;
    QLabel *label_6;
    QLineEdit *lineEdit_date_code;
    QLabel *label_4;
    QLineEdit *lineEdit_uut_name;
    QLabel *label_5;
    QLineEdit *lineEdit_color;
    QLabel *label_3;
    QLineEdit *lineEdit_work_order;
    QLineEdit *lineEdit_supplier;
    QLineEdit *lineEdit_cavity;
    QLabel *label_8;
    QLineEdit *lineEdit_uut_code;
    QLabel *label_21;
    QLabel *label_42;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_6;
    QLabel *label_15;
    QLineEdit *lineEdit_ate_name;
    QLabel *label_13;
    QLineEdit *lineEdit_computer_name;
    QLabel *label_14;
    QLineEdit *lineEdit_fixuer_id;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_22;
    QLineEdit *lineEdit_operator;
    QLabel *label_23;
    QLineEdit *lineEdit_operation_sequence;
    QLabel *label_24;
    QLineEdit *lineEdit_site_code;
    QLabel *label_19;
    QLineEdit *lineEdit_program_name;
    QLabel *label_20;
    QLineEdit *lineEdit_program_ver;
    QGroupBox *groupBox_10;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *lineEdit_factory;
    QLabel *label_2;
    QLineEdit *lineEdit_line;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pushButton_10;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_2;
    QLabel *label_phase_num;
    QLabel *label_44;
    QLabel *label_51;
    QLabel *label_connecting_status;
    QPushButton *pushButton_12;
    QLabel *label_out_num;
    QLabel *label_device_type;
    QLabel *label_53;
    QLabel *label_49;
    QGroupBox *groupBox_5;
    QTextEdit *textEdit;
    QLabel *label_43;
    QLineEdit *lineEdit_ip;
    QPushButton *pushButton_11;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox_11;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_16;
    QLineEdit *lineEdit_predictvol;
    QLabel *label_29;
    QSpacerItem *horizontalSpacer;
    QLabel *label_17;
    QLineEdit *lineEdit_predictcur;
    QLabel *label_31;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_3;
    QRadioButton *radioButton_phase;
    QLineEdit *lineEdit_radio_phase;
    QLabel *label_18;
    QLabel *label_38;
    QLabel *label_37;
    QLabel *label_39;
    QRadioButton *radioButton_out;
    QLineEdit *lineEdit_radio_out;
    QLabel *label_41;
    QLabel *label_40;
    QPushButton *pushButton_5;
    QPushButton *pushButton_4;
    QPushButton *pushButton_9;
    QGroupBox *groupBox_13;
    QPushButton *pushButton_7;
    QPushButton *pushButton_3;
    QPushButton *pushButton_stop;
    QPushButton *pushButton_continue;
    QSpinBox *spinBox_time;
    QLabel *label_27;
    QLabel *label_28;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1110, 742);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        widget_2->setGeometry(QRect(20, 360, 1071, 371));
        groupBox = new QGroupBox(widget_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 10, 1031, 351));
        groupBox_6 = new QGroupBox(groupBox);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(10, 70, 1011, 104));
        groupBox_6->setMinimumSize(QSize(1011, 0));
        gridLayout = new QGridLayout(groupBox_6);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_7 = new QLabel(groupBox_6);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMinimumSize(QSize(100, 0));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_7, 2, 2, 1, 1);

        lineEdit_mould = new QLineEdit(groupBox_6);
        lineEdit_mould->setObjectName(QStringLiteral("lineEdit_mould"));

        gridLayout->addWidget(lineEdit_mould, 2, 5, 1, 1);

        label_11 = new QLabel(groupBox_6);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_11, 1, 6, 1, 1);

        lineEdit_lot_code = new QLineEdit(groupBox_6);
        lineEdit_lot_code->setObjectName(QStringLiteral("lineEdit_lot_code"));

        gridLayout->addWidget(lineEdit_lot_code, 1, 5, 1, 1);

        label_9 = new QLabel(groupBox_6);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_9, 2, 4, 1, 1);

        label_10 = new QLabel(groupBox_6);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_10, 0, 6, 1, 1);

        lineEdit_serial_number = new QLineEdit(groupBox_6);
        lineEdit_serial_number->setObjectName(QStringLiteral("lineEdit_serial_number"));

        gridLayout->addWidget(lineEdit_serial_number, 1, 3, 1, 1);

        lineEdit_uut_type = new QLineEdit(groupBox_6);
        lineEdit_uut_type->setObjectName(QStringLiteral("lineEdit_uut_type"));

        gridLayout->addWidget(lineEdit_uut_type, 1, 1, 1, 1);

        label_6 = new QLabel(groupBox_6);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMinimumSize(QSize(100, 0));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 0, 2, 1, 1);

        lineEdit_date_code = new QLineEdit(groupBox_6);
        lineEdit_date_code->setObjectName(QStringLiteral("lineEdit_date_code"));

        gridLayout->addWidget(lineEdit_date_code, 0, 5, 1, 1);

        label_4 = new QLabel(groupBox_6);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMinimumSize(QSize(100, 0));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        lineEdit_uut_name = new QLineEdit(groupBox_6);
        lineEdit_uut_name->setObjectName(QStringLiteral("lineEdit_uut_name"));

        gridLayout->addWidget(lineEdit_uut_name, 2, 1, 1, 1);

        label_5 = new QLabel(groupBox_6);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMinimumSize(QSize(100, 0));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        lineEdit_color = new QLineEdit(groupBox_6);
        lineEdit_color->setObjectName(QStringLiteral("lineEdit_color"));

        gridLayout->addWidget(lineEdit_color, 1, 7, 1, 1);

        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(100, 0));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        lineEdit_work_order = new QLineEdit(groupBox_6);
        lineEdit_work_order->setObjectName(QStringLiteral("lineEdit_work_order"));

        gridLayout->addWidget(lineEdit_work_order, 0, 1, 1, 1);

        lineEdit_supplier = new QLineEdit(groupBox_6);
        lineEdit_supplier->setObjectName(QStringLiteral("lineEdit_supplier"));

        gridLayout->addWidget(lineEdit_supplier, 2, 3, 1, 1);

        lineEdit_cavity = new QLineEdit(groupBox_6);
        lineEdit_cavity->setObjectName(QStringLiteral("lineEdit_cavity"));

        gridLayout->addWidget(lineEdit_cavity, 0, 7, 1, 1);

        label_8 = new QLabel(groupBox_6);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_8, 0, 4, 1, 1);

        lineEdit_uut_code = new QLineEdit(groupBox_6);
        lineEdit_uut_code->setObjectName(QStringLiteral("lineEdit_uut_code"));

        gridLayout->addWidget(lineEdit_uut_code, 0, 3, 1, 1);

        label_21 = new QLabel(groupBox_6);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setMinimumSize(QSize(100, 0));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_21, 1, 2, 1, 1);

        label_42 = new QLabel(groupBox_6);
        label_42->setObjectName(QStringLiteral("label_42"));
        label_42->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_42, 1, 4, 1, 1);

        groupBox_7 = new QGroupBox(groupBox);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(10, 190, 1011, 104));
        groupBox_7->setMinimumSize(QSize(1011, 0));
        gridLayout_6 = new QGridLayout(groupBox_7);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        label_15 = new QLabel(groupBox_7);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setMinimumSize(QSize(100, 0));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_15, 0, 0, 1, 1);

        lineEdit_ate_name = new QLineEdit(groupBox_7);
        lineEdit_ate_name->setObjectName(QStringLiteral("lineEdit_ate_name"));
        lineEdit_ate_name->setMaximumSize(QSize(144, 16777215));

        gridLayout_6->addWidget(lineEdit_ate_name, 0, 1, 1, 1);

        label_13 = new QLabel(groupBox_7);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setMinimumSize(QSize(100, 0));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_13, 0, 2, 1, 1);

        lineEdit_computer_name = new QLineEdit(groupBox_7);
        lineEdit_computer_name->setObjectName(QStringLiteral("lineEdit_computer_name"));
        lineEdit_computer_name->setMaximumSize(QSize(144, 16777215));

        gridLayout_6->addWidget(lineEdit_computer_name, 0, 3, 1, 1);

        label_14 = new QLabel(groupBox_7);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setMinimumSize(QSize(100, 0));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_14, 0, 4, 1, 1);

        lineEdit_fixuer_id = new QLineEdit(groupBox_7);
        lineEdit_fixuer_id->setObjectName(QStringLiteral("lineEdit_fixuer_id"));
        lineEdit_fixuer_id->setMaximumSize(QSize(144, 16777215));

        gridLayout_6->addWidget(lineEdit_fixuer_id, 0, 5, 1, 1);

        label_25 = new QLabel(groupBox_7);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setMinimumSize(QSize(100, 0));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_25, 0, 6, 1, 1);

        label_26 = new QLabel(groupBox_7);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setMinimumSize(QSize(100, 0));
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_26, 0, 7, 1, 1);

        label_22 = new QLabel(groupBox_7);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setMinimumSize(QSize(100, 0));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_22, 1, 0, 1, 1);

        lineEdit_operator = new QLineEdit(groupBox_7);
        lineEdit_operator->setObjectName(QStringLiteral("lineEdit_operator"));
        lineEdit_operator->setMaximumSize(QSize(144, 16777215));

        gridLayout_6->addWidget(lineEdit_operator, 1, 1, 1, 1);

        label_23 = new QLabel(groupBox_7);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setMinimumSize(QSize(100, 0));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_23, 1, 2, 1, 1);

        lineEdit_operation_sequence = new QLineEdit(groupBox_7);
        lineEdit_operation_sequence->setObjectName(QStringLiteral("lineEdit_operation_sequence"));
        lineEdit_operation_sequence->setMaximumSize(QSize(144, 16777215));

        gridLayout_6->addWidget(lineEdit_operation_sequence, 1, 3, 1, 1);

        label_24 = new QLabel(groupBox_7);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setMinimumSize(QSize(100, 0));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_24, 1, 4, 1, 1);

        lineEdit_site_code = new QLineEdit(groupBox_7);
        lineEdit_site_code->setObjectName(QStringLiteral("lineEdit_site_code"));
        lineEdit_site_code->setMaximumSize(QSize(144, 16777215));

        gridLayout_6->addWidget(lineEdit_site_code, 1, 5, 1, 1);

        label_19 = new QLabel(groupBox_7);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setMinimumSize(QSize(100, 0));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_19, 2, 0, 1, 1);

        lineEdit_program_name = new QLineEdit(groupBox_7);
        lineEdit_program_name->setObjectName(QStringLiteral("lineEdit_program_name"));
        lineEdit_program_name->setMaximumSize(QSize(144, 16777215));

        gridLayout_6->addWidget(lineEdit_program_name, 2, 1, 1, 1);

        label_20 = new QLabel(groupBox_7);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setMinimumSize(QSize(100, 0));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_6->addWidget(label_20, 2, 2, 1, 1);

        lineEdit_program_ver = new QLineEdit(groupBox_7);
        lineEdit_program_ver->setObjectName(QStringLiteral("lineEdit_program_ver"));
        lineEdit_program_ver->setMaximumSize(QSize(144, 16777215));

        gridLayout_6->addWidget(lineEdit_program_ver, 2, 3, 1, 1);

        groupBox_10 = new QGroupBox(groupBox);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        groupBox_10->setGeometry(QRect(10, 20, 1011, 43));
        groupBox_10->setMinimumSize(QSize(1011, 0));
        horizontalLayout_4 = new QHBoxLayout(groupBox_10);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label = new QLabel(groupBox_10);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(100, 0));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label);

        lineEdit_factory = new QLineEdit(groupBox_10);
        lineEdit_factory->setObjectName(QStringLiteral("lineEdit_factory"));
        lineEdit_factory->setMaximumSize(QSize(144, 16777215));

        horizontalLayout_4->addWidget(lineEdit_factory);

        label_2 = new QLabel(groupBox_10);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(100, 0));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_2);

        lineEdit_line = new QLineEdit(groupBox_10);
        lineEdit_line->setObjectName(QStringLiteral("lineEdit_line"));
        lineEdit_line->setMaximumSize(QSize(144, 16777215));

        horizontalLayout_4->addWidget(lineEdit_line);

        horizontalSpacer_10 = new QSpacerItem(476, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);

        pushButton_10 = new QPushButton(groupBox);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(410, 320, 221, 23));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 10, 511, 341));
        gridLayout_5 = new QGridLayout(groupBox_2);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(491, 0));
        gridLayout_2 = new QGridLayout(groupBox_4);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_phase_num = new QLabel(groupBox_4);
        label_phase_num->setObjectName(QStringLiteral("label_phase_num"));
        label_phase_num->setMaximumSize(QSize(16777215, 25));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(11);
        label_phase_num->setFont(font);
        label_phase_num->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_phase_num->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_phase_num, 2, 3, 1, 1);

        label_44 = new QLabel(groupBox_4);
        label_44->setObjectName(QStringLiteral("label_44"));
        label_44->setFont(font);
        label_44->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_44, 0, 0, 1, 1);

        label_51 = new QLabel(groupBox_4);
        label_51->setObjectName(QStringLiteral("label_51"));
        label_51->setFont(font);
        label_51->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_51, 0, 2, 1, 1);

        label_connecting_status = new QLabel(groupBox_4);
        label_connecting_status->setObjectName(QStringLiteral("label_connecting_status"));
        label_connecting_status->setMaximumSize(QSize(16777215, 25));
        label_connecting_status->setFont(font);
        label_connecting_status->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_connecting_status->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_connecting_status, 2, 1, 1, 1);

        pushButton_12 = new QPushButton(groupBox_4);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));

        gridLayout_2->addWidget(pushButton_12, 3, 1, 1, 2);

        label_out_num = new QLabel(groupBox_4);
        label_out_num->setObjectName(QStringLiteral("label_out_num"));
        label_out_num->setMaximumSize(QSize(16777215, 25));
        label_out_num->setFont(font);
        label_out_num->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_out_num->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_out_num, 0, 3, 1, 1);

        label_device_type = new QLabel(groupBox_4);
        label_device_type->setObjectName(QStringLiteral("label_device_type"));
        label_device_type->setMaximumSize(QSize(16777215, 25));
        label_device_type->setFont(font);
        label_device_type->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        label_device_type->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_device_type, 0, 1, 1, 1);

        label_53 = new QLabel(groupBox_4);
        label_53->setObjectName(QStringLiteral("label_53"));
        label_53->setFont(font);
        label_53->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_53, 2, 0, 1, 1);

        label_49 = new QLabel(groupBox_4);
        label_49->setObjectName(QStringLiteral("label_49"));
        label_49->setFont(font);
        label_49->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_49, 2, 2, 1, 1);


        gridLayout_5->addWidget(groupBox_4, 1, 0, 1, 4);

        groupBox_5 = new QGroupBox(groupBox_2);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        textEdit = new QTextEdit(groupBox_5);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(10, 20, 471, 131));

        gridLayout_5->addWidget(groupBox_5, 2, 0, 1, 4);

        label_43 = new QLabel(groupBox_2);
        label_43->setObjectName(QStringLiteral("label_43"));
        label_43->setMaximumSize(QSize(61, 16777215));
        label_43->setFont(font);
        label_43->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_5->addWidget(label_43, 0, 0, 1, 1);

        lineEdit_ip = new QLineEdit(groupBox_2);
        lineEdit_ip->setObjectName(QStringLiteral("lineEdit_ip"));
        lineEdit_ip->setMaximumSize(QSize(150, 16777215));

        gridLayout_5->addWidget(lineEdit_ip, 0, 1, 1, 1);

        pushButton_11 = new QPushButton(groupBox_2);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setMaximumSize(QSize(61, 16777215));

        gridLayout_5->addWidget(pushButton_11, 0, 2, 1, 1);

        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(560, 10, 521, 341));
        groupBox_3->setMinimumSize(QSize(0, 341));
        gridLayout_4 = new QGridLayout(groupBox_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        groupBox_11 = new QGroupBox(groupBox_3);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setMinimumSize(QSize(501, 0));
        horizontalLayout_5 = new QHBoxLayout(groupBox_11);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_16 = new QLabel(groupBox_11);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setFont(font);
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_16);

        lineEdit_predictvol = new QLineEdit(groupBox_11);
        lineEdit_predictvol->setObjectName(QStringLiteral("lineEdit_predictvol"));

        horizontalLayout_5->addWidget(lineEdit_predictvol);

        label_29 = new QLabel(groupBox_11);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setFont(font);
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_29);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        label_17 = new QLabel(groupBox_11);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setFont(font);
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_17);

        lineEdit_predictcur = new QLineEdit(groupBox_11);
        lineEdit_predictcur->setObjectName(QStringLiteral("lineEdit_predictcur"));

        horizontalLayout_5->addWidget(lineEdit_predictcur);

        label_31 = new QLabel(groupBox_11);
        label_31->setObjectName(QStringLiteral("label_31"));
        label_31->setFont(font);
        label_31->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_31);


        gridLayout_4->addWidget(groupBox_11, 0, 0, 1, 2);

        groupBox_12 = new QGroupBox(groupBox_3);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setMinimumSize(QSize(501, 0));
        gridLayout_3 = new QGridLayout(groupBox_12);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        radioButton_phase = new QRadioButton(groupBox_12);
        radioButton_phase->setObjectName(QStringLiteral("radioButton_phase"));
        radioButton_phase->setChecked(true);

        gridLayout_3->addWidget(radioButton_phase, 0, 0, 1, 1);

        lineEdit_radio_phase = new QLineEdit(groupBox_12);
        lineEdit_radio_phase->setObjectName(QStringLiteral("lineEdit_radio_phase"));
        lineEdit_radio_phase->setMinimumSize(QSize(25, 0));
        lineEdit_radio_phase->setMaximumSize(QSize(61, 16777215));

        gridLayout_3->addWidget(lineEdit_radio_phase, 0, 1, 1, 1);

        label_18 = new QLabel(groupBox_12);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setFont(font);
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_18, 0, 2, 1, 1);

        label_38 = new QLabel(groupBox_12);
        label_38->setObjectName(QStringLiteral("label_38"));
        label_38->setMinimumSize(QSize(61, 0));
        label_38->setStyleSheet(QStringLiteral("background-color: rgb(85, 255, 0);"));
        label_38->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_38, 0, 3, 1, 1);

        label_37 = new QLabel(groupBox_12);
        label_37->setObjectName(QStringLiteral("label_37"));
        label_37->setFont(font);
        label_37->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_37, 0, 4, 1, 1);

        label_39 = new QLabel(groupBox_12);
        label_39->setObjectName(QStringLiteral("label_39"));
        label_39->setMinimumSize(QSize(61, 0));
        label_39->setStyleSheet(QStringLiteral("background-color: rgb(85, 255, 0);"));
        label_39->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_39, 0, 5, 1, 1);

        radioButton_out = new QRadioButton(groupBox_12);
        radioButton_out->setObjectName(QStringLiteral("radioButton_out"));

        gridLayout_3->addWidget(radioButton_out, 1, 0, 1, 1);

        lineEdit_radio_out = new QLineEdit(groupBox_12);
        lineEdit_radio_out->setObjectName(QStringLiteral("lineEdit_radio_out"));
        lineEdit_radio_out->setMinimumSize(QSize(25, 0));
        lineEdit_radio_out->setMaximumSize(QSize(61, 16777215));

        gridLayout_3->addWidget(lineEdit_radio_out, 1, 1, 1, 1);

        label_41 = new QLabel(groupBox_12);
        label_41->setObjectName(QStringLiteral("label_41"));
        label_41->setFont(font);
        label_41->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_41, 1, 2, 1, 1);

        label_40 = new QLabel(groupBox_12);
        label_40->setObjectName(QStringLiteral("label_40"));
        label_40->setMinimumSize(QSize(61, 0));
        label_40->setStyleSheet(QStringLiteral("background-color: rgb(85, 255, 0);"));
        label_40->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_40, 1, 3, 1, 1);


        gridLayout_4->addWidget(groupBox_12, 1, 0, 1, 2);

        pushButton_5 = new QPushButton(groupBox_3);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        gridLayout_4->addWidget(pushButton_5, 3, 0, 1, 1);

        pushButton_4 = new QPushButton(groupBox_3);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout_4->addWidget(pushButton_4, 3, 1, 1, 1);

        pushButton_9 = new QPushButton(groupBox_3);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        gridLayout_4->addWidget(pushButton_9, 4, 0, 1, 1);

        groupBox_13 = new QGroupBox(groupBox_3);
        groupBox_13->setObjectName(QStringLiteral("groupBox_13"));
        groupBox_13->setMinimumSize(QSize(501, 0));
        pushButton_7 = new QPushButton(groupBox_13);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(210, 30, 75, 23));
        pushButton_3 = new QPushButton(groupBox_13);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(340, 30, 75, 23));
        pushButton_stop = new QPushButton(groupBox_13);
        pushButton_stop->setObjectName(QStringLiteral("pushButton_stop"));
        pushButton_stop->setGeometry(QRect(340, 30, 75, 23));
        pushButton_continue = new QPushButton(groupBox_13);
        pushButton_continue->setObjectName(QStringLiteral("pushButton_continue"));
        pushButton_continue->setGeometry(QRect(340, 30, 75, 23));
        spinBox_time = new QSpinBox(groupBox_13);
        spinBox_time->setObjectName(QStringLiteral("spinBox_time"));
        spinBox_time->setGeometry(QRect(100, 30, 41, 22));
        spinBox_time->setMinimum(1);
        spinBox_time->setMaximum(60);
        label_27 = new QLabel(groupBox_13);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(20, 30, 81, 20));
        label_27->setFont(font);
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_28 = new QLabel(groupBox_13);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(140, 30, 16, 20));
        label_28->setFont(font);
        label_28->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        pushButton_continue->raise();
        pushButton_7->raise();
        pushButton_stop->raise();
        pushButton_3->raise();
        spinBox_time->raise();
        label_27->raise();
        label_28->raise();

        gridLayout_4->addWidget(groupBox_13, 2, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\214\226\346\265\213\350\257\225\345\267\245\345\205\267", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "\350\257\246\347\273\206\344\277\241\346\201\257", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "\350\242\253\346\265\213\345\257\271\350\261\241\345\237\272\346\234\254\344\277\241\346\201\257", 0));
        label_7->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\344\276\233\345\272\224\345\225\206\357\274\232", 0));
        label_11->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\351\242\234\350\211\262\357\274\232", 0));
        label_9->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\346\250\241\345\205\267\357\274\232", 0));
        label_10->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\346\250\241\347\251\264\357\274\232", 0));
        label_6->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\347\274\226\347\240\201\357\274\232", 0));
        label_4->setText(QApplication::translate("MainWindow", "\350\242\253\346\265\213\345\257\271\350\261\241\347\261\273\345\236\213\357\274\232", 0));
        label_5->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\345\220\215\347\247\260\357\274\232", 0));
        label_3->setText(QApplication::translate("MainWindow", "\350\256\242\345\215\225\345\217\267\357\274\232", 0));
        label_8->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\346\211\271\346\254\241date_code\357\274\232", 0));
        label_21->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\346\235\241\347\240\201\357\274\232", 0));
        label_42->setText(QApplication::translate("MainWindow", "\347\211\251\346\226\231\346\211\271\346\254\241lot_code\357\274\232", 0));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "\346\265\213\350\257\225\350\256\276\345\244\207\344\277\241\346\201\257", 0));
        label_15->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\350\256\276\345\244\207\345\220\215\357\274\232", 0));
        label_13->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\344\270\273\346\234\272\345\220\215\357\274\232", 0));
        label_14->setText(QApplication::translate("MainWindow", "\345\244\271\345\205\267\345\224\257\344\270\200\346\240\207\350\257\206\357\274\232", 0));
        label_25->setText(QString());
        label_26->setText(QString());
        label_22->setText(QApplication::translate("MainWindow", "\346\223\215\344\275\234\345\221\230\357\274\232", 0));
        label_23->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\345\267\245\345\272\217\357\274\232", 0));
        label_24->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\345\267\245\347\253\231\357\274\232", 0));
        label_19->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\347\250\213\345\272\217\345\220\215\347\247\260\357\274\232", 0));
        lineEdit_program_name->setText(QApplication::translate("MainWindow", "clevermanager", 0));
        label_20->setText(QApplication::translate("MainWindow", "\346\265\213\350\257\225\347\250\213\345\272\217\347\211\210\346\234\254\357\274\232", 0));
        groupBox_10->setTitle(QString());
        label->setText(QApplication::translate("MainWindow", "\345\216\202\345\214\272\357\274\232", 0));
        lineEdit_factory->setText(QApplication::translate("MainWindow", "clever", 0));
        label_2->setText(QApplication::translate("MainWindow", "\345\212\240\345\267\245\347\272\277\344\275\223\357\274\232", 0));
        pushButton_10->setText(QApplication::translate("MainWindow", "\350\276\223\345\205\245\344\277\241\346\201\257\346\217\220\344\272\244", 0));
        groupBox_2->setTitle(QString());
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\350\256\276\345\244\207\344\277\241\346\201\257", 0));
        label_phase_num->setText(QString());
        label_44->setText(QApplication::translate("MainWindow", "\350\256\276\345\244\207\347\261\273\345\236\213\357\274\232", 0));
        label_51->setText(QApplication::translate("MainWindow", "\350\276\223\345\207\272\344\275\215\357\274\232", 0));
        label_connecting_status->setText(QApplication::translate("MainWindow", "\346\227\240\350\277\236\346\216\245", 0));
        pushButton_12->setText(QApplication::translate("MainWindow", "\346\226\255\345\274\200", 0));
        label_out_num->setText(QString());
        label_device_type->setText(QString());
        label_53->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245\347\212\266\346\200\201\357\274\232", 0));
        label_49->setText(QApplication::translate("MainWindow", "\350\276\223\345\205\245\347\233\270\357\274\232", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "\346\265\213\350\257\225\346\203\205\345\206\265", 0));
        label_43->setText(QApplication::translate("MainWindow", "IP\345\234\260\345\235\200\357\274\232", 0));
        lineEdit_ip->setText(QString());
        pushButton_11->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\346\220\234\347\264\242", 0));
        groupBox_3->setTitle(QString());
        groupBox_11->setTitle(QString());
        label_16->setText(QApplication::translate("MainWindow", "\351\242\204\346\265\213\347\224\265\345\216\213\357\274\232", 0));
        lineEdit_predictvol->setText(QApplication::translate("MainWindow", "220", 0));
        label_29->setText(QApplication::translate("MainWindow", "V", 0));
        label_17->setText(QApplication::translate("MainWindow", "\351\242\204\346\265\213\347\224\265\346\265\201\357\274\232", 0));
        lineEdit_predictcur->setText(QApplication::translate("MainWindow", "3", 0));
        label_31->setText(QApplication::translate("MainWindow", "A", 0));
        groupBox_12->setTitle(QString());
        radioButton_phase->setText(QApplication::translate("MainWindow", "\350\276\223\345\205\245\347\233\270", 0));
        lineEdit_radio_phase->setText(QApplication::translate("MainWindow", "1", 0));
        label_18->setText(QApplication::translate("MainWindow", "\347\224\265\345\216\213\357\274\232", 0));
        label_38->setText(QString());
        label_37->setText(QApplication::translate("MainWindow", "\347\224\265\346\265\201\357\274\232", 0));
        label_39->setText(QString());
        radioButton_out->setText(QApplication::translate("MainWindow", "\350\276\223\345\207\272\344\275\215", 0));
        lineEdit_radio_out->setText(QApplication::translate("MainWindow", "1", 0));
        label_41->setText(QApplication::translate("MainWindow", "\347\224\265\346\265\201\357\274\232", 0));
        label_40->setText(QString());
        pushButton_5->setText(QApplication::translate("MainWindow", "\345\257\274\345\207\272", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "\345\257\274\345\205\245", 0));
        pushButton_9->setText(QApplication::translate("MainWindow", "\350\257\246\347\273\206\344\277\241\346\201\257", 0));
        groupBox_13->setTitle(QString());
        pushButton_7->setText(QApplication::translate("MainWindow", "\345\215\225\345\205\203\346\265\213\350\257\225", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "\344\270\200\351\224\256\346\265\213\350\257\225", 0));
        pushButton_stop->setText(QApplication::translate("MainWindow", "\346\232\202\345\201\234", 0));
        pushButton_continue->setText(QApplication::translate("MainWindow", "\347\273\247\347\273\255", 0));
        label_27->setText(QApplication::translate("MainWindow", "\346\227\266\351\227\264\351\227\264\351\232\224\357\274\232", 0));
        label_28->setText(QApplication::translate("MainWindow", "S", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
