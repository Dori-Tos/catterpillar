    // Set up express with socket.io
    const app = express();
    const server = require("http").Server(app);
    const io = require("socket.io")(server);

    // Connect to TTN and subscribe to your device
    const client = mqtt.connect(...);
    client.on('connect', () => {
      client.subscribe('your-device');
    });

    // When TTN sends a message, notify the client via socket.io
    client.on('message', (topic, message) => {
      io.emit('event-name', message);
    });
