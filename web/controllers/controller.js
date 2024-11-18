const path = require("path");

const index = (req, res) => {
    try {
        res.render("index");
    } catch (error) {
        console.log(error);
    }
};

module.exports = { index };
