#### Chapter 9 Notes:
##### Summary:
* As noted in the previous section, whilst discussing the _multi-level feedback queue_ scheduler, optimising for turnaround or response-rate can negatively affect _fairness_ - some long jobs can get stuck in the lowest priority queue awaiting a priority boost in order to continue. However, other approached, such has the _proportional-share_ implementation, focus on dividing computation time between processes fairly.
* One method of implementing this approach is to use a _lottery system_ in which CPU time is distributed by ticketing each process and then drawing a winning process. This allows for a system where multiple jobs have a difference chance of being run dependent on how many _tickets_ they are assigned (e.g. job A has 75 tickets, B has 25, A should have a 75% chance of getting prioritised)<sub>[1]</sub>.
* Running with the analogy of tickets as a resource for schedulers, processes (such as a client/server application with multiple processes) can use its current weighting in tickets to improve the chances of a child/partner process of being scheduled - _ticket transfer_. In a closed system where the processes acknowledge each-other as safe, a scheduling implementation may increase its ticket-count in order to signal to the operating system that it is running something of high-importance and have a better chance of being scheduled.
* As, in this case, job scheduling is selected using a randomised process, there are scenarios where jobs fail to finish due to other jobs being highly-prioritised and, thus, selected more frequently. This can be defined using an _unfairness_ metric which can be determined using: _U = <sup>a completion</sup>/<sub>b completion</sub> (e.g. job A finished at 10s, B at 20s, U = 10/20 = 0.5)_. As a general rule-of-thumb, the longer jobs run for, the more fair the scheduling of these jobs becomes (i.e. shorter job lengths will produce lower _U_ values of 0.5-0.7 and higher job lengths with produce higher _U_ values of 0.8-1.0, obviously given multiple jobs).
* Creating a _fair_ scheduler requires that job completion _fairness_ is uniform, meaning that _U (unfairness)_ is equal to or close to __1__.
* The main crux of how a system behaves and will ultimately perform rests upon the ticket allocation method.
* The selection process may be randomised (like a typical lottery draw), enforcing a probabilistic rate of scheduling for each process, or it may be deterministic as in the case of _stride scheduling_, where a value is given to each process generated using its ticket value (e.g. divide 10,000 by each jobs ticket value to calculate its stride value), and a _pass_ value is used to keep track of the accumulating stride value. When scheduling selection occurs, the process with the lowest _pass_ value is selected (with a randomised tie-break) and its _pass_ values is incremented by its _stride_ value, this allows processes with more tickets to run more frequently (as the _stride_ acts as the inverse of ticket count) but does not deny processes from running eventually. The main issue with deterministically handling scheduling is that a global state is required per process so a new process must be setup with a _stride_ and _pass_ value, if its _pass_ value is zero, it will effectively monopolise the CPU time for a certain period (e.g. if Job A has a pass of _600_, and B a pass of _500_, a new job _C_ will run until its pass is _<=500_).
  
