import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import "./Transaction.css";
import {toast,ToastContainer} from "react-toastify";
import fs from "fs";

export default function Transaction() {
    const navigate = useNavigate();

    const handleBack = () => {
        navigate('/');
    };
    const [transaction, setTransaction] = useState({
        to: "",
        amount: "",
        message: "",
    });

    const handleChange = (e) => {
        const { name, value } = e.target;
        setTransaction((prev) => ({
            ...prev,
            [name]: value,
        }));
    };

    const handleSubmit = async (e) => {
        e.preventDefault();
        const user=localStorage.getItem('id');
        console.log(user);
        console.log("Transaction Submitted:");
        const transaction_data={
            sender: user,
            amount: transaction.amount,
            to:transaction.to,
        }
        const res = await fetch("http://localhost:5001/signtransaction",{
            method:"POST",
            headers : { "Content-Type": "application/json" },
            body: JSON.stringify(transaction_data),
        });
        const data1=await res.json();
        console.log("data recieved transc:",data1)
        if(res.ok) {
            console.log("signed!");
        }
        else {
            toast.error(data1.error);
        }
        const response = await fetch("http://localhost:5001/singlebroadcast",{
            method:"POST",
            headers : { "Content-Type": "application/json" },
            body: JSON.stringify({data:data1,message:"transaction"}),
        })
        const data2=await response.json();
        if(response.ok) {
            console.log(data2);
            
        }
        else {
            toast.error(data2.error);
        }
        setTransaction({ to: "", amount: "", message: "" });
    };

    return (
        <div className="transcontainer">
            <h3 className="title">Create a New Transaction</h3>
            <p className="description">
                Easily send funds by completing the form below. Make sure to double-check the recipient address and amount.
            </p>
            <form onSubmit={handleSubmit} className="form">
                <label className="label">
                    Recipient
                    <input
                        type="text"
                        name="to"
                        value={transaction.to}
                        onChange={handleChange}
                        placeholder="Recipient's address"
                        className="input"
                        required
                    />
                </label>
                <label className="label">
                    Amount
                    <input
                        type="number"
                        name="amount"
                        value={transaction.amount}
                        onChange={handleChange}
                        placeholder="Amount to send"
                        className="input"
                        required
                    />
                </label>
                <label className="label">
                    Message (Optional)
                    <input
                        name="message"
                        value={transaction.message}
                        onChange={handleChange}
                        placeholder="Add a message for the recipient"
                        className="input"
                    />
                </label>
                <button type="submit" className="submitButton" onClick={handleSubmit}>
                    Submit Transaction
                </button>
            </form>
            <button className="backButton" onClick={handleBack}>
                Back
            </button>
            <ToastContainer/>
        </div>
    );
}
