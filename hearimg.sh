#!/bin/bash

get_win_id()
{
    xwininfo | sed -n 's/.*Window id: \([^ ]*\).*/\1/p'
}

running=1
trap running=0 2

pipe_name=screenshots
win_id=$(get_win_id)
if [[ -p $pipe_name ]]; then
    rm $pipe_name
fi
mkfifo $pipe_name

./hearimg --continuous < $pipe_name &

while [[ $running -ne 0 ]]; do
    import -window $win_id ppm:-
done > $pipe_name
