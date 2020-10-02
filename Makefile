CFLAGS = -g -Wall -Wpedantic
	
default: shash
	
shash: main.c printprompt.c directory.c echo.c ls.c run.c pinfo.c history.c nightswatch.c env.c jobs.c
	gcc $(CFLAGS) main.c printprompt.c directory.c echo.c ls.c run.c pinfo.c history.c nightswatch.c env.c jobs.c -o shash -lm
clean:
	rm shash
