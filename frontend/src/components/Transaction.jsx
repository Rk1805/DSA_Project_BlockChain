import React, { useState } from "react";
import { useNavigate } from "react-router-dom";
import "./Transaction.css";

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

    const handleSubmit = (e) => {
        e.preventDefault();
        console.log("Transaction Submitted:", transaction);
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
                <button type="submit" className="submitButton">
                    Submit Transaction
                </button>
            </form>
            <button className="backButton" onClick={handleBack}>
                Back
            </button>
        </div>
    );
}