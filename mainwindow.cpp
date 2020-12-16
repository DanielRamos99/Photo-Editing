#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<opencv2/core/core.hpp>
#include<opencv2/ml/ml.hpp>
#include<opencv/cv.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/video/background_segm.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/imgcodecs.hpp>

#include<QDebug>
#include<QTimer>
#include<QFileDialog>
#include<QMouseEvent>
#include<QProcess>
#include<QtConcurrent>
#include<QThread>
#include<stdlib.h>

using namespace cv;

//Variables globales
int contador = 0;
bool activacion = false;
int sizeX;
int sizeY;
int sizeXEditado;
int sizeYEditado;
QString nombreImagenGuardar;
bool guardadoValido;
QString nombreExtension;
bool enviarCorreoValido = false;
QString destinatario;
QString asunto;
QString cuerpo;

//Variables globales tipo Mat
Mat imagenOriginal;
Mat imagenOriginalEscalada;
Mat imagenEditar;
Mat imagenEditarEscalada;
Mat imagenEditada;
Mat imagenFinal;

//Variables para determinar la región de interés
//Las variables que utilizará la ROI
Point puntoOrigen;
Size tamanoROI;
//Opción automática
Point puntoOrigenClic;
Size tamanoClic;
//Variables para los dos eventos automáticos
Point clic1;
Point clic2;
int contadorClic = 0;
Rect region1;

//Función para el evento del mouse
void MainWindow::mousePressEvent(QMouseEvent *event){
    //Definiendo el origen de la imagen (0,0)
    int xLocal = event->x() - 20;
    int yLocal = event->y() - 80;

    //Validando si el evento del mouse se relizó en un área válida
    if(xLocal >= 0 && xLocal <= 399 && yLocal >= 0 && yLocal <= 299){
        qDebug() << "X local: " << xLocal << "    Y local: " << yLocal << endl;
        contadorClic++;
        if(contadorClic > 2){
            contadorClic = 1;
        }
        if(contadorClic == 1){
            //Ocurrió el primer clic
            clic1.x = xLocal;
            clic1.y = yLocal;
        }
        else if(contadorClic == 2){
            //Ocurrió el segundo clic
            clic2.x = xLocal;
            clic2.y = yLocal;
        }

        //Verificar que clic2 sea mayor a clic1
        if(clic2.x > clic1.x && clic2.y > clic1.y){
            //Definición de las coordenadas (x, y)
            puntoOrigenClic.x = clic1.x;
            puntoOrigenClic.y = clic1.y;
            //Calculando el ancho y largo a partir de las coordenadas (x, y)
            tamanoClic.width = clic2.x - clic1.x;
            tamanoClic.height = clic2.y - clic1.y;
        }
    }
}

