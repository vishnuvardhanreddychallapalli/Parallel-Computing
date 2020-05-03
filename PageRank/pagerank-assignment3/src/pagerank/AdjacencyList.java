package pagerank;
import java.io.BufferedReader;
import java.util.HashMap;
import java.util.Map;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Reducer.Context;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.lib.jobcontrol.ControlledJob;
import org.apache.hadoop.mapreduce.lib.jobcontrol.JobControl;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
public class AdjacencyList extends Configured implements Tool {
	public static class TokenizerMapper1
    extends Mapper<Object, Text, Text,Text>{
		private final static IntWritable one = new IntWritable(1);
		 private Text word = new Text();
		 private Text word1 = new Text();
		 private int no_of_files;
		 int set_no_of_files(int curr_no)
		 {
			 this.no_of_files=curr_no;
			 return no_of_files;
		 }
		 public void map(Object key, Text value, Context context
		                 ) throws IOException, InterruptedException {
		  // StringTokenizer itr = new StringTokenizer(value.toString());
			HashMap<String,String> map=new HashMap<String,String>(); 
		   	String curr_word=value.toString();
		   	String total_no_of_files="";
		   	Path pt=new Path("Graph_Count/part-r-00000");
		   	FileSystem fs = FileSystem.get(context.getConfiguration());
		   	BufferedReader br=new BufferedReader(new InputStreamReader(fs.open(pt)));
		   	try {
		   	  String first_line;
		   	  first_line=br.readLine();
		   	  while (first_line != null){
		   	     String[] arr=first_line.split(",");
		   	     if(arr.length>1)
		   	     {
		   	     total_no_of_files=arr[1];
		   	     map.put(arr[0].trim(),arr[1].trim());
		   	     }
		   	     first_line = br.readLine();
		   	  }
		   	} finally {
		   	  // you should close out the BufferedReader
		   	  br.close();
		   	}
		   	if(curr_word.isEmpty())
		   		    return;
		   	/// Regular expression to catch the file names of the titles...
		   	final Pattern main_link=Pattern.compile("<title>(.*?)</title>");
			final Matcher matcher = main_link.matcher(curr_word);
			String main_web_link="";
			//System.out.println(matcher.find()+"      "+matcher.group(1));
		    /// All the titles will be grouped by one and i will return the length of the list as my total N-value as my page rank N-value..
			/// If you do not have the main web link then do not check for outlinks ....
			/*if(!matcher.find())
			{
				/// This is a empty line or random text without title tag....
				return ;
			}*/
			String my_file_name="";
			while(matcher.find())
			{
				///You found the value..
				 FileSplit fileSplit = (FileSplit)context.getInputSplit();
				 String filename = fileSplit.getPath().getName();
				 String[] parts = filename.split("/");
				int last_index=parts.length-1;
				main_web_link=matcher.group(1);
				my_file_name=parts[last_index].trim();
				 word.set(parts[last_index].trim()+"%"+main_web_link.trim()+"->");
				// word1.set(matcher.group(1));
				// context.write(word,word1);
			}
			/// Now you need to find the outlinks they are located in the <text>[[]]</text> ...tags...
			//final Pattern outlink=Pattern.compile("<text>(.*?)</text>");
			final Pattern outlink=Pattern.compile("\\[\\[.*?]\\]");
			final Matcher matcher1=outlink.matcher(curr_word);
			String all_outlinks="";
			int total_outlinks=0;
			while(matcher1.find())
			{
				String curr_outlink=matcher1.group().replace("[[", "").replace("]]", "").trim();
				if(curr_outlink.length()==0)
					  continue;
				all_outlinks=all_outlinks+my_file_name+"%"+curr_outlink.trim()+",";
				total_outlinks++;
			}
			Double d =new Double(1.0/total_outlinks);
			context.write(new Text(word), new Text(d+""+","+all_outlinks));
		
		 }
		// }
	/*public static class TokenizerMapper2
    extends Mapper<Text, Text, Text,Text>{
		private final static IntWritable one = new IntWritable(1);
		 private Text word = new Text();
		 private Text word1 = new Text();
		 public void map(Text key, Text value, Context context
		                 ) throws IOException, InterruptedException {
		  // StringTokenizer itr = new StringTokenizer(value.toString());
		   /// Now i need to distribute the page rank equally among all the nodes in such a way for each outlink i should give a distributes page rank....
			 String curr_value=value.toString();
			 String[] curr_arr=curr_value.split(",");
			 /// Now i divided all my values files and values and last value is my single page rank needs to be distributed be careful with dangling nodes...
			 if(curr_arr.length>1)
			 {
				 /// Then this is not a dangling nodee...
				double page_rank_score=0;
				String fraction=curr_arr[curr_arr.length-1];
				int numerator=Integer.parseInt(fraction.split("/")[0]);
				int denominator=Integer.parseInt(fraction.split("/")[1]);
				page_rank_score=numerator*1.0/denominator;
				/// Now i got the single page rank value in double this needs to be propagated..
				int no_of_outlinks=curr_arr.length-1;
				/// because curr_arr.length contains pagerank fraction also so need to remove this inorder to find the unique pages...
				for(int i=0;i<curr_arr.length-1;i++)
				{
					double distributed_page_rank=page_rank_score/no_of_outlinks;
					//to get each outlink name..
					context.write(new Text(curr_arr[i]+"->"),new Text(String.valueOf(distributed_page_rank)));
				}
				
			 }
			 context.write(key,value);
		   }
		 }*/
		


 
	/*public int run(String[] arg0) throws Exception {
		// TODO Auto-generated method stub
		Configuration conf = new Configuration();
		 Job job1= Job.getInstance(conf, "Adjacency_list");
		 JobControl jobCtrl = new JobControl("mygrp");
		 job1.setJarByClass(AdjacencyList.class);
		 job1.setMapperClass(TokenizerMapper1.class);
		// job1.setReducerClass(IntSumReducer.class);
		 job1.setMapOutputKeyClass(Text.class);
		 job1.setMapOutputValueClass(Text.class);
		 job1.setOutputKeyClass(Text.class);
		 job1.setOutputValueClass(Text.class);
		 
		 /// Not writing reduce because it is just an identity function....
		 FileInputFormat.addInputPath(job1, new Path(arg0[0]));
		 FileOutputFormat.setOutputPath(job1, new Path(arg0[1]));
		 ControlledJob cJob1 = new ControlledJob(conf);
		 cJob1.setJob(job1);
		 jobCtrl.addJob(cJob1);
		// jobCtrl.addJob(cJob2);
		 Thread thread = new Thread(jobCtrl);
			thread.start();
			while (!jobCtrl.allFinished()) {
				Thread.sleep(5000);
			}
		 //System.exit(job1.waitForCompletion(true) ? 0 : 1);
			return job1.waitForCompletion(true) ? 0 : 1;
		
	}*/
}

