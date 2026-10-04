import argparse
import datetime
import glob
import zipfile
import logging
import os
import platform
import shutil
import sys
import subprocess
import requests
from ghp_import import ghp_import
from src import a2fp

exe_ext = ""
if platform.system().lower().startswith("windows"):
    exe_ext = ".exe"
    
try:
    import git
    repo = git.Repo(".")
    branch_name = repo.active_branch.name
    branch_sha = repo.active_branch.commit.hexsha
    __version_git__ = f"Source: {branch_name}:{branch_sha}"
except:
    __version_git__ = ""

# Get the current version number
with open("version.txt", "r") as f:
    __version__ = f.read().strip()
    
# Get the current time/date
__version_date__ = datetime.datetime.now().isoformat(timespec='minutes', sep=" ") 


def download_file(url, filename) -> bool:
    """Download a file from a given URL and save it to a specified filename.
    
    :param url: str The URL to download the file from.
    :param filename: str The local filename to save the downloaded file.
    :return: bool True if the download was successful, False otherwise.
    """
    try:
        # Send a GET request to the URL, enabling streaming for large files
        response = requests.get(url, stream=True)
        response.raise_for_status()  # Raise an HTTPError for bad responses (4xx or 5xx)

        # Open the local file in binary write mode
        with open(filename, 'wb') as file:
            # Iterate over content in chunks to handle large files efficiently
            for chunk in response.iter_content(chunk_size=8192):
                file.write(chunk)
        log.info(f"File '{filename}' downloaded successfully from '{url}'")
        return True
    except requests.exceptions.RequestException as e:
        log.warning(f"Error downloading file from '{url}': {e}")
        return False


def find_compiler() -> None:
    """
    Check to see if the cc65 CLI toolchain is installed, and if not, download it.
    """
    cc_name = os.path.join("cc65", "bin", f"cc65{exe_ext}")
    if os.path.exists(cc_name):
        return
    try:
        shutil.rmtree("cc65")
    except OSError:
        pass
    os.mkdir("cc65")
    url = "https://sourceforge.net/projects/cc65/files/cc65-snapshot-win64.zip"
    zip_filename = os.path.join("cc65", "cc65_win.zip")
    if not download_file(url, zip_filename):
        raise RuntimeError("Unable to download cc65 CLI tools.")
    try:
        with zipfile.ZipFile(zip_filename, 'r') as zip_ref:
            zip_ref.extractall("cc65")
        log.info(f"All files extracted from '{zip_filename}' to 'cc65'.")
    except zipfile.BadZipFile:
        log.error(f"Error: '{zip_filename}' is not a valid ZIP file.")
    except FileNotFoundError:
        log.error(f"Error: ZIP file '{zip_filename}' not found.")
    except Exception as e:
        log.error(f"An error occurred: {e}")            


def find_ciderpress() -> str:
    if not os.path.exists("ciderpress"):
        url = "https://github.com/fadden/CiderPress2/releases/download/v1.1.1/cp2_1.1.1_win-x86_sc.zip"
        if not download_file(url, "ciderpress.zip"):
            raise RuntimeError("Unable to download ciderpress tools.")
        # unpack
        try:
            os.makedirs("ciderpress")
            with zipfile.ZipFile("ciderpress.zip", "r") as zf:
                zf.extractall("ciderpress")
            os.unlink("ciderpress.zip")
        except Exception as e:
            log.warning(f"Unable to unpack ciderpress: {e}")
    cpapp = os.path.join("ciderpress", "cp2")
    if platform.system().lower().startswith("windows"):
        cpapp += ".exe"
    log.info(f"Using CiderPress2: {cpapp}")
    return cpapp


def find_doxygen() -> str:
    if not os.path.exists("doxygen"):
        url = "https://www.doxygen.nl/files/doxygen-1.18.0.windows.x64.bin.zip"
        if not download_file(url, "doxygen.zip"):
            raise RuntimeError("Unable to download doxygen tools.")
        # unpack
        try:
            os.makedirs("doxygen")
            with zipfile.ZipFile("doxygen.zip", "r") as zf:
                zf.extractall("doxygen")
            os.unlink("doxygen.zip")
        except Exception as e:
            log.warning(f"Unable to unpack doxygen: {e}")
    doxapp = os.path.join(os.getcwd(), "doxygen", "doxygen")
    if platform.system().lower().startswith("windows"):
        doxapp += ".exe"
    log.info(f"Using doxygen: {doxapp}")
    return doxapp


def clean(remove_cli_tools: bool = False) -> None:
    """
    Clear out the "build" directory and remove any 'intermediate' build files
    
    :param remove_cli_tools: bool If True, remove the cc65 directory as well.
    :return: None
    """
    try:
        shutil.rmtree("build")
        shutil.rmtree("html")
    except OSError:
        pass
    if remove_cli_tools:
        try:
            shutil.rmtree("cc65")
            shutil.rmtree("ciderpress")
            shutil.rmtree("doxygen")
        except OSError:
            pass
    os.mkdir("build")
    os.mkdir("html")

    build_files = glob.glob(os.path.join("src","*.o"))
    build_files.extend(glob.glob(os.path.join("src","*.lst")))
    for filename in build_files:
        try:
            os.unlink(filename)
        except OSError:
            pass

