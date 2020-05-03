package Search;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.StringTokenizer;
import org.apache.avro.generic.GenericData.Array;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.lib.jobcontrol.ControlledJob;
import org.apache.hadoop.mapreduce.lib.jobcontrol.JobControl;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;


public class Search extends Configured implements Tool {
public static class TokenizerMapper5
extends Mapper<Object, Text, Text, Text>{
	private Text word1 = new Text();
	private Text word2 = new Text();

//private final static IntWritable one = new IntWritable(1);

// here we are getting the input from the output of the Docwordfrequency.java i.e we are getting input from TFidf which is the file from hdfs

public void map(Object key, Text value, Context context
             ) throws IOException, InterruptedException {

	 //String word3="yellow";
	 //String word4="hadoop";
	 Configuration conf=context.getConfiguration();
	 String query=conf.get("test");
	 if(!query.isEmpty())
	 {
	   String[] query_words=query.split(" ");
	   String [] arr=value.toString().split(",");
	   String curr_key=arr[0].toString();
	   // splitting based on the delimiter
	 String [] curr_file=curr_key.split("#####");
	 String curr_file_name=curr_file[1];
	 for(int i=0;i<query_words.length;i++)
	{
	 if(curr_file[0].equals(query_words[i].toLowerCase()))
	 {
		// Text word1 = new Text();
		// Text word2 = new Text();
	     word1.set(curr_file_name);
	     word2.set(arr[1].toString());
	    context.write(word1,word2);
	  // break;
	 }
   }
  }
}
}
/// once we got the key value pairs which contains filenames and their respective scores of tfidf for each file so we are adding all tfidf score inorder to measure the amount of relevance
public static class IntSumReducer2
extends Reducer<Text,Text,Text,Text> {
	
	private Text word1 = new Text();
	private Text word2 = new Text();

public void reduce(Text key, Iterable<Text> values,
                Context context
                ) throws IOException, InterruptedException {
	if(!key.toString().isEmpty())
	{
	 String smallans="";
	 Double total_score=new Double(0);
	 for(Text val:values)
	  {
		 // group by of all scores..
		total_score+=Double.parseDouble(val.toString());
		
	  }
	  Text key1=new Text(key);
	  Text value1=new Text(total_score+"");
	context.write(key1,value1);	
    }
}
}

public static void main(String[] args) throws Exception {
 JobControl jobCtrl = new JobControl("mygrp");
 Configuration conf4 = new Configuration();
 String query="";
 // collecting the user specified query from the command line arguments
 for(int i=1;i<args.length;i++)
 {
	 if(i==args.length-1)
	 {
		 query=query+args[i];
	 }
	 else
	 {
	   query=args[i]+" ";
	 }
 }
 //running jobs by job controller
 conf4.set("test",query);
 //String query=args[2];
 Job job5= Job.getInstance(conf4, "word count");
  job5.setJarByClass(Search.class);
  job5.setMapperClass(TokenizerMapper5.class);
 //job4.setCombinerClass(IntSumReducer1.class);
  job5.setReducerClass(IntSumReducer2.class);
  job5.setMapOutputKeyClass(Text.class);
  job5.setMapOutputValueClass(Text.class);
  job5.setOutputKeyClass(Text.class);
  job5.setOutputValueClass(Text.class);
 FileInputFormat.addInputPath(job5, new Path("Tfidf"));
 FileOutputFormat.setOutputPath(job5, new Path(args[0]));
 ControlledJob cJob5 = new ControlledJob(conf4);
 cJob5.setJob(job5);
 jobCtrl.addJob(cJob5);
 Thread thread = new Thread(jobCtrl);
	thread.start();
	while (!jobCtrl.allFinished()) {
		Thread.sleep(5000);
	}
 
System.exit(job5.waitForCompletion(true) ? 0 : 1);
}

@Override
public int run(String[] arg0) throws Exception {
	// TODO Auto-generated method stub
	return 0;
}
}
