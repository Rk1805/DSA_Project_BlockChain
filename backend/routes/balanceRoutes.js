import express from "express";
import {
  getBalance
} from "../controllers/balanceControllers.js";

const router = express.Router();

// Public routes
router.get("/", getBalance);
export default router;