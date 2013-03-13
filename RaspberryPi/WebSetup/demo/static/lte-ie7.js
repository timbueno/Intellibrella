/* Use this script if you need to support IE 7 and IE 6. */

window.onload = function() {
	function addIcon(el, entity) {
		var html = el.innerHTML;
		el.innerHTML = '<span style="font-family: \'icoweather\'">' + entity + '</span>' + html;
	}
	var icons = {
			'icon-untitled' : '&#x21;',
			'icon-untitled-2' : '&#x22;',
			'icon-untitled-3' : '&#x23;',
			'icon-untitled-4' : '&#x24;',
			'icon-untitled-5' : '&#x25;',
			'icon-untitled-6' : '&#x26;',
			'icon-untitled-7' : '&#x27;',
			'icon-untitled-8' : '&#x28;',
			'icon-untitled-9' : '&#x29;',
			'icon-untitled-10' : '&#x30;',
			'icon-untitled-11' : '&#x31;',
			'icon-untitled-12' : '&#x32;',
			'icon-untitled-13' : '&#x33;',
			'icon-untitled-14' : '&#x34;',
			'icon-untitled-15' : '&#x35;',
			'icon-untitled-16' : '&#x36;',
			'icon-untitled-17' : '&#x37;',
			'icon-untitled-18' : '&#x38;',
			'icon-untitled-19' : '&#x39;',
			'icon-untitled-20' : '&#x40;',
			'icon-untitled-21' : '&#x41;',
			'icon-untitled-22' : '&#x42;',
			'icon-untitled-23' : '&#x43;',
			'icon-untitled-24' : '&#x44;'
		},
		els = document.getElementsByTagName('*'),
		i, attr, html, c, el;
	for (i = 0; i < els.length; i += 1) {
		el = els[i];
		attr = el.getAttribute('data-icon');
		if (attr) {
			addIcon(el, attr);
		}
		c = el.className;
		c = c.match(/icon-[^s'"]+/);
		if (c) {
			addIcon(el, icons[c[0]]);
		}
	}
};