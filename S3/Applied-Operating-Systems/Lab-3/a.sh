#!/bin/bash

LSTART="$(ps -p "$$" -o lstart --no-headers | sed "s/ /_/g" )"

mkdir "$HOME/test" && {
    echo "catalog test was created successfully" > "$HOME/report"
    touch "$HOME/test/$LSTART"
}

ping "www.net_nikogo.ru" || echo "$(date "+%d/%m/%y_%H:%M:%S") : 'www.net_nikogo.ru' is not avaliable." >> "$HOME/report"
