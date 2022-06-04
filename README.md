# pinball_space_cadet_forever
<p>Tool that allows you to enable infinite balls in Pinball Space Cadet. You can trigger a game end whenever you want.</p>
<p>The program works by patching the ball decreasing instruction (converting it to a NOP) and the menu's Demo option functionality (using it to trigger a game end manually).</p>

## Usage
- If you want to ****enable**** the hack: `python patchPinball.py pathToYourPinball.exe patch`.
- If you want to **disable** the hack: `python patchPinball.py pathToYourPinball.exe unpatch`.
- To **finish the game and save your score**, click **Demo** in the menu.
