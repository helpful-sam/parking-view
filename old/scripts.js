const parkingArea = document.querySelector('#parking-area');
const availableCount = document.querySelector('#available-count');
const occupiedCount = document.querySelector('#occupied-count');

let parkingData = [0, 0, 0, 0]; // 0 for available (green), 1 for occupied (red)

// Simulate data stream
setInterval(() => {
    parkingData = parkingData.map(() => Math.round(Math.random())); // Randomly set 0 or 1
    updateParkingSlots();
}, 2000);

function updateParkingSlots() {
    let available = 0;
    let occupied = 0;

    parkingData.forEach((status, index) => {
        const slot = document.querySelector(`#slot-${index + 1}`);
        if (status === 0) {
            slot.classList.remove('occupied');
            slot.classList.add('available');
            available++;
        } else {
            slot.classList.remove('available');
            slot.classList.add('occupied');
            occupied++;
        }
    });

    availableCount.innerHTML = `<span class="green"></span> Available: ${available}`;
    occupiedCount.innerHTML = `<span class="red"></span> Occupied: ${occupied}`;
}

// Initial render
updateParkingSlots();
