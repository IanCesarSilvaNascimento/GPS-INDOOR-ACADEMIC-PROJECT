#!/usr/bin/python2.7
import serial
import time
import DbContext as db
from DadosParaPlotarEntity import DadosParaPlotar as Maquina
import datetime



db.DbContext.create_all_tables()

viagens = 0
ser = serial.Serial('COM3')


x = 200
y = 800
range_x = False
range_y = False
VALUE_SERIAL = 0

id_maquina = input('Digite a identificacao da maquina: ')
maquina = Maquina(periodo = 8, dataEHora = datetime.datetime.now(), viagens = 0, 
Id_maquina = id_maquina)


db.DbContext.add(maquina)





def range_ponto_fixo():
    global range_x
    global range_y
    if x >= 100 and x <= 300:
        print ('entrou range Coord')
        range_x = True
    else:
        range_x = False
    if y >= 700 and y <= 900:
        print ('entrou range router')
        range_y = True
    else:
        range_y = False

def ponto_de_carga():
    global viagens
    print ('range y ' + str(range_y) + ' range x: ' + str(range_x))
    if  range_y and range_x:
        maquina = db.s.query(Maquina).filter(Maquina.Id_maquina == id_maquina).first()
        maquina.viagens += 1
        db.s.commit()
        print ('viagens = ' + str(maquina.viagens))
        while range_y and range_x:
            print ('Esta na posicao')
            break

    return


while 1==1:
    VALUE_SERIAL = ser.readline(29)
       
    if b'valorx' in VALUE_SERIAL:
        x = float(VALUE_SERIAL[21:28].strip())
        print (" o valor de x é " + str(x))

    if b'valory' in VALUE_SERIAL :
        y = float(VALUE_SERIAL[21:28].strip())
        print (" o valor de y é " + str(y))

    if ( x != 0 and y != 0):
        print ('verificando range e ponto de carga')
        range_ponto_fixo()
        ponto_de_carga()
        x = 0
        y = 0




ser.close()

#while 1==1:
#    time.sleep(2)
#    if( x != 0 and y != 0):
#        print ('verificando range e ponto de carga')
#        range_ponto_fixo()
#        ponto_de_carga()
     
#x = 0
#y = 0


