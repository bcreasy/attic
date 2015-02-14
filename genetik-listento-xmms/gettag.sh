#!/bin/sh

# 06.29.03 - genetik <genetik@caffeine.nu>
# this script was written to provide a generic format of the tag information
#  regardless of being v1.2 or v2.
#  requires `id3info` of the id3lib package because mp3info does not support
#  id3v2 tags. (which is why i came to write-in the id3v2 support)

FILE=$1

# does mp3info complain about not having a 1.x tag?
TEST=`mp3info -p '%y' "$FILE"`

if [ "${TEST}" != "" ]; then # id3v1.x
   ARTIST=`mp3info -p '%a' "$FILE"`
   ALBUM=`mp3info -p '%l' "$FILE"`
   TITLE=`mp3info -p '%t' "$FILE"`
   GENRE=`mp3info -p '%g' "$FILE"`
   YEAR=`mp3info -p '%y' "$FILE"`
   LENGTH=`mp3info -p '%S' "$FILE"`
else # id3v2
   ARTIST=`id3info "$FILE" | grep "TPE1" | cut -d: -f2 | sed s/^\ //`
   ALBUM=`id3info "$FILE" | grep "TALB" | cut -d: -f2 | sed s/^\ //`
   TITLE=`id3info "$FILE" | grep "TIT2" | cut -d: -f2 | sed s/^\ //`
   GENRE=`id3info "$FILE" | grep "TCON" | cut -d: -f2 | cut -d\) -f2`
   YEAR=`id3info "$FILE" | grep "TYER" | cut -d: -f2 | sed s/^\ //`
   LEN=`id3info "$FILE" | grep "TLEN" | cut -d: -f2 | sed s/^\ //`
   LENGTH=`echo "${LEN}/1000" | bc`
fi
echo "${ARTIST}:${ALBUM}:${TITLE}:${GENRE}:${YEAR}:${LENGTH}"
