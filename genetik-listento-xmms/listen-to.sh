#!/bin/sh

# 06.26.03 - genetik <genetik@caffeine.nu>
# ogg vorbis tag reading support added
#   requires: /usr/ports/audio/vorbis-tools/ [http://www.vorbis.com/files/1.0/unix/]
#
# 06.28.03 - genetik <genetik@caffeine.nu>
# id3v2 tag reading support added
#   requires: /usr/ports/audio/id3lib/ [http://id3lib.sourceforge.net/]
#           : ./gettag.sh

USERNAME=""
PASSWORD=""

if [ -z "${USERNAME}" ]; then
   echo 'Please edit this file and add the USERNAME'
   exit 1
fi
if [ -z "${PASSWORD}" ]; then
   echo 'Please edit this file and add the PASSWORD'
   exit 1
fi

LOGIN="-d USER=${USERNAME} -d PASS=${PASSWORD}"
COMMAND='curl -o /dev/null -A ListenTo_xmms1.1 --url http://www.listen-to.com/postmp3.php'
LAST=''
WAIT=10

TRANSFORM="-e s/'/%27/g;s/\"/%22/g;s/ /+/g;s/&/%26/g"

while [ '1' ]; do
   FILE=`xmms-file 2>/dev/null`
   OGGTEST=`echo $FILE | grep ogg$` # does the current file end with .ogg?

   if [ ! -z "${FILE}" ]; then

      if [ ! -z "${OGGTEST}" ]; then # this is an ogg file
         ARTIST=`ogginfo "$FILE" | grep ARTIST | cut -d= -f2 | sed "${TRANSFORM}"`
         ALBUM=`ogginfo "$FILE" | grep ALBUM | cut -d= -f2 | sed "${TRANSFORM}"`
         TITLE=`ogginfo "$FILE" | grep TITLE | cut -d= -f2 | sed "${TRANSFORM}"`
         GENRE=`ogginfo "$FILE" | grep GENRE | cut -d= -f2 | sed "${TRANSFORM}"`
         YEAR=`ogginfo "$FILE" | grep DATE | cut -d= -f2 | sed "${TRANSFORM}"`
            MINUTES=`ogginfo "$FILE" | grep "Playback length" | cut -d: -f2 | sed s/m//`
            SECONDS=`ogginfo "$FILE" | grep "Playback length" | cut -d: -f3 | sed s/s//`
         LENGTH=`echo "($MINUTES*60)+$SECONDS" | bc`
      else # this is an mp3 file (or not an ogg file)
         # gettag.sh will get the tag information from the file and put the
         #  info into a colon-delimited string - artist:album:title:genre:year:length
         # then we'll want to parse what we get into a style that listen-to can deal with
         TAGS=`./gettag.sh "$FILE" 2>/dev/null`
         ARTIST=`echo $TAGS | cut -d: -f1 | sed "${TRANSFORM}"`
         ALBUM=`echo $TAGS | cut -d: -f2 | sed "${TRANSFORM}"`
         TITLE=`echo $TAGS | cut -d: -f3 | sed "${TRANSFORM}"`
         GENRE=`echo $TAGS | cut -d: -f4 | sed "${TRANSFORM}"`
         YEAR=`echo $TAGS | cut -d: -f5 | sed "${TRANSFORM}"`
         LENGTH=`echo $TAGS | cut -d: -f6`
      fi

      CURRENT=${ARTIST}${ALBUM}${TITLE}${GENRE}${YEAR}${LENGTH}
      echo $CURRENT # for debugging purposes

      if [ "${LAST}" != "${CURRENT}" ]; then
         if [ ! -z "${ARTIST}" ]; then
            ARGS="${ARGS} -d ARTIST=${ARTIST}"
         fi
         if [ ! -z "${ALBUM}" ]; then
            ARGS="${ARGS} -d ALBUM=${ALBUM}"
         fi
         if [ ! -z "${TITLE}" ]; then
            ARGS="${ARGS} -d TITLE=${TITLE}"
         fi
         if [ -z "${ARGS}" ]; then
            continue # the following aren't interesting w/o the previous
         fi
         if [ ! -z "${GENRE}" ]; then
            ARGS="${ARGS} -d GENRE=${GENRE}"
         fi
         if [ ! -z "${YEAR}" ]; then
            ARGS="${ARGS} -d YEAR=${YEAR}"
         fi
         if [ ! -z "${LENGTH}" ]; then
            ARGS="${ARGS} -d LENGTH=${LENGTH}"
         fi

         # nothing to upload, move on.
         if [ -z "${ARGS}" ]; then
            continue
         fi

         ${COMMAND} ${LOGIN} ${ARGS} 2>/dev/null

         # we'll want to reset these so, if for some reason a tag
         #  can't be rendered, it won't keep reporting the same
         #  song
         ARTIST=""
         ALBUM=""
         LENGTH=""
         TITLE=""
         GENRE=""
         YEAR=""

         LAST=${CURRENT}
      fi
   fi

   sleep ${WAIT}
done
