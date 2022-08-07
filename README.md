# FreePoll

This is a highly-experimental attempt to create an open source client program
for the [iClicker](https://www.iclicker.com/) system for classroom
polling.

Clickers are widely used as a way of implementing
[Peer Instruction](https://mazur.harvard.edu/research-areas/peer-instruction)
in college courses.

## Why?

The basic premise of clickers is pretty simple. You show students a
multiple choice quiz question, let them think about the question,
and answer individually by pressing one of the buttons (A through E)
on their clickers.  After the individual response, students discuss
the question in small peer groups, and then answer again. Clickers
are simply a way for the instructor to record student responses.

The iClicker hardware is quite nice. The student remotes are
reasonably inexpensive and fairly robust. The base station
connects to a computer using USB, and allows the student responses
to be recorded.

The problem is the iClicker *software*.  I have two main complaints.

First, Linux is no longer a supported operating system. In theory, the
[iClicker Classic](https://www.iclicker.com/downloads/iclicker-classic/)
software works on Linux. In practice, it doesn't run on any recent version
of Linux (at least in my experience.)

Second, the modern [iClicker Cloud](https://www.iclicker.com/instructors/software/iclicker-cloud/)
software only supports storing the collected data remotely (in the "cloud").
So, the instructor must create an account to use it. Amazingly,
the data available for download does not actually contain the student
responses! At best, you can see who participated, and if you've
indicated which are the correct answer, you can find out who answered
the question correctly. But you can't actually find out which specific
choices the students made in their responses! (I am trying to avoid going
into an extended rant about how frustrating this is. Suffice it to say
that I think this is highly sub-optimal.)

So, what I want is a portable, open source client program that gives
me full access to the students' responses.  This is what I would like
FreePoll to be.

## Technical details

[Jason Siefken](https://www.math.toronto.edu/siefkenj/homepage/index.html)
has already done the hard part, which is reverse-engineering enough of the
USB protocol that the iClicker base station uses to talk to the host PC
to allow polls to be run and responses collected. He created
[iclickerpoll](https://github.com/siefkenj/iclickerpoll), which is a
command line program written in Python which allows a single poll to be
run and the responses written to a CSV file.

The iClicker base station appears as a Human Interface Device (HID),
which is probably the easiest kind of USB device to communicate with.
FreePoll uses [libusb/hidapi](https://github.com/libusb/hidapi)
to talk to the base station, using the same approach as iclickerpoll.

## Status

Progress on FreePoll is proceeding rapidly. A GUI implemented using
[wxWidgets](https://www.wxwidgets.org/) is well underway, and is already
capable of running polls.  Mostly what's missing is storing collected
poll data to files.  I'm pretty confident that this will be working
soon.

I am endeavoring to make FreePoll completely portable. Right now it
is written in (theoretically) portable C++, other than the reliance
on libusb/hidapi and wxWidgets.

# Compilation

On Linux, you will need the libusb/hidapi development package and the
wxWidgets development package to be installed.  On Debian or Ubuntu:

```
sudo apt install libhidapi-dev libwxgtk3.0-gtk3-dev
```

You'll also need `g++` and `make`.

To compile the program:

```
make depend
make -j
```

(The `-j` option is just so `make` uses all of your CPU cores.)

The command line executable is called `freepoll`, and the GUI executable
is called `freepoll-gui`.

## License

FreePoll is distributed under the terms of the GNU General Public License
version 3.  See the [LICENSE](LICENSE) for details.

## Contact

If you're interested in helping out, you can contact me at
<mailto:david.hovemeyer@gmail.com>.

