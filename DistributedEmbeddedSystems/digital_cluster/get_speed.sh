#!/bin/bash
end=$((SECONDS+40)) # Stop after x seconds
while [ $SECONDS -lt $end ]; do
    echo $((RANDOM % 10))
    sleep 0.3
done