import React,{useEffect,useState} from "react";
import { useNavigate } from "react-router-dom";
import "./History.css";
import { ToastContainer, toast } from 'react-toastify';

export default function History() {
    const navigate = useNavigate();
    const handleBack = () => {
        navigate('/');
    };
    const [txs, setTxs] = useState([]);
    const getHistory =async(e)=> {
                try {
                    const response = await fetch("http://localhost:5001/history");
              
                    const data = await response.json();
                    console.log(data);
                    if (response.ok) {
                        setTxs(data.transactions);
                      console.log(txs);
                    } else {
                      toast.error(data.error || `${isLogin ? "Login" : "Sign-up"} failed.`);
                    }
                  } catch (error) {
                    toast.error(error);
                  }
    };
    useEffect(() => {
        getHistory();
    }, []);

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
                            <th className="tableHeader">Sender</th>
                            <th className="tableHeader">Reciever</th>
                            <th className="tableHeader">Description</th>
                        </tr>
                    </thead>
                    <tbody>
                        {txs.map((transaction,index) => (
                            <tr key={index} className="tableRow">
                                <td className="tableData">{transaction.date || "N/A"}</td>
                                <td className="tableData">
                                    <span className={transaction.amount < 0 ? "negativeAmount" : "positiveAmount"}>
                                        ${transaction.amount.toFixed(2)}
                                    </span>
                                </td>
                                <td className="tableData">{transaction.sender}</td>
                                <td className="tableData">{transaction.receiver}</td>
                                <td className="tableData">description</td>
                            </tr>
                        ))}
                    </tbody>
                </table>
            </div>

            <button className="backButton" onClick={handleBack}>
                Back
            </button>
        <ToastContainer/>
        </div>
    );
}
