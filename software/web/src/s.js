function G(id) {
    return document.getElementById(id);
}

var vector = {
    x    : 0,
    y    : 0,
    z    : 0,
    angZ : 0
};

var axisInvert = {
    x    : false,
    y    : true,
    z    : true,
    angZ : false,
    invert: function (axis) {
		return axis?-1:1;
	}
};

var gamepad = {
    init: function () {
		window.addEventListener("gamepadconnected", gamepad.connect);
		window.addEventListener("gamepaddisconnected", gamepad.disconnect);
    },
    
    state: false,

    connect: function(evt) {
		clearInterval(gamepad.updateInterval);
		gamepad.updateInterval = setInterval(gamepad.update, 50);
		gamepad.state = true;
    },

    disconnect: function(evt) {
		clearInterval(gamepad.updateInterval);
		gamepad.updateInterval = null;
		gamepad.state = false;
		
		failsafe.setFS();
    },

    update: function() {
		var gp = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
		if (gp.length === 0) {
			return;
		}
		gp = gp[0];
		
		vector.angZ = gamepad.axeData(gp.axes[0])*axisInvert.invert(axisInvert.angZ);
		vector.z    = gamepad.axeData(gp.axes[1])*axisInvert.invert(axisInvert.z);
		vector.x    = gamepad.axeData(gp.axes[2])*axisInvert.invert(axisInvert.x);
		vector.y    = gamepad.axeData(gp.axes[3])*axisInvert.invert(axisInvert.y);
    },

    axeData: function (raw) {
		if (raw >= -gamepad.deadband && raw <= gamepad.deadband) raw = 0;
		if (raw < -1) raw = -1;
		if (raw >  1) raw =  1;
		return parseInt(raw*10000)/10000;
    },

    deadband: 0.005,
    updateInterval: null,
};

var ws = {
    ws: null,
    status: false,
    error: false,
    updateInterval: null,
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

			ws.updateInterval = setInterval(ws.update, 50);
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
    }
};

var failsafe = {
	setFS: function () {
		vector.angZ = 0;
		vector.z    = 0;
		vector.x    = 0;
		vector.y    = 0;
		
		gamepad.state = false;
	}
};

var gui ={
    init: function () {
		gui.obj.vector_x    = G('vector_x');
		gui.obj.vector_y    = G('vector_y');
		gui.obj.vector_z    = G('vector_z');
		gui.obj.vector_angZ = G('vector_angZ');
		gui.obj.gamepadState = G('gamepad_state');
		
		gui.updateInterval = setInterval(gui.update, 100);
		document.addEventListener("visibilitychange", gui.onVisibilityChange);
    },
    update: function () {
		gui.showVector();
    },
    showVector: function () {
		gui.obj.vector_x.innerHTML    = vector.x;
		gui.obj.vector_y.innerHTML    = vector.y;
		gui.obj.vector_z.innerHTML    = vector.z;
		gui.obj.vector_angZ.innerHTML = vector.angZ;
		
		gui.obj.gamepadState.innerHTML = gamepad.state?'connected':'disconnected';
    },
    obj: {},
    updateInterval: null,
    onVisibilityChange: function () {
		if (document.visibilityState == "hidden") {
			failsafe.setFS();
		}
	}
};

var packet = {
	init: function () {
		packet.pMove = new ArrayBuffer(10);
		packet.vMove = new Uint8Array(packet.pMove);
	},
	_norm1: function (value) {
		return (value+1)*10000;
	},
    _uint16: function (view, num, offset) {
		view[offset]   = (num>>8)&255;
		view[offset+1] = num&255;
    },

    move: function () {
		packet.vMove[0] = 77;
		packet.vMove[1] = gamepad.state?1:0;
		packet._uint16(packet.vMove, packet._norm1(vector.x),    2);
		packet._uint16(packet.vMove, packet._norm1(vector.y),    4);
		packet._uint16(packet.vMove, packet._norm1(vector.z),    6);
		packet._uint16(packet.vMove, packet._norm1(vector.angZ), 8);
		return packet.pMove;
	}
}

packet.init();
gui.init();
ws.init();
gamepad.init();
