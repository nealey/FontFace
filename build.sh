#! /bin/sh -e

echo PATH=$PATH:$HOME/pebble-dev/PebbleSDK-3.1/bin
cd $(dirname $0)

mkdir -p pbw

build () {
    font=$1
    color=$2
    uuid=$3
    name=${font}Face-$color
    tgt=pbw/$name.pbw
    
    sed "s/%COLOR/$color/;s/%FONT/$font/;s/%UUID/$uuid/" appinfo.json.tmpl > appinfo.json
    pebble build
    cp build/FontFace.pbw $tgt
    echo $tgt
}

if [ -n "$1" ]; then
    pbw=$(build $1 White 879db5ec-8bfc-41ca-8988-96da8ae23d14)
    pebble install $pbw
else
    while read font color uuid; do
	build $font $color $uuid
    done <<EOF
Helvetica White cd20e38f-4e2d-4a01-83a5-a02c67fa4ce4
Helvetica Black cd20e38f-4e2d-4a01-83a5-a02c67fa4ce5
Ubuntu White 534e5853-7be7-447e-a67d-b236ac9f4f50
Ubuntu Black 534e5853-7be7-447e-a67d-b236ac9f4f51
LobsterTwo White 09f6a566-5e2e-4927-8bb2-e84de862af78
LobsterTwo Black 09f6a566-5e2e-4927-8bb2-e84de862af79
AveriaSerifLibre White e03bd7a7-a8a5-4584-98aa-44496d74d766
AveriaSerifLibre Black e03bd7a7-a8a5-4584-98aa-44496d74d767
ExpletusSans White 4ba792cf-26c4-4c3a-a194-4db5ae7e4be8
ExpletusSans Black 4ba792cf-26c4-4c3a-a194-4db5ae7e4be9
Rosario White 6bd61758-576d-4895-9e73-3979f46a9b02
Rosario Black 6bd61758-576d-4895-9e73-3979f46a9b03
EOF
fi
