import React from "react";
import { useNavigate } from "react-router-dom";
import "./History.css";

export default function History() {
    const navigate = useNavigate();

    const handleBack = () => {
        navigate('/');
    };

    const transactions = [
        { id: 1, date: "2024-01-01", amount: 250.00, description: "Payment to ABC Corp" },
        { id: 2, date: "2024-01-10", amount: 500.00, description: "Deposit from XYZ Ltd" },
        { id: 3, date: "2024-02-01", amount: -150.00, description: "Utility Bill Payment" },
        { id: 4, date: "2024-02-15", amount: 300.00, description: "Salary Credit" },
        { id: 5, date: "2024-03-01", amount: -75.00, description: "Grocery Purchase" },
    ];

    return (
        <div className="hiscontainer">
            <h3 className="title">Transaction History</h3>
            <p className="description">
                Here is a detailed list of your recent transactions. Stay updated with your expenses and credits.
            </p>
            
            <div className="tableContainer">
                <table className="table">
                    <thead>
                        <tr>
                            <th className="tableHeader">Date</th>
                            <th className="tableHeader">Amount</th>
                            <th className="tableHeader">Description</th>
                        </tr>
                    </thead>
                    <tbody>
                        {transactions.map((transaction) => (
                            <tr key={transaction.id} className="tableRow">
                                <td className="tableData">{transaction.date}</td>
                                <td className="tableData">
                                    <span className={transaction.amount < 0 ? "negativeAmount" : "positiveAmount"}>
                                        ${transaction.amount.toFixed(2)}
                                    </span>
                                </td>
                                <td className="tableData">{transaction.description}</td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            </div>

            <button className="backButton" onClick={handleBack}>
                Back
            </button>
        </div>
    );
}
