# Automata
An automata project, that has been done for as the homework.So what was the homework? 
So basically we will have an .txt file lets say its name is finiteautomata.txt, and it has to have 4 rows.
first row should have the set of states (Q)
second row should have the vocabulary
third row should have the trasition function
forth row should have the final and accepting states (F)

An example finiteautomata.txt file would be like this : 
{q,p,r}
{0,1}
{(<q,0>,p),(<q,1>,r),(<p,0>,q),(<p,1>,r),(<r,0>,r),(<r,1>,r)}
{r}


The homework takes these automata file and examine
a) Is the language empty ? 
b) Is the string w in language ? 
c) Does minimization can be done on Automata ? if its save it minimal version into the minimaldfa.txt
d)Is L infinite ? 
e)find out if 2 automatas are same or not ? return true if they are, return false if they are not.

How does this code works ? User enters a number between 1-5. 
According to his/her choice an answer will return which is either true or false.
If user enters 1 , he/she ll get the answer of a part of the homework
If user enters 2 , he/she ll get the answer of b part of the homework
and so on.

Most of comments are written in english, but some are written in turkish.All of the comments are written for me to remember what i did before except the ones in main. 

If you have any problems with the code pls contact me.
