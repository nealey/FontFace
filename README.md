FontFace
========

Watch Face for Pebble, like the built-in "Simplicity",
but with a better font.

This project actually builds a whole lot of .pbw files,
each with the same functionality but different appearance.

I like fonts, okay.

Building
--------

The `pebble` SDK tool makes a lot of assumptions
about how your directory is set up.
In order to build multiple projects from the same source tree,
I created a `build.sh` script,
which shuffles things into the right places,
runs `pebble build`,
then renames the generated files.

You can also use `build.sh`
to experiment with new fonts.
Simply drop the `-Regular` and `-Bold` variants
of your font into `resources/fonts`,
and run `build.sh $FONTNAME`.
