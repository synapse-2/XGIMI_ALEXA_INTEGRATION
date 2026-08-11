
export function updateWiFiandBleIcon(){
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
}


// Wait for the HTML document to be fully loaded
// The 'DOMContentLoaded' event ensures that your script runs only after the DOM is ready.
document.addEventListener('DOMContentLoaded', function () {
    updateWiFiandBleIcon();
});


export function reloadGlobalJS() {
  const scriptId = 'refreshable-app-logic';
  const oldScript = document.getElementById(scriptId);
  
  if (!oldScript) {
    console.error(`Script with ID "${scriptId}" not found in document.`);
    return;
  }

  // 1. Capture the original clean URL path before any older cache-busters
  const originalSrc = oldScript.src.split('?')[0];

  // 2. Remove the existing script node from the DOM
  oldScript.remove();

  // 3. Create the replacement tag
  const newScript = document.createElement('script');
  newScript.id = scriptId;
  //newScript.type = 'module'; // Defines this script as an ES Module
  
  // 4. Force a fresh download 
  newScript.src = `${originalSrc}`;

  // 5. Append to execute the fresh code
  document.body.appendChild(newScript);
  console.log(`Successfully refreshed: ${originalSrc}`);
}


// Manually bind reloadGlobalJS() to the global window object
window.reloadGlobalJS = reloadGlobalJS;
window.updateWiFiandBleIcon = updateWiFiandBleIcon;