//Función principal
void MainWindow::funcionCronometro(){
    guardadoValido = false;
    contador ++;

    //Verificar que no se esté seleccionando una imagen
    if(activacion == false){
        if(imagenOriginal.empty()){
            //Crear una matriz error
            Mat error(250, 250, CV_8UC3, Scalar(22, 222, 110));
            error.copyTo(imagenOriginal);
        }

        //Determinando dimensiones de imagenOriginal
        float largo = imagenOriginal.cols;
        float ancho = imagenOriginal.rows;
        qDebug() << "Largo: " << largo << "Ancho: " << ancho << endl;

        //Ajuste de resolución
        if(largo/ancho > 16.0/9.0 - 0.002 && largo/ancho < 16.0/9.0 + 0.002){   //16:9
            sizeX = 384;
            sizeY = 216;
        }
        else if(largo/ancho > 4.0/3.0 - 0.002 && largo/ancho < 4.0/3.0 + 0.002){    //4:3
            sizeX = 400;
            sizeY = 300;
        }
        else if(largo/ancho > 16.0/10.0 - 0.002 && largo/ancho < 16.0/10.0 + 0.002){    //16:10
            sizeX = 320;
            sizeY = 200;
        }
        else if(largo/ancho > 5.0/4.0 - 0.002 && largo/ancho < 5.0/4.0 + 0.002){    //5:4
            sizeX = 320;
            sizeY = 256;
        }
        else{               //Default
            sizeX = 400;
            sizeY = 300;
        }
        qDebug() << sizeX << " x " << sizeY << endl;
        //Ajuste por medio de la función resize()
        cv::resize(imagenOriginal, imagenOriginalEscalada, Size(sizeX, sizeY), 0, 0, INTER_LINEAR);

        //Mostrar la imgenOriginalEscalada en un etiqueta de QT
        //Convertir la imagen Mat a una imagen de QT
        QImage imagenQT = Mat2QImage(imagenOriginalEscalada);
        //Convertir la imagen de QT a un mapa de píxeles
        QPixmap imagenMapa = QPixmap::fromImage(imagenQT);
        //Limpiar el contenido de la etiqueta
        ui -> label_2 -> clear();
        //Mostrar el mapa de píxeles en la etiqueta
        ui -> label_2 -> setPixmap(imagenMapa);

        if(ui->checkBox->isChecked()){  //Editar imagen completa
            //Se proseguirá a pasar toda la imagen a una nueva etiqueta
            imagenOriginalEscalada.copyTo(imagenEditar);

            //Realizando un reajuste de resolución
            if(largo/ancho > 16.0/9.0 - 0.002 && largo/ancho < 16.0/9.0 + 0.002){   //16:9
                sizeXEditado = 256;
                sizeYEditado = 144;
            }
            else if(largo/ancho > 4.0/3.0 - 0.002 && largo/ancho < 4.0/3.0 + 0.002){    //4:3
                sizeXEditado = 320;
                sizeYEditado = 240;
            }
            else if(largo/ancho > 16.0/10.0 - 0.002 && largo/ancho < 16.0/10.0 + 0.002){    //16:10
                sizeXEditado = 320;
                sizeYEditado = 200;
            }
            else if(largo/ancho > 5.0/4.0 - 0.002 && largo/ancho < 5.0/4.0 + 0.002){    //5:4
                sizeXEditado = 320;
                sizeYEditado = 256;
            }
            else{                       //Default
                sizeXEditado = 320;
                sizeYEditado = 256;
            }
            cv::resize(imagenEditar, imagenEditarEscalada, Size(sizeXEditado, sizeYEditado), 0, 0, INTER_LINEAR);
        }
        else{
            //Se proseguirá a pasar la imagen de región de interés
            puntoOrigen.x = puntoOrigenClic.x;
            puntoOrigen.y = puntoOrigenClic.y;
            tamanoROI.width = tamanoClic.width;
            tamanoROI.height = tamanoClic.height;

            //Dibujando círculos pequeños en cada clic del evento del mouse
            circle(imagenOriginalEscalada, clic1, 5, 2, 8, 0);

            if(clic2.x > clic1.x && clic2.y > clic1.y){
                circle(imagenOriginalEscalada, clic2, 5, 2, 8, 0);
            }

            //Determinando límites para la región de interés
            int limite1 = puntoOrigen.x + tamanoROI.width;
            int limite2 = puntoOrigen.y + tamanoROI.height;

            //Validando los valores para crear la región de interés
            if(puntoOrigen.x >= 0 && puntoOrigen.x <= 399 && puntoOrigen.y >= 0 && puntoOrigen.y <= 299 && limite1 < 399 && limite2 < 299){
                region1.x = puntoOrigen.x;
                region1.y = puntoOrigen.y;
                region1.width = tamanoROI.width;
                region1.height = tamanoROI.height;

                //Se crea una imagen nueva a partir de la región de interés
                imagenEditar = imagenOriginalEscalada(region1);
                if(!imagenEditar.empty()){
                    cv::resize(imagenEditar, imagenEditarEscalada, Size(320, 256), 0, 0, INTER_LINEAR);
                }
            }
        }

        //Verificar que imagenEditarEscalada no esté vacía
        if(!imagenEditarEscalada.empty()){
            //Proceso de cambio de color por medio de la función cvtColor()
            //Esto dependerá del botón radial seleccionado
            if(ui->radioButton->isChecked()){
                imagenEditarEscalada.copyTo(imagenEditada);
            }
            else if(ui->radioButton_2->isChecked()){
                cvtColor(imagenEditarEscalada, imagenEditada, CV_BGR2GRAY); //Girs
            }
            else if(ui->radioButton_3->isChecked()){
                cvtColor(imagenEditarEscalada, imagenEditada, CV_BGR2HSV);  //HSV
            }
            else if(ui->radioButton_4->isChecked()){
                cvtColor(imagenEditarEscalada, imagenEditada, CV_BGR2Luv);  //LUV
            }
            else if(ui->radioButton_5->isChecked()){
                cvtColor(imagenEditarEscalada, imagenEditada, CV_BGR2Lab);  //LAB
            }
            else if(ui->radioButton_6->isChecked()){
                cvtColor(imagenEditarEscalada, imagenEditada, CV_BGR2HLS);  //HLS
            }
            else if(ui->radioButton_7->isChecked()){
                cvtColor(imagenEditarEscalada, imagenEditada, CV_BGR2YCrCb);    //YCRCB
            }

            //Fijando el tamaño de la imagen a guardar
            if(ui->radioButton_8->isChecked()){ //Dimensión por default
                //Se pasa la imagen en el tamaño por default
                imagenEditada.copyTo(imagenFinal);
                guardadoValido = true;      //Activando la opción de guardar imagen
            }
            else if(ui->radioButton_9->isChecked()){    //Dimensión manual
                //Se pasa la imagen a un tamaño especificado
                QString ancho = ui->lineEdit->text();
                QString largo = ui->lineEdit_2->text();
                int anchoEntero = ancho.toInt();
                int largoEntero = largo.toInt();

                //Validando que los valores colocados estén dentro del rango establecido
                if(anchoEntero > 0 && anchoEntero < 2000 && largoEntero > 0 && largoEntero < 1500){
                    cv::resize(imagenEditada, imagenFinal, Size(anchoEntero, largoEntero), 0, 0, INTER_LINEAR);
                    guardadoValido = true;      //Activando la opción de guardar imagen
                }
                else{
                    //Mensaje de error
                    if((ancho != "" && largo != "") && (anchoEntero < 0 || anchoEntero > 2000 || largoEntero < 0 || largoEntero > 1500)){
                        ui->textBrowser->setText("Las dimensiones colocadas no son válidas.");
                    }
                }
            }
            //Nombre de la imagen a guardar
            nombreImagenGuardar = ui->lineEdit_3->text();

        }

        //Mostrar la imgenEditar en un etiqueta de QT
        QImage imagenQT2 = Mat2QImage(imagenEditada);
        QPixmap imagenMapa2 = QPixmap::fromImage(imagenQT2);
        ui -> label_3 -> clear();
        ui -> label_3 -> setPixmap(imagenMapa2);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Configuración del cronómetro
    //Se crea una variable para la configuración del cronómetro
    QTimer *cronometro = new QTimer(this);
    //Cada vez que se genera la señal del cronómetro (30ms) se llamará a la función de esta ventana para que se ejecute
    connect(cronometro, SIGNAL(timeout()), this, SLOT(funcionCronometro()));
    //Se inicia el cronómetro
    cronometro -> start(30);   //Interrumpe al programa cada 30ms

}

MainWindow::~MainWindow()
{
    //Se borra la ventana
    delete ui;
}

//Función que se ejecuta al momento de presionar el botón de "Abrir Imagen"
void MainWindow::on_pushButton_clicked()
{
    activacion = true;  //Activando la variable "activacion" para interrumpir el programa y buscar la imagen
    //Ruta de la imagen (jpg o png)
    QString nombreImagen = QFileDialog::getOpenFileName(this, tr("Abrir Imagen"), "/home/daniel", tr("Images (*.jpg *.png)"));
    qDebug() << nombreImagen << endl;
    //Lyendo la imagen seleccionada
    imagenOriginal = imread(nombreImagen.toUtf8().constData());
    activacion = false;     //Regresando "activacion" a falso para proseguir con el procesamiento
}

//Función que se ejecuta al momento de presionar el botón de "Guardar Imagen"
void MainWindow::on_pushButton_2_clicked()
{
    //Validando que guardadoValido sea verdadero
    if(guardadoValido){
        //Se proseguirá a guardar la imagen

        //Se genera la ruta donde se guardará la imagen
        nombreExtension = "/home/daniel/Escritorio/" + nombreImagenGuardar + ".jpg";
        //Se guarda la imagen
        imwrite(nombreExtension.toUtf8().constData(), imagenFinal);
        ui->textBrowser->setText("La imagen se guardó correctamente.");
        guardadoValido = false;         //Regresando guardadoValido a falso
        enviarCorreoValido = true;      //Activando la opción de envío por correo electrónico
    }
}

//Función que se ejecuta al momento de presionar el botón de "Enviar Imagen"
void MainWindow::on_pushButton_3_clicked()
{
    //Validando que enviarCorreoValido sea verdadero
    if(enviarCorreoValido){
        //Se proseguirá a enviar la imagen

        //Se crea una variable de archivo contenida en correo.txt
        QFile archivo("/home/daniel/INTERFACES/QT-Proyecto/correo.txt");

        //Leyendo la información del LineEdit
        destinatario = ui->lineEdit_4->text();
        asunto = ui->lineEdit_5->text();
        cuerpo = ui->lineEdit_6->text();

        //Validando si el archivo esta abierto para escritura
        if(archivo.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream datosArchivo(&archivo);
            //Editando el contendio de archivo (correo.txt)
            datosArchivo << "To: " << destinatario << '\n' << "From: daniel.ramos.030799@gmail.com" << '\n' << "Subject: " << asunto << "\n\n" << cuerpo;
        }
        archivo.close();

        //Creando la instrucción para enviar el correo
        QString comando1 = "ssmtp ";
        comando1 += destinatario + " < ../correo.txt";

        //Enviando el correo con la información del archivo correo.txt editado
        system(comando1.toUtf8().constData());
        qDebug() << "Se envio el correo" << endl;

        //Creando la instrucción para enviar la imagen al correo
        QString comando2 = "mpack -s subject";
        comando2 += " " + nombreExtension + " " + destinatario;

        //Enviando la imagen al correo del archivo correo.txt
        system(comando2.toUtf8().constData());

        //Comprobación
        ui->textBrowser->setText("Correo enviado.");
    }
    else{
        //Mensaje de error
        ui->textBrowser->setText("La imagen no se ha guardado.");
    }
}
