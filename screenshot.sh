#! /bin/sh -e

cd $(dirname $0)

mkdir -p screenshots
for pbw in pbw/*.pbw; do
    bn=$(basename $pbw .pbw)
    img=screenshots/$bn.png

    if [ -f $img ]; then
	echo "$img already exists, skipping"
	continue
    fi

    pebble install $pbw
    sleep 0.75
    fn=$(pebble screenshot 2>&1 | grep -o pebble-screenshot_.*png)
    sleep 0.5
    mv $fn $img
    echo $img
done
