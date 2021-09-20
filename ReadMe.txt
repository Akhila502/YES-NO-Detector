========================================================================
    CONSOLE APPLICATION : Assignment_yes_no Project Overview
========================================================================

Assignment_yes_no.cpp
----------------------------
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:
---------------------------
StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named Assignment_yes_no.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////

Instructions to run the file
-----------------------------
* open the [Assignment_yes_no.sln] file in Visual Studio 10+.
* Run the Assignment_yes_no.cpp ( F5 ).

/////////////////////////////////////////////////////////////////////////////

Input 
-----------------------------

Sample inputs are provided in the following files 
=> yes_no.txt ( which contains 4 YES's and 4 No's )
=> input.txt ( which contains 3 YES's and 3 NO's )

/////////////////////////////////////////////////////////////////////////////

Prerossing
-----------------------------

* Maximum value is found from the given input sample data.
* DC Shift if present has to be removed from all the sample values for the zero line correctness
* Normalization of the sample data is done based on the threshold values of amplitudes ( +5000/-5000 )

/////////////////////////////////////////////////////////////////////////////

Processing
------------------------------

* Dont analyze the wave from the beginning as the start of recording captures unwanted  signal data.(500 samples are omitted)
* Taken the whole wave form and put it into 100 chunks each and the features energy and zcr are calculated for every frame(100 samples in this case)
* Find out the average energy and calculate threshold energy (10% deviation from average energy)

           Threshold Energy = (Average Energy * 0.1)/length

* Starting and ending points of the word are now identified using the obtained threshold energy.
* For every word, last 30% of the word is taken and average ZCR is calculated.
=> if the calculated average zcr value is less then 30 then it ends with a vowel so it's a NO
=> else if the calculated average zcr value is greater then 30 then it's a YES as it ends with a fricative

/////////////////////////////////////////////////////////////////////////////


