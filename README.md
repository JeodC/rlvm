### COMPILING UNDER LINUX
`$ apt install -y build-essential scons libboost-all-dev libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsndfile1-dev libfreetype6-dev gettext`

`$ scons --release --puresdl --portmaster --fullstatic -j$(nproc)`

## KNOWN ISSUES
Little Busters' baseball doesn't work because it is implemented as a
Windows DLL instead. For the same reason, the dungeon crawling minigame
in Tomoyo After does not work.

## GETTING INVOLVED
Little Buster's baseball minigame appears to be implemented by a DLL,
not RealLive bytecode. Figuring out what its doing is the first step to
getting it working.