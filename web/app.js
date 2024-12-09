const express = require("express");
const app = express();
const server = require("http").Server(app);
const io = require("socket.io")(server);
const mqtt = require("mqtt");
const path = require("path");
require("dotenv").config();

// Set EJS as the template engine
app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));
app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, "public"))); // Serve static files

// MQTT Configuration
const TTN_HOST = "eu1.cloud.thethings.network";
const TTN_PORT = "1883";
const TTN_USERNAME = "caterpillar@ttn";
const TTN_PASSWORD = process.env.TTN_PASSWORD;

const connectUrl = `mqtt://${TTN_HOST}:${TTN_PORT}`;
const client = mqtt.connect(connectUrl, {
    clientId: `mqtt_${Math.random().toString(16).slice(3)}`,
    username: TTN_USERNAME,
    password: TTN_PASSWORD,
    reconnectPeriod: 1000,
});

client.on("connect", () => {
    console.log("Connected to TTN");
    client.subscribe([`v3/${TTN_USERNAME}/devices/+/up`], () => {
        console.log("Subscribed to TTN topic");
    });
});


let vehiclesTypesData;
let employeesData;

// Read and parse vehicles_id_data.json
fs.readFile(path.join(__dirname, "public", "data", "vehicles_id_data.json"), "utf8", (err, data) => {
    if (err) {
        console.error("Error reading vehicles_id_data.json:", err);
        return;
    }
    vehiclesTypesData = JSON.parse(data);
});

// Read and parse employees_data.json
fs.readFile(path.join(__dirname, "public", "data", "employees_data.json"), "utf8", (err, data) => {
    if (err) {
        console.error("Error reading employees_data.json:", err);
        return;
    }
    employeesData = JSON.parse(data);
});

function getVehicleTypeById(vehicleId, vehiclesTypesFile) {
    const vehicleTypes = vehiclesTypesFile.vehicle_types;

    for (const vehicleType of vehicleTypes) {
        for (const [type, vehicles] of Object.entries(vehicleType)) {
            for (const vehicle of vehicles) {
                if (vehicle.vehicle_id === vehicleId) {
                    return type;
                }
            }
        }
    }

    return null; 
}

function getEmployeeNameById(employeeId, employeesFile) {
    const employees = employeesFile.employees;

    for (const employee of employees) {
        if (employee.employee_id === employeeId) {
            return employee.employee_name;
        }
    }

    return null;
}

client.on("error", (error) => console.error("TTN MQTT error:", error));

client.on("message", (topic, message) => {
    console.log("Received TTN message:", topic, message.toString());
    const parsedMessage = JSON.parse(message.toString());

    const parsedVehicleData = {
        id: parsedMessage.vehicleID,
        lat: parsedMessage.latitude,
        lng: parsedMessage.longitude,
        rollAngle: parsedMessage.rollAngle,
        pitchAngle: parsedMessage.pitchAngle,
        rfidData: parsedMessage.rfidData
    };

    let vehicleType = getVehicleTypeById(vehicleID, vehiclesTypesData)

    let employee_name = getEmployeeNameById(rfidData, employeesData)

    const maxRollAngle = 30;
    const maxPitchAngle = 40;

    let needsAssistance = false
    if (rollAngle >= maxRollAngle || rollAngle <= -1 * maxRollAngle || pitchAngle >= maxPitchAngle || pitchAngle <= -1* maxPitchAngle)
        needsAssistance = true;

    let location = [lat, lng]

    const vehicleData = {
        "id": parsedVehicleData.vehicleID,
        "type": vehicleType,
        "driver_name": employee_name,
        "needsAssistance": needsAssistance,
        "location": location
    };

    io.emit("ttn-event", { vehicles: [vehicleData] });
});

// Route
app.get("/", (req, res) => {
    res.render("index");
});

// Start server
const PORT = process.env.PORT || 3000;
server.listen(PORT, () => console.log(`Server running on port ${PORT}`));