	@Override
	public int run(String[] arg0) throws Exception {
		Configuration conf = new Configuration();
		 Job job1= Job.getInstance(conf, "Adjacency_list");
		 JobControl jobCtrl = new JobControl("mygrp");
		 job1.setJarByClass(AdjacencyList.class);
		 job1.setMapperClass(TokenizerMapper1.class);
		// job1.setReducerClass(IntSumReducer.class);
		 job1.setMapOutputKeyClass(Text.class);
		 job1.setMapOutputValueClass(Text.class);
		 job1.setOutputKeyClass(Text.class);
		 job1.setOutputValueClass(Text.class);
		 
		 /// Not writing reduce because it is just an identity function....
		 FileInputFormat.addInputPath(job1, new Path(arg0[0]));
		 FileOutputFormat.setOutputPath(job1, new Path(arg0[1]));
		 ControlledJob cJob1 = new ControlledJob(conf);
		 cJob1.setJob(job1);
		 jobCtrl.addJob(cJob1);
		// jobCtrl.addJob(cJob2);
		 Thread thread = new Thread(jobCtrl);
			thread.start();
			while (!jobCtrl.allFinished()) {
				Thread.sleep(5000);
			}
		 //System.exit(job1.waitForCompletion(true) ? 0 : 1);
			return job1.waitForCompletion(true) ? 0 : 1;
	}

}
