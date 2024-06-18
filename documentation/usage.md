# Usage

The raytracer binary can be run in three different ways:

1. **Regular Mode**: This mode is used to just render a scene file.

    ```bash
    ./raytracer <SCENE_FILE>
    ```

    Replace `<SCENE_FILE>` with the path to your scene file.

2. **Server Mode**: This mode is used to run the binary as a server.

    ```bash
    ./raytracer <SCENE_FILE> -s <port> <nb clients>
    ```

    Replace `<SCENE_FILE>` with the path to your scene file, `<port>` with the port number you want the server to listen on, and `<nb clients>` with the number of clients that can connect to the server.

3. **Client Mode**: This mode is used to run the binary as a client.

    ```bash
    ./raytracer -c <ip> <port>
    ```

    Replace `<ip>` with the IP address of the server and `<port>` with the port number of the server.

Please ensure that the SFML and libconfig libraries are properly installed and linked before running the binary.
*If not, please refer to the install.md file of the documentation directory*