##### Linux's CFS (Completely Fair Scheduler):
* Linux's scheduling methodology (CFS) approaches _fair-share scheduling_ with concerns of scalability: _how does a scheduler make decisions in the shortest time and with as little overhead as possible?<sub>[2]</sub>_ 
* CFS is different from traditional scheduling disciplines that use queues and treat waiting processes as outside of the run queue, rather, CFS uses _sleeper fairness_ (this may be why it's _completely_ fair) in which even waiting tasks are treated equally to the _to-run_ tasks.
* CFS determines process runtime not in fixed, adjustable _q_ periods but using a division of `sched_latency` (typically a value of 48ms, when normalised, divided by _n_)<sub>[3]</sub>. When a process is finished running within a time-slice (of length _sched _latency / n_), its duration is accumulated in a `vruntime` value. This is the value used when scheduling jobs, lowest `vruntime` value first.
* `vruntime` stands for _virtual runtime_, it does not measure runtime exactly - rather it weights the runtime of a process dependent on its niceness value. Both the `vruntime` accumulation and the time-slice/cpu bandwidth are calculated using weighting in this manner<sub>[4]</sub>. These weights are contained in the `prio_to_weight` table in `kernel/sched/sched.h` (see: below, equations below that)

```
static const int prio_to_weight[40] = {
 /* -20 */     88761,     71755,     56483,     46273,     36291,
 /* -15 */     29154,     23254,     18705,     14949,     11916,
 /* -10 */      9548,      7620,      6100,      4904,      3906,
 /*  -5 */      3121,      2501,      1991,      1586,      1277,
 /*   0 */      1024,       820,       655,       526,       423,
 /*   5 */       335,       272,       215,       172,       137,
 /*  10 */       110,        87,        70,        56,        45,
 /*  15 */        36,        29,        23,        18,        15,
};
```

<center>_timeslice<sub>k</sub> = <sup>weight<sub>k</sub></sup>/sum(weights<sub>i</sub>) * `sched_latency`_

_`vruntime`<sub>i</sub> = `vruntime`<sub>i</sub> + <sup>weight<sub>0</sub></sup>/<sub>weight<sub>i</sub></sub> * runtime<sub>i</sub>_ </center>

* CFS uses a data structure called the _red-black tree_, which enables its efficiency and scalability. A linked-list data structure such as that outlined in `code/lotterysched.c` does not scale very well due to the need to traverse it linearly, this is especially true when the number of processes increases and the scheduling interval decreases (to a number of milliseconds)<sub>[5]</sub>. The _red-black tree_ structure allows CFS to be so efficient and fast due to being _balanced_ (assuring no side of the tree is more than twice as long as the other keeps depth shallow when tree is modified), it does this by storing an extra bit per node (colour). 
* CFS holds all _running_ and _runnable_ process in this tree and removes and re-inserts (upon wake) sleeping/blocked tasks - setting their `vruntime` to the minimum value in the tree.
* Whilst CFS and fair-share schedulers are useful and, as such, are widely used, they are not perfect and suffer from a number of problems: they have a tendency to favour non-I/O tasks, and they leave a large part of the implementation unknown - how many tickets do you allocate, how is niceness decided by default, etc.
* As a side-note to the above point, scheduling is not a _solved_ science as maintained by Torvalds and many other figures in the 2000's, multi-core processing highlighted flaws in the Linux system, as highlighted in Lozi et al., 2016<sub>[6]</sub>.

##### Homework notes:

* __Q1:__ 

``` 
Seed: 1
Job A: Length (1) Tickets (84)
Job B: Length (7) Tickets (25)
Job C: Length (4) Tickets (44)
	
	| | | | | | | | | |
	C A B C C B B B B B

Seed: 2
Job A: Length (9) Tickets (94)
Job B: Length (8) Tickets (73)
Job C: Length (6) Tickets (30)

	| | | | | | | | | | | | | | | | | | | | | | | 
	C A A C A B A C A A A B A A B C B B B C B B C
	
Seed: 3
Job A: Length (2) Tickets (54)
Job B: Length (3) Tickets (60)
Job C: Length (6) Tickets (6)

	| | | | | | | | | | | 
	B B A B A C C C C C C
```

* __Q2__: 

``` 
Job A: Length (10) Tickets (1)
Job B: Length (10) Tickets (100)

	| | | | | | | | | | | | | | | | | | | |
	A A A A A A A A A A B B B B B B B B B B
```

Job A has a <1% of running before Job B - this kind of imbalance in tickets forces a priority job to completion before another job can be scheduled.

* __Q3__:

Fairness for a scheduling run of two jobs of equal length (100) with the same ticket value (100).

Unfairness for seed (1): 196/200 = 0.98

Unfairness for seed (2): 190/200 = 0.95

Unfairness for seed (3): 196/200 = 0.98

Unfairness for seed (4): 199/200 = 1.00

Unfairness for seed (5): 181/200 = 0.95

" " seed (6): 193/200 = 0.97

" " seed (7): 185/200 = 0.93

" " seed (8) : 191/200 = 0.96

" " seed (9): 192/200 = 0.96

" " seed (10): 197/200 = 0.99

Average: 0.967

* __Q4__:

_Fairness_ decreases as quantum length increases up to a maximum of 0.5 at _q_ = 100 (job A runs for 100ms then job B runs for 100ms = (100/200) = 0.5)

* __Q5__:

[xxyzz did a great job of this here](https://github.com/xxyzz/ostep-hw/tree/master/9).
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
##### Footnotes:

[1] _Tickets_, in this case, just represent the idea of weighting. You could replace the word _ticket_ with whatever you like, as long as its still known that the job with the most foobazes, chunks, or pints is most-likely to be chosen to be run in a _lottery-type_ scheduling draw. The idea of _tickets_, in fact, is commonly used in prioritising processes between users in a multi-user system (e.g. user A and B both have 50 tickets and can use them to have their applications run, but once they have assigned so-many tickets to processes, they will run out of tickets and the operating system knows that they are hogging the CPU and may then distribute CPU time to other users accordingly).

[2] This is a requirement for modern applications. Datacenters, even with scalable, highly efficient scheduling, still uses 5% of its total CPU time on scheduling.

[3] This is true to an extent, there is a parameter (`min_granularity`) that caps the minimum division possible, stopping time-slices from becoming _too small_ and therefore wasting CPU resource on constant context-switching (e.g. if `min_granularity = 6` then time-slices determined by `sched_latency / n` will not be able to subceed 6ms). This does cause a small issue in that CFS relies upon interrupts at fixed time intervals, if the time-slice is not a perfect multiple of this interval, it will miss the end of the time-slice - but this is averaged out in the long run.

[4] The timeslice calculation requires the weighting of the current process (_k_) and the sum of the weights of each process in the run queue (_i_). `vruntime` accumulation is not exactly equal to `+= timeslice runtime` as the `v` in `vruntime` would suggest. Rather each process (_i_) has a `vruntime` scaled inversely by the weight of the process in `prio_to_weight`: e.g.

* A process (_i_) has a niceness of -10, it is in a queue with another process (_x_) which has a niceness of -3, so its runtime value is equal to: `(9548 / (9548 + 1991)) * sched_latency [48] = ~40ms. `
* If process _i_ runs (as it has priority), it's weighted value to be accrued onto `vruntime` would be: `(1024 / 9548) * 40 = 4.3ms`. 
* As noted in _OSTEP_, these weighting calculations with differing niceness ratings can produce the same outcomes, e.g. _A_ [-5] vs. _B_ [0] and _C_ [5] vs. _D_ [10] both produce time-slices of 36ms.

[5] Removing a value and entering it back into a list in order requires searching through the list to find the correct point at which to insert it, this is an _O(n)_ operation, the same operation using a red-black tree is _O(log n)_ which is noticeably more efficient when approaching _n > 1000_.

[6] tl;dr: The general summary of the paper is that for modern applications the scheduler has become too complex and feature-heavy leading to bugs relating to the scheduling of threads, highlighting four bugs that caused a ~13-24% reduction in performance metrics. The actual cause of the bugs, in this context, is not important but the outcome was always the same: cores remained idle whilst runnable threads awaited scheduling in the run queue. An interesting side-note on this paper is the highlighting of how important it is for developers to have time to build tools to better debug and test their code - finding these kinds of kernel-level bugs is difficult and often requires proprietary solutions.