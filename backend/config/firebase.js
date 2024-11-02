// firebase.js
import { initializeApp } from "firebase/app";
import { getFirestore } from "firebase/firestore";

// Your Firebase config object
const firebaseConfig = {
  apiKey: "AIzaxSyC6IIKQu5m4SAPA5Dntr2o2yv5amwmkRy8",
  authDomain: "dsa-project-c5b6f.firebaseapp.com",
  projectId: "dsa-project-c5b6f",
  storageBucket: "dsa-project-c5b6f.appspot.com",
  messagingSenderId: "912603969519",
  appId: "1:912603969519:web:d3f107346484f81485e81a",
  measurementId: "G-JSSRWYMP4S"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
// Get Firestore instance
const db = getFirestore(app);

export { db };
