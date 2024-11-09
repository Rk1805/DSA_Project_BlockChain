import express from "express";
import authenticateUser from "../middlewares/authmiddleware.js"; // Adjust path if necessary
import {
  signupUser,
  getAllPeers,
  loginUser,
  broadcast,
  recieveTransaction,
  signTransaction,
} from "../controllers/transactionController.js";

const router = express.Router();

// Public routes
router.post("/signup", signupUser);
router.post("/login",loginUser);
router.get("/peers", getAllPeers);
router.post('/broadcast',broadcast);
router.post('/recieveTransaction',recieveTransaction);
router.post('/signtransaction',signTransaction);
export default router;
