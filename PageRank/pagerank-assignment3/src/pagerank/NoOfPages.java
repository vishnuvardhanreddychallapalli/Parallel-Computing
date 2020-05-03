package pagerank;

import java.io.IOException;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.FileStatus;
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



public class NoOfPages extends Configured implements Tool {
	public static class TokenizerMapper
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
		   String str = value.toString();
		   FileSplit fileSplit = (FileSplit)context.getInputSplit();
		   String filename = fileSplit.getPath().getName();
		   String[] parts = filename.split("/");
		   int last_index=parts.length-1;
		   String curr_word=str;
		   	/// Regular expression to catch the file names of the titles...
		   	final Pattern p=Pattern.compile("<title>(.*?)</title>");
			final Matcher matcher = p.matcher(value.toString());
			//System.out.println(matcher.find()+"      "+matcher.group(1));
		    /// All the titles will be grouped by one and i will return the length of the list as my total N-value as my page rank N-value..
			if(matcher.find())
			{
				///You found the value..
				 word.set(parts[last_index]);
				 word1.set(matcher.group(1));
				 context.write(word,word1);
			}
		   }
		 
		}

		public static class IntSumReducer
		    extends Reducer<Text,Text,Text,Text> {
		 private IntWritable result = new IntWritable();
		 private Text word = new Text();
		 

		 public void reduce(Text key, Iterable<Text> values,
		                    Context context
		                    ) throws IOException, InterruptedException {
		   int sum = 0;
		   String new_key=key.toString()+",";
		   for (Text val : values) {
		     sum += 1;
		   //  context.write(key,val);
		   }
		   //result.set(sum);
		   context.write(new Text(new_key),new Text(sum+""));
		 }
		} 

	@Override
	public int run(String[] arg0) throws Exception {
		
		Configuration conf = new Configuration();
		Job job1= Job.getInstance(conf, "page_count");
		 JobControl jobCtrl = new JobControl("mygrp");
		 job1.setJarByClass(NoOfPages.class);
		 job1.setMapperClass(TokenizerMapper.class);
		 job1.setReducerClass(IntSumReducer.class);
		 job1.setMapOutputKeyClass(Text.class);
		 job1.setMapOutputValueClass(Text.class);
		 job1.setOutputKeyClass(Text.class);
		 job1.setOutputValueClass(Text.class);
		// String temp_path=args[1]+"temp1";
		 FileInputFormat.addInputPath(job1, new Path(arg0[0]));
		 FileOutputFormat.setOutputPath(job1, new Path(arg0[1]));
		 ControlledJob cJob1 = new ControlledJob(conf);
		 cJob1.setJob(job1);
		 jobCtrl.addJob(cJob1);
		 Thread thread = new Thread(jobCtrl);
			thread.start();
			while (!jobCtrl.allFinished()) {
				Thread.sleep(5000);
			}
			return job1.waitForCompletion(true) ? 0 : 1;
	}

}
