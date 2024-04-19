const socket = new WebSocket('ws://192.168.0.106');

// Event listener para quando a conexão WebSocket é aberta
socket.addEventListener('open', function(event) {
  console.log('Conectado ao Arduino via WebSocket.');
});

// Event listener para quando uma mensagem é recebida do servidor
socket.addEventListener('message', function(event) {
  console.log('Mensagem recebida do Arduino:', event.data);
});

// Event listener para quando a conexão WebSocket é fechada
socket.addEventListener('close', function(event) {
  console.log('Conexão WebSocket fechada.');
});