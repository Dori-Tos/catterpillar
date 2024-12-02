document.addEventListener("DOMContentLoaded", () => {
    const socket = io();

    const vehicleList = document.getElementById("vehicle-list");
    const map = L.map("map").setView([21.39, 84.29], 13);

    // Add OpenStreetMap tile layer
    L.tileLayer("https://tile.openstreetmap.org/{z}/{x}/{y}.png", {
        attribution: "© OpenStreetMap contributors",
    }).addTo(map);

    // To keep track of vehicle markers
    const vehicleMarkers = {};

    // Function to update the vehicle list
    function updateVehicleList(data) {
        vehicleList.innerHTML = "";
        data.vehicles.forEach((vehicle) => {
            const listItem = document.createElement("li");
            listItem.className = "p-2 border-b border-gray-300 cursor-pointer vehicle-item";
            listItem.dataset.vehicleId = vehicle.id; // Add data attribute for vehicle ID
            listItem.innerHTML = `
                <div class="flex justify-between items-center">
                    <span>${vehicle.type} - ${vehicle.id}</span>
                    ${vehicle.needsAssistance ? '<span class="text-red-500">⚠️</span>' : ""}
                </div>
                <div class="text-sm text-gray-600">Last used by: ${vehicle.driver_name}</div>
            `;
            vehicleList.appendChild(listItem);
        });

        // Add click event to highlight specific vehicle on the map
        document.querySelectorAll('.vehicle-item').forEach(item => {
            item.addEventListener('click', () => {
                // Remove precendent highlights
                document.querySelectorAll('.vehicle-item').forEach(el => {
                    el.classList.remove('bg-blue-100');
                });

                // Highlight clicked list item
                item.classList.add('bg-blue-100');

                // Get the vehicle ID
                const vehicleId = item.dataset.vehicleId;

                // Find and highlight the corresponding marker
                if (vehicleMarkers[vehicleId]) {
                    // Open popup and zoom to marker
                    const marker = vehicleMarkers[vehicleId];
                    marker.openPopup();
                    map.setView(marker.getLatLng(), 15);
                }
            });
        });
    }

    // Update the map with new data
    function updateMap(data) {
        data.vehicles.forEach((vehicle) => {
            if (vehicleMarkers[vehicle.id]) {
                // Update the existing marker
                vehicleMarkers[vehicle.id].setLatLng(vehicle.location);
            } else {
                // Create a new marker
                const marker = L.marker(vehicle.location)
                    .bindPopup(
                        `<b>${vehicle.type}</b><br>ID: ${vehicle.id}<br>Driver: ${vehicle.driver_name}`
                    )
                    .addTo(map);
                vehicleMarkers[vehicle.id] = marker;
            }
        });
    }

    // Fetch initial data from the JSON file
    fetch("/data/vehicles_data.json")
        .then((response) => response.json())
        .then((vehicles) => {
            const formattedData = { vehicles: Object.values(vehicles) };
            updateVehicleList(formattedData);
            updateMap(formattedData);
        })
        .catch((error) => console.error("Error loading vehicle data:", error));

    // Listen for real-time updates from Socket.IO
    socket.on("ttn-event", (data) => {
        console.log("Real-time data:", data);
        updateVehicleList(data);
        updateMap(data);
    });
});