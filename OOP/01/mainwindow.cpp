#include "mainwindow.h"
#include "entry.h"
#include "errors.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QVector3D>
#include <QMessageBox>
#include <chrono>
#include <ctime>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

QGraphicsScene *scene;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->x_shift_input->setPlaceholderText(QString("x shift"));
    ui->y_shift_input->setPlaceholderText(QString("y shift"));
    ui->z_shift_input->setPlaceholderText(QString("z shift"));
    ui->x_centre_input->setPlaceholderText(QString("x centre"));
    ui->y_centre_input->setPlaceholderText(QString("y centre"));
    ui->z_centre_input->setPlaceholderText(QString("z centre"));
    ui->x_scale_input->setPlaceholderText(QString("x scale coef"));
    ui->y_scale_input->setPlaceholderText(QString("y scale coef"));
    ui->z_scale_input->setPlaceholderText(QString("z scale coef"));
    ui->xy_angle_input->setPlaceholderText(QString("xy flat angle"));
    ui->yz_angle_input->setPlaceholderText(QString("yz flat angle"));
    ui->zx_angle_input->setPlaceholderText(QString("zx flat angle"));
    ui->file_name_input->setPlaceholderText(QString("file name"));

    scene = new QGraphicsScene();
}

MainWindow::~MainWindow() {
    delete scene;

    delete ui;

    parametrs par;
    entry(par, FREE);
}

void MainWindow::on_shift_button_clicked() {
    parametrs par;

    par.shift.x_shift = ui->x_shift_input->text().toDouble();
    par.shift.y_shift = ui->y_shift_input->text().toDouble();
    par.shift.z_shift = ui->z_shift_input->text().toDouble();

    entry(par, SHIFT);

    draw();
}

void MainWindow::on_rotate_button_clicked() {
    parametrs par;

    par.rotate.centre.x = ui->x_centre_input->text().toDouble();
    par.rotate.centre.y = ui->y_centre_input->text().toDouble();
    par.rotate.centre.z = ui->z_centre_input->text().toDouble();

    par.rotate.xy_angle = ui->xy_angle_input->text().toDouble();
    par.rotate.yz_angle = ui->yz_angle_input->text().toDouble();
    par.rotate.zx_angle = ui->zx_angle_input->text().toDouble();

    entry(par, ROTATE);

    draw();
}

void MainWindow::on_scale_button_clicked() {
    parametrs par;

    par.scale.centre.x = ui->x_centre_input->text().toDouble();
    par.scale.centre.y = ui->y_centre_input->text().toDouble();
    par.scale.centre.z = ui->z_centre_input->text().toDouble();

    par.scale.coefs.x_scale = ui->x_scale_input->text().toDouble();
    par.scale.coefs.y_scale = ui->y_scale_input->text().toDouble();
    par.scale.coefs.z_scale = ui->z_scale_input->text().toDouble();

    entry(par, SCALE);

    draw();
}

void MainWindow::on_import_button_clicked() {
    int rc = OK;
    parametrs file;
    file.file_name = strdup(ui->file_name_input->text().toStdString().c_str());

    rc = entry(file, IMPORT);

    if (rc == WRONG_DATA) {
        char str[16] = "Wrong file data";
        print_message(str);
    }
    else if (rc == EMPTY_FIGURE) {
        char str[16] = "Empty file";
        print_message(str);
    }
    else if (rc == MEM_ERR) {
        char str[16] = "Memory error";
        print_message(str);
    }
    else if (rc == WRONG_FILE_NAME) {
        char str[16] = "Wrong file name";
        print_message(str);
    }
    else
        draw();
}

void MainWindow::on_export_button_clicked()
{
    int rc = OK;
    parametrs file;
    file.file_name = strdup(ui->file_name_input->text().toStdString().c_str());

    rc = entry(file, EXPORT);

    if (rc == WRONG_FILE_NAME) {
        char str[16] = "Wrong file name";
        print_message(str);
    }
}

void MainWindow::draw() {
    ui->graphicsView->items().clear();
    QImage image = QImage(820, 490, QImage::Format_RGB32);
    image.fill(0);
    QPainter p(&image);

    p.setBrush(QColor(0, 0, 0));
    p.setPen(QColor(200, 0, 0));

    parametrs par;
    int rc = entry(par, GET_FIG);

    if (!rc) {
        for (int i = 0; i < par.screen_fig.fig_edges.count; i++) {
            p.drawLine(par.screen_fig.fig_dots.dots[par.screen_fig.fig_edges.edges[i].dot1].x,
                       par.screen_fig.fig_dots.dots[par.screen_fig.fig_edges.edges[i].dot1].y,
                       par.screen_fig.fig_dots.dots[par.screen_fig.fig_edges.edges[i].dot2].x,
                       par.screen_fig.fig_dots.dots[par.screen_fig.fig_edges.edges[i].dot2].y);
        }

        QPixmap pixmap = QPixmap::fromImage(image);
        scene->addPixmap(pixmap);
        ui->graphicsView->setScene(scene);
    }
}

void MainWindow::print_message(char *str) {
    QMessageBox msg;
    msg.setText(QString(str));
    msg.exec();
}

void MainWindow::on_MainWindow_destroyed() {}
