package pagerank;
import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.io.WritableComparator;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Reducer.Context;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.jobcontrol.ControlledJob;
import org.apache.hadoop.mapreduce.lib.jobcontrol.JobControl;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;
public class Sorting extends Configured implements Tool {
	public static class TokenizerMapper10
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
				/*double curr_page_rank=Double.parseDouble(curr_values[0]);
			for(int i=1;i<curr_values.length;i++)
			{
				/// you will get the key or outlinks..
				double distributed_page_rank=curr_page_rank/(curr_values.length-1);
				context.write(new Text(curr_values[i]+"->"),new Text(distributed_page_rank+""));
				all_outlinks=all_outlinks+curr_values[i]+",";
			}*/
				/// Make key as the pagerank and replace page rank by the source web link...
				String curr_page_rank_value=curr_values[0].trim();
				Double val=Double.parseDouble(curr_page_rank_value);
				///Now i got the page_rank_value;;;
				//	all_outlinks=all_outlinks+curr_key_value+",";
				/*	for(int i=1;i<curr_values.length;i++)
				{
					/// you will get the key or outlinks..
					//double distributed_page_rank=curr_page_rank/(curr_values.length-1);
					//context.write(new Text(curr_values[i]+"->"),new Text(distributed_page_rank+""));
					if(i==1)
					{
						all_outlinks=all_outlinks+curr_values[i].split("-")[1]+",";
					}
					else
					{
						if(i==curr_values.length-1)
						all_outlinks=all_outlinks+curr_values[i].split("-")[1];
						else
						 all_outlinks=all_outlinks+curr_values[i]+",";
					}
					//all_outlinks=all_outlinks+curr_values[i]+",";
				}
				//all_outlinks=all_outlinks+"?";*/

				String[] curr_keys=curr_key_value.split("%");
				if(curr_keys.length>1)
					context.write(new Text(val+""),new Text(curr_keys[1].trim()));

				/// Now i am passing the (-ve value of page_rank,all_outlinks)
			}
			/// By now we pass all the outlinks and its page rank all this distributed pageranks need to be added in the reducer phase...
			//  Now just pass the curr_key with all_outlinks but not page rank value..
			//context.write(new Text(curr_key[0]),new Text("?"));
			//context.write(new Text(curr_key[0]+"->"),new Text("####"+all_outlinks));
		}
	}


// This following Reverse Comparator is taken from the following link 
	/// CREDITS:   http://jarvishere.blogspot.com/2013/12/sort-mapreduce-output-keys-in.html
	public static class ReverseComparator extends WritableComparator {
	     
	    private static final Text.Comparator TEXT_COMPARATOR = new Text.Comparator();
	    public ReverseComparator() {
	        super(Text.class);
	    }
	 
	    @Override
	    public int compare(byte[] b1, int s1, int l1, byte[] b2, int s2, int l2) {
	       return (-1)* TEXT_COMPARATOR.compare(b1, s1, l1, b2, s2, l2);
	    }
	 
	    @SuppressWarnings("rawtypes")
	    @Override
	    public int compare(WritableComparable a, WritableComparable b) {
	        if (a instanceof Text && b instanceof Text) {
	                return (-1)*(((Text) a).compareTo((Text) b));
	        }
	        return super.compare(a, b);
	    }
	}
	public static class IntSumReducer10
	extends Reducer<Text,Text,Text,Text> {
		private IntWritable result = new IntWritable();
		private Text word = new Text();


		public void reduce(Text key, Iterable<Text> values,
				Context context
				) throws IOException, InterruptedException {

			/// Now you have pagerank followed by all outlinks sometimes we need to handle equal pageranks...
			String my_page_rank=key.toString().trim();
			/*for(Text val:values)
 {
	 String curr_val=val.toString();
	 String[] all_outlinks=curr_val.split(",");
	 String value_data="";
	 value_data=value_data+(Double.parseDouble(my_page_rank)*-1.0)+",";
 for(int i=1;i<all_outlinks.length;i++)
	 {
		 if(i==all_outlinks.length-1)
		 {
			 value_data=value_data+all_outlinks[i];
			 continue;
		 }
		 value_data=value_data+all_outlinks[i]+",";
	 }
	 /// So the first value here is the source weblink so make sure to catch it and write this to the context..
	// Double neg_page_rank=Double.parseDouble(all_outlinks[0])*-1;
	 context.write(new Text(all_outlinks[0]+"->"),new Text(value_data));
 }*/
			Double d=Double.parseDouble(my_page_rank);
			// String value_data=(Double.parseDouble(my_page_rank))+"";
			for(Text val:values)
				context.write(new Text(val.toString().trim()+"->"),new Text(d+""));
		}
	} 

	@Override
	public int run(String[] arg0) throws Exception {

		Configuration conf = new Configuration();
		Job job1= Job.getInstance(conf, "Sorting");
		JobControl jobCtrl = new JobControl("mygrp");
		job1.setJarByClass(Sorting.class);
		job1.setMapperClass(TokenizerMapper10.class);
		job1.setSortComparatorClass(ReverseComparator.class);
		job1.setReducerClass(IntSumReducer10.class);
		job1.setMapOutputKeyClass(Text.class);
		job1.setMapOutputValueClass(Text.class);
		job1.setOutputKeyClass(Text.class);
		job1.setOutputValueClass(Text.class);
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
		//System.exit(job1.waitForCompletion(true) ? 0 : 1);
		return job1.waitForCompletion(true) ? 0 : 1;
	}
}


