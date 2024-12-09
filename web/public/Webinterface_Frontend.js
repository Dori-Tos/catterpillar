document.addEventListener("DOMContentLoaded", () => {
    const socket = io();

    const vehicleList = document.getElementById("vehicle-list");
    const searchInput = document.getElementById("vehicle-search");
    const assistanceFilter = document.getElementById("assistance-filter");
    const map = L.map("map").setView([21.39, 84.29], 13); //Center of map. Need to change according to our location

    // Add OpenStreetMap tile layer
    L.tileLayer("https://tile.openstreetMap.org/{z}/{x}/{y}.png", {
        attribution: "© OpenStreetMap contributors",
    }).addTo(map);

    // To keep track of vehicle markers
    const vehicleMarkers = {};

    // Function to apply filters
    function applyFilters() {
        const searchTerm = searchInput.value.toLowerCase();
        const showOnlyNeedsAssistance = assistanceFilter.checked;
        const vehicleItems = document.querySelectorAll('.vehicle-item');
        
        vehicleItems.forEach(item => {
            const text = item.textContent.toLowerCase();
            const needsAssistance = item.querySelector('.needs-assistance') !== null;
            
            const matchesSearch = text.includes(searchTerm);
            const matchesAssistanceFilter = !showOnlyNeedsAssistance || needsAssistance;
            
            item.style.display = (matchesSearch && matchesAssistanceFilter) ? 'flex' : 'none';
        });
    }

    // Function to update the vehicle list
    function updateVehicleList(data) {
        vehicleList.innerHTML = "";
        data.vehicles.forEach((vehicle) => {
            const listItem = document.createElement("li");
            listItem.className = "p-2 border-b border-gray-300 cursor-pointer vehicle-item flex items-center";
            listItem.dataset.vehicleId = vehicle.id;
            listItem.innerHTML = `
                <div class="flex items-center mr-2">
                    <input type="checkbox" class="vehicle-checkbox mr-2" 
                           data-vehicle-id="${vehicle.id}" 
                           ${localStorage.getItem(`vehicle_${vehicle.id}_visible`) !== 'false' ? 'checked' : ''}>
                </div>
                <div class="flex-grow">
                    <div class="flex justify-between items-center">
                        <span>${vehicle.type} - ${vehicle.id}</span>
                        ${vehicle.needsAssistance ? '<span class="needs-assistance text-red-500">⚠️</span>' : ""}
                    </div>
                    <div class="text-sm text-gray-600">Last used by: ${vehicle.driver_name}</div>
                </div>
                <button class="remove-vehicle ml-2 text-red-500" data-vehicle-id="${vehicle.id}">✖️</button>
            `;
            vehicleList.appendChild(listItem);
        });

        // Add event listeners for checkboxes
        document.querySelectorAll('.vehicle-checkbox').forEach(checkbox => {
            checkbox.addEventListener('change', (e) => {
                e.stopPropagation();

                const vehicleId = e.target.dataset.vehicleId;
                const isVisible = e.target.checked;
                
                localStorage.setItem(`vehicle_${vehicleId}_visible`, isVisible);
                
                const marker = vehicleMarkers[vehicleId];
                if (marker) {
                    if (isVisible) {
                        marker.addTo(map);
                    } else {
                        map.removeLayer(marker);
                    }
                }
            });

            checkbox.addEventListener('click', (e) => {
                e.stopPropagation();
            });
        });

        // Add event listeners for remove buttons
        document.querySelectorAll('.remove-vehicle').forEach(button => {
            button.addEventListener('click', (e) => {
                e.stopPropagation();

                const vehicleId = e.target.dataset.vehicleId;
                
                if (vehicleMarkers[vehicleId]) {
                    map.removeLayer(vehicleMarkers[vehicleId]);
                    delete vehicleMarkers[vehicleId];
                }
                
                e.target.closest('li').remove();
            });
        });

        // Add click event to highlight specific vehicle on the map
        document.querySelectorAll('.vehicle-item').forEach(item => {
            item.addEventListener('click', (e) => {
                if (e.target.closest('.vehicle-checkbox, .remove-vehicle')) {
                    return;
                }

                document.querySelectorAll('.vehicle-item').forEach(el => {
                    el.classList.remove('bg-blue-100');
                });

                item.classList.add('bg-blue-100');

                const vehicleId = item.dataset.vehicleId;

                if (vehicleMarkers[vehicleId]) {
                    const marker = vehicleMarkers[vehicleId];
                    marker.openPopup();
                    map.setView(marker.getLatLng(), 15);
                }
            });
        });

        // Apply filters after updating the list
        applyFilters();
    }

    // Search functionality
    searchInput.addEventListener('input', applyFilters);

    // Assistance filter functionality
    assistanceFilter.addEventListener('change', applyFilters);

    // Update the map with new data
    function updateMap(data) {
        data.vehicles.forEach((vehicle) => {
            const isVisible = localStorage.getItem(`vehicle_${vehicle.id}_visible`) !== 'false';

            if (vehicleMarkers[vehicle.id]) {
                vehicleMarkers[vehicle.id].setLatLng(vehicle.location);
                
                if (isVisible) {
                    vehicleMarkers[vehicle.id].addTo(map);
                } else {
                    map.removeLayer(vehicleMarkers[vehicle.id]);
                }
            } else {
                const marker = L.marker(vehicle.location)
                    .bindPopup(
                        `<b>${vehicle.type}</b><br>ID: ${vehicle.id}<br>Driver: ${vehicle.driver_name}`
                    );
                
                if (isVisible) {
                    marker.addTo(map);
                }
                
                vehicleMarkers[vehicle.id] = marker;
            }
        });
    }

    // Fetch and update data function
    function fetchAndUpdateVehicleData() {
        fetch("/data/vehicles_data.json")
        .then((response) => response.json())
        .then((data) => {
            updateVehicleList(data);
            updateMap(data);
        })
        .catch((error) => console.error("Error loading vehicle data:", error));
    }

    // Fetch initial data
    fetchAndUpdateVehicleData();

    // Listen for real-time updates from Socket.IO
    socket.on("ttn-event", () => {
        // When a new event is received, re-fetch the entire JSON
        fetchAndUpdateVehicleData();
    });
});