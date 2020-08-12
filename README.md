# Orbatext
A simple program to help fit level names for Commander Keens 4-6.

Introduction
============

In Commander Keen 4-6, level names and level entry text are stored
in executable segments that are sized just large enough to fit them. 
This makes it difficult to create a new set of level names and level
introductions, since they have to each fit in the same set of boxes
that the original game uses. This is especially true when using 
more levels than the original games have.

Technical Background
====================

In Commander Keen 4-6, level names and level introductions consist
of two parts: a text block (as described above) and a pointer to
the text block. While by default, the pointers point to the 
beginning of the text block, you can actually have them point
anywhere within the text block. This can be seen when examining
the pointer patches:

In `%patch $30764 $1F040000RL`, the first five digits of
`$1F040000RL` refer to the address of the text segment,
while the last three refer to an offset within the segment.
Thus, if the level introduction ends with the level name, as in

`"Keen makes a run for\nthe Border Village"`

... you can use the same address for both the level introduction
and the level name, but with different offsets into each.


See http://www.shikadi.net/keenwiki/Patch:Level_Entry_Window#Level_Name_Pointer_Lists
for a list of the pointer locations.

See http://www.shikadi.net/keenwiki/Patch:Executable_Maps#Keen_5 for a full list of
executable segments (Keen 5 and 6 only; read the address column).
