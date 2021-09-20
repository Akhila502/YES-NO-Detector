// Assignment_yes_no.cpp : Defines the entry point for the console application.



#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int retrieve_maximum(FILE *);		
int normalize_input();


FILE *fin, *fout, *fnorm, *fresult;      //file pointers
double energy_arr[10000];                //energy computed for every 100 samples is stored into this array
int zcr_arr[10000];                      //zcr computed for every 100 samples is stored into this array
long  double energy=0.0, avg_energy=0.0;
int zcr = 0;


int _tmain(int argc, _TCHAR* argv[])
{
	char lineData[20];
	int i = 0, len = 0;

	//----------------------------------------------------------------------------------------------------------------
	
	//Retrive the sample data for speech processing
	fin = fopen("yes_no.txt", "r");		
	//Create and open the file for storing the normalized data
	fnorm = fopen("normalized_input", "w");	
	//Create and open the file for storing the features(energy and zcr) obtained from the sample data
	fout = fopen("features.txt", "w");				
	//Create and open the file for storing the final result containing YES's and NO's sspoken
	fresult=fopen("final_result.txt","w");       
	
	//-----------------------------------------------------------------------------------------------------------------

	//Normalizing the input data and producing the output into a file containing normalized data
	len = normalize_input();

	//-----------------------------------------------------------------------------------------------------------------
	
	//computing threshold energy

	int threshold_energy=0;

	threshold_energy = avg_energy * 0.1 /len;     

	//-------------------------------------------------------------------------------------------------------------------

	//starting and ending points of the word are now identified using the obtained threshold energy

	
	int res[100],count=0, flag=0;

	for(i = 0; i < len-5; i++)                 
	{
		if( !flag &&                        //condition for checking the starting index of the word
			( energy_arr[i] >  threshold_energy && 
			energy_arr[i+1] >  threshold_energy && 
			energy_arr[i+2] > threshold_energy &&
			energy_arr[i+3] > threshold_energy && 
			energy_arr [i+4] > threshold_energy&& 
			energy_arr[i+5] > threshold_energy && 
			energy_arr [i+6] > threshold_energy && 
			energy_arr[i+7] > threshold_energy && 
			energy_arr[i+8] > threshold_energy && 
			energy_arr[i+9] > threshold_energy ))
		{
			res[count++] = i;              //starting index of the word is stored into the array res
			flag = 1;
		}
		else if( flag &&                   //condition for checking the end index of the word
			( energy_arr[i] < threshold_energy && 
			energy_arr[i+1] < threshold_energy && 
			energy_arr[i+2] < threshold_energy &&
			energy_arr[i+3] < threshold_energy && 
			energy_arr [i+4] < threshold_energy&& 
			energy_arr[i+5] < threshold_energy && 
			energy_arr [i+6] < threshold_energy && 
			energy_arr[i+7] < threshold_energy && 
			energy_arr[i+8] < threshold_energy && 
			energy_arr[i+9] < threshold_energy ))
		{
			res[count++] = i;
			flag = 0;
		}
	}

	//---------------------------------------------------------------------------------------------------------------------
	int word_length = 0,start = 0,j = 0,avg_zcr = 0; 

	printf("******OUTPUT*******\n\n");
	for(i = 0; i < count;i ++)
	{
		word_length = res[i+1] - res[i] ;                //length of the word
		word_length *= 0.3;                     //retrieving last 30% of the word length         
		start = res[i+1] - word_length;
		for(j = start; j < res[i+1]; j++)
			avg_zcr += zcr_arr[j];		        //average ZCR for the last 30% of the word	

		// if the calculated average zcr value is less then 30 then it ends with a vowel so it's a NO
        //else if the calculated average zcr value is greater then 30 then it's a YES as it ends with a fricative 
		if( (avg_zcr / word_length) > 30)        
		{
			fputs( "YES\n", fresult);  
			printf("YES\n");
		}
		else
		{
			fputs( "NO\n", fresult);
			printf("NO\n");
		}
		avg_zcr=0;
		i++;
	}
	fclose(fin);
	fclose(fout);
	fclose(fnorm);
	return 0;
}
//find the maximum value in the given sample data
int retrieve_maximum(FILE *fin)						
{														
	int max = 0, len = 0, line_no = 0, data = 0;
	char lineData[20];
	while( !feof(fin) )
	{
		fgets(lineData,100,fin);
		len = strlen(lineData);
		if(lineData[len-1] == '\n')
		{
			line_no++;
		}
		if(line_no > 500)                //skip the first 500 samples as it contains the header information and unwanted signals
		{
			fgets(lineData, 20, fin);
			data = _atoi64(lineData);
			if(data > 0) 
			{
				if (max <= data)
				{
					max = data;
				}
			}
		}
	}
	rewind(fin);    
	return max;

}

//Normalizing the input data and producing the output into a file containing normalized data
int normalize_input()
{
	int max = 0, line_no = 0, count = 0,  len = 0, prev_x = 0, curr_x = 0, i = 0;
	char lineData[20];
	max = retrieve_maximum(fin);  //retrieve the maximum value in the given input data
	while( !feof(fin) )
	{
		fgets(lineData, 20, fin);
		len = strlen(lineData);
		if(lineData[len-1] == '\n')
			line_no++;
		if(line_no > 500)        //skip the first 500 samples as it contains the header information and unwanted signals       
		{
			fgets(lineData, 20, fin);
			line_no++;
			curr_x = _atoi64(lineData);
			curr_x = curr_x * 5000 / max;        //Nomalizing the sample data      
			fprintf(fnorm, "%d\n", curr_x);      //Storing the normalized data into th temporary file normalized_input.txt
			count++;                             //for counting every 100 samples
			energy += curr_x * curr_x;
			if(prev_x * curr_x < 0)              //calculating the zcr value
			{
				zcr++;
			}
			prev_x = curr_x;

			//--------------------------------------------------------------------------------------------------------------------
			//Feature Extraction ( Energy and ZCR values for every 100 data samples )
			if(count==100)                      
			{
					energy /= 100.0;
					avg_energy += energy;
					//Storing the computed energy and zcr values for every 100 samples into the feature  file
					fprintf(fout, "%.2Lf\t\t%d\n", energy, zcr);  
					energy_arr[i] = energy;
					zcr_arr[i] = zcr;
					i++;
					energy = 0;
					zcr = 0;
					count = 0;
					
			}

		}
	}
	return i;
}


