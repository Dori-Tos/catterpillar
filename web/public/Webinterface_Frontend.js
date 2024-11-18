// Ensure the DOM (HTML elements) is fully loaded before running the script
document.addEventListener("DOMContentLoaded", () => {
    const socket = io();

    const vehicleList = document.getElementById("vehicle-list");
    const vehicleModal = document.getElementById("vehicle-modal");
    const vehicleTitle = document.getElementById("vehicle-title");
    const vehicleDetails = document.getElementById("vehicle-details");
    const closeModalButton = document.getElementById("close-modal");
    const openModalButton = document.getElementById("open-modal");

    // Handle incoming data
    socket.on("ttn-event", (data) => {
        console.log(data);
        updateVehicleList(data);
        updateMap(data);
    });

    function updateVehicleList(data) {
        vehicleList.innerHTML = "";
        data.vehicles.forEach((vehicle) => {
            const listItem = document.createElement("li");
            listItem.className = "p-2 border-b border-gray-300 cursor-pointer";
            listItem.innerHTML = `
            <div class="flex justify-between items-center">
            <span>${vehicle.type} - ${vehicle.id}</span>
            ${vehicle.needsAssistance ? '<span class="text-red-500">⚠️</span>' : ""}
            </div>
            <div class="text-sm text-gray-600">Last used by: ${vehicle.lastUser}</div>
            `;
            listItem.addEventListener("click", () => showVehicleDetails(vehicle));
            vehicleList.appendChild(listItem);
        });
    }

    const vehicleMarkers = {};
    function updateMap(data) {
        data.vehicles.forEach((vehicle) => {
            // If the vehicle marker already exists, update its position
            if (vehicleMarkers[vehicle.id]) {
                vehicleMarkers[vehicle.id].setLatLng([vehicle.lat, vehicle.lng]);
            } else {
                // Create a new marker
            }
        });
    }

    function showVehicleDetails(vehicle) {
        vehicleTitle.textContent = `${vehicle.type} - ${vehicle.id}`;
        vehicleDetails.innerHTML = `
            <p>Last used by: ${vehicle.lastUser}</p>
            <p>Needs assistance: ${vehicle.needsAssistance ? "Yes" : "No"}</p>
            <h3 class="text-lg font-bold mt-4">Usage History</h3>
            <ul>
                ${vehicle.history.map((user) => `<li>${user}</li>`).join("")}
            </ul>
        `;
        vehicleModal.classList.remove("hidden");
    }

    if (openModalButton && closeModalButton && vehicleModal) {
        openModalButton.addEventListener("click", () => {
            vehicleModal.classList.remove("hidden");
            console.log("Open modal");
        });

        closeModalButton.addEventListener("click", () => {
            vehicleModal.classList.add("hidden");
            console.log("Close modal");
        });
    } else {
        console.error("One or more elements not found.");
    }
});
