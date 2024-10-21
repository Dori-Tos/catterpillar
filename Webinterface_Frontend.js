const socket = io();
socket.on('ttn-event', (data) => {
  // Update the DOM with the data from TTN
  console.log(data);
});