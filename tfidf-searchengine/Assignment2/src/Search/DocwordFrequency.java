package Search;

import java.io.IOException;
import java.util.StringTokenizer;
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
import org.apache.hadoop.mapreduce.Mapper.Context;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.lib.jobcontrol.ControlledJob;
import org.apache.hadoop.mapreduce.lib.jobcontrol.JobControl;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;


public class DocwordFrequency  extends Configured implements Tool {
	
	// Map task to give (word+"#####"+filename,1)
	public static class TokenizerMapper
    extends Mapper<Object, Text, Text, IntWritable>{
		private final static IntWritable one = new IntWritable(1);
		 private Text word = new Text();
		 private int no_of_files;
		 int set_no_of_files(int curr_no)
		 {
			 this.no_of_files=curr_no;
			 return no_of_files;
		 }
		 public void map(Object key, Text value, Context context
		                 ) throws IOException, InterruptedException {
		   StringTokenizer itr = new StringTokenizer(value.toString());
		   while (itr.hasMoreTokens()) {
		   	FileSplit fileSplit = (FileSplit)context.getInputSplit();
		   	String filename = fileSplit.getPath().getName();
		   	String[] parts = filename.split("/");
		   	int last_index=parts.length-1;
		   	String curr_word=itr.nextToken();
		   	// Regular expression to clear unnecessary symbols
		   	curr_word=curr_word.replaceAll("[^a-zA-Z0-9]", "");
		   	System.out.println(parts[last_index]);
		   	// Lowering the case of the words
		    word.set(curr_word.toLowerCase()+"#####"+parts[last_index]);
		    context.write(word, one);
		   }
		 }
		}

	  // This reducer will do the groupby operation inorder to count the values of each word in a file
		public static class IntSumReducer
		    extends Reducer<Text,IntWritable,Text,IntWritable> {
		 private IntWritable result = new IntWritable();

		 public void reduce(Text key, Iterable<IntWritable> values,
		                    Context context
		                    ) throws IOException, InterruptedException {
		   int sum = 0;
		   for (IntWritable val : values) {
		     sum += val.get();
		   }
		   result.set(sum);
		   context.write(key, result);
		 }
		}
		public static void main(String[] args) throws Exception {
			 Configuration conf = new Configuration();
			 FileSystem fs= FileSystem.get(conf); 
			//get the FileStatus list from given dir
			FileStatus[] status_list = fs.listStatus(new Path(args[0]));
			int curr_file_count=0;
			if(status_list != null){
			   for(FileStatus status : status_list){
			       //add each file to the list of inputs for the map-reduce job
				   curr_file_count++;
			   }
			}
			Job job1= Job.getInstance(conf, "word count");
			 JobControl jobCtrl = new JobControl("mygrp");
			 job1.setJarByClass(DocwordFrequency.class);
			 job1.setMapperClass(TokenizerMapper.class);
			 job1.setReducerClass(IntSumReducer.class);
			 job1.setMapOutputKeyClass(Text.class);
			 job1.setMapOutputValueClass(IntWritable.class);
			 job1.setOutputKeyClass(Text.class);
			 job1.setOutputValueClass(IntWritable.class);
			// String temp_path=args[1]+"temp1";
			 FileInputFormat.addInputPath(job1, new Path(args[0]));
			 FileOutputFormat.setOutputPath(job1, new Path("DocWordFrequency"));
			 ControlledJob cJob1 = new ControlledJob(conf);
			 cJob1.setJob(job1);
			 jobCtrl.addJob(cJob1);
			 Thread thread = new Thread(jobCtrl);
				thread.start();
				while (!jobCtrl.allFinished()) {
					Thread.sleep(5000);
				}
			 System.exit(job1.waitForCompletion(true) ? 0 : 1);
		}
		@Override
		public int run(String[] arg0) throws Exception {
			// TODO Auto-generated method stub
			return 0;
		}
	}