import express from "express";
import {
  getHistory
} from "../controllers/historyControllers.js";

const router = express.Router();

// Public routes
router.get("/", getHistory);
export default router;