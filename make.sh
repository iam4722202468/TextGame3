if [ "$1" = "" ] || [ "$1" = "--all" ] ; then
	cd ./build && make
	cd ../ && mv build/game .
	cd ./build_win && make
	cd ../ && mv build_win/game.exe .
elif [ "$1" = "--clean" ]
then
	rm ./build/*.o
	rm ./build_win/*.o
	rm ./game
	rm ./game.exe
fi
