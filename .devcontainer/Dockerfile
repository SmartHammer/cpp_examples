FROM alpine:latest

# Install C++ development tools
RUN apk update && apk add --no-cache \
    # Build essentials
    build-base \
    gcc \
    g++ \
    clang \
    clang-dev \
    clang-extra-tools \
    # Build tools
    cmake \
    ninja \
    make \
    # Version control
    git \
    # Utilities
    bash \
    sudo \
    shadow \
    && rm -rf /var/cache/apk/*

# Note: clang-format and clang-tidy are included in clang-extra-tools

# Install rho_oss (reMarkable CMake library)
RUN git clone https://github.com/reMarkable/rho-oss.git /opt/rho-oss

# Add rho_oss to CMAKE_MODULE_PATH so CMake can find it
ENV CMAKE_MODULE_PATH="/opt/rho-oss:${CMAKE_MODULE_PATH}"

# Create a non-root user
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /bin/bash \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME
WORKDIR /workspace
