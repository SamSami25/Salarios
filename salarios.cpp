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

