// https://fidisys.com/blog/serial-port-devices/

// https://github.com/mmiscool/serialTerminal.com

if ("serial" in navigator) {
  console.log("Awesome, the serial port is supported.")
}

let port, textEncoder, writableStreamClosed, writer;
let connectedFlag = false;
const regex =/(at\+recv=)(-?[0-9]+),(-?[0-9]+),([0-9]+):([a-zA-z0-9]*A)/g;
let response = "";
let p_response = "";


let display = document.getElementById("display");
let temp = document.getElementById("temp");
let hum = document.getElementById("hum");
let bat = document.getElementById("bat");
let rssi = document.getElementById("rssi");
let snr = document.getElementById("snr");
let fire_status = document.getElementById("fire_stat");

  async function connectSerial() {
      try {
          // Prompt user to select any serial port.
          port = await navigator.serial.requestPort();
          await port.open({ baudRate: document.getElementById("cmdLine").value });
          listenToPort();

          textEncoder = new TextEncoderStream();
          writableStreamClosed = textEncoder.readable.pipeTo(port.writable);

          writer = textEncoder.writable.getWriter();
          connected();
      } catch {
          alert("Serial Connection Failed");
      }
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
        if(response.length >= 40 && response.endsWith("A"))
      {
          let ar = [...response.matchAll(regex)];
          // console.log(hex2a(ar[0][5]));
          console.log(ar);
         let txt = hex2a(ar[0][5]).trim().split("\t");
            temp.innerHTML = txt[0];
            hum.innerHTML = txt[1];
            bat.innerHTML = txt[2];
            rssi.innerHTML = `${ar[0][2]}dBm`;
            snr.innerHTML = `${ar[0][3]}dB`;
            if(txt[3]=='1')
            {
               fire_status.innerHTML = "fire";
            }
            else if(txt[3] == '0')
            {
              fire_status.innerHTML = "no fire";
            }
           
          // display.innerHTML = `${hex2a(ar[0][5])}`;
        p_response = "";
      }
      else {
          p_response +=response;
          p_response = response.split('\\')[0];
          p_response.trim();
          
          // console.log(p_response,p_response.length);
          if(p_response.length >=40)
          {

             let ar = [...p_response.matchAll(regex)];
            // console.log(hex2a(ar[0][5]));
            console.log(ar);
            let txt = hex2a(ar[0][5]).trim().split("\t");
            console.log(txt);
            temp.innerHTML = txt[0];
            hum.innerHTML = txt[1];
            bat.innerHTML = txt[2];
            rssi.innerHTML = `${ar[0][2]}dBm`;
            snr.innerHTML = `${ar[0][3]}dB`;
            if(txt[3]=='1')
            {
               fire_status.innerHTML = "fire";
            }
            else if( txt[3]== '0')
            {
              fire_status.innerHTML = "no fire";
            }

           
          // display.innerHTML = `${hex2a(ar[0][5])}`;
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

