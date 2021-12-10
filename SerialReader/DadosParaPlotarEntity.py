from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String, Text, DateTime, Float, Boolean, PickleType


Base = declarative_base()

class DadosParaPlotar(Base):
    
    __tablename__="DadosParaPlotar"
    id = Column(Integer, primary_key = True)
    periodo = Column(Integer, nullable = False)
    dataEHora = Column(DateTime, nullable = False)
    viagens = Column(Integer, nullable = False)
    Id_maquina = Column(String, nullable = False)

    def __repr__(self):
        return "<DadosParaPlotar(periodo= '{}', dataEHora= '{}', viagens= '{}', Id_maquina= '{}')>"\
            .format(self.periodo, self.dataEHora, self.viagens, self.Id_maquina)

