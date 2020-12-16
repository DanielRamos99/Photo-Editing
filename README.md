# Photo Editing with Mail Sending Option

This project shows the design of a graphical interface where the user can edit various images in both color and size and save them on their computer. In addition, you will have the option of being able to share the edited image through email.  

## How does it work?

When executing the program, a button will appear on the left side with the option to open an image, where the user will select an image to be modified. Depending on the size of the image, it will receive an adjustment to be able to view it optimally on the label, preserving its pixel ratio (this taking into account the most common pixel ratios. If the image does not have any of the ratios previously described, an adjustment will be made with a default size). Once the image is displayed on the screen, the user will be able to select the region of interest of the image that she/he wishes to modify by means of the action of the mouse, thus creating an image cut from the original.Being in this label, the user will be able to select by means of other radio buttons if he/she wishes to make a color change in the image of interest (gray, HSV, etc.). Once this process is finished, the user must set the size in which the image will be saved by means of text elements (one for the length and one for the width). Once the size is selected, clicking on the save image button will save the edited image on the computer. Finally, three text elements will appear (reciever, subject, body) where the data of the person who wants to send the image by email will be placed if desired. Each instruction carried out will be reflected in a text box at the bottom of the interface, so that the user knows if what he/she is doing is really being carried out (For example, notify when saving the image or sending the mail).

## Requirements of Software and OS

* Ubuntu 18.04 or above
* Qt Creator 5.8 or above
* OpenCV 3.2 or above
* SSMTP
* MPACK

## Intsallation and Configuration of SSMTP and MPACK

SSMTP is a program capable of sending emails from a local computer to a configured mail server. It is not a mail server and it does not receive emails, nor does it expand aliases, or manage a queue, it simply sends them. One of its main uses is to forward automated email (such as system alerts) from your computer to an external email address.  

If you are on Linux, just open a terminal and place the following command line: `sudo apt-get install ssmtp`  

Once the program is downloaded, it is necessary to configure the ssmtp.conf file, where the mail from where the e-mails will be sent will be edited. For this, it is necessary to place the following command line: `sudo nano /etc/ssmtp/ssmtp.conf`  
  
```
#user is the name of your Linux user, the symbol # indicates a comment
hostname = user  

root = mail@gmail.com  

#for gmail, the next option is:  
mailhub=smtp.gmail.com:465  

#Establish the mail domain, in this case it should be gmail  
rewriteDomain = gmail.com  

AuthUser = mail@gmail.com  
AuthPass = mailPassword  

#The following option allows you to specify the "From:" address of the basic structure of an email  
FromLineOverride = YES  

UseTLS = YES
```  

The next thing is to create a text file (mail.txt) where all the information of the mail that will be sent will be saved. For this, the following command line is necessary: `gedit correo.txt &`  

```
To: Receiver
From: Sender (mail configured in ssmtp.conf)
Subject: Reason of the email
Anything here is considered the body of the email.
```  

  
MPACK is a program that allows the sending of attachments via email, so it will be used to send images. To install it, simply place the following command line: `sudo apt-get install mpack`  

## Installation of OpenCV and Qt Creator

OpenCV (Open Computer Vision Library) is a free computer vision library originally developed by Intel. It is fully developed in object-oriented C ++, just like its API, however, it has connectors available for other languages such as Python, Java, Matlab, Octave, and JavaScript. It was created under the BSD license, which allows it to be used freely for commercial and research purposes.  
To install it just place the following command line: `sudo apt install libopencv-dev`  

Qt Creator es un entorno de desarrollo integrado (IDE) multiplataforma creado para la máxima experiencia de desarrollado. Se ejecuta en sistemas operativos de escritorio Windows, Linux y macOS, y permite a los desarrolladores crear aplicaciones en plataformas de escritorio, móviles e integradas. For this reason, it can be easily downloaded [here](https://www.qt.io/download).







