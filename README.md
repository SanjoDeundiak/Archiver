<h1>Overview</h1>
This is my course work in KPI using Qt.
This app is an archiver based on Huffman coding (see for details http://en.wikipedia.org/wiki/Huffman_coding)

<h2>Functionality</h2>
1. Compress one or more files (of cource do not try to compress *.avi *.jpg *. mp3 etc. size wouldn't decrease)
2. Composing several files into 1 archive file (folders are now allowed)

<h3>Installation</h3>
1. To use this app you should have some QT libraries (see list of libraries in LIBRARIES.TXT, you can download
them on the Internet or ask me)
2. Executables Encode and Decode don't require installation, they are totally cross-platform, but they work only
by passing pathes of files in their command line arguments.
For coding and encoding there are available Windows Shell Context Menu Extensions (in folders ExplorerMenu and 
ExplorerMenu1). They are, of course, platform-dependent.
3. In order to use this apps with Windows Shell Extensions you should firstly register extension's dlls (use
cmd with administator's privileges, then type regsvr32 "path-to-dll") and then you should add path to Encode and
Decode executables in your register:
  1. Start regedit
  2. Go to HKLM//SOFTWARE
  3. Create subkey Deundiak//Compr with 2 string values named decode and encode with pathes to executables as values.

<h4>Usage</h4>
1. Stand-alone version
  1. Start program using command line
  2. Parameters are pathes to files that you need encode (for Encode) or path to archive that you need to be decoded
     (for Decode)

2. Version with extensions
  1. Open your Windows Shell
  2. Select files that you need to be encoded
  3. Click right button and choose appropriate menu item
  4. In order to unpack archive click right button on it and choose appropriate menu item
