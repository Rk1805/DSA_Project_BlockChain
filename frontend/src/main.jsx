import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import { BrowserRouter as Router, Route, Routes } from 'react-router-dom';
import './index.css'
import App from '../src/components/App'
import History from './components/History'
import Balance from '../src/components/Balance'
import Transaction from '../src/components/Transaction'
import Signup from './components/Signup'


createRoot(document.getElementById('root')).render(
  <StrictMode>
    <Router>
    <Routes>
    <Route path="/" element={<App/>} />
    <Route path="/history" element={<History/>} />
    <Route path="/Balance" element={<Balance/>} />
    <Route path="/Transaction" element={<Transaction/>} />
    <Route path="/signup" element={<Signup/>} />
    </Routes>
    </Router>
  </StrictMode>,
)
