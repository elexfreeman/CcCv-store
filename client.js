// Include Nodejs' net module.
const net = require("net")
const events = require("events");
const readline = require("readline");

// The port number and hostname of the server.
const port = 8888;
const host = '127.0.0.1';

class MyEmitter extends events.EventEmitter { }
const myEmitter = new MyEmitter();
const client = new net.Socket();



client.connect(port, host, function () {
  console.log(`Connected to server ${host}:${port}`);
  // start the typing event
  myEmitter.emit('event');
});

client.on('data', data => {
  console.log(data.toString());
});

client.on('close', function () {
  console.log('Connection closed');
});


myEmitter.on('event', () => {

  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
  });

  rl.prompt();

  // recerve text from keyboard
  rl.on('line', line => {
    const msg = line.trim();
    // send msg
    client.write(msg);
    rl.prompt();
  }).on('close', () => {
    process.exit(0);
  });
});
