# Photo Editing with Mail Sending Option

This project shows the design of a graphical interface where the user can edit various images in both color and size and save them on their computer. In addition, you will have the option of being able to share the edited image through email.  

## How does it work?

When executing the program, a button will appear on the left side with the option to open an image, where the user will select an image to be modified. Depending on the size of the image, it will receive an adjustment to be able to view it optimally on the label, preserving its pixel ratio (this taking into account the most common pixel ratios. If the image does not have any of the ratios previously described, an adjustment will be made with a default size). Once the image is displayed on the screen, the user will be able to select the region of interest of the image that she/he wishes to modify by means of the action of the mouse, thus creating an image cut from the original.Being in this label, the user will be able to select by means of other radio buttons if he/she wishes to make a color change in the image of interest (gray, HSV, etc.). Once this process is finished, the user must set the size in which the image will be saved by means of text elements (one for the length and one for the width). Once the size is selected, clicking on the save image button will save the edited image on the computer. Finally, three text elements will appear (destinatary, subject, body) where the data of the person who wants to send the image by email will be placed if desired. Each instruction carried out will be reflected in a text box at the bottom of the interface, so that the user knows if what he/she is doing is really being carried out (For example, notify when saving the image or sending the mail).

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
  
  

