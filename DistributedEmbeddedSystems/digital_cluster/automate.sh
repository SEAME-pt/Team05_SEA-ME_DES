#!/bin/bash

# Step 1: Build Docker image for ARM platform
docker buildx build --platform linux/arm64 --load -t cross-env .

# Step 2: Remove the old executable from the local system (if it exists)
if [ -f "./digital_cluster" ]; then
    echo "Removing existing executable from local machine"
    rm ./digital_cluster
fi

# Step 3: Copy the executable from the Docker container (after build)
echo "Copying new executable from Docker container"
docker cp $(docker create cross-env):/workspace/build/digital_cluster ./digital_cluster

# Step 4: Remove the old executable from the JetRacer (Raspberry Pi)
echo "Removing existing executable from JetRacer"
ssh jetracer@10.21.221.67 "rm -f /home/jetracer/qt_cluster/build/digital_cluster"

# Step 5: Copy the new executable to the Raspberry Pi
echo "Copying new executable to Raspberry Pi"
scp ./digital_cluster jetracer@10.21.221.67:/home/jetracer/qt_cluster/build

