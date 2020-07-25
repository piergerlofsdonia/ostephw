* FIFO -j 3 -s 100 gives the following output:
	* Job 0 ( length = 2 )
	* Job 1 ( length = 5 )
	* Job 2 ( length = 8 )

( Response and wait time are equal here).

			| | | | | | | | | | | | | | |
	FIFO:	0 0 1 1 1 1 1 2 2 2 2 2 2 2 2 
			2	7		  15				T-turnaround = 8s
			0	2		  7					T-response = 3s
			0	2		  7					T-wait_time = 3s

__Q1:__

* FIFO (3 jobs, length of 200):

		| | | | | | | | | | | | 
		0 1 1 1 2 2 2 2 2 2 2 2
		1 4     12				T-turnaround = 5.7s
		0 1     4				T-response = 1.7s
		0 1     4				T-wait_time = 1.7s
	
* SJF (3 jobs, length of 200): Identical to 	FIFO.

__Q2:__

* FIFO & SJF (identical) for jobs of length 100, 200, 300.
* T-turnaround = 300.00s
* T-response & T-wait = 133.34s

__Q3:__ 

* RR (3 jobs with quantum of 1s):

T-turnaround 6.34s
	0 1 2						T-response = 1s
	0         3          4		T-wait_time = 2.33s
	
__Q4:__

* Any scenario where the job lengths increase (job 0 < job 1 < job 2 ... etc).
* A sequence of jobs with the same length.

__Q5:__

* Jobs of length 1, quantum of 1.
* Any scenario where quantum = job length
* See above: Where job lengths increase in size AND the quantum size is greater to the length of the shortest job (in the case where you have three jobs, e.g. 1, 2, 3 and a q of 3).

__Q6:__

* T values increase as times increase (correlated - doubling times doubles t values). 
e.g. 
1,2,3: Tr: 1.75s, Tt: 3.5, Tw: 1.75
2,4,6: Tr: 2.67, Tt: 6.67, Tw: 2.67
4,8,12: Tr: 5.33, Tt: 13.33, Tw: 5.33
8,16,24 Tr: 10.67, Tt: 26.67, Tw: 10.67
3,5,7: Tr: 3.67 Tt: 8.67 Tw: 3.67
6,10,14: Tr: 7.33, Tt: 17.33, Tw: 7.33

__Q7:__ 

* For job lengths >= q value:
	* T<sub>response</sub> = _n_<sub>-1</sub> + q
* For job lengths < q value:
	* T<sub>response</sub> = _n_<sub>-1</sub>
* For job n == 0
	* T<sub>response</sub> = 0
* Worst-case response time:
	* ((n-1) * q) - r
	
		_where r is equal to:_ (q - j<sub>n</sub>) + (q - j<sub>n+1</sub>) + ... _for each input job that has a length < q_

	e.g:
	
	Round-robin with jobs of _length_ 1, 2, 3, 4, 5 and a _q_ of 1:
	
	T<sub>response(w.c.)</sub> = ((n-1)*q)-0 = 4

	Round-robin with jobs of _length_ 2, 3, 4, 5, 6 and a _q_ of 4:
	
	T<sub>response(w.c.)</sub> = ((n-1)*q)-3 = 13 