def build(verbose: bool = False, symbols: bool = False, debug: bool = False) -> None:
    """
    Build the library from source files.

    :param verbose: bool If True, include verbose output.
    :param symbols: bool If True, generate listing files (.lst).
    :return: None
    """
    try:
        shutil.rmtree("build")
    except OSError:
        pass
    os.mkdir("build")

    # get tool paths
    find_compiler()
    asm = os.path.join("cc65", "bin", f"ca65{exe_ext}")
    ar = os.path.join("cc65", "bin", f"ar65{exe_ext}")
    ln = os.path.join("cc65", "bin", f"cl65{exe_ext}")       
    lib_bins = []
    
    with open(os.path.join("src","asfp_vers.inc"), "w") as fp:
        fp.write(f'AS_VERSION: .asciiz "{__version__}"')
    
    # assemble library sources
    lib_sources = ["apple2_asfp.s"]
    for name in lib_sources:
        filename = os.path.join("src", name)
        cmd = [asm, "-t", "apple2"]
        if verbose:
            cmd.append("-v")
        if symbols:
            cmd.append("--listing")
            cmd.append(filename.replace(".s", ".lst"))
        cmd.append(filename)
        log.info(f"Assembling: {filename}")
        result = subprocess.run(cmd, capture_output=True, text=True)
        if result.returncode != 0:
            log.error(f"assembling: {filename}: {result.stdout}: {result.stderr}")
            sys.exit(1)
        lib_bins.append(filename.replace(".s", ".o"))
            
    # build library
    lib_name = os.path.join("build", "libasfp.a")
    cmd = [ar, "a", lib_name]
    cmd.extend(lib_bins)
    log.info(f"Building library: {lib_name}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        log.error(f"ar: {lib_name}: {result.stdout}: {result.stderr}")
        sys.exit(1)
        
    # include the header
    shutil.copy(os.path.join("src", "apple2_asfp.h"), "build")
    # include the test program source
    shutil.copy(os.path.join("src", "testmain.c"), "build")
    # build a zip file for distribution
    shutil.make_archive("a2fp_release", 'zip', "build")
    
    # build test app
    test_name = os.path.join("SYSTEM", "FPTEST.SYSTEM#FF2000")
    test_src = os.path.join("src", "testmain.c")
    system_file = ["-C", "apple2-system.cfg"]
    cmd = [ln, "-O", "-t", "apple2", "-I", "."]
    cmd.extend(system_file)
    cmd.extend(["-o", test_name])
    cmd.append(test_src)
    cmd.append(lib_name)
    log.info(f"Building test app: {test_name}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        log.error(f"test app: {test_name}: {result.stdout}: {result.stderr}")
        sys.exit(1)
        
    # remove apple single header (58 bytes)
    with open(test_name, "rb") as f:
        data = f.read()
    if data[:4] == b'\x00\x05\x16\x00':
        data = data[58:]
    with open(test_name, "wb") as f:
        data = f.write(data)

    # build .po disk image
    ciderpresscli = find_ciderpress()
    log.info("Building .po disk image...")
    # Create a release .po image
    rel_filename = "a2fp_release.po"
    try:
        os.remove(rel_filename)
    except Exception:
        pass
    cmd = [ciderpresscli, "create-disk-image", rel_filename, "140K", "prodos"]
    result = subprocess.run(cmd, capture_output=True, text=True, check=True)
    log.info(f"Created release disk image: {result.stdout} {result.stderr}")
    cmd = [ciderpresscli, "rename", rel_filename, ":", f"A2FP_{__version__}"]
    result = subprocess.run(cmd, capture_output=True, text=True, check=True)
    log.info(f"Renamed release disk image: {result.stdout} {result.stderr}")

    # Copy system files - PRODOS, BASIC...  
    try:
        os.remove("SYSTEM/_FileInformation.txt")
    except Exception:
        pass
    cmd = [ciderpresscli, "add", "--strip-paths", rel_filename, "SYSTEM"]
    result = subprocess.run(cmd, capture_output=True, text=True, check=True)
    log.info(f"System files added to disk image: {result.stdout} {result.stderr}")

    # in release mode builds, remove SYSTEM/BASIC.SYSTEM
    if not debug:
        cmd = [ciderpresscli, "rm", rel_filename, "BASIC.SYSTEM"]
        result = subprocess.run(cmd, capture_output=True, text=True, check=True)
        log.info(f"System files added to disk image: {result.stdout} {result.stderr}")
    

def build_docs(verbose: bool = False) -> None:
    doxygen = find_doxygen()
    try:
        shutil.rmtree("html")
    except OSError:
        pass
    os.mkdir("html")
    original_dir = os.getcwd()
    os.chdir("docs")
    try:
        if not os.path.exists(doxygen):
            log.error(f"Could not find doxygen executable")
            sys.exit(1)
        cmd = [doxygen]
        env = os.environ.copy()
        env["ASFP_VERSION"] = __version__
        env["ASFP_YEAR"] = str(datetime.datetime.now().year)
        result = subprocess.run(cmd, capture_output=True, env=env, text=True)
        if result.returncode != 0:
            log.error(f"Doxygen error: {result.stdout}\n{result.stderr}")
            sys.exit(1)
        if verbose:
            log.info(f"Doxygen output: {result.stdout}\n{result.stderr}")
            log.info("Documentation built successfully")
    finally:
        os.chdir(original_dir)


def gh_pages(commit_str: str = "Update pages") -> None:
    """
    Deploy the current build directory to GitHub Pages.
    
    :return: None
    """
    build_docs()
    # Check if we are in a git repository
    if not os.path.exists(".git"):
        log.error("Not in a git repository")
    ghp_import('html', push=True, mesg=commit_str)


def five_bytes(value: str) -> bytearray:
    """Parse a string of 10 hex characters into 5 bytes

    :param value: The string to parse, 10 characters: 8080000000
    :type value: str
    :return: A five element bytearray
    :rtype: bytearray
    """
    # Check length
    if len(value) != 10:
        raise argparse.ArgumentTypeError(f"Must be exactly 10 characters, got {len(value)}")
    
    # Try parsing as hex to bytes
    try:
        return bytearray.fromhex(value)
    except ValueError:
        raise argparse.ArgumentTypeError("Must be a valid hexadecimal string")

    
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-V",
        "--version",
        action="version",
        version="%(prog)s {version}".format(version=__version__),
    )
    parser.add_argument("--verbose", action="store_true", default=False, help="Run in verbose mode")
    parser.add_argument("--logfile", help="Log file for verbose output", default="")
    
    cmd_parsers = parser.add_subparsers(help="Command", dest="cmd")
    cmd_parsers.required = True

    build_parser = cmd_parsers.add_parser("build", aliases=["fullbuild"],
                                          help="Rebuild the entire build directory contents")
    build_parser.add_argument("--symbols", action="store_true", default=False, help="Output the symbol listing.")
    build_parser.add_argument("--debug", action="store_true", default=False, help="Include BASIC.SYSTEM")

    docs_parser = cmd_parsers.add_parser("docs", help="Build the documentation for the project")
    
    clean_parser = cmd_parsers.add_parser("clean", help="Remove all build directory contents")
    clean_parser.add_argument("--full", help="Remove CLI and build directories", action="store_true", default=False,)
  
    gh_pages_parser = cmd_parsers.add_parser("ghpages", help="Rebuild & push 'build' directory to 'gh_pages' branch")
    gh_pages_parser.add_argument("--ghmsg", help=f"Commit message. default:'Release version:{__version__}'", 
                                 default=f"Release version:{__version__}")
   
    cvt_flt_as = cmd_parsers.add_parser("flt2as", help="Convert a float to AppleSoft floating point format")
    cvt_flt_as.add_argument("float", type=float, help="The float to convert")
    cvt_flt_as.add_argument("--varname", type=str, default="foo", help="The name of the variable to define. Default: 'foo'")
    cvt_flt_as.add_argument("--assembly",action="store_true", default=False, help="Output as assembly (.s) source. Default: output as C source.")

    cvt_as_flt = cmd_parsers.add_parser("as2flt", help="Convert an Applesoft floating point format to a float")
    cvt_as_flt.add_argument("asfloat", type=five_bytes, help="A 10-char hex string representing 5 bytes (e.g., 0102030405)")

    args = parser.parse_args()

    # Set up logging
    level = logging.INFO
    if args.verbose:
        level = logging.DEBUG
    log = logging.getLogger("applecfp")
    logging.basicConfig(filename=args.logfile, level=level)
    log.debug(f"Command line args: {args}")
    
    if args.cmd.endswith("build"):
        build(verbose=args.verbose, symbols=args.symbols, debug=args.debug)
    elif args.cmd == "clean":
        clean(remove_cli_tools=args.full)
    elif args.cmd == "ghpages":
        gh_pages(commit_str=args.ghmsg)
    elif args.cmd == "docs":
        build_docs(verbose=args.verbose)
    elif args.cmd == "flt2as":
        v = a2fp.float_to_applesoft_fp(args.float, verbose=args.verbose)
        if args.assembly:
            print(f"{args.varname}:\n    .byte ${v[0]:02X}, ${v[1]:02X}, ${v[2]:02X}, ${v[3]:02X}, ${v[4]:02X}  ; {args.float}")
        else:    
            print(f"AS_FAC_FP {args.varname} = {{0x{v[0]:02X}, {{0x{v[1]:02X}, 0x{v[2]:02X}, 0x{v[3]:02X}, 0x{v[4]:02X}}}}}; /* {args.float} */")
        exit(0)
    elif args.cmd == "as2flt":
        v = a2fp.applesoft_fp_to_float(args.asfloat, verbose=args.verbose)
        print(f"{v}")
        exit(0)
    else:
        print(f"Unknown command: {args.cmd}")
        parser.print_help()
        exit(-1)
    
    log.info("Operation complete")
    
    exit(0)

