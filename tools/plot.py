"""
ldr.py
Display analog data from Arduino using Python (matplotlib)
Author: Mahesh Venkitachalam
Website: electronut.in
"""

import argparse
import serial
from collections import deque

import matplotlib.pyplot as plt
import matplotlib.animation as animation


# plot class
class AnalogPlot(object):
    # constr
    def __init__(self, strPort, maxLen):
        # open serial port
        self.ser = serial.Serial(strPort, 115200)

        self.ax = deque([0.0]*maxLen)
        self.ay = deque([0.0]*maxLen)
        self.az = deque([0.0]*maxLen)
        self.maxLen = maxLen

    # add to buffer
    def addToBuf(self, buf, val):
        if len(buf) < self.maxLen:
            buf.append(val)
        else:
            buf.pop()
            buf.appendleft(val)

    # add data
    def add(self, data):
        assert(len(data) == 4)
        self.addToBuf(self.ax, data[0])
        self.addToBuf(self.ay, data[1])
        self.addToBuf(self.az, data[2])

    # update plot
    def update(self, frameNum, a0, a1, a2):
        try:
            line = self.ser.readline()
            data = [float(val) for val in line.split(',')]
            # print data
            if(len(data) == 4):
                self.add(data)
                a0.set_data(range(self.maxLen), self.ax)
                a1.set_data(range(self.maxLen), self.ay)
                a2.set_data(range(self.maxLen), self.az)
        except ValueError:
            import pdb; pdb.set_trace()
            print("Invalid input data")
        except KeyboardInterrupt:
            print('exiting')

        return a0,

    # clean up
    def close(self):
        # close serial
        self.ser.flush()
        self.ser.close()


# main() function
def main():
    # create parser
    parser = argparse.ArgumentParser(description="LDR serial")
    # add expected arguments
    parser.add_argument('--port', dest='port', required=True)

    # parse args
    args = parser.parse_args()

    strPort = args.port

    print('reading from serial port %s...' % strPort)

    # plot parameters
    analogPlot = AnalogPlot(strPort, 100)

    print('plotting data...')

    # set up animation
    fig = plt.figure()
    ax = plt.axes(xlim=(0, 100), ylim=(450, 700))
    a0, = ax.plot([], [])
    a1, = ax.plot([], [])
    a2, = ax.plot([], [])
    plt.setp(a0, linewidth=2.0)
    plt.setp(a1, linewidth=2.0)
    plt.setp(a2, linewidth=2.0)
    anim = animation.FuncAnimation(
        fig, analogPlot.update,
        fargs=(a0, a1, a2),
        interval=50)

    # show plot
    plt.show()

    # clean up
    analogPlot.close()

    print('exiting.')


# call main
if __name__ == '__main__':
    main()
