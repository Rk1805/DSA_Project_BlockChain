import React from "react";
import { useNavigate } from "react-router-dom";
import "./Balance.css";

export default function Balance() {
    const navigate = useNavigate();

    const handleBack = () => {
        navigate('/');
    };

    const balanceData = {
        balance: "3.752 BTC",
        usdEquivalent: "$102,345.67",
        recentTransactions: [
            { id: 1, description: "Received BTC", amount: "+0.5 BTC", date: "2024-10-21" },
            { id: 2, description: "Sent BTC", amount: "-0.2 BTC", date: "2024-10-15" },
            { id: 3, description: "Received BTC", amount: "+1.0 BTC", date: "2024-10-10" },
        ]
    };

    return (
        <div className="balcontainer">
            <h3 className="title">Your Balance</h3>
            <div className="balanceContainer">
                <h1 className="balance">{balanceData.balance}</h1>
                <p className="usdEquivalent">Equivalent: {balanceData.usdEquivalent}</p>
            </div>

            <div className="transactionsContainer">
                <h4 className="transactionsTitle">Recent Transactions</h4>
                <ul className="transactionList">
                    {balanceData.recentTransactions.map((tx) => (
                        <li key={tx.id} className="transactionItem">
                            <span className="transactionDescription">{tx.description}</span>
                            <span className={tx.amount.startsWith('-') ? "negativeAmount" : "positiveAmount"}>{tx.amount}</span>
                            <small className="transactionDate">{tx.date}</small>
                        </li>
                    ))}
                </ul>
            </div>

            <button className="backButton" onClick={handleBack}>
                Back
            </button>
        </div>
    );
}
