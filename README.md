Squamigera: A tool for sorting the pixels on your screen.

When you feel disorganized, the squamigera project can help. It takes all those nasty unordered pixels on your screen and sorts them on their rgb value, giving you a nicely sorted screen.
As an example take this unsorted image below:

![unsorted](/res/unsorted.png)

Now once squamigera starts running, it will start sorting your pixels with a given algorithm, for example, for the random swap sorting algorithm we might get the following:

![progress](/res/progress.png)

Finally, after a while the image will start looking a lot better:

![sorted](/res/sorted.png)

NOTE: the program currently does not support screen scaling, so set the scale to 100% for it to capture the entire screen.

OPTIONS:
The program can take a sorting algorithm as argument, the accepted algorithms are:

insertion
merge
quick
random_swap