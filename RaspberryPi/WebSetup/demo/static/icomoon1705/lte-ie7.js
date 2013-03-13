/* Load this script using conditional IE comments if you need to support IE 7 and IE 6. */

window.onload = function() {
	function addIcon(el, entity) {
		var html = el.innerHTML;
		el.innerHTML = '<span style="font-family: \'icomoon\'">' + entity + '</span>' + html;
	}
	var icons = {
			'icon-home' : '&#xe000;',
			'icon-warning' : '&#xe001;',
			'icon-notification' : '&#xe002;',
			'icon-question' : '&#xe003;',
			'icon-info' : '&#xe004;',
			'icon-info-2' : '&#xe005;',
			'icon-blocked' : '&#xe006;',
			'icon-cancel-circle' : '&#xe007;',
			'icon-checkmark-circle' : '&#xe008;',
			'icon-spam' : '&#xe009;'
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
		c = c.match(/icon-[^\s'"]+/);
		if (c && icons[c[0]]) {
			addIcon(el, icons[c[0]]);
		}
	}
};