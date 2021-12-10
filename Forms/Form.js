import React, { Component } from 'react';
import Planta from './imagem_tcc.png';
import './Form.css';


class Form extends Component {

    constructor(props){
        super(props);

        this.stateInicial = {
            nome: '',
            status_aplicacao: false,
            maquina: [],
            cor: 'rgb(220, 0 , 0)'
        };
        this.timer1 = null;

        this.timer2 = null;

        this.timer3 = null;

        this.stateHistory = this.stateInicial;

        this.state = this.stateInicial;
    }

    inputListener = e => {
        const {value} = e.target;
        this.setState({
            nome: value
        });
    };

    

    submit = (e) => {
        e.preventDefault();
        console.log('entrou aqui');
        const id_maquina = this.state.nome;
        fetch(`http://localhost:5000/Arduino/DadosEmpilhadeira?id_maquina=${id_maquina}`)
            .then(res => res.json())
            .then(json => this.setState({maquina: json}));
        this.setState({status_aplicacao: true});
        setTimeout(() => console.log("Delay"), 2000);
        this.componentDidMount();
    };

    // componentDidUpdate(prevProps, prevState){
    //     const id_maquina = this.state.nome;
    //     console.log("entrou no loop");
    //     setInterval(() => {
    //         fetch(`http://localhost:5000/Arduino/DadosEmpilhadeira?id_maquina=${id_maquina}`)
    //             .then(res => res.json())
    //             .then(json => this.setState({maquina: json}));
    //     }, 10000);
    //     if(prevState.maquina.viagens !== this.state.maquina.viagens){
    //         setInterval(this.setState({cor: 'rgb(0,100,0)'}), 2000);
    //     }
    //     setTimeout(() => {
    //         this.setState({cor: 'rgb(220,0,0)'});
    //     }, 2000);
    // }

    componentDidUpdate(prevProps, prevState) {
        if (prevState.maquina.viagens !== this.state.maquina.viagens) {
          setTimeout(() => {
            this.setState({ cor: "rgb(0,100,0)" });
          }, 2000);
        }
        setTimeout(() => {
            this.setState({cor: "rgb(255,0,0)"});
          }, 9000);
    }

    polling(id_maquina){
        fetch(`http://localhost:5000/Arduino/DadosEmpilhadeira?id_maquina=${id_maquina}`)
            .then(res => res.json())
            .then(json => this.setState({maquina: json}));
    }

    // componentDidMount(){
    //     if(this.state.status_aplicacao === true){
    //         console.log("Entrou no loop");
    //         const id_maquina = this.state.nome;
        
    //         this.stateHistory = this.state;
    //         this.timer1 = setInterval(this.polling(id_maquina), 10000);
    //         if(this.stateHistory.maquina.viagens !== this.state.maquina.viagens){
    //             this.timer2 = setInterval(this.setState({cor: 'rgb(0,100,0)'}), 2000);
    //         }
    //         this.timer3 = setInterval(() => {
    //             this.setState({cor: 'rgb(220,0,0)'});
    //         }, 2000);
    //     }
    // }


    componentDidMount(){
        const id_maquina = this.state.nome;
        if(id_maquina !== '')
        this.intervalID = setInterval(() => {
            fetch(`http://localhost:5000/Arduino/DadosEmpilhadeira?id_maquina=${id_maquina}`)
                .then(res => res.json())
                .then(json => this.setState({maquina: json}));
        }, 10000);
        else{

        }
    }
    
    componentWillUnmount(){
            clearInterval(this.intervalID);
    }


    render(){
        const {nome, status_aplicacao} = this.state;
        return(
            <div className="row" id="wrapper">
                <form className="col s3" onSubmit={e => this.submit(e)}>
                    <div className="row">
                        <div className="input-field col s6">
                            <input value={nome} id="first_name2" type="text" className="validate" disabled={status_aplicacao} 
                            onChange = {this.inputListener}/>
                            <label className="active" htmlFor="first_name2">Id Maquina</label>
                            <button type="submit">Enviar</button>
                        </div>
                    </div>
                </form>
                <div class="col s9">
                    <div class="col s6">
                        <div class="card-panel card medium blue">
                            <span class="white-text">
                                <table>
                                    <thead>
                                    <tr>
                                        <th>Id da Maquina</th>
                                        <th>Viagens</th>
                                        <th>Periodo</th>
                                        {/* <th>Dia</th> */}
                                    </tr>
                                    </thead>

                                    <tbody>
                                    <tr>
                                        <td>{this.state.maquina.id_maquina}</td>
                                        <td>{this.state.maquina.viagens}</td>
                                        <td>{this.state.maquina.periodo}</td>
                                        {/* <td>{this.state.dia}</td> */}
                                    </tr>
                                    </tbody>
                                </table>
                            </span>
                        </div>
                    </div>
                    <div class="card-image card large col s6">
                        <span className="atualizaDados" style={{color: this.state.cor}}>&#9679;</span>
                        <img src={Planta} alt="Imagem não encontrada"/>
                        {/* <span class="card-title">Card Title</span> */}
                        {/*  rgb(0,100,0) cor verde */}
                    </div>
                </div>
            </div>
            
        );
    }
}

export default Form;