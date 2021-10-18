// Include Nodejs' net module.
const net = require("net")
const events = require("events");
const readline = require("readline");
const { resolve } = require("path/posix");

// The port number and hostname of the server.
const port = 8888;
const host = '127.0.0.1';

class MyEmitter extends events.EventEmitter { }

class CcCvStore {

  constructor(_host, _port) {
    this.host = _host;
    this.port = _port;
    this.myEmitter = new MyEmitter();
    this.client = new net.Socket();
  }

  connect() {
    return new Promise((resolve, reject) => {
      this.client.connect(this.port, this.host, () => {
        console.log(`CcCvStore connected to server ${this.host}:${this.port}`);
        resolve(true);
      });

      this.client.on('data', (data) => {
        this.myEmitter.emit('event', data);
      });

      this.client.on('close', () => {
        console.log('CCcCvStore connection closed');
      });

    });
  }

  /**
   * @param {string} key 
   * @param {string} data 
   * @returns {void}
   */
  setData(key, data) {
    const d = `1|${key}|${data}`
    console.log("Try write data",d);
    this.client.write(d);
  }

  /**
   * @param {string} key 
   * @returns {Promise<string>}
   */
  getData(key) {
    this.client.write(`2|${key}`);
    return new Promise((resolve, reject) => {
      this.myEmitter.on('event', (binData) => {
        let data = binData.toString();
        // example 2|mykey|mydata
        let aData = data.split('|');
        if ((aData[0] == '2') && (aData[1] == key)) {
          resolve(aData[2]);
        }
      })
    });
  }

}

function main() {
  const maxKeys = 1000000
  const vCcCv = new CcCvStore(host, port);
  vCcCv.connect().then(data => {
    console.log('Connenced +++');
    for (let k = 0; k < maxKeys; k++) {
      vCcCv.setData(`KK_${k}`, `DATA_${k}`);
    }

  })


}

main();