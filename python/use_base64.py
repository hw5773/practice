import base64
import argparse
import logging

def run(num):
    decoded = base64.b64decode(num)
    logging.info("Decoded: {}".format(decoded))

def command_line_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--decode", metavar="<number to be decoded>", help="number to be decoded", type=str, required=True)
    parser.add_argument("-l", "--log", metavar="<log level (DEBUG/INFO/WARNING/ERROR/CRITICAL>", help="Log level (DEBUG/INFO/WARNING/ERROR/CRITICAL)", type=str, default="INFO")
    args = parser.parse_args()
    return args

def main():
    args = command_line_args()
    log_level = args.log
    logging.basicConfig(level=log_level)

    run(args.decode)

if __name__ == "__main__":
    main()
