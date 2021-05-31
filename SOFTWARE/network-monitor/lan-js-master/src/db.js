/*
 * devices.js - a database of <img> fingerprints from the web interfaces of
 * popular routers and other LAN devices.
 *
 * MIT license.
 * @author joev
 */
(function(){

// set up namespaces
this.lan = this.lan || {};
this.lan.db = this.lan.db || {};

// Most of this database came from the jslanscanner project (https://code.google.com/p/jslanscanner).
// Big thanks to Gareth Heyes (thespanner.co.uk) for creating the database
// and allowing me to relicense it under MIT.
// I'm still deciding what to do with this exactly.

this.lan.db.devices = [
  {
    make: "Phrozen",
    model: "Sonic XL 4k",
    fingerprints: [
      {
        type: 'image',
        url: '/assets/images/static/phrozen_logo_w.png',
        width: 2262,
        height: 705
      }
    ]
  },
  {
    make: "Phrozen",
    model: "Shuffle XL",
    fingerprints: [
	{
		type: 'image',
		url: '/static/logo.png',
		width: 79,
		height: 18
	}
    ]
  },
  {
    make: "UGS",
    model: "CNC Router",
    fingerprints: [
      {
        type: "image",
        url: ":8080/favicon.ico",
        width: 32,
        height: 32
      },
      {
        type: "js",
        url: ":8080/runtime-es2015.js",
        expression: "true"
      }
    ]
  }
];



this.lan.db.manufacturers = {
  Cisco: {

  },
  Linksys: {
    default_addresses: [
      '192.168.0.1', '192.168.1.1'
    ]
  },
  Dlink: {
    default_addresses: [
      '192.168.1.30',
      '192.168.1.50'
    ]
  },
  Motorola: {
    default_addresses: [
      '192.168.0.1'
    ]
  },
  Thomson: {

  }
};

}).call(window);
