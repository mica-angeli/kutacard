# Working With PlayStation Memory Cards

*Adapted from [this archived website](http://web.archive.org/web/20070110212056/http://www.darklock.com/thps/dexdrive.html)*

If you're writing programs that deal with PlayStation saves, you will need a strong understanding of the PlayStation memory card format and the normal PC-based formats used to store it. This page will detail exactly what these formats include, and how to work with them effectively.

## Native Sony Format

Internally, a PlayStation memory card has a single 1-megabit EEPROM chip. This chip can store a total of 128 kilobytes of data, divided into sixteen blocks. Each block, in turn, contains 64 frames, each of which is 128 bytes. While this arrangement may appear strange, it's primarily an optimisation measure, since the hardware reads and writes the card in one kilobit or 128 byte chunks.
The first thing most people say when they hear that a memory card has 16 blocks is generally "you mean FIFTEEN blocks". No, sixteen; the first block contains a directory of the other fifteen blocks. Each directory entry consumes one of the 64 frames in this block, and takes the following format.

* FRAME 0: Identification frame, used to determine whether the data is a valid PlayStation memory card.
* FRAMES 1 - 15: Directory frame, detailing the nature of the save data in the appropriate block.
* FRAMES 16 - 35: Reserved. Formatted as per normal directory frames, with the "reserved" block type and all implied data.
* FRAMES 36 - 62: Unused. All bytes in this region are set to 0xFF.
* FRAME 63: Write test. If it is desired to do a "test write" of data, this frame can be used without damaging the memory card.

The identification frame is constant, and always contains the following data. If the data differs from this, the card has not yet been formatted.

* The two characters "MC".
* 125 NUL bytes (0x00).
* The XOR byte (0x0E).

The XOR byte needs a little clarification. This byte is calculated through an XOR of the preceding 127 bytes; one simply loops through from byte to byte as follows:

```c
char xor=0;
for(int x=0;x<127;x++) xor^=frame[x];
frame[127]=xor;
```

Since any number XORed with itself is 0, you can verify this byte with the following code:

```c
char xor=0;
for(int x=0;x<128;x++) xor^=frame[x];
if(xor==0) return 1; // XOR code correct
return 0; // XOR code incorrect
```

In all sixteen directory frames, the XOR code is calculated and stored in the last byte of the frame. The format of other frames in the directory are as follows:

* BLOCK TYPE: The type of a given block is a 32-bit number from the following set of possibilities.
  * 81 (Q): Initial. This is the first block in a series. All saves must have an initial block. All fields in an initial block are as listed, and have the specified meaning.
  * 82 (R): Medial. This is not the first block in the series, but it is also not the last. Only occurs in saves with three or more blocks. Only the next block member and the XOR code are valid.
  * 83 (S): Final. This is the last block in a series. Only occurs in saves with two or more blocks. Only the next block member and the XOR code are valid.
  * 160: Formatted. This block either has never been used, or has been wiped from the card entirely to prevent undeletion. Only the XOR code is valid.
  * 161-163: Deleted block, original block type + 80. Only the XOR code is guaranteed to be valid, but a consistency check on the chain can verify the validity of other fields. If all relevant fields are valid in all blocks of the chain, the chain may be undeleted by restoring the original block types and recalculating the XOR codes for all blocks in the chain.
  * 17229: The "MC" string from the first directory block. Included only for completeness. Only the XOR code is valid.
  * 0xFFFFFFFF: Reserved. Do not use. A reserved block has a save size of zero and a next block pointer of 0xFFFFFFFF. Only the XOR code is meaningful.
* SAVE SIZE: the size of the save's data in bytes, as a 32-bit number. In practice, this number is 8192 * number of blocks, but there is no known reason why the number could not be an actual byte count. This information is only stored in the initial block, and is meaningless in medial or final blocks.
* NEXT BLOCK: 32-bit number; in a multiple block save (save size > 8192), this is the number of the next block in the chain minus one (0 - 14). If there is no next block or the entry is reserved, this field is 0xFFFFFFFF.
* TERRITORY CODE: 2 bytes. The first byte of this code is always "B", and the second specifies the game's territory: "A" for American markets, "E" for European, and "I" for Japanese.
* LICENSE CODE: 10 bytes. This is the Sony license string for the game which created the save. This string can be found on the spine of the CD case, in the game's manual, and on the game CD itself (usually under the ESRB rating). No license string is ever common to more than one game, even across territories, so this is a reliable indicator of which game created the save.
* SAVE CODE: 8 bytes. This code is used by the game itself to differentiate between multiple save files it has created. No two saves on the same card are permitted to have the same territory, license, and save code combination. The meaning of this save code is determined by the individual game, and the only rule regarding its use is that none of the meaningful characters may be 0x00 and any unused space be set to 0x00.
* CODE END: 1 byte, always 0x00. This is in place strictly to terminate the save code, so it may be addressed as a C/C++ string field.
* UNUSED: 94 bytes. Could be anything. Only meaningful in calculation of XOR code.
* XOR CODE: The standard XOR code of the previous 127 bytes.

If a game is released in multiple territories, you can USUALLY -- not always, but usually -- convert a save from one territory to another by changing the territory code and the license code. It is extremely rare that a game will change its internal data structures for another territory, but it does happen, so your mileage may vary on this matter.

There is a concept inherent in the memory card format of a "save chain", a series of blocks which comprise a single save. If you have a multiple block save, it is formatted as follows:

* An "initial" block is placed somewhere on the memory card. If the save needs more than one block, a blank slot is located on the card and stored in the "next block" field of this directory entry.
* If the save needs 2 or more additional blocks, a "medial" block is placed on the memory card in the blank slot located previously. A blank slot is located on the card and stored in the "next block" field of this directory entry.
* If the save needs only one additional block, a "final" block is placed on the memory card in the blank slot located previously. The "next block" field of this directory entry is set to 0xFFFFFFFF.

A basic consistency check can be performed on the card by ensuring that all 15 directory blocks contain pointers which are either 0xFFFFFFFF or point to a medial or final block. None of them should point to reserved, formatted, deleted, or initial blocks. Every medial block must contain a valid pointer, and every final block must have an 0xFFFFFFFF pointer.

Medial blocks must also not point to each other in a cycle. It is possible within the memory card format to have an initial block pointing to medial block 2, medial block 2 pointing to medial block 3, medial block 3 pointing to medial block 4, and medial block 4 pointing back to medial block 2. This is rather difficult to detect without maintaining some sort of counter, but this is easily done by keeping track of the save size as stated by the initial block: if the actual size of the save is greater than that reported by the first block, this is an error. Simply hold onto that size, and subtract 8192 from it every time you check a new block. If the size becomes negative, the chain is invalid.

The actual save data is raw binary information, with a save header at the beginning of initial blocks. This save header consists of one to four frames of data, with the first formatted in this fashion:

* 16-bit magic number 17,235 ("SC")
* 8-bit icon display: 17 - static, 18 - two frame, 19 - three frame. There is/was a bug in European PlayStation titles that caused screen corruption with animated icons, so it is recommended by Sony Computer Entertainment Europe that the icon display byte for European titles always be 17.
* 8-bit blocks used, 1-15. This can be compared to the directory information to validate the directory against the save.
* 64-byte save title in Shift-JIS character set, kanji only.
* 28-byte padding (0x00).
* 32-byte icon palette, 16 color references of 16 bits each, xBBBBBGGGGGRRRRR. WARNING: in Windows, this is the OPPOSITE of "normal" 16-bit color data, and must be specifically accounted for if you don't want the red and blue channels of the icon swapped.

The second through fourth frames contain icons. If the icon display byte is 19, there must be three frames; if it is 18, there must be two frames and may be three; if it is 17, there must be one frame and may be two or three. Many game manufacturers use all three icon frames, since this simplifies things greatly; rather than checking the icon display byte, you can simply assume that there are three icons and skip them. Each icon consists of 128 bytes of data, which are packed 16-bit palette references; each byte contains the color data for two pixels as a pair of indexes into the icon palette. This results in a 16 by 16 bitmapped icon.

The rest of the frames in the initial block contain save data formatted in any way desired by the game's developers. Any additional blocks consist entirely of such arbitrarily formatted data.

## Third-Party Formats

There are a lot of different ways to get a save from a memory card onto your PC's hard disk, and these ways sometimes involve sticking some additional information into a header at the beginning of the file. Most formats... SmartLink .PSM, WinPSM .PS, DataDeck .DDF, FPSX .MCR, ePSXe .MCD... don't stick any header on the data at all, so you can just read it in and treat it like a raw memory card.

All of these headers contain a signature at the top of the file. The three most common formats and their signatures are:

* InterAct DexDrive format (.GME): "123-456-STD", 3904 bytes
* Connectix Virtual Game Station format (.MEM): "VgsM", 64 bytes
* PlayStation Magazine format (.PSX): "PSV", 256 bytes

If a signature is not known, you can rely on the memory card's raw format by searching the file for the constant ID header in frame 0 of block 0. The next 128K or less of the file should be a valid PSX memory card. By "or less", I mean that some programs will OMIT any blank or unallocated blocks from the end of the memory card -- if only three save blocks on the card are in use, for example, saving the other twelve is pointless.

Finally, we must cover the single-save format used by the Xploder and Action Replay cartridges. This format is economical and flexible, holding only the bare minimum of information necessary to reconstruct the save in a 54 byte header. The first 21 bytes of this header are the proper territory, license, and game codes for the index block. This is followed by a 32-byte title in US-ASCII for convenience, and a final terminating NUL character. The rest of the file is the raw memory card data and can be written directly to memory card blocks, but the index frames must be calculated and updated manually.

There exists another single-save format with a 128 byte header containing a raw index frame for the initial block, which must be updated to match the destination card, and the raw save data. I have seen this format once, but I don't remember what it was called or where it came from. You may want to account for this possibility in your format detection logic.

## InterAct GME Format

The most common PC save format for PlayStation memory cards is by far the InterAct GME format, produced by the DexDrive. This is the format that every program MUST support both reading and writing; it is suggested that every program also support the AR/GS/XP format for single saves, which is also a rather common save format.

The InterAct GME file format consists of three basic parts.

* A header (64 bytes)
* Fifteen 256-byte save descriptions (3840 bytes)
* The memory card data itself (131,072 bytes)

The 64-byte header contains only nine actual data fields, some of which are unknown but apparently unused.

* 16 bytes, the string "123-456-STD" followed by five 0x00 characters.
* A 16-bit value, always 0
* A 16-bit value, always 1
* An 8-bit value, always 1
* 16 bytes, the bottom 8 bits of each directory entry's block type
* 1 byte unknown value, varies but modification never seems to change anything
* 16 bytes, the bottom 8 bits of each directory entry's next block pointer
* 1 byte unknown value, varies but modification never seems to change anything
* 9 bytes, unused

The description format is simply a single string of data, without formatting or control characters, any unused space at the end of the description filled with NUL bytes (0x00).

It is worth noting that the DexDrive format SOMETIMES omits any deleted or empty blocks at the end of the file. This was done in early versions of the DexDrive software, and has been discontinued with more recent versions. However, there are still GME files out there which are missing several blocks from the end of the file, and these missing blocks should be assumed empty.

## DexDrive Protocol

If you've read this far, and you're still with me, you're probably interested in writing some sort of program that will read and write GME files. If you have such an interest, the natural extension of this plan is to talk directly to the DexDrive and get the save data from the memory card yourself -- because, let's face it, the InterAct DexPlorer provided for this purpose is a complete piece of shit that does absolutely nothing useful. About the coolest thing it does is email saves to other people, and it never seems to do that right. (It can't figure out what my email client is, and keeps trying to send mail through AOL. I don't use AOL, so it fails miserably. Occasionally it will -- under circumstances I can't fathom -- try to send mail through Microsoft Exchange, which I also don't use, so it fails miserably again. When I tell it explicitly to send email through Netscape, it commits an access violation, throws an exception, and crashes.)

The DexDrive is a reasonably simple piece of machinery. It has about a half-dozen basic commands, most of which are pretty meaningless. Every command you send to the DexDrive begins with the letters IAI, for "InterAct Incorporated". This is followed by a command code, and may then be followed by a data parameter. The DexDrive will respond with a similar IAI, response, and potentially a data parameter.

The DexDrive is located by opening the first four serial ports in order, COM1 through COM4, and attempting to detect the DexDrive on that port. If the port cannot be opened, it is assumed that something else is using it. Since only the DexPlorer is supposed to be using the DexDrive, that means no DexDrive is on it, and that port is skipped.

Before you try to detect the DexDrive, you need to set up the serial port. This is a reasonably involved process. First, set up the port to use 4K internal buffers for data transfer, and then clear all pending send and receive operations. Now you need to turn off all flow control completely. If your system performs automatic CR/LF translation, turn that feature off too. The baud rate should be 38400, and the byte format should be 8 bits, no parity, 1 stop bit. After this is done, toggle DSR and RTS on for 200 ms, off for 200 ms, and then back on. Wait another 200 ms before trying to detect the DexDrive.

Detecting the DexDrive is really simple. All you need to do is send some random garbage to the port, and the DexDrive will return an error response "IAI!". The DexPlorer sends "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", but I prefer to send something else just because it doesn't really matter what you send. Lately, I've been sending "JUMP UP MY BUTT" because I think it's really funny when the DexDrive says "IAI!" back. Okay, so I'm easily amused.

After you've gotten an error back from the DexDrive, you need to figure out what kind of DexDrive it is -- PlayStation or N64. This is determined with the NUL command: "IAI\0\0". You can also send some data to the DexDrive with this command, by specifying the length of the data in the second '\0' character -- the DexPlorer uses 0x10 -- and the data itself immediately afterward. The DexPlorer generates sixteen random numbers and sends them to the DexDrive. I don't know why. I don't bother with that. The DexDrive will respond to this command with "IAI@", a status byte I can't figure out, and the string "PSX" or "N64" followed by one more byte of data which is always "D" on my DexDrive but always "F" for everyone else I've talked to. I usually get "IAI@+PSXD", though the '+' changes sometimes and I don't know why.

At this point, the DexPlorer sends "IAI'" ten times and ignores whatever the DexDrive says back. The DexDrive always says "IAI!", which is an error, so I don't know why it does this. I don't do it, and everything works fine.

Finally, you determine whether the DexDrive has a memory card inserted. This is done with the command "IAI" 0x01. The DexDrive will respond with IAI" if there is no memory card, and IAI# if there is. If a memory card is inserted and that memory card has changed since the last time you asked, the IAI# will be followed by the number of blocks on the memory card, which is always 0x10 for standard memory cards.

Some people have told me the DexDrive must be "turned on" with the command "IAI" 0x07 0x01 before you do anything, and turned off with "IAI" 0x07 0x00 when you stop working with it. I've never gotten a non-error response from this, but others tell me I should get the same response I get from "IAI" 0x01. Just to be safe, I send these commands and ignore the DexDrive's response.

The only useful operations on the card are reading and writing the frames on the card. This is done with the two commands 0x02 and 0x04.

To read a frame from the card, simply send the frame number as a 16-bit integer in little-endian byte order.

For frames 0-255, send: IAI 0x02 frame 0x00
For frames 256-511, send: IAI 0x02 frame-256 0x01
For frames 512-767, send: IAI 0x02 frame-512 0x02
For frames 768-1023, send: IAI 0x02 frame-768 0x03

The DexDrive will respond with "IAIA", the 128 bytes of data for that frame, and a sequence number of some sort that I can't figure out. It may have something to do with what you get in the IAI@ response.

To write a frame to the card, things are a bit more complicated. You send the frame number as a 16-bit integer in big-endian byte order, the binary reverse of those two bytes in the same order, the 128 bytes of data, and the standard XOR of all 132 bytes in this series. The DexDrive will respond with "IAI)" if things are fine, and "IAI!" if they're not. So for frame 532, you write:

```none
IAI 0x04 0x02 0x14 0x40 0x28 [128 bytes of data] [XOR]
```

That's pretty much all there is for the DexDrive. I'm working on a class library that will handle communication with the DexDrive in Windows, but I'm not sure when it will be done.

Pioneering work in this area was done by:

William Ono (wmono@users.sourceforge.net)
Ben Galliart (bgallia@users.sourceforge.net)
Brian Perkins (Brian_Robert_Perkins@brown.edu)
