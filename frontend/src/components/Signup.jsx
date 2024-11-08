import React, { useState,} from "react";
import { useNavigate } from "react-router-dom";
import { ToastContainer, toast } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import './Signup.css';

const AuthPage = () => {
  const [isLogin, setIsLogin] = useState(true);
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [username, setUsername] = useState("");
  const [ip, setip] = useState("");

  const API_BASE_URL = "http://localhost:5001";
  const navigate = useNavigate();



  const handleSubmit = async (e) => {
    e.preventDefault();

    try {
      const endpoint = isLogin ? `${API_BASE_URL}/login` : `${API_BASE_URL}/signup`;
      const body = isLogin
        ? { email, password }
        : { email, password, username,ip};
        console.log(body);
      const response = await fetch(endpoint, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(body),
      });

      const data = await response.json();
      console.log(body);
      if (response.ok) {
        if (isLogin) {
          localStorage.setItem("id", data.id);
          console.log(data);
          console.log(username);
          console.log(localStorage.getItem("id"));
          toast.success("Logged in successfully!");
          navigate("/");
        } else {
          console.log(data);
          toast.success("Sign-up successful! Please log in.");
          setIsLogin(true);
        }
      } else {
        toast.error(data.error || `${isLogin ? "Login" : "Sign-up"} failed.`);
      }
    } catch (error) {
      console.error(`${isLogin ? "Login" : "Sign-up"} error: `, error);
      toast.error(`An error occurred during ${isLogin ? "login" : "sign-up"}.`);
    }
  };

  return (
    <div className="auth-container">
      <div className="auth-card">
        <div className="auth-tabs">
          <button
            className={`auth-tab ${isLogin ? "active" : ""}`}
            onClick={() => setIsLogin(true)}
          >
            Login
          </button>
          <button
            className={`auth-tab ${!isLogin ? "active" : ""}`}
            onClick={() => setIsLogin(false)}
          >
            Sign Up
          </button>
        </div>
        <form onSubmit={handleSubmit}>
          {!isLogin && (
            <>
              <div className="form-group">
                <label htmlFor="username">Username</label>
                <input
                  type="text"
                  id="username"
                  value={username}
                  onChange={(e) => setUsername(e.target.value)}
                  required
                />
              </div>
              <div className="form-group">
                <label htmlFor="username">Ip Address</label>
                <input
                  type="password"
                  id="ip"
                  value={ip}
                  onChange={(e) => setip(e.target.value)}
                  required
                />
              </div>
              
              
            </>
          )}
          <div className="form-group">
            <label htmlFor="email">Email</label>
            <input
              type="email"
              id="email"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              required
            />
          </div>
          <div className="form-group">
            <label htmlFor="password">Password</label>
            <input
              type="password"
              id="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
              required
            />
          </div>
          <button type="submit" className="submit-btn" onClick={handleSubmit}>
            {isLogin ? "Login" : "Sign Up"}
          </button>
        </form>
      </div>
      <ToastContainer />
    </div>
  );
};

export default AuthPage;