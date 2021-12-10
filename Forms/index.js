import React from 'react';
import ReactDOM from 'react-dom';
import App from './App';
import Home from './Home';
import { BrowserRouter as Router, Switch, Route} from 'react-router-dom';
import './index.css';

ReactDOM.render(
  <Router>
    <Switch>
      <Route path='/' exact={true} component={Home} />
      <Route path='/App' component={App}/>
    </Switch>
  </Router>,
  document.getElementById('root')
);

