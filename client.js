// Include Nodejs' net module.
const net = require("net")
const events = require("events");
const readline = require("readline");
const {resolve} = require("path/posix");

// The port number and hostname of the server.
const port = 8888;
const host = '127.0.0.1';

class MyEmitter extends events.EventEmitter {}

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
  async setData(key, data) {
    const d = `1|${key}|${data}`
    this.client.write(d);
    return new Promise((resolve) => {
      this.myEmitter.on('event', (binData) => {
        let data = binData.toString();
        // example 2|mykey|mydata
        let aData = data.split('|');
        if ((aData[0] == '1') && (aData[1] == key)) {
          resolve(true);
        }
      })
    });
  }


  /**
   * @param {string} key 
   * @returns {Promise<string>}
   */
  removeData(key) {
    this.client.write(`3|${key}`);
  }

  /**
   * @param {string} key 
   * @returns {Promise<string>}
   */
  getData(key) {
    this.client.write(`2|${key}`);
    return new Promise((resolve) => {
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

const testSetData = (vCcCv) => {

  const maxKeys = 10000;
  for (let k = 0; k < maxKeys; k++) {
    vCcCv.setData(`KK_${k}`, `DATA_${k}`);
  }
  console.log('send data done');
  setTimeout(() => {
    for (let k = 0; k < maxKeys; k++) {
      vCcCv.setData(`KK_${k}`, `DATA_${k}`);
    }
    console.log('send data done');
  }, 2000)

}

const testRemoveDate = (vCcCv, key) => {
  vCcCv.setData(key, 'mydata');
  setTimeout(() => {
    console.log('try remove data');
    vCcCv.removeData(key);
  }, 1000);
}




async function main() {
  const vCcCv = new CcCvStore(host, port);
  await vCcCv.connect();

  console.log('Connenced +++');

  //    const key = 'mykey_to_remove';
  //   testRemoveDate(vCcCv, key)

  const count = 1000;
  for (let k = 0; k < count; k++) {
    await vCcCv.setData(`kk_${k}`, `mydata ${k}`);
  }

  setTimeout(async () => {
    for (let k = 0; k < count; k++) {
      console.log('>> ', k, await vCcCv.getData(`kk_${k}`));
    }
  }, 1000);



}

main();
