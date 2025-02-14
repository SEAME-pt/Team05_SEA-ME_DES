FROM arm64v8/ubuntu:22.04
ENV DEBIAN_FRONTEND=noninteractive
ENV LD_LIBRARY_PATH=/usr/lib/arm-linux-gnueabihf:/usr/lib/aarch64-linux-gnu:/opt/qt5/lib

RUN sed -i 's|http://archive.ubuntu.com/ubuntu|http://ports.ubuntu.com/ubuntu-ports|' /etc/apt/sources.list && \
    sed -i 's|http://security.ubuntu.com/ubuntu|http://ports.ubuntu.com/ubuntu-ports|' /etc/apt/sources.list && \
    apt-get update --fix-missing

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    gcc-aarch64-linux-gnu \
    g++-aarch64-linux-gnu \
    libopencv-dev \
    curl \
    qtbase5-dev \
    qt5-qmake \
    qtbase5-dev:arm64 \
    qt5-qmake:arm64 \
    libqt5widgets5:arm64 \
    qtchooser \
    qtbase5-dev-tools \
    libssl-dev \
    libqt5core5a:arm64 \
    libqt5gui5:arm64 \
    qtdeclarative5-dev \
    qtpositioning5-dev \
    libqt5network5:arm64 \
    libqt5serialport5-dev:arm64 \
    libc6-dev:arm64 libglib2.0-dev:arm64 \
    libicu-dev:arm64 libpng-dev:arm64 libjpeg-dev:arm64 \
    qtbase5-private-dev

RUN git clone -b 5.15 https://code.qt.io/qt/qtmqtt.git /qtmqtt && \
    cd /qtmqtt && \
    qmake && \
    make && \
    make install

RUN apt-get update && apt-get install -y fonts-noto-core

RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

#Set working directory to /workspace
WORKDIR /workspace

#Copy local project files into the container
COPY . /workspace
COPY .env /workspace/.env

#Build project
RUN rm -rf build && mkdir -p build && cd build && \
    rm -rf CMakeCache.txt CMakeFiles/ && \
    cmake .. && \
    make VERBOSE=1 && \
    chmod +x ./digital_cluster

ENTRYPOINT ["./digital_cluster"]