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
and the level name, but with different offsets into each. Another
consequence of this is that, since all strings are null-terminated,
you can fit multiple strings into the same text segment, and then
just use the offset to pick the individual string. Finally, you 
can simply point to the same text either for both the introduction
and the level name, and even point to the same intro/name for 
multiple levels. This program allows you to do all these things
automatically and easily.


See http://www.shikadi.net/keenwiki/Patch:Level_Entry_Window#Level_Name_Pointer_Lists
for a list of the pointer locations.

See http://www.shikadi.net/keenwiki/Patch:Executable_Maps#Keen_5 for a full list of
executable segments (Keen 5 and 6 only; read the address column).

Usage
=====

This program is fairly simple; it takes a list of level introductions
and names, and produces both a list of text patches and pointer patches.
The full formatting details can be found in FORMAT.txt, but the
basic format is a first line with the episode number and other
switches, followed by a list of level introductions and names.
In most cases, you can just use the following intro line:

`--episode x`

where `x` is the episode number.

Level Entries
-------------

Pipes are used to separate introductions from names. For example,

`Keen makes a run for\nthe |Border Village`

would produce a patch that uses the same text box for both
intro and name, but uses different offsets for each. On the other
hand, using two pipes, as in

`Keen makes a run for\nthe Border Village||Border Village`

would produce output that uses different text boxes for the intro and
name, unnecessasrily using space that could be used elsewhere.
This is usually only used when the level introduction *doesn't*
end with the level name, as in

`Keen bravely enters the\nBloog Control Centre,\nlooking for Molly||Bloog Control Centre`

You can also just use no pipes at all to use the same text for both
the level introduction and the level name, which is usually best used
for levels that are supposed to be inacessible, e.g.

`High Scores`

You can also reference level intros and names that are used elsewhere;
this is useful for when multiple levels that have a similar theme and
thus can use the same introduction. To use a level name/intro defined
elsewhere, start the entry with a star, followed by a reference to
the desired entry and name. For example,

`* e12 n14`

... means "use the level introduction from level 12, and the level name
from level 14". You can even use a different level's name as the intro
text, and vice-versa. For more details, see FORMAT.txt.

Links
=====

PCKF topic:
https://pckf.com/viewtopic.php?f=4&t=11670

KeenWiki: General Text Patching info:
https://keenwiki.shikadi.net/wiki/Patch:Text_patches

KeenWiki: Level Entry Window Patches:
https://keenwiki.shikadi.net/wiki/Patch:Level_Entry_Window
