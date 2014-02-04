# TODO: Add RMS reporting
"""Modified version of the example code from Janert,
Feedback Control For Computer Systems

This modified version requires pandas, numpy, and matplotlib.

If you use apt:

sudo apt-get install python-pandas python-numpy python-matplotlib
"""

import numpy
import pandas
import random
import matplotlib.pyplot as pyplot

class Buffer(object):
    def __init__( self, max_wip, max_flow ):
        """Initializes the buffer:

        max_wip: maximum work in progress
        max_flow: maximum work completed per time step
        """
        self.queued = 0
        self.wip = 0             # work-in-progress ("ready pool")

        self.max_wip = max_wip
        self.max_flow = max_flow # avg outflow is max_flow/2

    def work( self, u ):
        # Add to ready pool
        u = max( 0, int(round(u)) )
        u = min( u, self.max_wip )
        self.wip += u

        # Transfer from ready pool to queue
        r = int( round( random.uniform( 0, self.wip ) ) )
        self.wip -= r
        self.queued += r

        # Release from queue to downstream process
        r = int( round( random.uniform( 0, self.max_flow ) ) )
        r = min( r, self.queued )
        self.queued -= r

        return self.queued

class Controller(object):
    def __init__( self, kp, ki, kd ):
        """Initializes the controller.

        kp: proportional gain
        ki: integral gain
        kd: derivative gain
        """
        self.kp, self.ki, self.kd = kp, ki, kd
        self.i = 0       # Cumulative error ("integral")
        # df/dt = (f(t+dt)-f(t))/dt
        self.e_prior = 0	 # Derivative of error
        self.has_worked = False

    def work( self, e, dt=1 ):
        """Computes the number of jobs to be added to the ready queue.

        e: error

        returns: float number of jobs
        """
        self.i += e
        if self.has_worked:
	  deriv = (e-self.e_prior)/dt
	else:
	  deriv = 0
	  self.has_worked = True
        self.e_prior = e

        return self.kp*e + self.ki*self.i + self.kd*deriv

# ============================================================

def closed_loop( c, p, tm=5000, func = None):
    """Simulates a closed loop control system.

    c: Controller object
    p: Buffer object
    tm: number of time steps
    func(t) = int: setpoint as a function of time

    returns: tuple of sequences (times, targets, errors)
    """
    def setpoint( t, func=None ):
      if func is None:
	if t < 100: return 0
	if t < 300: return 50
	return 10
      else: return func(t)
    
    y = 0
    res = []
    for t in range( tm ):
        r = setpoint(t,func)
        e = r - y
        u = c.work(e)
        y = p.work(u)

        #print t, r, e, u, y
        res.append((t, r, e, u, y))

    return zip(*res)

# ============================================================

def test_PID(kp, ki, kd, max_wip=50, max_flow=10, time = 1000):
  c = Controller( kp=1.25, ki=0.01, kd = 1.75 )
  p = Buffer( 50, 10 )
  
  # run the simulation
  ts, rs, es, us, ys = closed_loop( c, p, time, func = lambda t: (0.005*t)**2+0.02*t+10 )
  
  # Root Mean Squared error
  return numpy.sqrt(numpy.mean(numpy.array(es)**2))
# ============================================================

#c = Controller( kp=1.25, ki=0.01, kd = 1.75 ) # setting ki to 0 makes the number of jobs in the buffer oscillate
				   ## noticeably below the target point.
#p = Buffer( 50, 10 )

# run the simulation
#ts, rs, es, us, ys = closed_loop( c, p, 1000, func = lambda t: (0.005*t)**2+0.02*t+10 )
verbose = True
min_err = -1
min_tup = () 
m,n = 0.0,0.0
print "Begin the long search..." # Brute force PID tuner...0.005%/sec = almost 
                                 # six hours to complete...
# Found least RMS (6.7178633746) with (1.3700000000000003, 0.013600000000000001, 
# 0.58000000000000029)
for kd in numpy.arange(0.25,1.25,0.01):
  m += 1.0
  n = 0
  for ki in numpy.arange(0.00,0.02,0.0002):
    n += 1.0
    for kp in numpy.arange(1.0,1.5,0.01):
      errs = []
      for reps in range(5):
        #kp=1.25
        #ki=0.01
        #kd = 0.55
        errs += [test_PID(kp, ki, kd)]
      mean_err = numpy.mean(errs)
      #print("Average RMS error ({p}, {i}, {d}): {rms}".format(p = kp, i = ki, d = kd, rms=mean_err))
      if (min_err == -1) or (min_err > mean_err):
        min_err, min_tup = mean_err, (kp,ki,kd)
        if verbose: print "Found new min {err} at {tup}".format(err=min_err,tup=min_tup)
    print "\n\n{}% done".format(m+n/100)
  print "\n\n{}% done".format(m)

print "\n Found least RMS ({err}) with {tup}".format(err=min_err, tup=min_tup)

# generate the smoothed curve using a rolling mean
# (I think the curves in the book use loess)
#ys_smooth = pandas.rolling_mean(numpy.array(ys), 20)

# make the plot
#pyplot.plot(ts, rs, color='green', label='target')
#pyplot.plot(ts, ys, color='red', label='queue length')
#pyplot.plot(ts, ys_smooth, color='blue', label='trend')
#pyplot.legend(['target','queue length','trend'])
#pyplot.show()



