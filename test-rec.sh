
EASY="sleep 2"


rm -rf rectest
mkdir rectest

# build/bin/fa_record_streaming -o rectest/app_id1.raw
# $EASY
# build/bin/fa_record_streaming -o rectest/app_id2.raw
# $EASY
# build/bin/fa_record_streaming -o rectest/app_id3.raw
# $EASY
# build/bin/fa_record_streaming -o rectest/app_id4.raw
# $EASY
# build/bin/fa_record_streaming -o rectest/app_id5.raw
# $EASY
# build/bin/fa_record_streaming -o rectest/app_id6.raw
# $EASY
# build/bin/fa_record_streaming -o rectest/app_id7.raw
# $EASY
# build/bin/fa_record_streaming -o rectest/app_id8.raw
# $EASY
# build/bin/fa_record_streaming -o rectest/app_id9.raw
# $EASY

build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg1.ogg
$EASY
build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg2.ogg
$EASY
build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg3.ogg
$EASY
build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg4.ogg
$EASY
build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg5.ogg
$EASY
build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg6.ogg
$EASY
build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg7.ogg
$EASY
build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg8.ogg
$EASY
build/bin/fa_record_streaming --ogg-vorbis -o rectest/app_ogg9.ogg
$EASY

