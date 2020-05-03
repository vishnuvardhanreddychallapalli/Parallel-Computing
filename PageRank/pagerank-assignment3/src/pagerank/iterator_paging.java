package pagerank;
import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
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
public class iterator_paging {
	public static void main(String[] args) throws Exception {
		/// This is my Driver class where the flow would be 
		/// First calculating total pages...
		/// second creating graph
		/// Third calling pagerank 10 times using pagerank algortihm from the current class
		/// Call sorting at the end of 10 th iteration...
		/// while submitting give top 100 page ranks...
		String main_output=args[1];
		String curr_input1=args[0];
		String curr_output1="Graph_Count";
		args[0]=curr_input1;
		args[1]=curr_output1;
		int result=ToolRunner.run(new NoOfPages(),args);
		if(result==1)
		{
			System.exit((result==1) ? 0 : 1);
		}
		/// BY this time all the values would have been present in Graph_count;;;
		args[0]=curr_input1;
		args[1]="Adjacency_List";
		/// All the results are present in the Adjacency List...
		result=ToolRunner.run(new AdjacencyList(),args);
		if(result==1)
		{
			System.exit((result==1) ? 0 : 1);
		}

		iterator_paging p=new iterator_paging();
		args[0]=args[1];
		for(int i=1;i<=10;i++)
		{

			String input="";
			String output="";
			if(i==1)
			{
				input=input+args[0];
				output="niter"+new Integer(i).toString()+"";
				// FileInputFormat.addInputPath(job1, new Path(input));
				//FileOutputFormat.setOutputPath(job1, new Path(output));

			}
			else
			{

				int curr_value=i-1;
				input=input+"niter"+new Integer(curr_value).toString();
				output="niter"+new Integer(i).toString()+"";

				// FileInputFormat.addInputPath(job1, new Path(input));
				// FileOutputFormat.setOutputPath(job1, new Path(output));
			}	
			args[0]=input;
			args[1]=output;
			// ToolRunner.run(new page_rank_algorithm(), new String[] {input,output});
			 result=ToolRunner.run(new pagerank.page_rank_algorithm(),args);
			if(result==1)
			{
				System.exit((result==1) ? 0 : 1);
			}
			if(i>1)
				p.remove_path(new Path(input));
		}
		String input="niter"+new Integer(10).toString();
		String output=main_output;
		args[0]=input;
		args[1]=output;
	   result=ToolRunner.run(new Sorting(),args);
		if(result==1)
		{
			System.exit((result==1) ? 0 : 1);
		}
		System.exit(1);
	}

	@SuppressWarnings("deprecation")
	public void remove_path(Path path) throws IOException {
		Configuration conf = new Configuration();
		FileSystem fs = FileSystem.get(conf);
		fs.delete(path);
	}
}
