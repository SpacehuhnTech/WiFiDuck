# Fork synchronization

The fork combines official WiFiDuck master (`fb8f859`, version 1.2.0) with
the history of jLynx/WiFiDuck master (`d6e8de8`, version 1.0.0.4), plus mouse
support. The resulting fork firmware starts at version **1.3.0.0**.

Both histories are retained. The official source tree provides the current
firmware, keyboard layouts, serial bridge, and web editor. Features unique to
the old fork were reviewed and carried forward as follows:

| Original fork change | Result |
| --- | --- |
| SSID/password/channel saving, including spaces | Already merged upstream; retained |
| Startup script execution and selection | Upstream autorun is retained, with `startup` as an alias for `autorun` |
| Persisted startup path | The settings layout and magic number are unchanged; the old startup field maps to autorun |
| Reset clears the startup path | Retained; resetting settings disables autorun |
| EEPROM initialization | Upstream initializes EEPROM from settings; its initialization sequence is retained |
| Automatic four-part version increments | Updated workflow increments the build component in both firmware headers on master pushes |
| Web version display | Upstream WebSocket display is retained; a firmware cookie also supplies the version on About/404 pages |
| NeoPixel enabled on pin 7 | Retained as a default; board-selected DotStar/RGB LEDs take precedence; `NO_LED` disables the default |
| File write without a newline | Retained as `spiffs::write(filename, text, false)`; existing calls keep their newline |
| Initialized stream-read delimiter state | Already present upstream; retained |
| Board checks and explanatory HID comment | Already present upstream; retained |

The existing experimental branches remain available in the fork. This update
integrates published master behavior; it does not merge the incomplete
`eeprom-fix` branch or replace the newer upstream UI with older branch versions.

## Versioning

`esp_duck/version.h` and `atmega_duck/version.h` define the same
`CURRENT_VERSION`. Both sketches expose it through `VERSION`.
The workflow uses the repository's GitHub Actions token and does not create tags.
Version-only commits are excluded from the push trigger.

To check versions without modifying them:

```
python tools/bump_version.py --check
```

For the next feature release, change the first three components in both headers
and reset the fourth (build) component. The workflow then increments that build
number on subsequent master updates.

## Validation

Run `python tests/run_tests.py` with a C++14 compiler and Node.js available.
The host tests cover mouse reports, parsing, transport reset/queue handling,
keyboard compatibility, script stopping/repetition, migration of old settings,
startup aliases, persistence, version increments, version display, and embedded
web assets. Both Arduino sketches also require real firmware compilation.

Flash both microcontrollers together because mouse cleanup uses communication
protocol 5. USB enumeration and physical pointer/LED behavior need hardware
verification.
