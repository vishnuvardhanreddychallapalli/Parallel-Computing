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
import org.apache.hadoop.mapreduce.Reducer.Context;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileSplit;
import org.apache.hadoop.mapreduce.lib.jobcontrol.ControlledJob;
import org.apache.hadoop.mapreduce.lib.jobcontrol.JobControl;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.Tool;


public class Tfidf extends Configured implements Tool {

		public static class TokenizerMapper3
		extends Mapper<Object, Text, Text, Text>{

		private final static IntWritable one = new IntWritable(1);
		private Text word1 = new Text();
		private Text word2 = new Text();

		public void map(Object key, Text value, Context context
		             ) throws IOException, InterruptedException {

			  String curr_line=value.toString();
			  String[] parts = curr_line.split("#####");
			  char[] a=parts[1].toCharArray();
			  System.out.println(a.length);
			  String mystring="";
			  int flag=0;
			  // Logic to get the ouptut as expected in the assignment 2 file
			  for(int i=0;i<a.length;i++)
			  {
			      if(a[i]>='a' && a[i]<='z' || a[i]=='.' || a[i]>='0' && a[i]<='9'|| a[i]=='_')
			      {
			          mystring=mystring+""+a[i];
			         
			      }
			      else
			      {
			          if(flag==0)
			          {
			              mystring+="=";
			              flag=1;
			          }
			      }
			  }
			  word1.set(parts[0]);
			  word2.set(mystring);
			  context.write(word1, word2);
		}
		}
		public static class IntSumReducer1
		extends Reducer<Text,Text,Text,Text> {
			
			private Text word1 = new Text();
			private Text word2 = new Text();

		public void reduce(Text key, Iterable<Text> values,
		                Context context
		                ) throws IOException, InterruptedException {
			
			String smallans="";
			/*ArrayList<Text> al =new ArrayList<Text>();
			for(Text val:values)
			{
				al.add(val);
			}*/
			//System.out.println("Keyying and valuinig...");
			
			//for(int i=0;i<al.size();i++)
			int count=0;
			for(Text val:values)
			{
				smallans=smallans+val.toString()+",";
				count++;
			}
			smallans=smallans+count;
			//smallans=smallans+al.size();
			Text key1=new Text(key+"*");
			Text value1=new Text(smallans);
			//word1.set(key);
			//word2.set(smallans);
			context.write(key1,value1);	
		}
		}
		public static class TokenizerMapper4
		extends Mapper<Object, Text, Text, Text>{
			private Text word1 = new Text();
			private Text word2 = new Text();

		//private final static IntWritable one = new IntWritable(1);



		public void map(Object key, Text value, Context context
		             ) throws IOException, InterruptedException {

			
			
			  
			 String curr_string=value.toString();
			 /// splitting based on "*" as a delimiter...
			  String [] parts=curr_string.split("\\*");
			  String curr_key=parts[0];
			  // Removing spaces from both left and right part of the string..
			  String curr_value=parts[1].trim();
			  String [] small_parts=curr_value.split(",");
			  // Taking the count of unique files which are present in the input given to tfidf
			  int no_of_unique_files=Integer.parseInt(small_parts[small_parts.length-1]);
			  /// The formula to compute idf is log((1+no_of_files)/no_of_unique_files))....
			  Configuration conf=context.getConfiguration();
			  String value_count=conf.get("test");
			  if(!value_count.isEmpty())
			 {
			  int total_no_files=Integer.parseInt(value_count);
			  double x=(total_no_files/no_of_unique_files);
			  // Formula fo rcomputing the idf value....
			  double idf_value=Math.log10(1+x);
			  for(int i=0;i<small_parts.length-1;i++)
			  {
				  String [] waste=small_parts[i].split("=");
				  String file_name=waste[0];
				  // Computing the tfidf on the fly...
				  double tf_idf=idf_value*Double.parseDouble(waste[1]);
				   Text word3 = new Text();
				   Text word4 = new Text();
				   word3.set(curr_key+"#####"+file_name+",");
				   word4.set(tf_idf+"");
				   context.write(word3,word4);
				  
			  }
			  //word1.set(curr_key);
			  //word2.set(idf_value+"");
			  //context.write(word1,word2);      
			}  
		}
		}
	

	public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
		
		Configuration conf = new Configuration();
		// logic to find the no of files in a directory
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
		//Running jobs by job controller...
		 JobControl jobCtrl = new JobControl("mygrp");
		 Configuration conf2 = new Configuration();
		 Job job3= Job.getInstance(conf2, "word count");
		 job3.setJarByClass(Tfidf.class);
		 job3.setMapperClass(TokenizerMapper3.class);
		 job3.setReducerClass(IntSumReducer1.class);
		 job3.setMapOutputKeyClass(Text.class);
		 job3.setMapOutputValueClass(Text.class);
		 job3.setOutputKeyClass(Text.class);
		 job3.setOutputValueClass(Text.class);
		 FileInputFormat.addInputPath(job3, new Path("TermFrequency"));
		 FileOutputFormat.setOutputPath(job3, new Path("InverseDocument"));
		ControlledJob cJob3 = new ControlledJob(conf2);
		 cJob3.setJob(job3);
		 Configuration conf3 = new Configuration();
		 conf3.set("test",curr_file_count+"");
		 Job job4= Job.getInstance(conf3, "word count");
		 job4.setJarByClass(Tfidf.class);
		 job4.setMapperClass(TokenizerMapper4.class);
		 //job4.setCombinerClass(IntSumReducer1.class);
		 //job4.setReducerClass(IntSumReducer1.class);
		 job4.setMapOutputKeyClass(Text.class);
		 job4.setMapOutputValueClass(Text.class);
		 //job4.setOutputKeyClass(Text.class);
		 //job4.setOutputValueClass(Text.class);
		// String temp_path3=args[1]+"temp4";
		 FileInputFormat.addInputPath(job4, new Path("InverseDocument"));
		 FileOutputFormat.setOutputPath(job4, new Path("Tfidf"));
		 ControlledJob cJob4 = new ControlledJob(conf3);
		 cJob4.setJob(job4);
		 cJob4.addDependingJob(cJob3);
		// cJob3.addDependingJob(cJob2);
		 //cJob2.addDependingJob(cJob1);
		 //jobCtrl.addJob(cJob1);
		 //jobCtrl.addJob(cJob2);
		 jobCtrl.addJob(cJob3);
		 jobCtrl.addJob(cJob4);
		 Thread thread = new Thread(jobCtrl);
			thread.start();
			while (!jobCtrl.allFinished()) {
				Thread.sleep(5000);
			}
		 System.exit(job4.waitForCompletion(true) ? 0 : 1);
	}



		@Override
		public int run(String[] arg0) throws Exception {
			// TODO Auto-generated method stub
			return 0;
		}
	}