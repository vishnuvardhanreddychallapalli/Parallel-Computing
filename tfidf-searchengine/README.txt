Assignment2 : Readme file:
All java files are present in assignment2 folder in search package.

Note:Please execute the files in the order specified below:
1.DocWordFrequency.java.
2.TermFrequency.java.
3.Tfidf.java
4.Search.java

1. How to execute DocwordFrequency.java file?
Steps to execute DocwordFreq.java file:
1.Here just pass the input path cantrbry from hdfs and output path is hardcoded as "DocWordFrequency".
2.So at the end of the program all the Docword frquencies will be stored in "DocWordFrequency" in hdfs.
3.Export the jar file and rename the file as DocWordFreq.jar.
4.Now open the cloud era terminal and execute the following commands in order to run the DocwordFreq.java file.
                a.   hadoop jar DocwordFrequency.jar Search.DocwordFrequency cantrbry
5.I hardcoded the output path as DocWordFrequency so all output docwordfrequencies will be stored in DocWordFrequency directory of Hdfs.
6.To visualize the files from hdfs please type the following commands.
       a.   hadoop fs -cat DocWordFrequency/part-r-00000




2.How to execute the TermFrequency.java file?
Steps to execute TermFrequency.java file:
Note:Please make sure to execute the DocwordFrequency.java before executing TermFrequency.java.
1.Export the jar file and rename the file as TermFrequency.jar.
2.Now open the cloud era terminal and execute the following commands in order to run the DocwordFreq.java file.
                a.   hadoop jar TermFrequency.jar Search.TermFrequency
3.I hardcoded the output path as TermFrequency so all output termfrequencies will be stored in TermFrequency directory of Hdfs.
4.To visualize the files from hdfs please type the following commands.
       a.   hadoop fs -cat TermFrequency/part-r-00000





3.1. How to execute Tfidf.java file?
Steps to execute Tfidf.java file:
Note: Please make sure to execute the DocwordFrequency.java and TermFrequency.java prior to the exection of Tfidf.java respectively in order to get the results else you will get an errors saying file not present.
1.Here no need to pass the input path because the path is hardcoded in the program as TermFrequency in the program  and output path is hardcoded as Tfidf.
2.So at the end of the program all the Tfidf will be stored in Tfidf directory in hdfs.
3.Export the jar file and rename the file as Tfidf.jar.
4.Now open the cloud era terminal and execute the following commands in order to run the Tfidf.java file.
                a.   hadoop jar Tfidf.jar Search.Tfidf cantrbry
Note: Please pass cantrbry file in the command line because i need the total file count of cantrbry in order to compute tfidf.
5.I hardcoded the output path as Tfidf so all outputs  will be stored in Tfidf directory of Hdfs.
6.To visualize the files from hdfs please type the following commands.
       a.   hadoop fs -cat Tfidf/part-r-00000



4.How to execute Search.java file?

Steps to execute Search.java file:
1.Please make sure that the Tfidf.java is executed prior to the execution of the Search.java because the input to Search comes from the output of Tfidf.java which will be stored in Hdfs at "Tfidf".
2.Export the jar file and rename the file as Search.jar.
3.Now open the cloud era terminal and execuete the following command.
                  a.   hadoop jar Search.jar Search.Search query1.out "computer science"
                  b.   hadoop jar Search.jar Search.Search query2.out "data analysis"
please make sure to add the output path as above for example in point 3(a) and 3(b) i passed the ouptut file as query1.out and query2.out
4.Once you execute the above commands you will get the outputs stored in HDFS with file name query1.out and query2.out which contains the results related to "computer science" and "data analysis" respectively.
5.Observations i tried executing the queries like "computer science" and computer science and i got same results.
6.To visualize the files from hdfs please type the following commands in cloudera terminal.
               a.  hadoop fs -cat query1.out/part-r-00000
               b.  hadoop fs -cat query2.out/part-r-00000 
