function G(id) {
	return document.getElementById(id);
}

var vector = {
	move: {
		x: 0.0,
		y: 0.0,
		z: 0.0,
	},
	rotate: {
		pitch: 0.0,
		roll:  0.0,
		yaw:   0.0,
	},
};

var telemetry = {
	status:   {},
	voltage:  0.0,
	current:  0.0,
	loopTime: 0,
};

var gui ={
	updateInterval: null,
	obj: {
		status: null,
	},

	init: function () {
		document.addEventListener("visibilitychange", gui.onVisibilityChange);
		gui.obj.status = G('status');

		gui.updateInterval = setInterval(gui.update, 100);
	},

	update: function () {
		gui.updateStatus();
	},

	onVisibilityChange: function () {
		if (document.visibilityState == "hidden") {
			// If browser closed
			failsafe.setFS();
		}
	},

	displayNumber: function (n) {
		if (n == 0) {
			return "0.000";
		}

		return (n + "000000").slice(0, 5);
	},

	updateStatus: function (){
		gui.obj.status.innerHTML = gui.displayNumber(telemetry.voltage) + 'V | ' + gui.displayNumber(telemetry.current) + 'A | LoopTime: ' + telemetry.loopTime;
	},
};

let ws = {
	ws:                null,
	status:            false,
	error:             false,
	updateInterval:    null,
	telemetryInterval: null,
	
	init: function () {
		clearInterval(ws.updateInterval);
		try {
			ws.ws = new WebSocket(c.w);
			ws.ws.onopen = function() {
				ws.status = true;
			};
			ws.ws.onerror = function() {
				ws.status = false;
			};
			ws.ws.onmessage = function (event) {
				event.data.arrayBuffer().then(buffer => {ws.parseEvent(new Uint8Array(buffer))});
			};

			ws.updateInterval    = setInterval(ws.update, 50);
			ws.telemetryInterval = setInterval(ws.telemetryRequest, 1000);
		} catch(e) {
			clearInterval(ws.updateInterval);
			ws.status = false;
			ws.error = true;
			console.log(e);
		};
	},

	update: function(data) {
		if (ws.status) {
			ws.ws.send(packet.move());
		}
	},

	parseEvent: function (binaryData) {
		switch (binaryData[0]) {
			case 84:
				ws.telemetryResponse(binaryData);
				break;
		}
	},

	telemetryRequest: function (data) {
		if (ws.status) {
			ws.ws.send(packet.telemetry());
		}
	},

	telemetryResponse: function (binaryData) {
		packet.telemetryParse(binaryData);
	}
};

let failsafe = {
	setFS: function () {
		vector.move.x       = 0;
		vector.move.y       = 0;
		vector.move.z       = 0;
		vector.rotate.roll  = 0;
		vector.rotate.pitch = 0;
		vector.rotate.yaw   = 0;
	}
};


let packet = {
	init: function () {
		packet.pMove = new ArrayBuffer(14);
		packet.vMove = new Uint8Array(packet.pMove);
	},
	_norm1: function (value) {
		return (value+1)*10000;
	},
	_uint16: function (view, num, offset) {
		view[offset]   = (num>>8)&255;
		view[offset+1] = num&255;
	},
	_getUint16: function (data, offset) {
		return parseInt(data[offset]<<8 | data[offset+1]);
	},
	move: function () {
		packet.vMove[0] = 77;
		packet.vMove[1] = 1;
		packet._uint16(packet.vMove, packet._norm1(vector.move.x),        2);
		packet._uint16(packet.vMove, packet._norm1(vector.move.y),        4);
		packet._uint16(packet.vMove, packet._norm1(vector.move.z),        6);
		packet._uint16(packet.vMove, packet._norm1(vector.rotate.pitch),  8);
		packet._uint16(packet.vMove, packet._norm1(vector.rotate.roll),  10);
		packet._uint16(packet.vMove, packet._norm1(vector.rotate.yaw),   12);

		return packet.pMove;
	},
	telemetry: function () {
		packet.vMove[0] = 84;
		packet.vMove[1] = 1;

		return packet.pMove;
	},
	telemetryParse: function (binaryTelemetry) {
		telemetry.voltage  = packet._getUint16(binaryTelemetry, 4)/1000;
		telemetry.current  = packet._getUint16(binaryTelemetry, 6)/1000;
		telemetry.loopTime = packet._getUint16(binaryTelemetry, 8);
	}
}

class onScreenGamepad {
	constructor(obj, deadband, callback) {
		this.obj = obj,
		this.deadband = deadband || 0.05,
		this.callback = callback;
		
		this.isEvent = false;
		this.vector = {
				x: 0.0,
				y: 0.0,
			};
	}
	
	init () {
		this.obj.addEventListener('mousedown',   (event) => this.eventStart(event));
		this.obj.addEventListener('touchstart',  (event) => this.eventStart(event));
		
		this.obj.addEventListener('mouseup',     (event) => this.eventFinish(event));
		this.obj.addEventListener('mouseout',    (event) => this.eventFinish(event));
		this.obj.addEventListener('mouseleave',  (event) => this.eventFinish(event));
		this.obj.addEventListener('touchend',    (event) => this.eventFinish(event));
		this.obj.addEventListener('touchcancel', (event) => this.eventFinish(event));
		
		this.obj.addEventListener('mousemove',   (event) => this.eventMove(false, event));
		this.obj.addEventListener('touchmove',   (event) => this.eventMove(true, event));
	}
	
	eventStart() {
		this.isEvent = true;
		this.obj.classList.add('active');
	}
	
	eventFinish() {
		this.isEvent = false;
		this.vector.x = 0.0;
		this.vector.y = 0.0;
		this.display(0, 0);
		this.obj.classList.remove('active');
		
		this.callback(this.vector);
	}
	
	eventMove(isTouch, event) {
		let sendEvent = false;
		let x = ((isTouch ? event.targetTouches[0].clientX : event.clientX) - event.target.offsetLeft) / this.obj.offsetWidth*2-1;
		let y = ((isTouch ? event.targetTouches[0].clientY : event.clientY) - event.target.offsetTop) / this.obj.offsetHeight*2-1;
		if (Math.abs(x) <= this.deadband) x = 0;
		if (Math.abs(y) <= this.deadband) y = 0;
		if (x > 1) x = 1;
		if (x < -1) x =-1;
		if (y > 1) y = 1;
		if (y < -1) y =-1;
		if (!this.isEvent) {
			x = 0;
			y = 0;
		}
		if (this.vector.x !== x || this.vector.y !== y) {
			sendEvent = true;
		}
		this.vector.x =  x;
		this.vector.y = -y;
		
		if (sendEvent) {
			this.display(x, y);
			this.callback(this.vector);
		}
	}
	
	display(x, y) {
		this.obj.style.backgroundPosition = (x+1)/2*100 + '% ' + (y+1)/2*100 + '%';
	}
};

let control = {
	init() {
		let leftJ  = new onScreenGamepad(G('leftJ'),  0.05, this.leftJcallback);
		let rightJ = new onScreenGamepad(G('rightJ'), 0.05, this.rightJcallback);
		leftJ.init();
		rightJ.init();
	},
	leftJcallback(v) {
		vector.rotate.yaw = v.x;
	},
	rightJcallback(v) {
		vector.move.x = v.x;
		vector.move.y = v.y;
	}
}

control.init();
gui.init();
packet.init();
ws.init();
