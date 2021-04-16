# AI_MazeCourseHW

Students:
Shay Rashinsky:	204736920
Yoni Agai:		308418367
Aviv Lazar:		311514236

How To Run:
open vs2017test/vs2017test.sln with visual studio 2017/2019
and use either build/run with debug or CTRL-F5 to build and run.

This Project is a great learning expirence, in order to
really benefit from it, the team decided to work on it like
a real project include the usage of:
- JIRA: for tickets handling (https://ai-game-course-project.atlassian.net/secure/RapidBoard.jspa?projectKey=AG&rapidView=1)
  we used jira for handling assignments (which student implement task) and for bug reporting.
- GitHub: source control, in order to remove visual studio files we splitted the project
  into:
	* includes: contains all headers of the project
	* src: contains all the custom sources of the project
	* app: contains the main of the program.
  you can check it out using the following link: https://github.com/ShayR22/AI_MazeCourseHW
  under branch: game_project.
- In the project files:
	* project_class_diagrm.mdj
	* AIGameProjectDesign.jpg
   which shows the design used by the team

Project Requirment:
- 2 teams x3 (2 regular bots, 1 support bot) to fight each other by AI algorithm.
- Have a map which contains block (Wall) Health Boxes and Ammo Boxes.
- Support bot can give health/ammo if another bot is close enough to him (2 cells).
- Path Finding is splitted to paths between rooms and paths inside the rooms.
- Bot logic is to search for health if low health, same for ammo, otherwise fight.
- Shootables will be grenades and bullets, where a bullet damage is decided by its
  distance (closer equal stronger)

Extra implemention by the team:
- multi thread collision detection, after each frame a collision logic component
  is being ran, to calculate all the collision in the game, this step is multithreaded
  between the different teams
- eye tracking, each bot has an eye which always follow its direction of moving/firing
- grande's fragments exploasion is random with different fragments size, the larger
  the fragment the slower it is, and the lower its damage (faster equal stronger)
- In order to run at the same pace on different machines (as the game is frame based) we
  added frameRate availiblity to change in the game main while compiling and FPS counter which
  shows in the bottom left corrner of the screen (Note: this is limited by the screen HZ).
- smart physics, instead of calculating an enitre field map of saftey we used linear algebra
  to pre determine if line of sight exist and the target trajectory, this means when a bot
  shoot at another bot a simple calculation is done at the start to calculate the real target
  which is the wall behind and if a hit would occur (bot will not dodge the projectile) the 
  CollisionLogic component will handle it.
