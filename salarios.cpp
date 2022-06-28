#include "salarios.h"
#include "ui_salarios.h"
#include "obrero.h"
#include "controlador.h"

#include <QDebug>

Salarios::Salarios(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Salarios)
{
    ui->setupUi(this);
    m_controlador = new Controlador();
}

Salarios::~Salarios()
{
    delete ui;
}


void Salarios::on_BtnCalcular_clicked()
{
    //Obrero modelo("Juan",25,TipoJornada::Matutina);
    //qDebug() << modelo.toString();

    //Obtener Datos de GUI

    QString nombre = ui->inNombre->text();
        int horas = ui->inHoras->value();
        TipoJornada jornada;
        if(ui->inMatutina->isChecked()){
            jornada = TipoJornada::Matutina;
        }else if (ui->inVespertina->isChecked()){
            jornada = TipoJornada::Vespertina;
        }else{
            jornada = TipoJornada::Nocturna;
        }

     // Validacion de datos
        if(nombre == "" || horas == 0){
            QMessageBox::warning(this,"Advertencia","El nombre y/o el número de horas es incorrecto");
            return;
        }

    //Establecer Datos Al Controlador
    m_controlador->setDatos(nombre, horas, jornada);

    //Calcular Salarios
    if(m_controlador->calcular()){
        // Muestra los Resultados
        ui->outDatos->appendPlainText(m_controlador->getDatos());
    }else{
        QMessageBox::critical(
                this,
                "ERROR",
                "No se puede calcular el salario.");
    }

    // Mostrar Mensaje en la Barra de Estado
    ui->statusbar->showMessage("Salario de " + nombre + "calculado. ", 5000);
}

void Salarios::limpiar()
{
    // Limpiar Widgets
    ui->inNombre->setText(" ");
    ui->inHoras->setValue(0);
    ui->inMatutina->setChecked(true);

    // Widgets que tenga el foco
    ui->inNombre->setFocus(); 
}

void Salarios::calcular()
{
    // Obtener datos de la GUI
    QString nombre = ui->inNombre->text();
    int horas = ui->inHoras->value();
    TipoJornada jornada;
    if(ui->inMatutina->isChecked()){
        jornada = TipoJornada::Matituna;
    }else if (ui->inVespertina->isChecked()){
        jornada = TipoJornada::Vespertina;
    }else{
        jornada = TipoJornada::Nocturna;
    }
    // Validacion de datos
    if(nombre == "" || horas == 0){
        QMessageBox::warning(this,"Advertencia","El Nombre y/o el Número de Horas es Incorrecto");
        return;
    }

    // Establecer datos al controlador
    m_contolador->setDatos(nombre, horas, jornada);
    // Calcular salarios
    if (m_contolador->calcular()) {
        // Muestra los resultados
        ui->outCalculos->appendPlainText(m_contolador->getDatos());
    } else {
        QMessageBox::critical(
                    this,
                    "Error",
                    "No se puede Calcular el Salario.");
    }
    // Limpiar widgets
    limpiar();
    // Mostrar mensaje en la barra de estado
    ui->statusbar->showMessage("Salario de " + nombre + " calculado.",5000);

}


void Salarios::on_actionCalcular_triggered()
{
    calcular();
}


void Salarios::on_actionSalir_triggered()
{
    if(ui->outCalculos->toPlainText().isEmpty()){
        this->close();
    }
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Salir","¿Desea guardar los cambios realizados?",QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);

    if(reply == QMessageBox::Save){
        on_actionGuardar_triggered();
    }
    if(reply == QMessageBox::Cancel){
        return;
    }
    if(reply == QMessageBox::No){
        QApplication::quit();
    }
}


void Salarios::on_actionGuardar_triggered()
{
    // Abrir un cuadro de diálogo para seleccionar el path y archivo a guardar
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                   "Guardar Calculos de Salarios",
                                                   QDir::home().absolutePath() + "/salarios.txt",
                                                   "Archivos de texto (*.txt)");
    // Crear un objeto File o un Archivo
    QFile archivo(nombreArchivo);
    // Tartar de abrir para escritura
    if(archivo.open(QFile::WriteOnly | QFile::Truncate)){
        // cRear un objeto 'stream' de texto
        QTextStream salida(&archivo);
        // Enviar los datos del resultado a la salida
        salida << ui->outCalculos->toPlainText();
        // Mostrar mensaje en la barra de estados
        ui->statusbar->showMessage("Datos guardados en: " + nombreArchivo, 5000);
        // Cerrar el archivo
        archivo.close();
    }else {
        // Mensaje de error
        QMessageBox::warning(this,
                             "Guardar archivo",
                             "No se puede acceder al archivo para guardar los datos.");
    }


}


void Salarios::on_actionAcerca_de_triggered()
{
    // Crear un objeto del cuadro de diálogo
    Acerca *dialog = new Acerca(this);
    // Enviar datos a la otra ventana
    dialog->setVersion(VERSION);
    // Mostrar la venta en modo MODAL
    dialog->exec();
    // Luego de cerrar la ventana, se acceden a los datos de la misma
    qDebug() << dialog->valor();

}

void Salarios::on_actionAbrir_triggered()
{
    QFile arch;
    QTextStream io;
    QString nombreArch;
    nombreArch = QFileDialog::getOpenFileName(this, "Abrir");
    arch.setFileName(nombreArch);
    arch.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!arch.isOpen()){
        QMessageBox::critical(this, "Error !!!", arch.errorString());
        return;
    }
    io.setDevice(&arch);
    ui->outCalculos->setPlainText(io.readAll());
    arch.flush();
    arch.close();
}


