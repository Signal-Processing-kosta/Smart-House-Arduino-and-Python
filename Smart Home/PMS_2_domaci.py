import serial
from threading import Timer
import time
from tkinter import *
from tkinter import ttk

try:
    serijski_port=serial.Serial('COM4',9600)    ##otvaranje serijskog porta i zapocinjanje serijske komunikacije
except serial.SerialException:
    print('Proverite port i probajte ponovo!')
    time.sleep(3)
    exit()

def alarm():         ##definicija funkcije alarm, koja salje na serijski port arudinu infromaciju da upali alarm
     print ('***Nije vam dozvoljen pristup***')
     serijski_port.write(str.encode('a'))
     time.sleep(3)
     
sifra='blabla'  
broj_pokusaja=3
tajmer=15
t=Timer(tajmer,alarm)

def unosenje_sifre(event): ##ova funkcija je vezana za dugme POTVRDI(poziva se na levi klik na dugme) 
    global sifra
    global broj_pokusaja
    sifra=entry_sifra.get() ##unosi se sifra iz entry-ja u promenljivu sifra
    entry_sifra.delete(0,'end')
    broj_pokusaja=broj_pokusaja-1
    entry_pokusaji.delete(0,'end')
    entry_pokusaji.insert(0,broj_pokusaja)
    t.cancel() ##prekida se thread jer je uneta sifra
    
root=Tk()  ##pravljenje GUI-ja
root.geometry('400x400')
root.rowconfigure(0,minsize=100)
root.rowconfigure(1,minsize=100)

label_sifra=Label(root, text='Sifra')
label_sifra.grid(row=0,column=0)
entry_sifra=Entry(root)
entry_sifra.grid(row=0,column=1)
button_potvrdi=Button(root,text='Potvrdi')
button_potvrdi.bind('<Button-1>',unosenje_sifre)
button_potvrdi.grid(row=0,column=2)
label_pokusaji=Label(root,text='preostalo pokusaja')
label_pokusaji.grid(row=2,column=1,sticky=W)
entry_pokusaji=Entry(root)
entry_pokusaji.insert(0,broj_pokusaja)
entry_pokusaji.grid(row=2,column=2)

root.update()

t=Timer(tajmer,alarm) ##zapocinje se thread koji ima ulogu da izbroji 15 sekundi 
t.start()
while (broj_pokusaja>0):
    if not t.isAlive():
        t=Timer(tajmer,alarm)
        t.start()
    root.update() ##update-uje se GUI
    if sifra=='12345': ##sifra je tacna
        print ('***Pristup dozvoljen***')
        serijski_port.write(str.encode('u')) ##salje se arduino informacija da je dozvoljen ulazak
        time.sleep(3)
        root.destroy()
        exit()
alarm() ##pali se alarm na kraju ako posle 3 pokusaja nije uneta tacna sifra
root.mainloop()
