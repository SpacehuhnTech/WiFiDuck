"""Keep both firmware versions in sync and increment the fork's build number."""

import argparse
from pathlib import Path
import re

VERSION_PATTERN = re.compile(r'^#define CURRENT_VERSION "(\d+\.\d+\.\d+\.\d+)"$', re.MULTILINE)
VERSION_FILES = ("esp_duck/version.h", "atmega_duck/version.h")


def update(root, check=False):
    paths = [root / name for name in VERSION_FILES]
    matches = [VERSION_PATTERN.search(path.read_text(encoding="utf-8")) for path in paths]
    if any(match is None for match in matches):
        raise ValueError("Both version headers must define a four-part CURRENT_VERSION")
    versions = [match.group(1) for match in matches]
    if len(set(versions)) != 1:
        raise ValueError("ATmega and ESP8266 firmware versions do not match")
    if check:
        return versions[0]
    parts = [int(part) for part in versions[0].split(".")]
    parts[-1] += 1
    version = ".".join(str(part) for part in parts)
    for path in paths:
        source = path.read_text(encoding="utf-8")
        source = VERSION_PATTERN.sub('#define CURRENT_VERSION "' + version + '"', source)
        with path.open("w", encoding="utf-8", newline="\n") as output:
            output.write(source)
    return version


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parent.parent)
    parser.add_argument("--check", action="store_true", help="Check versions without changing files")
    args = parser.parse_args()
    try:
        print(update(args.root, args.check))
    except ValueError as error:
        parser.error(str(error))


if __name__ == "__main__":
    main()
