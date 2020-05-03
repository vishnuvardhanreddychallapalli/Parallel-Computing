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

public class TermFrequency  extends Configured implements Tool {
	
    	
	//computes the term frequency from the document frequency values
	public static class TokenizerMapper1
	extends Mapper<Object, Text, Text, Text>{

	//private final static IntWritable one = new IntWritable(1);
	private Text word = new Text();
	private Text word1 = new Text();

	public void map(Object key, Text value, Context context
	             ) throws IOException, InterruptedException {
	StringTokenizer itr = new StringTokenizer(value.toString());
	int i=0;
	String s="";
	double d=0;
	while (itr.hasMoreTokens()) {
		 if(i==0)
		 {
			 s=itr.nextToken();
		 }
		 if(i==1)
		 {
			 /// logic to calculate the term frequency 
			 d=1+(Math.log(Integer.parseInt(itr.nextToken()))/Math.log(10));
		 }
		 i++;
	}
	     word.set(s);
	     word1.set(d+"");
	   //  Text one1 = new Text(d);
	     context.write(word,word1);
	}
	}
	/// Running all the jobs with the help of the job controller..
		
	public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
		Configuration conf = new Configuration();
	
		//TokenizerMapper4 t =new TokenizerMapper4(curr_file_count);


		// Job job1= Job.getInstance(conf, "word count");
		  JobControl jobCtrl = new JobControl("mygrp");
		// job1.setJarByClass(TermFrequency.class);
		// job1.setMapperClass(TokenizerMapper.class);
		 //job1.setCombinerClass(IntSumReducer.class);
		 //job1.setReducerClass(IntSumReducer.class);
		// job1.setMapOutputKeyClass(Text.class);
		// job1.setMapOutputValueClass(IntWritable.class);
		// job1.setOutputKeyClass(Text.class);
		// job1.setOutputValueClass(IntWritable.class);
		// String temp_path=args[1]+"temp1";
		// FileInputFormat.addInputPath(job1, new Path(args[1]));
		// FileOutputFormat.setOutputPath(job1, new Path(temp_path));
		// ControlledJob cJob1 = new ControlledJob(conf);
		 //cJob1.setJob(job1);
		 //System.exit(job1.waitForCompletion(true) ? 0 : 1);
		 Configuration conf1 = new Configuration();
		 Job job2= Job.getInstance(conf1, "word count");
		 job2.setJarByClass(TermFrequency.class);
		 job2.setMapperClass(TokenizerMapper1.class);
		 job2.setMapOutputKeyClass(Text.class);
		 job2.setMapOutputValueClass(Text.class);
		// String temp_path1=args[1]+"temp2";
		 FileInputFormat.addInputPath(job2, new Path("DocWordFrequency"));
		 FileOutputFormat.setOutputPath(job2, new Path("TermFrequency"));
		 ControlledJob cJob2 = new ControlledJob(conf1);
		 cJob2.setJob(job2);
		 //jobCtrl.addJob(cJob1);
		// cJob2.addDependingJob(cJob1);
		// jobCtrl.addJob(cJob1);
		 jobCtrl.addJob(cJob2);
		 Thread thread = new Thread(jobCtrl);
			thread.start();
			while (!jobCtrl.allFinished()) {
				Thread.sleep(5000);
			}
		 System.exit(job2.waitForCompletion(true) ? 0 : 1);

	}
	

	@Override
	public int run(String[] arg0) throws Exception {
		// TODO Auto-generated method stub
		return 0;
	}
}