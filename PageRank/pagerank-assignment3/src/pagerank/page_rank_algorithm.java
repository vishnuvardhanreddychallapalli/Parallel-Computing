package pagerank;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
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
import org.apache.hadoop.mapreduce.lib.jobcontrol.ControlledJob;
import org.apache.hadoop.mapreduce.lib.jobcontrol.JobControl;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class page_rank_algorithm  extends Configured implements Tool {
	public static class TokenizerMapper
	extends Mapper<Object, Text, Text,Text>{
		private final static IntWritable one = new IntWritable(1);
		private Text word = new Text();
		private Text word1 = new Text();
		public void map(Object key, Text value, Context context
				) throws IOException, InterruptedException {

			String curr_value=value.toString();
			// first separate the key and value in order to further change the page ranks...
			String[] curr_key=curr_value.split("->");
			String curr_key_value=curr_key[0];
			String [] curr_values=null;
			String all_outlinks="";
			if(curr_key.length>1)
			{
				curr_values=curr_key[1].split(",");
				double curr_page_rank=Double.parseDouble(curr_values[0]);
				for(int i=1;i<curr_values.length;i++)
				{
					/// you will get the key or outlinks..
					double distributed_page_rank=curr_page_rank/(curr_values.length-1);
					context.write(new Text(curr_values[i]+"->"),new Text(distributed_page_rank+""));
					all_outlinks=all_outlinks+curr_values[i]+",";
				}
			}
			/// By now we pass all the outlinks and its page rank all this distributed pageranks need to be added in the reducer phase...
			//  Now just pass the curr_key with all_outlinks but not page rank value..
			//context.write(new Text(curr_key[0]),new Text("?"));
			context.write(new Text(curr_key[0]+"->"),new Text("####"+all_outlinks));
		}
	}

	public static class IntSumReducer
	extends Reducer<Text,Text,Text,Text> {
		private IntWritable result = new IntWritable();
		private Text word = new Text();


		public void reduce(Text key, Iterable<Text> values,
				Context context
				) throws IOException, InterruptedException {

			// Now you will get all the group by operations... based on the key...
			String curr_key=key.toString();
			double updated_page_rank=0;
			String all_outlinks="";
			for(Text val:values)
			{
				String curr_val=val.toString();
				char [] arr=curr_val.toCharArray();
				if(arr[0]!='#')
					updated_page_rank=updated_page_rank+Double.parseDouble(curr_val);
				else
				{
					/// You will come here when you hit "#####"
					all_outlinks=curr_val.substring(4);
				}
			}
			double damping_factor=0.85;
			double final_page_rank=(1-damping_factor)+(damping_factor)*(updated_page_rank);
			context.write(new Text(curr_key),new Text(final_page_rank+""+","+all_outlinks));
		}
	} 
	/*	public static void main(String[] args) throws Exception {
		// emptyh mAIN...

		 int ret=ToolRunner.run(new page_rank_algorithm(),args);
		// ToolRunner.run(new page_rank_algorithm(), new String[] {input,output});
		 //System.out.println()
		 System.exit(ret);
	}*/
	@Override
	public int run(String[] args0) throws Exception {

		Configuration conf = new Configuration();
		Job job1= Job.getInstance(conf, "page_rank_algorithm");
		JobControl jobCtrl = new JobControl("mygrp");
		job1.setJarByClass(page_rank_algorithm.class);
		job1.setMapperClass(TokenizerMapper.class);
		job1.setReducerClass(IntSumReducer.class);
		job1.setMapOutputKeyClass(Text.class);
		job1.setMapOutputValueClass(Text.class);
		job1.setOutputKeyClass(Text.class);
		job1.setOutputValueClass(Text.class);
		// String temp_path=args[1]+"temp1";
		// String input="";
		// String output="niter"+new Integer(i).toString()+"";
		/* if(i==1)
	 {
		input=input+args[0];
	    FileInputFormat.addInputPath(job1, new Path(input));
	    FileOutputFormat.setOutputPath(job1, new Path(output));
	 }
	 else
	 {
		 int curr_value=i-1;
		 input=input+"niter"+new Integer(curr_value).toString();
		 FileInputFormat.addInputPath(job1, new Path(input));
		 FileOutputFormat.setOutputPath(job1, new Path(output));
	 }*/
		FileInputFormat.addInputPath(job1, new Path(args0[0]));
		FileOutputFormat.setOutputPath(job1, new Path(args0[1]));
		ControlledJob cJob1 = new ControlledJob(conf);
		cJob1.setJob(job1);
		jobCtrl.addJob(cJob1);
		Thread thread = new Thread(jobCtrl);
		thread.start();
		while (!jobCtrl.allFinished()) {
			Thread.sleep(5000);
		}
		//System.exit(job1.waitForCompletion(true) ? 0 : 1);
		return job1.waitForCompletion(true) ? 0 : 1;
	}

}