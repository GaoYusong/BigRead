Run these commands to free caches
---------------------------------

	sync
	sudo sh -c "echo 3 > /proc/sys/vm/drop_caches"

weather_onefile.cpp use next commands to generate input file
------------------------------------------------------------
	tar zxvf weather.tz -O >input.txt 2>&1
