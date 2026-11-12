
import socket
import threading

ESP32_IP = "192.168.4.1"
ESP32_PORT = 5000


def receive_data(sock):
    """Receive newline-delimited messages."""
    buffer = ""

    try:
        while True:
            data = sock.recv(1024)

            if not data:
                print("\nESP32 disconnected.")
                break

            buffer += data.decode("utf-8", errors="replace")

            while "\n" in buffer:
                line, buffer = buffer.split("\n", 1)

                line = line.strip()

                if line:
                    print(f"\nESP32 -> {line}")

    except OSError as error:
        print(f"\nReceive stopped: {error}")


def main():
    print("Connecting to ESP32...")

    try:
        with socket.create_connection(
            (ESP32_IP, ESP32_PORT),
            timeout=5
        ) as sock:

            sock.settimeout(None)

            print("Connected successfully!")
            print("Type PING or STATUS.")
            print("Type EXIT to disconnect.\n")

            receiver = threading.Thread(
                target=receive_data,
                args=(sock,),
                daemon=True
            )

            receiver.start()

            while receiver.is_alive():
                command = input("Command: ").strip()

                if command.upper() == "EXIT":
                    break

                if command:
                    sock.sendall(
                        (command + "\n").encode("utf-8")
                    )

    except (OSError, EOFError, KeyboardInterrupt) as error:
        print(f"\nConnection ended: {error}")


if __name__ == "__main__":
    main()
