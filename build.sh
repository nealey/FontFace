#! /bin/sh -e

build () {
	echo $1
	[ $1 = Black ] && uuid=1 || uuid=0
	sed "s/%COLOR/$1/;s/%UUID/$uuid/" appinfo.json.tmpl > appinfo.json
	pebble build
	cp build/HelveticaFace.pbw HelveticaFace-$1.pbw
}

build Black
build White
