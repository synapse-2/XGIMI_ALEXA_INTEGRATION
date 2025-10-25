
// Wait for the HTML document to be fully loaded
// The 'DOMContentLoaded' event ensures that your script runs only after the DOM is ready.
document.addEventListener('DOMContentLoaded', function () {
    // Get a reference to the HTML elements by their ID

    const wifiIconElement = document.getElementById("Wifi-icon");
    const bluetoothIconElement = document.getElementById("bluetooth-icon");
    const wifiTitleElement = document.getElementById("wifi-title");

    // Update the content of the elements

    wifiIconElement.classList.remove("wifi-low");
    wifiIconElement.classList.remove("wifi-medium");
    wifiIconElement.classList.remove("wifi-high");
    wifiIconElement.classList.remove("wifi-none");
    wifiIconElement.classList.add(wifiStrength);
    wifiTitleElement.textContent = "RSSI:" + WifiRssi;
    bluetoothIconElement.classList.remove("bt-advertising");
    bluetoothIconElement.classList.remove("bt-connected");
    bluetoothIconElement.classList.add(bluetoothStatus);
});