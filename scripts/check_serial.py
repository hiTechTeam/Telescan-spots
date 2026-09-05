import argparse
import time

import serial


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("port")
    parser.add_argument("--seconds", type=float, default=5)
    args = parser.parse_args()

    with serial.Serial(args.port, 115200, timeout=0.2) as device:
        device.dtr = False
        device.rts = True
        time.sleep(0.1)
        device.rts = False

        deadline = time.monotonic() + args.seconds
        while time.monotonic() < deadline:
            line = device.readline()
            if line:
                print(line.decode("utf-8", errors="replace").rstrip())


if __name__ == "__main__":
    main()
