import React, { useState, useEffect } from "react";
import { useNavigate } from "react-router-dom";
import { CreditCard, History, LogOut, User, Wallet } from "lucide-react";
import './App.css';

export default function HomePage() {
  const [isProfileOpen, setIsProfileOpen] = useState(false);
  const navigate = useNavigate();

  const TransactionHistory = () => navigate('/history');
  const Balance = () => navigate('/balance');
  const Transaction = () => navigate('/transaction');

  useEffect(()=>{
    const checkToken= ()=> {
      const token=localStorage.getItem("id")
      if (!token) {
        navigate("/signup");
        
      }
    }
    checkToken();
  },[navigate]);

  const Logout =()=> {
    localStorage.removeItem("id");
    navigate('/signup');
  }
  const UpdateChain=async(e) => {
    const res = await fetch("http://localhost:5001/broadcast",{
            method:"POST",
            headers : { "Content-Type": "application/json" },
            body: JSON.stringify({message:"update"}),
        });
        const data1=await res.json();
        console.log(data1);
  }
  UpdateChain();
  return (
    <div className="container">
      <header className="header">
        <h1 className="title">BlockChain App</h1>
        <div>
          <button
            className="profile-button"
            onClick={() => setIsProfileOpen(!isProfileOpen)}
          >
            <User size={24}/>
          </button>
          {isProfileOpen && (
            <div className="dropdown">
              <button className="logout-button" onClick={Logout}>
                <LogOut size={16} className="logout-icon" />
                Logout
              </button>
            </div>
          )}
        </div>
      </header>

      <main className="main">
        <div className="card-container">
          <Card 
            title="Transaction History" 
            icon={<History size={40} className="card-icon" />} 
            onClick={TransactionHistory}
          >
            Check your recent blockchain transactions
          </Card>
          <Card 
            title="Make Transaction" 
            icon={<CreditCard size={40} className="card-icon" />} 
            onClick={Transaction}
          >
            Initiate a new blockchain transaction
          </Card>
          <Card 
            title="Check Balance" 
            icon={<Wallet size={40} className="card-icon" />} 
            onClick={Balance}
          >
            View your current blockchain balance
          </Card>
        </div>
      </main>

      <footer className="footer">
        © 2024 BlockChain App. All rights reserved.
      </footer>
    </div>
  );
}

function Card({ title, icon, children, onClick }) {
  return (
    <div className="card" onClick={onClick}>
      <div className="card-header">
        {icon}
        <p className="card-title">{title}</p>
      </div>
      <p>{children}</p>
      <button className="btn">{title}</button>
    </div>
  );
}
