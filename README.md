### HAVE TO UPDATE THIS DOC...
# pinball_space_cadet_forever
A tool that to enable infinite balls in Pinball Space Cadet. You can trigger a game end whenever you want (to save your score).

The program works by patching the binary. It overwrites the ball decreasing instruction (converting it to a NOP) and the menu's 'Demo' option functionality (using it to trigger a game end manually).

## Usage
- If you want to ****enable**** the hack: `python patchPinball.py pathToYourPinballProgram.exe patch`.
- If you want to **disable** the hack: `python patchPinball.py pathToYourPinballProgram.exe unpatch`.
- To **finish the game and save your score**, click **Demo** in the menu.
