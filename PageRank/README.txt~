Assignment3: Page-rank assignment.

This program consists of 5 java classes..
Order of execution:
1.All the file names are Hardcoded in the iterator_paging.java class(This is my Driver class)
2.So the order of execution happens in the following way.
  1.First Graph_Count(no_of_pages.java) is executed.
  2.Adjacency_List.java will be executed.
  3.Page_rank_initialization is executed.
  4.iterator_paging will be exceuting and clean ups are happening parallely.
  5.At the end of convergence the 10th iteration will be given input to sorting.java class to sort all pageranks in descending order.
3. So how to run this program?
   The running of this program is very simple this program expects you to give the input file and the output file.
   eg for input file can wiki-micro.txt,smallgraphs.txt,etc...
   for output file you can give any name as you wish.
4. Here i am writing the command which is used in my cloudera in order to run the input files you provided in canvas.

       1.command:       hadoop jar page_rank_algorithm.jar pagerank.iterator_paging simplewiki-20150901-pages-articles-processed.xml assignment3_big_wiki
5. Notes of caution:
    1.please make sure you do not have the hardcoded file names in your cloudera HDFS prior with same file names inorder for the efficient exection.
    2.If program gets terminated on any cast due to this hardcodings make sure to do the following.
    3.First try to clean the Graph_Count directory from HDFS as the following.
      hadoop fs -rm -r Graph_Count
    4. Secondly try to clean the Adjacency_List directory from HDFS for the following.
       hadoop fs -rm -r Adjacency_List
    5.Thirdly try to clean the intermediate niter files by using the following command.
      hadoop fs -rm -r niter*
    6. Rerun the program.

6. How to collect the outputs...
    1. if you run the command which i shown in 4.1 then you will get the output stored in asssignment3_big_wiki.
    2. So in order to collect the output to your local file.
         1. Hadoop fs -get assignment3_big_wiki
         2.This assignment3_big_wiki contains two files.
            a) assignment3_big_wiki/part-r-00000
            b) assignment3_big_wiki/Success
         3. YOu can view the output file however it is hard to visualize all the outputs in one shot on the terminal.if you wish to see it use the command hadoop fs -cat assignment3_big_wiki/part-r-00000
7. What this assignment3_big_wiki contains?
     This assignement3_big_wiki contains pageranks in desending order of its pageranks...
     so if you want top 100 just type the tail command.(Make sure to have this assignment_3_big_wiki in your local in order to run the head command.
        head -n 100 assignment3_big_wiki/part-r-00000
       The outputs which are going to see based on the previous command are top 100 pageranks in the big_wiki file.
   
