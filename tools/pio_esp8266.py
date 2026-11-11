"""Set up the DSTIKE variant and embedded web assets before ESP8266 builds."""

from pathlib import Path
import subprocess
import sys

Import("env")
root = Path(env.subst("$PROJECT_DIR"))
# Arduino core 2.7.4's builder predates project-local variants_dir support.
# An absolute variant path works with its existing os.path.join calls.
env.BoardConfig().update("build.variant", str(root / "boards/variants/esp8266/dstike"))
subprocess.run([sys.executable, str(root / "webconverter.py")], cwd=root, check=True)
