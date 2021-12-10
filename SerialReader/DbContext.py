from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
import Config
from DadosParaPlotarEntity import Base

engine = create_engine(Config.DATABASE_URI)
Session = sessionmaker(bind=engine)
s = Session()

class DbContext:

    @staticmethod
    def create_all_tables():
        Base.metadata.create_all(engine)

    @staticmethod
    def delete_all_tables():
        Base.metadata.drop_all(engine)

    @staticmethod
    def add(model):
        s.add(model)
        s.commit()
        print ('add the model')
        s.close()
        print ('close the session')