# pinball_space_cadet_forever
A tool to get infinite balls in Pinball Space Cadet (the classic Windows game).

It overwrites the ball decreasing instruction with a NOP so that the ball counter is not decreased and also overwrites the UI's 'Demo' option functionality with code to trigger a game end manually and thus save your score.


The program has two versions:
- One that patches the static game `.exe` file.
- One that patches the binary on runtime.


## Static .exe patch usage
### To ****enable**** infinite balls
```python
python3 patchPinball.py pathToYourPinballProgram.exe patch
```


### To **disable** infinite balls
```python
python3 patchPinball.py pathToYourPinballProgram.exe unpatch
```


## **Finishing the game and saving your score**
- Just click the game UI's **Demo** option menu.
