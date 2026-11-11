"""Check automatic versioning without changing repository files."""

from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from bump_version import update


class VersionTests(unittest.TestCase):
    def test_repository_versions_match(self):
        self.assertRegex(update(ROOT, check=True), r"^\d+\.\d+\.\d+\.\d+$")

    def test_build_increment_and_validation(self):
        with tempfile.TemporaryDirectory(prefix="wifiduck-version-test-") as directory:
            root = Path(directory)
            headers = [root / name / "version.h" for name in ("esp_duck", "atmega_duck")]
            for header in headers:
                header.parent.mkdir()
                header.write_text('#pragma once\n#define CURRENT_VERSION "1.3.0.9"\n')
            self.assertEqual(update(root, check=True), "1.3.0.9")
            self.assertEqual(update(root), "1.3.0.10")
            self.assertEqual(headers[0].read_bytes(), headers[1].read_bytes())
            original = headers[0].read_bytes()
            headers[1].write_text('#define CURRENT_VERSION "1.2.0.0"\n')
            with self.assertRaises(ValueError):
                update(root)
            self.assertEqual(headers[0].read_bytes(), original)
            headers[1].write_text("malformed version\n")
            with self.assertRaises(ValueError):
                update(root)
            self.assertEqual(headers[0].read_bytes(), original)


if __name__ == "__main__":
    unittest.main()
