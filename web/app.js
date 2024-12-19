const express = require("express");
const app = express();
const server = require("http").Server(app);
const io = require("socket.io")(server);
const mqtt = require("mqtt");
const path = require("path");
const fs = require("fs");
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

// Path to vehicles JSON file
const VEHICLES_FILE_PATH = path.join(__dirname, "public", "data", "vehicles_data.json");

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
            return employee.employee_name; // Return name if ID is found
        }
    }

    return employeeId; // Return the ID itself if not found
}


// Function to read vehicles from JSON file
function readVehiclesFile() {
    try {
        const data = fs.readFileSync(VEHICLES_FILE_PATH, 'utf8');
        return JSON.parse(data);
    } catch (error) {
        console.error("Error reading vehicles file:", error);
        return { vehicles: [] };
    }
}

// Function to write vehicles to JSON file
function writeVehiclesFile(data) {
    try {
        fs.writeFileSync(VEHICLES_FILE_PATH, JSON.stringify(data, null, 4));
    } catch (error) {
        console.error("Error writing vehicles file:", error);
    }
}

client.on("connect", () => {
    console.log("Connected to TTN");
    client.subscribe([`v3/${TTN_USERNAME}/devices/+/up`], () => {
        console.log("Subscribed to TTN topic");
    });
});

client.on("error", (error) => console.error("TTN MQTT error:", error));

client.on("message", (topic, message) => {
    console.log("Received TTN message:", topic, message.toString());
    
    try {
        const parsedMessage = JSON.parse(message.toString());

        const parsedVehicleData = {
            vehicleID: parsedMessage.uplink_message.decoded_payload.vehicleID,
            lat: parsedMessage.uplink_message.decoded_payload.latitude,
            lng: parsedMessage.uplink_message.decoded_payload.longitude,
            rollAngle: parsedMessage.uplink_message.decoded_payload.rollAngle,
            pitchAngle: parsedMessage.uplink_message.decoded_payload.pitchAngle,
            rfidData: parsedMessage.uplink_message.decoded_payload.rfidData
        };

        // Validate that required data is loaded
        if (!vehiclesTypesData || !employeesData) {
            console.error("Vehicle types or employees data not loaded");
            return;
        }

        let vehicleType = getVehicleTypeById(parsedVehicleData.vehicleID, vehiclesTypesData);
        let employee_name = getEmployeeNameById(parsedVehicleData.rfidData, employeesData);

        const maxRollAngle = 30;
        const maxPitchAngle = 40;

        let needsAssistance = false;
        if (parsedVehicleData.rollAngle >= maxRollAngle ||
            parsedVehicleData.rollAngle <= -1 * maxRollAngle ||
            parsedVehicleData.pitchAngle >= maxPitchAngle || 
            parsedVehicleData.pitchAngle <= -1 * maxPitchAngle){

                needsAssistance = true;
            }

        let location = [parsedVehicleData.lat, parsedVehicleData.lng];

        const vehicleData = {
            "id": parsedVehicleData.vehicleID,
            "type": vehicleType,
            "driver_name": employee_name,
            "needsAssistance": needsAssistance,
            "location": location
        };

        // Read current vehicles data
        const vehiclesData = readVehiclesFile();

        // Check if vehicle already exists
        const existingVehicleIndex = vehiclesData.vehicles.findIndex(
            vehicle => vehicle.id === vehicleData.id
        );

        if (existingVehicleIndex !== -1) {
            // Update existing vehicle
            vehiclesData.vehicles[existingVehicleIndex] = {
                ...vehiclesData.vehicles[existingVehicleIndex],
                ...vehicleData
            };
        } else {
            // Add new vehicle
            vehiclesData.vehicles.push(vehicleData);
        }

        // Write updated vehicles data back to file
        writeVehiclesFile(vehiclesData);

        // Emit event to socket.io clients
        io.emit("ttn-event", { vehicles: [vehicleData] });

        console.log(vehicleData);

    } catch (error) {
        console.error("Error processing TTN message:", error);
    }
});

// Route
app.get("/", (req, res) => {
    res.render("index");
});

// Start server
const PORT = process.env.PORT || 3000;
server.listen(PORT, () => console.log(`Server running on port ${PORT}`));