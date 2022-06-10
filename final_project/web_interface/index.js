// https://fidisys.com/blog/serial-port-devices/

// https://github.com/mmiscool/serialTerminal.com

if ("serial" in navigator) {
  console.log("Awesome, the serial port is supported.")
}

let port, textEncoder, writableStreamClosed, writer;
let connectedFlag = false;
const regex =/(at\+recv=)(-?[0-9]+),(-?[0-9]+),([0-9]+):([a-zA-z0-9]*250A)/g;
let response = "";
let p_response = "";


let display = document.getElementById("display");

  async function connectSerial() {
      try {
          // Prompt user to select any serial port.
          port = await navigator.serial.requestPort();
          await port.open({ baudRate: document.getElementById("cmdLine").value });
          listenToPort();

          textEncoder = new TextEncoderStream();
          writableStreamClosed = textEncoder.readable.pipeTo(port.writable);

          writer = textEncoder.writable.getWriter();
      } catch {
          alert("Serial Connection Failed");
      }

      connected();
  }

  function connected() {
    //Update UI
    connectedFlag = true;
    document.getElementById("cmdLine").value = "";
    document.getElementById("cmdLine").placeholder = "Send data";
    document.getElementById("cmdButton").onclick = sendSerialLine;
    document.getElementById("cmdButton").innerHTML = "Send";

  }
 
  async function sendSerialLine() {
      dataToSend = document.getElementById("cmdLine").value;
    //   console.log(dataToSend);
      await writer.write(`${dataToSend}\r\n`);
      document.getElementById("cmdLine").value = "";
      //await writer.releaseLock();
  }

  async function listenToPort() {
      const textDecoder = new TextDecoderStream();
      const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
      const reader = textDecoder.readable.getReader();
    
      // Listen to data coming from the serial device.
      while (true) {
          const { value, done } = await reader.read();
          if (done) {
              // Allow the serial port to be closed later.
              //reader.releaseLock();
              break;
          }
          // value is a string.
          appendToTerminal(value);
      }
  }

  const serialResultsDiv = document.getElementById("serialResults");

  async function appendToTerminal(newStuff) {
      serialResultsDiv.innerHTML += newStuff;
      if (serialResultsDiv.innerHTML.length > 3000) serialResultsDiv.innerHTML = serialResultsDiv.innerHTML.slice(serialResultsDiv.innerHTML.length - 3000);

      //scroll down to bottom of div
      serialResultsDiv.scrollTop = serialResultsDiv.scrollHeight;

        response = newStuff;
        response = response.split('\\')[0];
        response.trim();
        
        // console.log(response,response.length);
        if(response.length >= 52 && response.endsWith("A"))
      {
          let ar = [...response.matchAll(regex)];
          console.log(hex2a(ar[0][5]));
          display.innerHTML = `${hex2a(ar[0][5])}`;
        p_response = "";
      }
      else {
          p_response +=response;
          p_response = response.split('\\')[0];
          p_response.trim();
          
        //   console.log(p_response,p_response.length);
          if(p_response.length >=52)
          {

             let ar = [...p_response.matchAll(regex)];
            console.log(hex2a(ar[0][5]));
            display.innerHTML = `${hex2a(ar[0][5])}`;
            p_response = "";
          }
      }

  }

  document.getElementById("cmdLine").addEventListener("keyup", async function (event) {
      if(!connectedFlag && event.key === 13) {
          connectSerial();
      }
    
      if (event.keyCode === 13) {
          sendSerialLine();
      }
  })

function hex2a(hex) 
{
    let str = '';
    for (let i = 0; i < hex.length; i += 2)
        str += String.fromCharCode(parseInt(hex.substr(i, 2), 16));
    return str;
}

