# 🎵 MP3 Tag Reader & Editor (C Language)

## 📌 Project Overview

The MP3 Tag Reader & Editor is a command-line based application developed in C to read and modify metadata tags from MP3 files.

This project parses the ID3v1 tag format, which stores metadata in the last 128 bytes of an MP3 file, and allows users to extract and update fields such as Title, Artist, Album, Year, and Genre.

The project demonstrates strong understanding of low-level file handling and binary data manipulation, which are essential in systems programming and embedded development.

---

## 🚀 Features

- Read MP3 metadata:
  - Title
  - Artist
  - Album
  - Year
  - Genre
- Edit existing tag fields
- Binary file parsing
- Structured modular design
- Command-line argument support
- Error handling for invalid or corrupted files

---

## 🛠️ Technologies Used

- C Programming
- Standard I/O Library
- File Handling (`fopen`, `fread`, `fwrite`, `fseek`)
- Structures
- Pointers
- String Manipulation
- Modular Programming

---

## 🧠 Technical Concept

This project works based on the ID3v1 tag structure:

- The last 128 bytes of an MP3 file contain metadata.
- First 3 bytes: "TAG" identifier
- Next 30 bytes: Title
- Next 30 bytes: Artist
- Next 30 bytes: Album
- Next 4 bytes: Year
- Remaining bytes: Comment and Genre

The program uses `fseek()` to move to the last 128 bytes of the file and reads the metadata using `fread()`.

For editing, it modifies the required field and writes the updated data back using `fwrite()`.

---


---

## ▶️ How to Compile and Run

### 🔧 Compile (Single File Version)

```bash
gcc main.c -o mp3tag
```

---

### ▶️  How to Run

#### 🔍 View MP3 Metadata

```bash
./mp3tag -v sample.mp3
```

#### ✏️ Edit MP3 Metadata

```bash
./mp3tag -e -A "hello world" sample.mp3
-t : To edit Title
-y : To edit Year
-a : To edit Album
-A : To edit Artist
-c : To edit Comment
-g : To edit Genre
```


