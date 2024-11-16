const express = require("express");
const router = express.Router();

const controller = require("../controllers/controller");

// Add all GET/POST routes here
router.get("/", controller.index);

module.exports = router;
