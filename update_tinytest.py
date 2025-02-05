"""
Allows the update of TinyTest from the official repo.

Arguments :
- (positional)           path :          The path to your local TinyTest file.
- (positional, optional) upgrade_type :  One of the following :
    - 'major' (upgrade to the next major version, default behaviour), 
    - 'minor' (upgrade to the next minor version), 
    - 'patch' (upgrade to the next patch).
- (non-positional, optional) help, -h :  Shows this message
- (non-positional, optional) force, -f : Runs the program without user input, and forces the upgrade/overwrite of existing files.

Example use :
    ./update_tinytest.py "include/testing/tinytest.hpp" minor
Will upgrade tinytest at the specified location to the next minor version,
e.g. if local version is 1.1.3, and online versions 2.0.2, 1.2.7, and 1.1.5 exist,
your local install will be upgraded to version 1.2.7

Dependencies :
This module requires the Python 'requests' module.
"""
import sys
import enum
import re
try:
    import requests
except ImportError:
    print("This module requires the Python 'requests' library to be installed.", file=sys.stderr)
    sys.exit(1)

# Constants
TINYTEST_ONLINE_REPO_HOST  = "https://raw.githubusercontent.com"
TINYTEST_ONLINE_REPO_OWNER = "megat69"
TINYTEST_ONLINE_REPO_NAME  = "tinytest"
TINYTEST_ONLINE_FILE_URL   = f"{TINYTEST_ONLINE_REPO_HOST}/{TINYTEST_ONLINE_REPO_OWNER}/{TINYTEST_ONLINE_REPO_NAME}/refs/heads/main/src/tinytest.hpp"


# Checks the command line arguments
if "-h" in sys.argv or "help" in sys.argv:
    print(__doc__)
    sys.exit(0)
force_mode: bool = ("-f" in sys.argv or "force" in sys.argv)
if force_mode:
    try:
        sys.argv.remove("-f")
    except ValueError: pass
    try:
        sys.argv.remove("force")
    except ValueError: pass

if len(sys.argv) == 1:
    print(__doc__)
    sys.exit(1)

# Registers the path to the local TinyTest installation
local_tinytest_path: str = sys.argv[1]

# Loads the update type from the command line
class UpgradeType(enum.Enum):
    major = enum.auto()
    minor = enum.auto()
    patch = enum.auto()

# If the upgrade type wasn't specified, defaults to 'major'
if len(sys.argv) == 2:
    upgrade_type: UpgradeType = UpgradeType.major
else:
    if sys.argv[2].lower() in ("major", "minor", "patch"):
        upgrade_type: UpgradeType = {
            "major": UpgradeType.major,
            "minor": UpgradeType.minor,
            "patch": UpgradeType.patch,
        }[sys.argv[2].lower()]
    else:
        print(f"ERROR: Upgrade type should be 'major', 'minor', or 'patch', got {sys.argv[2]}", file=sys.stderr)
        sys.exit(1)

# Loads the online version of TinyTest
request_data: requests.Response = requests.get(TINYTEST_ONLINE_FILE_URL)
if request_data.status_code != 200:
    print("ERROR: Could not fetch TinyTest online version", file=sys.stderr)
    sys.exit(1)

# Gets the code of the tinytest online version
version_find_regex: str = r'#define TINYTEST_VERSION "(\d+\.\d+\.\d+)"'
tinytest_online_version_code: str = request_data.content.decode("utf-8")

# Finds the online version
tinytest_online_version_number: str = re.findall(version_find_regex, tinytest_online_version_code)[0]

# Finds the local version
try:
    with open(local_tinytest_path, "r", encoding="utf-8") as f:
        tinytest_local_version_number: str = re.findall(version_find_regex, f.read())[0]
except FileNotFoundError:
    print(f"ERROR: File '{local_tinytest_path}' does not exist", file=sys.stderr)
    sys.exit(1)
except PermissionError:
    print(f"ERROR: Permissions to access '{local_tinytest_path}' are missing", file=sys.stderr)
    sys.exit(1)

# Displays the versions
print(f"Local version:\t{tinytest_local_version_number}\nOnline version:\t{tinytest_online_version_number}")

# Splits the versions
local_version = [int(n) for n in tinytest_local_version_number.split('.')]
online_version = [int(n) for n in tinytest_online_version_number.split('.')]

do_upgrade: bool = False
if upgrade_type == UpgradeType.patch and local_version[0] == online_version[0] and local_version[1] == online_version[1] and local_version[2] < online_version[2]:
    do_upgrade = True
elif upgrade_type == UpgradeType.minor and local_version[0] == online_version[0] and (local_version[1] < online_version[1] or local_version[2] < online_version[2]):
    do_upgrade = True
elif upgrade_type == UpgradeType.major and (local_version[0] < online_version[0] or local_version[1] < online_version[1] or local_version[2] < online_version[2]):
    do_upgrade = True

if not do_upgrade:
    print("According to your settings, an upgrade is not recommended.")
    sys.exit(0)

# Asks the user if they want to upgrade
if not force_mode:
    try:
        user_answer = input("An upgrade is available ! Do you wish to apply it ? (y/N) ").lower()[0]
    except IndexError:
        user_answer = 'n'
    
    if user_answer == 'n':
        print("Aborting install.")
        sys.exit(0)

# Upgrades the current tinytest installation
with open(local_tinytest_path, 'w', encoding="utf-8") as f:
    f.write(tinytest_online_version_code)

print("Upgrade applied !")