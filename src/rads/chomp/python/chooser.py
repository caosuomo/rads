"""
This is the CHomP Programs Chooser, an interactive application which
helps compose the command line for most programs in the CHomP package,
and is also a good source of information about these progams.

Copyright (C) 2007-2010 by Pawel Pilarczyk. All rights reserved.

This file is part of the Homology Library.  This library is free software;
you can redistribute it and/or modify it under the terms of the GNU
General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this software; see the file "license.txt".  If not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA.

Started on July 22, 2007. Last revision: July 30, 2010.
"""

# ---------------------------------------------------
# -------------------- overture ---------------------
# ---------------------------------------------------

from Tkinter import *
#from Tix import *
#from Tkconstants import *
from tkFileDialog import *
from tkMessageBox import *
import os
import string


# ---------------------------------------------------
# -------------------- argument ---------------------
# ---------------------------------------------------

class Argument:
	"This is a class for storing types of command line arguments."
	def __init__ (self, label = None, command = None, description = None,
		text = None, multiple = None, readfile = None,
		writefile = None, always = None, separator = None,
		short = None, emptyOK = None):
		""" Argument configuration settings:
		label - label that is displayed in front of an argument box
		command - command-line option that is passed to the program
		text - set to 1 if the option is followed by a text value
		multiple - 1 if the option can be entered multiple times
		readfile - set to 1 if followed by a file name to read
		writefile - set to 1 if followed by a write file name
		always - set to 1 if the argument does not the checkbox"""
		self.label = label
		self.command = command
		self.description = description
		self.text = text
		self.multiple = multiple
		self.readfile = readfile
		self.writefile = writefile
		self.always = always
		self.separator = separator
		self.short = short

		# a nonempty value is required
		self.value = (self.text or self.multiple or \
			self.readfile or self.writefile) and not emptyOK
		# a value field exists but its value may be empty
		self.emptyOK = emptyOK
		if emptyOK and not self.text and not self.multiple \
			and not self.readfile and not self.writefile:
			self.emptyOK = 0

		# the width of the text field: longer for file names,
		# shorter for other types of data, even multiple
		if self.readfile or self.writefile:
			self.width = 30
		else:
			self.width = 10

		# set to 1 if the value field was empty
		self.empty = 1

		# are the variables already defined?
		self.defined = None

	def __updateDesc (self, event = None):
		if self.description != self.desc.get ():
			self.desc.set (self.description)

	def __update (self, master):
		if self.empty and self.textVar.get () != '':
			self.empty = 0
			self.checkVar.set (1)
		if not self.empty and self.textVar.get () == '':
			self.empty = 1
		master.updateCmd ()
		self.__updateDesc ()

	def maxcols (self = None):
		return 3

	def setWidgets (self, master = None, row = None, column = None,
		columnspan = None, desc = None):

		# initialize variables for storing the settings
		if not self.defined:
			self.defined = 1
			self.textVar = StringVar ('')
			self.checkVar = IntVar (0)
			if self.always:
				self.checkVar.set (1)
			self.desc = desc

		# if there is no text value then make the checkbox be wide
		if not self.value and not self.emptyOK:
			columnspan = 3 * columnspan

		# create a checkbutton of the variable
		if self.label and not self.always:
			self.wcheck = Checkbutton (master, text = self.label,
				variable = self.checkVar,
				command = master.updateCmd)
			self.wcheck.grid (row = row, column = column,
				columnspan = columnspan, sticky = W)
			self.wcheck.bind ("<Enter>",
				lambda event: desc.set (self.description))
			self.wcheck.bind ("<FocusIn>",
				lambda event: desc.set (self.description))
			column = column + columnspan

		# create a label of the variable
		if self.label and self.always:
			self.wlabel = Label (master, text = self.label)
			self.wlabel.grid (row = row, column = column,
				columnspan = columnspan, sticky = W)
			self.wlabel.bind ("<Enter>",
				lambda event: desc.set (self.description))
			column = column + columnspan

		# create a text field if necessary
		if self.value or self.emptyOK:
			self.wtext = Entry (master, width = self.width,
				textvariable = self.textVar,
				background = "#FFFFFF")
			self.wtext.grid (row = row, column = column,
				columnspan = columnspan, sticky = E+W)
			column = column + columnspan
			self.wtext.bind ("<Any-KeyPress>", lambda event:
				self.__update (master))
			#	lambda event: master.updateCmd ())
			self.wtext.bind ("<Any-KeyRelease>", lambda event:
				self.__update (master))
			self.wtext.bind ("<Any-Button>", lambda event:
				self.__update (master))
			self.wtext.bind ("<Enter>",
				lambda event: desc.set (self.description))

		# add a button to browse for a file
		browse = None
		if self.readfile and self.multiple:
			browse = lambda: (self.textVar.set \
				(string.join (askopenfilenames (), ' ')),
				self.__update (master))
		elif self.readfile and not self.multiple:
			browse = lambda: (self.textVar.set \
				(askopenfilename ()), self.__update (master))
		elif self.writefile:
			browse = lambda: (self.textVar.set \
				(asksaveasfilename ()),
				self.__update (master))
		if self.readfile or self.writefile:
			self.browse = Button (master,
				text = "Browse...", command = browse)
			self.browse.grid (row = row, column = column,
				columnspan = columnspan, sticky = W)
			self.browse.bind ("<Enter>",
				lambda event: desc.set (self.description))
			self.browse.bind ("<FocusIn>",
				lambda event: desc.set (self.description))
			column = column + columnspan

		# create a separator area
		if self.separator:
			self.sep = Canvas (master, height = 1,
				background = "#CCFFCC")
			self.sep.grid (row = row, column = column,
				columnspan = columnspan * self.maxcols (),
				sticky = E+W, pady = 6)
			column = column + columnspan


# ---------------------------------------------------
# ------------------ program frame ------------------
# ---------------------------------------------------

class ProgFrame (Frame):
	def updateCmd (self):
		cmd = [os.path.join (self.path.get (), self.progName)]
		for arg in self.args:
			if not arg.checkVar.get ():
				continue
			if arg.value and arg.textVar.get () == '':
				continue
			if arg.multiple and arg.text != '':
				lst = string.split (arg.textVar.get (), ' ')
				for word in lst:
					if word == '':
						continue
					if arg.command:
						cmd.append (arg.command)
					cmd.append (word)
				continue;
			if arg.command:
				cmd.append (arg.command)
			if arg.textVar.get () != '':
				cmd.append (arg.textVar.get ())
		for n in range (len (cmd)):
			if string.find (cmd [n], '"') >= 0:
				continue;
			if string.find (cmd [n], " ") >= 0:
				cmd [n] = '"' + cmd [n] + '"'
		self.commandline.set (string.join (cmd, ' '))

	def __init__ (self, master = None, desc = None, cmd = None,
		path = None, prog = None):
		Frame.__init__ (self, master, class_ = "ProgFrame")
		self.description = desc
		self.commandline = cmd
		self.path = path
		self.progNumber = prog
		self.progDesc = ProgDesc [Progs [prog]]
		self.argNames = ProgArgs [Progs [prog]]
		self.progName = string.split (Progs [prog]) [0]

		# set the yellow background for all the objects in this frame
		self.config (background = "#EEEEBB")
		self.option_add (pattern = "*ProgFrame*activebackground",
			value = "#EEEEBB")
		self.option_add (pattern = "*ProgFrame*highlightbackground",
			value = "#EEEEBB")
		self.option_add (pattern = "*ProgFrame*background",
			value = "#EEEEBB")

		# create widgets for all the arguments
		maxcol = 1
		row = 0
		column = 0
		self.args = []
		for argName in self.argNames:
			# determine the argument object
			arg = Args [argName]
			self.args.append (arg)
			width = arg.maxcols ()

			# prepare the row and column configuration
			if arg.short:
				if (column >= width * maxcol):
					column = 0
					row = row + 1
				columnspan = 1
			else:
				if (column > 0):
					row = row + 1
				column = 0
				columnspan = maxcol

			# create the widgets for this argument
			arg.setWidgets (master = self, row = row,
				column = column, columnspan = columnspan,
				desc = self.description)
			if arg.separator:
				self.rowconfigure (row, weight = 0)
			else:
				self.rowconfigure (row, weight = 1)

			# prepare the next row or column
			if arg.short:
				column = column + width
			else:
				row = row + 1

		# make this frame resizeable
	#	for r in range (row):
	#		self.rowconfigure (r, weight = 1)
		for c in range (width * maxcol + 1):
			self.columnconfigure (c, weight = 0)
		self.columnconfigure (maxcol, weight = 1)


# ---------------------------------------------------
# ------------------ program list -------------------
# ---------------------------------------------------

class ProgList (Frame):
	def __setProgList (self):
		"Creates or changes the list of programs in the listbox."
		# remember of fix a program that is selected
		sel = self.progs.curselection ()
		if len (sel) == 1:
			selected = self.proglist [int (sel [0])]
		elif self.curProg != '':
			selected = self.curProg
		else:
			selected = "chomp"

		# create a new array of programs
		self.proglist = []
		for prog in range (len (Progs)):
			if self.full.get () or not Optional [Progs [prog]]:
				self.proglist.append (Progs [prog])

		# add the programs to the listbox
		if self.progs.size ():
			self.progs.delete (0, self.progs.size () - 1)
		for prog in self.proglist:
			self.progs.insert ("end", prog)

		# set the height of the listbox to be enough for all progs
		self.progs.config (height = len (self.proglist))

		# set up the selected program
		try:
			defindex = self.proglist.index (selected)
		except:
			defindex = self.proglist.index ("chomp")
		self.progs.selection_set (first = defindex)
		self.progs.activate (index = defindex)
		self.progs.see (index = defindex)
		if not self.curProg == self.proglist [defindex]:
			self.curProg = self.proglist [defindex]
			self.master.setProgFrame (progLabel = self.curProg)

	def __setProgEvent (self, event):
		"Sets the program frame as chosen from the listbox."
		sel = self.progs.curselection ()
		if len (sel) != 1:
			self.description.set (Desc ["proglist"])
			return
		progNumber = int (sel [0])
		prog = self.proglist [progNumber]
		if prog == self.curProg:
			return
		self.master.setProgFrame (progLabel = prog)
		self.progs.see (index = progNumber)
		self.description.set (ProgDesc [prog])
		self.curProg = prog

	def __setSelectedProgDesc (self, event):
		sel = self.progs.curselection ()
		if len (sel) != 1:
			return
		prog = self.proglist [int (sel [0])]
		self.description.set (ProgDesc [prog])

	def __setNearestProgDesc (self, event):
		prog = self.proglist [self.progs.nearest (event.y)]
		self.description.set (ProgDesc [prog])

	def __init__ (self, master = None, desc = None, cmd = None,
		path = None):
		Frame.__init__ (self, master)
		self.description = desc
		self.config (background = "#CCFFCC")

		# make this frame resizeable
		self.rowconfigure (0, weight = 1)
		self.rowconfigure (1, weight = 0)
		self.columnconfigure (0, weight = 1)
		self.columnconfigure (1, weight = 0)

		# create frames that correspond to the programs
		for prog in range (len (Progs)):
			frame = ProgFrame (master, desc = desc,
				cmd = cmd, path = path, prog = prog)
			ProgFrames [Progs [prog]] = frame

		# create a list box for choosing programs
		self.progs = Listbox (self, width = 16,
			background = "#CCFFCC")
		self.full = IntVar ()
		self.full.set (0)
		self.curProg = ''
		self.__setProgList ()
		self.progs.grid (row = 0, column = 0, sticky = N+S+E+W)
		self.progs.bind ("<Enter>", self.__setNearestProgDesc)
		self.progs.bind ("<Motion>", self.__setNearestProgDesc)
		self.progs.bind ("<Leave>", self.__setSelectedProgDesc)
		self.progs.bind ("<Any-ButtonRelease>", self.__setProgEvent)
		self.progs.bind ("<Any-Button>", self.__setProgEvent)
		self.progs.bind ("<Any-KeyPress>", self.__setProgEvent)
		self.progs.bind ("<Any-KeyRelease>", self.__setProgEvent)
		self.progs.bind ("<FocusIn>", self.__setProgEvent)
		self.progs.focus_set ()

		# create a checkbox for the full list of programs
		self.fullchg = Checkbutton (self, text = "Show full list   ",
			background = "#CCFFCC", activebackground = "#CCFFCC",
			variable = self.full, command = self.__setProgList)
		self.fullchg.grid (row = 1, column = 0, sticky = N+S+W+E)
		self.fullchg.bind ("<Enter>", lambda event:
			self.description.set (Desc ["full"]))
		self.fullchg.bind ("<FocusIn>", lambda event:
			self.description.set (Desc ["full"]))

		# create a scrollbar for the list of programs
		self.progScroll = Scrollbar (self, orient = VERTICAL)
		self.progScroll["command"] = self.progs.yview
		self.progs["yscrollcommand"] = self.progScroll.set
	#	self.progScroll.grid (row = 0, column = 1,
	#		rowspan = 2, sticky = N+S)


# ---------------------------------------------------
# -------------- program chooser frame --------------
# ---------------------------------------------------

class ChooserFrame (Frame):
	def setProgFrame (self, progLabel):
		if self.progLabel == progLabel:
			return
		if self.progFrame:
			self.progFrame.grid_forget ()
		self.progLabel = progLabel
		self.progFrame = ProgFrames [progLabel]
		self.progFrame.grid (row = 1, column = 2, columnspan = 3,
			sticky = N+S+E+W, padx = 6, pady = 6)
		self.progShort.set (self.progFrame.progName + " - " + \
			Short [progLabel])
		self.progFrame.updateCmd ()

	def __init__ (self, master = None, desc = None, cmd = None,
		path = None):
		Frame.__init__ (self, master)
		self.description = desc
		self.commandline = cmd
		self.path = path

		# make this frame resizeable
		self.rowconfigure (0, weight = 0)
		self.rowconfigure (1, weight = 1)
		self.columnconfigure (0, weight = 0)
		self.columnconfigure (1, weight = 0)
		self.columnconfigure (2, weight = 0)
		self.columnconfigure (3, weight = 1)
		self.columnconfigure (4, weight = 0)

		# create a label of the programs list
		self.config (background = "#EEEEBB")
		self.label = Label (self, text = "CHomP Programs:",
			background = "#EEEEBB")
		self.label.grid (row = 0, column = 0, sticky = N+S+W)
		self.label.bind ("<Enter>",
			lambda event: desc.set (Desc ["proglist"]))
	#	self.blank1 = Label (self, text = "",
	#		background = "#EEEEBB")
	#	self.blank1.grid (row = 0, column = 1, sticky = N+S+E+W)
	#	self.blank1.bind ("<Enter>",
	#		lambda event: desc.set (Desc ["proglist"]))

		# create parameters for a frame with the program options
		self.progFrame = None
		self.progLabel = ''
		self.progShort = StringVar ()

		# create a list box for choosing programs
		self.progs = ProgList (self, desc = desc, cmd = cmd,
			path = path)
		self.progs.grid (row = 1, column = 0, columnspan = 2,
			sticky = N+S+E+W)

		# create a one-line description of the chosen program
		self.oneLine = Label (self, textvariable = self.progShort,
			background = "#CCFFCC")
		self.oneLine.grid (row = 0, column = 2, sticky = N+S+W)
		self.oneLine.bind ("<Enter>", lambda event:
			desc.set (self.progFrame.progDesc))
		self.blank2 = Label (self, text = "",
			background = "#CCFFCC")
		self.blank2.grid (row = 0, column = 3, sticky = N+S+E+W)
		self.blank2.bind ("<Enter>", lambda event:
			desc.set (self.progFrame.progDesc))


# ---------------------------------------------------
# ------------------ command line -------------------
# ---------------------------------------------------

class CommandLine (Frame):
	"The command-line area with the RUN button."

	def __changePath (self):
		dirWin = Toplevel ()
		dirWin.title ("Change Path")
		dirWin.grid ()
		dirLabel = Label (dirWin,
			text = "Enter the path for the CHomP programs:")
		dirLabel.grid ()
		path = StringVar ()
		path.set (self.path.get ())
		dirEnter = Entry (dirWin, textvariable = path)
		dirEnter.focus_set ()
		dirEnter.grid (sticky = E+W)
		dirOK = Button (dirWin, text = "OK", command = lambda:
			(dirWin.destroy (), self.path.set (path.get ()),
			self.master.chooser.progFrame.updateCmd ()))
		dirOK.grid ()
		dirWin.bind ("<Return>", lambda event: dirOK.invoke ())
		dirWin.bind ("<Escape>", lambda event: dirOK.invoke ())

	firsttime = 1
	def __run (self):
		self.master.master.withdraw ()
		cmd = self.commandline.get ()
		if self.firsttime:
			self.firsttime = 0
			print "-" * 64
		print "Running '%s':" % cmd
		print "-" * 64
		os.system (cmd)
		print "-" * 64
		self.master.master.deiconify ()

	def __cmdscroll (self, *L):
		operation, howMany = L [0], L [1]
		if operation == "scroll":
			units = L [2]
			self.cmdline.xview_scroll (howMany, units)
		elif operation == "moveto":
			self.cmdline.xview_moveto (howMany)

	def __init__ (self, master = None, desc = None, cmd = None,
		path = None):
		Frame.__init__ (self, master)
		self.description = desc
		self.commandline = cmd
		self.path = path

		# make this frame resizeable
		self.rowconfigure (0, weight = 1)
		self.rowconfigure (1, weight = 1)
		self.columnconfigure (0, weight = 1)
		self.columnconfigure (1, weight = 1)
		self.columnconfigure (2, weight = 0)

		# set a description for this area
		self.bind ("<Enter>",
			lambda event: desc.set (Desc ["cmd"]))

		# create a label of the command line
		self.cmdlabel = Label (self, text = "Command line: ");
		self.cmdlabel.grid (row = 0, column = 0, #columnspan = 2,
			sticky = N+W)

		# create a button for changing the path
		self.pathbutton = Button (self, text = "[change path]",
			border = 0, command = self.__changePath)
		self.pathbutton.grid (row = 0, column = 1, sticky = N+E)
		self.pathbutton.bind ("<Enter>",
			lambda event: desc.set (Desc ["path"]))
		self.pathbutton.bind ("<FocusIn>",
			lambda event: desc.set (Desc ["path"]))
		self.pathbutton.bind ("<Leave>",
			lambda event: desc.set (Desc ["cmd"]))

		# create a scrollbar below the command line
		self.cmdscroll = Scrollbar (self, orient = HORIZONTAL,
			command = self.__cmdscroll)
		self.cmdscroll.grid (row = 2, column = 0, columnspan = 2,
			sticky = N+E+W)

		# create the command line
		self.cmdline = Entry (self, textvariable = cmd, width = 85,
			xscrollcommand = self.cmdscroll.set)
		self.cmdline.grid (row = 1, column = 0, columnspan = 2,
			sticky = S+E+W)
		self.cmdline.bind ("<Return>", lambda event: self.__run ())
		self.cmdline.bind ("<FocusIn>",
			lambda event: desc.set (Desc ["cmd"]))

		# create the "RUN" button
		self.run = Button (self, text = " RUN ",
			command = self.__run, background = "#FF7777",
			activebackground = "#FF7777")
		self.run.grid (row = 0, column = 2, rowspan = 3,
			sticky = N+S+W+E)
		self.run.bind ("<FocusIn>",
			lambda event: desc.set (Desc ["cmd"]))


# ---------------------------------------------------
# ------------------ CHomP Chooser ------------------
# ---------------------------------------------------

class ChompChooser (Frame):
	"The main application class of the CHomP chooser program."

	def __setpath (self, dirname = None):
		"Sets the directory in which the programs are located."
		# if no directory name was given, try a few defaults
		if not dirname:
			self.__setpath (os.pardir + os.sep + "bin")
			self.__setpath ("bin")
			self.__setpath (os.curdir)
			return

		# verify if the directory exists
		if not os.path.exists (dirname):
			return

		# check if one of the programs is in that directory
		files = os.listdir (dirname)
		for filename in files:
			if string.find (filename, "homcubes") < 0:
				continue
			if os.path.isfile (os.path.join (dirname, filename)):
				self.path.set (dirname)
				return

	def __init__ (self, master = None):
		Frame.__init__ (self, master)

		# make the top window resizeable
		top = self.winfo_toplevel ()
		top.rowconfigure (0, weight = 1)
		top.columnconfigure (0, weight = 1)

		# make this window resizeable
		self.grid (sticky = N+S+E+W)
		self.rowconfigure (0, weight = 0)
		self.rowconfigure (1, weight = 1)
		self.rowconfigure (2, weight = 0)
		self.rowconfigure (3, weight = 0)
		self.rowconfigure (4, weight = 0)
		self.rowconfigure (5, weight = 0)
		self.columnconfigure (0, weight = 0)
		self.columnconfigure (1, weight = 1)

		# create a bottom area for short descriptions (3 lines)
		self.description = StringVar ()
		self.description.set (value = Desc ["initial"])
		self.desc = Label (self, textvariable = self.description,
			background = "#AAAAFF", justify = LEFT)
		self.desc.grid (row = 3, column = 0, sticky = N+S+W)
		self.blank = Label (self, text = " ", background = "#AAAAFF")
		self.blank.grid (row = 3, column = 1, sticky = N+S+E+W)

		# create the bottom line with brief program hint
		self.info = Label (self, justify = LEFT, text =
			"CHomP Programs Chooser. Ver. 0.02. " +
			"Copyright (C) 2007-2010 by Pawel Pilarczyk. " +
			"All rights reserved.")
		self.info.grid (row = 4, column = 0, columnspan = 2,
			sticky = N+S+E+W)
		self.info.bind ("<Enter>", lambda event:
			self.description.set (Desc ["initial"]))

		# create an additional line with a program warning
		self.warning = Label (self, text =
			"WARNING: THIS PROGRAM IS UNDER CONSTRUCTION " +
			"AND MAY NOT BE FULLY FUNCTIONAL YET!",
			background = "#FFFF77", foreground = "#FF0000")
	#	self.warning.grid (row = 0, column = 0, columnspan = 2,
	#		sticky = N+S+E+W, ipadx = 6, ipady = 6)
		self.warning.bind ("<Enter>", lambda event:
			self.description.set (Desc ["warning"]))

		# determine the path for the CHomP programs
		self.path = StringVar ()
		self.__setpath ()

		# create the command line area
		self.commandline = StringVar ()
		self.cmdline = CommandLine (self, desc = self.description,
			cmd = self.commandline, path = self.path)
		self.cmdline.grid (row = 2, column = 0, columnspan = 2,
			sticky = N+S+E+W)

		# create the top chooser frame for programs and options
		self.chooser = ChooserFrame (self, desc = self.description,
			cmd = self.commandline, path = self.path)
		self.chooser.grid (row = 1, column = 0, columnspan = 2,
			sticky = N+S+E+W)

		# set some global keys
		self.bind_all ("<Escape>", lambda event: self.quit ())


# ---------------------------------------------------
# ---------- descriptions of screen items -----------
# ---------------------------------------------------

# a dictionary of descriptions of various elements in the screen
Desc = {
"none": "\n\n",
"initial":"""\
Hover your mouse over the items in the screen to see their descriptions.
Select a program from the list on the left, then its arguments and options,
and run the program's command by clicking the RUN button. Have fun! :)""",
"warning":"""\
WARNING: This program is currently under construction. Although the interface
is basically functioning correctly, there is a lot of information and program
descriptions still missing. Your comments and suggestions are welcome!""",
"cmd":"""\
The command line area contains a command line which runs one of the programs
from the CHomP software package, as configured interactively above. It is
possible to edit the command. Click the RUN button to run the command.""",
"proglist":"""\
Click on one of the programs from the list to see its command line arguments,
compose its command line, and run the program by clicking the RUN button.
The output of the program will be displayed in the terminal window.""",
"path":"""\
Click the 'change path' button to set the directory in which the CHomP
programs are located, if they are not available in the system search path.
This path is set to './' or '../bin/' if the programs are found there.""",
"full":"""\
Click to switch between displaying the major programs only or a full list.
Note that there are a lot of programs in the CHomP package and in most
situations one only needs to use a few of the main ones.""",
}


# ---------------------------------------------------
# ----------------- programs' data ------------------
# ---------------------------------------------------

# an array of program names (possibly with variants)
Progs = []

# a dictionary of the option saying if this program is optional
Optional = {}

# a dictionary of program frames (created in the program's run)
ProgFrames = {}

# a cistionary of program descriptions
ProgDesc = {}

# a dictionary of short (one-line) descriptions of the programs
Short = {}

# a dictionary of arrays of programs' argument names
ProgArgs = {}

# a dictionary of argument classes identified by their names
Args = {}


# ---------------------------------------------------
# ------------- definitions of programs -------------
# ---------------------------------------------------

def AddProg (name, optional, description, short, *arguments):
	"Adds a program to the list of programs for the CHomP Chooser."
	Progs.append (name)
	if not name == "[more...]":
		Progs.sort ()
	Optional [name] = optional
	ProgDesc [name] = description
	Short [name] = short
	args = []
	for arg in arguments:
		args.append (arg)
	ProgArgs [name] = args


# ---------------------------------------------------
# -------------------- bmp2pset ---------------------
# ---------------------------------------------------

AddProg ("bmp2pset", 0, """\
The program 'bmp2pset' creates a list of points (squares) extracted from
a bitmap picture in the uncompressed Windows BMP format. Only those pixels
are extracted whose gray level is within the given range.""",
"extracts full cubical sets from bitmap pictures",
"b2pI", "b2pO", "|", "b2p-t", "b2p-x", "b2p-y", "b2p-i", "b2p-r", "|",
"--quiet", "--help", "--log",)

Args ["b2pI"] = Argument (label = "Input BMP file:", description = """\
Selects the input BMP file to extract a cubical set from.
This must be an uncompressed Windows Bitmap file, either black-and-white,
or grayscale (color pictures are translated into grayscale).""",
readfile = 1, always = 1,)

Args ["b2pO"] = Argument (label = "Output file(s):", description = """\
Defines output file or files to write the extracted pixels from the picture
to. Multiple file names must be specified as a space-separated list.
The files are overwritten without any warning.""",
writefile = 1, multiple = 1, always = 1,)

Args ["b2p-t"] = Argument (label = "Threshold(s):", description = """\
Sets the thresholding levels for deciding the ranges of pixels to extract.
If no levels are specified, reasonable levels are determined automatically.
Please, specify as many thresholds as necessary, separated by commas.""",
command = "-t", text = 1, multiple = 1,)

Args ["b2p-x"] = Argument (label = "The leftmost X:", description = """\
Sets the X coordinate of the leftmost pixel in the bitmap.
By default this value is set to 0.
All the other coordinates are greater than or equal tot his one.""",
command = "-x", text = 1,)

Args ["b2p-y"] = Argument (label = "The bottom Y:", description = """\
Sets the Y coordinate of the bottom pixel in the bitmap.
By default this value is set to 0. Unlike in some computer applications,
the vertical axix (Y) goes from the bottom of the picture to the top.""",
command = "-y", text = 1,)

Args ["b2p-i"] = Argument (label = "Include subsets", description = """\
Makes the saved cubical sets include the contents of the previous sets.
This changes the default where all the saved cubical sets are disjoint
and correspond to different ranges of gray levels in the picture.""",
command = "-i",)

Args ["b2p-r"] = Argument (label = "Invert grays", description = """\
Uses a reverse gray level scale bright-to-dark instead of dark-to-bright.
Use this option to extract the white (or bright) pixels from the image
instead of extracting the black (or dark) pixels.""",
command = "-r",)


# ---------------------------------------------------
# -------------------- pset2bmp ---------------------
# ---------------------------------------------------

AddProg ("pset2bmp", 1, """\
The program 'pset2bmp' plots a set of full cubes in a Windows BMP picture.
If the dimension of cubes is higher than 2, it plots a projection of the set.
Multpile sets can be plotted with different colors or shades of gray.""",
"plots full cubical sets as bitmap pictures",
"p2bI", "p2bO", "|",
"p2b-x", "p2b-y", "p2b-s", "p2b-p", "p2b-g", "p2b-m", "p2b-f",
"p2b--bar", "p2b-l", "p2b-b", "p2b-w", "p2b-h", "p2b-r", "p2b--gray",
"|", "--quiet", "--help", "--log",)

Args ["p2bI"] = Argument (label = "Input file(s):", description = """\
Selects the input file(s) that contain lists of full cubes to be plotted
in the BMP file. Each set of cubes is plotted with a different color.
Remember to define the output file, or the last file will be overwritten!""",
readfile = 1, multiple = 1, always = 1,)

Args ["p2bO"] = Argument (label = "Output BMP file:", description = """\
Defines the output file to write the bitmap picture with the cubical sets to.
The picture is either a black-and-white one (if plotting one set only),
a gray-scale one, or a color uncompressed Windows bitmap format.""",
writefile = 1, always = 1,)

Args ["p2b-x"] = Argument (label = "The X axis:", description = """\
Sets the number of the coordinate to be treated as the X axis (default: 1).
This option is especially useful if the dimension of cubes exceeds 2.
Any number from 1 to DIM can be used.""",
command = "-x", text = 1,)

Args ["p2b-y"] = Argument (label = "The Y axis:", description = """\
Sets the number of the coordinate to be treated as the Y axis (default: 2).
This option is especially useful if the dimension of cubes exceeds 2.
Thios number should be different from the one used with the '-x' option.""",
command = "-y", text = 1,)

Args ["p2b-s"] = Argument (label = "Size of squares:", description = """\
Sets the size of squares that correspond to single cubes. By default, this
value is set to 1, which means that each cube is plotted as a single pixel.
Set this size to a higher value and use the '-p' option for nice plots!""",
command = "-s", text = 1,)

Args ["p2b-p"] = Argument (label = "Pause between:", description = """\
Sets the size of the pause between squares to make them look disjoint.
The default value is 0, but one can use 1 or more to clearly distinguish
each square. Combine this option with '-s' for nice-looking plots!""",
command = "-p", text = 1,)

Args ["p2b-g"] = Argument (label = "Grid lines:", description = """\
Makes the program plot grid lines at the border of plotted squares.
The lines are plotted every n squares, where n is the number given
as a paramter. Combine this with '-p1' or '-p3' for better clarity.""",
command = "-g", text = 1,)

Args ["p2b-m"] = Argument (label = "Margins:", description = """\
Sets the margins that are going to be left at each side of the plotted
squares. The margins are given in terms of the number of blank squares,
not pixels. By default, the margins are 0.""",
command = "-m", text = 1,)

Args ["p2b-f"] = Argument (label = "Fill background", description = """\
Fills the background of the picture with the given gray shade.
0 corresponds to white (this is the default),
255 corresponds to black.""",
command = "-f", text = 1,)

Args ["p2b--bar"] = Argument (label = "Color bar:", description = """\
Adds a color bar at the bottom of the picture to indicate which colors
were used for plotting the consecutive sets of full cubes. The size of
the color bar may be given; otherwise it is automatically determined.""",
command = "--bar", text = 1, emptyOK = 1,)

Args ["p2b-l"] = Argument (label = "Left edge:", description = """\
Sets fixed X coordinate to the left edge of the picture.
By default, the minimum of all the cubes to plot is taken.
This option is useful to plot a series of pictures with cubical sets.""",
command = "-l", text = 1,)

Args ["p2b-b"] = Argument (label = "Bottom edge:", description = """\
Sets fixed Y coordinate to the bottom edge of the picture.
By default, the minimum of all the cubes to plot is taken.
This option is useful to plot a series of pictures with cubical sets.""",
command = "-b", text = 1,)

Args ["p2b-w"] = Argument (label = "Minimal width:", description = """\
Sets the minimal width of the area used for plotting cubes in a picture,
even if the cubes would normally fit within a smaller area.
This option is useful to plot a series of pictures with cubical sets.""",
command = "-w", text = 1,)

Args ["p2b-h"] = Argument (label = "Minimal height:", description = """\
Sets the minimal height of the area used for plotting cubes in a picture,
even if the cubes would normally fit within a smaller area.
This option is useful to plot a series of pictures with cubical sets.""",
command = "-h", text = 1,)

Args ["p2b-r"] = Argument (label = "Resolution:", description = """\
Sets the resolution of the output picture in dpi (dots per inch).
This may be important while importing the picture to some graphics programs
or while converting it to other formats (like eps or pdf).""",
command = "-r", text = 1,)

Args ["p2b--gray"] = Argument (label = "Use gray shades", description = """\
Plots the sets of full cubes using different levels of gray instead of
different colors. The first set is plotted in black, and the subsequent
ones are plotted with brighter shades of gray.""",
command = "--gray",)


# ---------------------------------------------------
# ---------------------- chomp ----------------------
# ---------------------------------------------------

AddProg ("chomp", 0, """\
The program 'chomp' is the most flexible and simple to use program in the
CHomP package for the homology computation of cubical sets. It supports
many input file formats, and it allows to use a variety of algorithms.""",
"computes homology of cubical sets",
"chompX", "chompA", "|", "-b", "--engine", "-w", "|",
"--quiet", "--verbose", "--help", "--engines", "--files", "--log",)

Args ["chompX"] = Argument (label = "Cubical set X:", description = """\
Choses a file which contains a cubical set whose homology is to be computed.
The cubical set can be defined in one of several formats accepted by the
program. For most engines, however, this file is translated to a bitmap.""",
readfile = 1, always = 1,)

Args ["chompA"] = Argument (label = "Cubical set A:", description = """\
Choses a file which contains the second cubical set in the pair (X,A)
for the relative homology computation. Note that only a few homology engines
are capable of computing relative homology, so use some caution.""",
readfile = 1, always = 1,)

Args ["--engine"] = Argument (label = "Homology engine:", description = """\
Chooses the homology engine to use. Available engines: BK, BK_LT, MM_AR,
MM_ASLT, MM_CR, PP. If no engine is selected, then an optimal one is chosen
judging by the contents of the files with cubical sets to be processed.""",
command = "--engine", text = 1,)

Args ["--engines"] = Argument (label = "Show engines", description = """\
Shows a list of homology engines available to choose in this program,
and does not run any computations. Each engine is shown with its brief
description. Note that some engines support more features than others.""",
command = "--engines", short = 1,)

Args ["--files"] = Argument (label = "Show file formats", description = """\
Shows a list of acceptable file formats with their brief descriptions.
Note that different file formats may be more suitable for storing cubical
sets. However, for most engines, these sets are converted to a bitmap.""",
command = "--files", short = 1,)

Args ["--verbose"] = Argument (label = "Verbose", description = """\
Switches the verbose output on, which results in a lot of detailed
information being displayed about the computation details - stages of
the algorithms, partial results, statistics, computation times, etc.""",
command = "--verbose", short = 1,)

Args ["-w"] = Argument (label = "Space wrapping:", description = """\
Sets space wrapping a.k.a. periodic boundary conditions. Enter a positive
integer to set equal space wrapping in all directions, or a space-separated
sequence of numbers to set wrapping in each direction (0 = no wrapping)""",
command = "-w", multiple = 1,)

Args ["-b"] = Argument (label = "Save Betti numbers:", description = """\
Saves the computed Betti numbers to the given file in the text format.
One number is written per line, including zero Betti numbers.
The torsion coefficients are not written to the file.""",
command = "-b", writefile = 1,)


# ---------------------------------------------------
# ----------------- homcubes (sets) -----------------
# ---------------------------------------------------

AddProg ("homcubes (sets)", 0, """\
The program 'homcubes' is a powerful program for the homology computation
of full cubical sets as well as cubical sets built of elementary cubes.
It can compute relative homology and it supports space wrapping.""",
"computes homology of cubical sets",
"homcubX", "homcubA", "|", "-w", "-b", "-g", "-p", "|",
"-k", "-c", "-s", "-l", "|", "--quiet", "--help", "--log",)

Args ["homcubX"] = Argument (label = "Cubical set X:", description = """\
Choses a file which contains a cubical set whose homology is to be computed.
This can be either a full cubical set, or a general cubical complex
encoded in the text format.""",
readfile = 1, always = 1,)

Args ["homcubA"] = Argument (label = "Cubical set A:", description = """\
Choses a file which contains the second cubical set in the pair (X,A)
for the relative homology computation. Note that if X is a full cubical set,
then A must also be a full cubical set. This argument is optional.""",
readfile = 1, always = 1,)

Args ["-k"] = Argument (label = "Cubes to keep:", description = """\
Selects a file that contains a set of cubes or cells to keep during the
reduction. These cubes or cells will not be removed from the set(s)
even if this removal would not change the homology.""",
command = "-k", readfile = 1,)

Args ["-g"] = Argument (label = "Save generators:", description = """\
Chooses a file to which the homology generators will be saved.
Note that this makes the computations slightly longer.
The generators are saved in a human-readable text format.""",
command = "-g", writefile = 1,)

Args ["-c"] = Argument (label = "A list of cubes:", description = """\
Makes the program read the given list of full cubes and assign integer
numbers to cubes in the order the cubes appear in this file. Then each full
cube can be represented by this number instead of a list of coordinates.""",
command = "-c", readfile = 1,)

Args ["-s"] = Argument (label = "Save intermediate data:", description = """\
Defines a file prefix to save intermediate data at several stages of the
homology computation. The actual files will have names that start with the
prefix and have some additional letters and suffixes appended.""",
command = "-s", writefile = 1,)

Args ["-l"] = Argument (label = "Level to compute:", description = """\
Makes the program compute only the specified homology level instead of
all the levels that may be nonzero, depending on the space dimension.
Use this option if interested in some Betti numbers only.""",
command = "-l", text = 1,)

Args ["-p"] = Argument (label = "Coefficients in Z_p:", description = """\
Sets the prime number 'p' to compute the homology with coefficients in the
field Z_p instead of the default ring Z. Note that this gets rid of torsion,
makes the computations simpler, and protects agains number overflow.""",
command = "-p", text = 1,)


# ---------------------------------------------------
# ----------------- homcubes (maps) -----------------
# ---------------------------------------------------

AddProg ("homcubes (maps)", 0, """\
The program 'homcubes' is also capable of computing the homomorphism induced
in homology by a combinatorial cubical multivalued map. The map must satisfy
certain assumptions, use the "chkmvmap" program to verify them.""",
"computes homomorphisms induced in homology by maps",
"mapF", "mapX", "mapA", "mapY", "mapB", "|",
"-w", "-k1", "-k2", "-g3", "-p", "-c", "-s", "-l", "-a", "-i", "|",
"--quiet", "--help", "--log",)

Args ["mapF"] = Argument (label = "Cubical map F:", description = """\
Choses a file which contains the definition of the combinatorial cubical
multivalued map to compute the homomorphism induced in homology by. The map
can be in one of two acceptable formats, see the CHomP documentation.""",
readfile = 1, always = 1,)

Args ["mapX"] = Argument (label = "Cubical set X:", description = """\
Choses a file which contains the definition of the domain of the map.
If relative homology is taken into consideration, this file should contain
the full cubes in X or X\\A, and the files for A and B should be given.""",
readfile = 1, always = 1,)

Args ["mapA"] = Argument (label = "Cubical set A:", description = """\
Choses a file which contains the second cubical set in the pair (X,A)
for the relative homology computation. Note that if this file name is
given, then also file names for Y and B must be provided.""",
readfile = 1, always = 1,)

Args ["mapY"] = Argument (label = "Cubical set Y:", description = """\
Choses a file which contains the definition of the codomain of the map.
If relative homology is taken into consideration, this file should contain
the full cubes in Y or Y\\B, and the files for A and B should be given.""",
readfile = 1, always = 1,)

Args ["mapB"] = Argument (label = "Cubical set B:", description = """\
Choses a file which contains the second cubical set in the pair (Y,B)
for the relative homology computation. Note that if this file name is
given, then also file names for X, Y and A must be provided.""",
readfile = 1, always = 1,)

Args ["-k1"] = Argument (label = "Cubes to keep in X:", description = """\
Selects a file that contains a set of cubes or cells to keep during the
reduction of X. These cubes or cells will not be removed from the set(s)
even if this removal would not change the homology.""",
command = "-k", readfile = 1,)

Args ["-k2"] = Argument (label = "Cubes to keep in Y:", description = """\
Selects a file that contains a set of cubes or cells to keep during the
reduction of Y. Make sure that a set of cubes to keep in X is also specified,
because this option corresponds to the same command-line switch '-k'.""",
command = "-k", readfile = 1,)

Args ["-g3"] = Argument (label = "Save generators:", description = """\
Chooses a file or files to save the computed homology generators to. If more
than one file is given, the generators of X or (X,A) are stored in the first
file, Y - in the second file, and the graph of F - in the third one.""",
command = "-g", writefile = 1, multiple = 1,)

Args ["-a"] = Argument (label = "Preserve acyclicity", description = """\
Turns on the thorough acyclicity verification during the reduction of full
cubical sets. This switch slows down the computations, but prevents the
situations in which the acyclicity of the map is lost during reduciton.""",
command = "-a", short = 1,)

Args ["-i"] = Argument (label = "Conley index map", description = """\
Makes the program compute the endomorphism induced in homology on the
Conley index pair. This is the map F_* composed with the inverse of the
inclusion (X,A) -> (Y,B). This inclusion must be an isomorphism.""",
command = "-i", short = 1,)


# ---------------------------------------------------
# -------------------- homcub2l ---------------------
# ---------------------------------------------------

AddProg ("homcub2l", 1, """\
The program 'homcub2l' computes the Conley index map for an index pair.
It uses a double-layer construction which preserves the excision property.
See a paper by K. Stolot and P. Pilarczyk for more information.""",
"computes Conley index maps in homology",
"mapF", "map2X", "map2A", "|", "-w", "-p", "-c", "-s", "-a", "|",
"--quiet", "--help", "--log",)

Args ["map2X"] = Argument (label = "Cubical set X:", description = """\
Choses a file which contains the definition of the domain of the map.
This should be a list of full cubes in X or in X\\A. The map must be
defined on all the cubes in this set.""",
readfile = 1, always = 1,)

Args ["map2A"] = Argument (label = "Cubical set A:", description = """\
Choses a file which contains the exit set, that is, the list of cubes
in A such that (X,A) is the index pair. By definition of the index pair,
the images of cubes from A must be disjoint with X\\A.""",
readfile = 1, always = 1,)


# ---------------------------------------------------
# -------------------- chkmvmap ---------------------
# ---------------------------------------------------

AddProg ("chkmvmap", 1, """\
The program 'chkmvmap' analyzes a combinatorial cubical multivalued map
to check if it satisfies various assumptions necessary for being suitable
for the homology computation with the programs 'homcubes' and 'homcub2l'.""",
"checks a combinatorial cubical multivalued map",
"mapF", "mapX", "mapA", "mapY", "mapB", "|",
"-w", "chk-i", "chk-a0", "chk-a1", "|",
"--quiet", "--help", "--log",)

Args ["chk-i"] = Argument (label = "Conley index map", description = """\
Verifies additional assumptions necessary to compute the endomorphism
induced in homology on the Conley index pair. It doesn't verify if the
inclusion (X,A) -> (Y,B) is an isomorphism, however.""",
command = "-i", short = 1,)

Args ["chk-a0"] = Argument (label = "Don't verify acyclicity",
description = """\
Turnes off acyclicity verification. The acyclicity is verified by the program
'homcubes' during the homology computation. If the image of each full cube
is a convex set (a rectangular box), then such a map is always acyclic.""",
command = "-a0", short = 1,)

Args ["chk-a1"] = Argument (label = "Verify acyclicity only",
description = """\
Makes the program verify the acyclicity of the map only. Other properties
of the map are not checked. This does not save much time, however,
because the other verifications are relatively fast.""",
command = "-a1", short = 1,)


# ---------------------------------------------------
# -------------------- cnvmvmap ---------------------
# ---------------------------------------------------

AddProg ("cnvmvmap", 1, """\
The program 'cnvmvmap' converts a combinatorial cubical multivalued map from
the format based on corners of rectangular areas to the general format, e.g.,
"[(0,0)(1,1)] [(3,3)(4,5)]" is converted to "(0,0) -> {(3,3) (3,4)}".""",
"converts a combinatorial cubical multivalued map",
"cnvmapI", "cnvmapO", "cnvmap-c", "|",
"--quiet", "--help", "--log",)

Args ["cnvmapI"] = Argument (label = "Input map:", description = """\
Selects the input file with a text definition of a combinatorial cubical
multivalued map to convert. The input assignments can be in both formats,
and they are both translated to the general format.""",
readfile = 1, always = 1,)

Args ["cnvmapO"] = Argument (label = "Output map:", description = """\
Selects the output file to write the converted map to. The assignments
are in the general format in which each full cube is mapped to a set
of cubes encoded as a space-separated list enclosed in braces.""",
writefile = 1, always = 1,)

Args ["cnvmap-c"] = Argument (label = "Do a reverse conversion",
description = """\
Makes the program do a conversion in the reverse direction, from a map in the
general format to a map with assignments defined as rectangular areas.
Note that this usage is limited and may not be fully functional.""",
command = "-c",)


# ---------------------------------------------------
# -------------------- homsimpl ---------------------
# ---------------------------------------------------

AddProg ("homsimpl", 0, """\
The program 'homsimpl' computes the homology of simplicial polyhedra.
Each vertex is given a positive number. A simplex is an n-tuple of vertices.
Faces of simplices are automatically added to the simplicial complex.""",
"computes homology of simplicial complexes",
"homsimplX", "homsimplA", "|",
"simpl-k", "-b", "-g", "-s", "-p", "simpl-d", "|",
"--quiet", "--help", "--log",)

Args ["homsimplX"] = Argument (label = "Simplices in X:", description = """\
Choses a file which contains a set of simplices which build a polyhedron
whose homology is to be computed. Lower-dimensional faces of simplices will
be added automatically, unless the option "-d" is chosen (see below).""",
readfile = 1, always = 1,)

Args ["homsimplA"] = Argument (label = "Simplices in A:", description = """\
Choses a file which contains a set of simplices which define the second
polyhedron for relative homology computation. Note that the polyhedron A
should be a subset of the polyhedron X. This argument is optional.""",
readfile = 1, always = 1,)

Args ["simpl-k"] = Argument (label = "Simplices to keep:", description = """\
Selects a file that contains a set of simplices to keep during the
reduction. These simplices will not be removed from the set(s)
even if this removal would not change the homology.""",
command = "-k", readfile = 1,)

Args ["simpl-d"] = Argument (label = "Don't add faces", description = """\
Suppresses the program from adding lower-dimensional faces to simplices
in order to generate a full simplicial complex. As a result, faces not listed
in X are assumed to be in A, and relative homology is computed.""",
command = "-d")


# ---------------------------------------------------
# -------------------- homchain ---------------------
# ---------------------------------------------------

AddProg ("homchain", 1, """\
The program 'homchain' computes the homology of chain complexes and the
homomorphisms induced by chain maps in homology. It is the "lowest-level"
homology program which works directly with algebraic data structures.""",
"computes homology of chain complexes and chain maps",
"homchainX", "homchainY", "homchainF", "|", "-g", "-p", "-d", "|",
"--quiet", "--help", "--log",)

Args ["homchainX"] = Argument (label = "Chain complex X:", description = """\
Choses a file which contains a chain complex whose homology module is to be
computed. For relative homology, this chain complex must already not contain
the generators of the other element of the pair (X,A).""",
readfile = 1, always = 1,)

Args ["homchainY"] = Argument (label = "Chain complex Y:", description = """\
Choses a file which contains a chain complex of the codomain of a map or maps
if there are any. If the homomorphism induced by endomorphisms in homology is
to be computed, then Y should not be specified at all.""",
readfile = 1, always = 1,)

Args ["homchainF"] = Argument (label = "Chain map(s):", description = """\
Choses a file or files which contain the definition of chain maps for which
the homomorphism(s) induced in homology are to be computed. The domain and
codomain of all the maps are the same and specified above.""",
readfile = 1, multiple = 1, always = 1,)


# ---------------------------------------------------
# -------------------- showcubes --------------------
# ---------------------------------------------------

AddProg ("showcubes", 0, """\
The program 'showcubes' displays 3-dimensional full cubical sets
in an interactive way with the use of OpenGL. Note that this program
must be compiled with the wxWidgets library to function correctly.""",
"shows 3-dimensional full cubical sets",
"show1", "show2", "show3", "show4", "show5", "show6", "|", "showY")

showcubesdesc = description = """\
Selects a file that defines a 3-dimensional full cubical set to display.
Each cube is represented by a triple of integer coordinates. Each set is
shown with a different color. The program is interactive."""

Args ["show1"] = Argument (label = "First cubical set:",
description = showcubesdesc, readfile = 1, always = 1,)

Args ["show2"] = Argument (label = "Second cubical set:",
description = showcubesdesc, readfile = 1, always = 1,)

Args ["show3"] = Argument (label = "Third cubical set:",
description = showcubesdesc, readfile = 1, always = 1,)

Args ["show4"] = Argument (label = "Fourth cubical set:",
description = showcubesdesc, readfile = 1, always = 1,)

Args ["show5"] = Argument (label = "Fifth cubical set:",
description = showcubesdesc, readfile = 1, always = 1,)

Args ["show6"] = Argument (label = "Sixth cubical set:",
description = showcubesdesc, readfile = 1, always = 1,)

Args ["showY"] = Argument (label = "More cubical sets:", description = """\
Lists additional files with 3-dimensional full cubical sets to display.
Note that the number of colors is limited, so additional files may be
displayed with the same colors as previous ones. Have fun!""",
readfile = 1, multiple = 1, always = 1,)


# ---------------------------------------------------
# --------------------- cub2map ---------------------
# ---------------------------------------------------

AddProg ("cub2map", 0, """\
The program 'cub2map' converts full cubical sets into combinatorial
cubical multivalued maps in two ways: 1. The cubical set is treated as the
graph of the map, 2. The identity map on the cubical set is created.""",
"converts a cubical set to a cubical map",
"cub2mapI", "cub2mapO", "cub2map-i", "|",
"cub2map-x", "cub2map-y", "cub2map-c", "|",
"--quiet", "--help", "--log",)

Args ["cub2mapI"] = Argument (label = "Set of cubes:", description = """\
Choses a file which contains a set of full cubes which define the input
cubical set. Depending on the switch "-i" (see below), this is the graph
of the map, or a set on which the identity map is to be created.""",
readfile = 1, always = 1,)

Args ["cub2mapO"] = Argument (label = "Output map file:", description = """\
Defines a file to which an output map will be written. If writing the
identity map, it can be written in one of two different formats,
depending on whether the "-c" switch is used or not.""",
writefile = 1, always = 1,)

Args ["cub2map-i"] = Argument (label = "Identity map", description = """\
Makes the program write the identity map on the input set of cubes (otherwise
the program extracts a map from its graph). The default format in which this
map is written can be changed by the "-c" switch (see below).""",
command = "-i",)

Args ["cub2map-x"] = Argument (label = "Dim of domain:", description = """\
Defines the dimension of the domain of the map if extracting a map from its
graph. If this argument is not specified, this dimension is determined as
a half of the dimension of the full cubes in the input file.""",
command = "-x", text = 1,)

Args ["cub2map-y"] = Argument (label = "Dim of codomain:", description = """\
Defines the dimension of the codomain of the map if extracting a map from its
graph. If this argument is not specified, this dimension is determined as
a half of the dimension of the full cubes in the input file.""",
command = "-y", text = 1,)

Args ["cub2map-c"] = Argument (label = "Use the \"chmap\" format for output",
description = """\
If "-i" is selected above, this option makes the program write the identity
map in the format used primarily in the "chmap" program. Each assignment is
defined by two cells specified by their minimal and maximal corners.""",
command = "-c",)


# ---------------------------------------------------
# --------------------- map2cub ---------------------
# ---------------------------------------------------

AddProg ("map2cub", 1, """\
The program 'map2cub' extracts full cubical sets from a combinatorial
cubical multivalued map. It can either create the graph of the map,
or export its domain or image, all three operations in one pass.""",
"extracts cubical sets from a cubical map",
"map2cubI", "map2cub-d", "map2cub-i", "map2cub-g", "|", "map2cub-m", "|",
"--quiet", "--help", "--log",)

Args ["map2cubI"] = Argument (label = "The input map:", description = """\
Choses a file which contains a definition of a combinatorial cubical
multivalued map to process. The assignments can be in the two accaptable
formats. At least one output type below must be specified.""",
readfile = 1, always = 1,)

Args ["map2cub-d"] = Argument (label = "Save the domain:", description = """\
Defines a file to which the domain of the map will be saved.
The domain consists of all the full cubes or cubical cells on which
the map images are defined.""",
command = "-d", writefile = 1,)

Args ["map2cub-i"] = Argument (label = "Save the image:", description = """\
Defines a file to which the image of the map will be saved.
The image consists of all the full cubes or cubical cells which
appear in the images of domain elements by the map.""",
command = "-i", writefile = 1,)

Args ["map2cub-g"] = Argument (label = "Save the graph:", description = """\
Defines a file to which the graph of the map will be saved. The graph
consists of all the full cubes or cubical cells which are the Cartesian
products of domain elements with the corresponding image elements.""",
command = "-g", writefile = 1,)

Args ["map2cub-m"] = Argument (label = "Multiply images:", description = """\
Makes the program multiply the coordinates of the images of cells by the map
by the given integer number. This can be useful to process maps whose domain
is at a different scale than the image, for example.""",
command = "-m", text = 1,)



# ---------------------------------------------------
# -------------------- maprestr ---------------------
# ---------------------------------------------------

AddProg ("maprestr", 1, """\
The program 'maprestr' computes the restriction of a combinatorial cubical
multivalued map to a subset of its domain. The result is saved in the same
format as the input map.""",
"extracts cubical sets from a cubical map",
"maprestrI", "maprestrD", "maprestrO", "|",
"--quiet", "--help", "--log",)

Args ["maprestrI"] = Argument (label = "The input map:", description = """\
Chooses a file for the input map whose restriction
is to be computed. The map can be in one of the two
text formats supported by other programs.""",
readfile = 1, always = 1,)

Args ["maprestrD"] = Argument (label = "The new domain:", description = """\
Selects a file with the full cubical set to which the map
is to be restricted. This must be a subset of the domain
of the map.""",
readfile = 1, always = 1,)

Args ["maprestrO"] = Argument (label = "The output map:", description = """\
Defines a file to which the restriction of the map is saved.
The map is saved in the same format as the input map.
Any previous contents of the file is overwritten.""",
writefile = 1, always = 1,)


# ---------------------------------------------------
# -------------------- cubchain ---------------------
# ---------------------------------------------------

AddProg ("cubchain", 1, """\
The program 'cubchain' generates a chain complex of a full cubical set.
All cubes and their faces are generators of the chain groups, and the
formulas for the boundaries of these generators are also computed.""",
"creates a chain complex of a full cubical set",
"cubchainX", "cubchainA", "cubsimchainO", "|",
"cubchain-c", "cubchain-p", "-d", "-w", "|",
"--quiet", "--help", "--log",)

Args ["cubchainX"] = Argument (label = "Cubical set X:", description = """\
Choses a file which contains a cubical set whose chain complex is to be
computed. This file is assumed to contain full cubes defined as n-tuples
of integers, unless the switch "-c" is chosen (see below).""",
readfile = 1, always = 1,)

Args ["cubchainA"] = Argument (label = "Cubical set A:", description = """\
Choses a file which contains the second cubical set in the pair (X,A)
whose relative chain complex is to be computed.
This argument is optional.""",
readfile = 1, always = 1,)

Args ["cubsimchainO"] = Argument (label = "Chain complex:",
description = """\
Defines a file to which the computed chain complex will be written. The
homology of this chain complex can be computed with the "homchain" program.
For maximum efficiency, use the "-d" switch below and with "homchain".""",
writefile = 1, always = 1,)

Args ["cubchain-c"] = Argument (label =
"Read cubical CELLS from the input file", description = """\
Makes the program read cubical cells from the input file, instead of full
cubes. The cells are defined either as Cartesian products of intervals,
or by specifying their opposite corners.""",
command = "-c",)

Args ["-d"] = Argument (label = "Digital names of generators",
description = """\
Makes the program use numbers for names of generators of chain complexes.
In general, these names can be arbitrary words, but such words can be long
and indexing them inefficient, so using numbers saves a lot of time.""",
command = "-d",)

Args ["cubchain-p"] = Argument (label =
"Write cubical cells as products of intervals", description = """\
Unless the "-d" option is used, makes the program write cubical cells in the
output file in terms or products of intervals, possibly degenerate, e.g.,
(0,1)x(2)x(4,5). Otherwise, the cells are represented by two corners.""",
command = "-p",)


# ---------------------------------------------------
# -------------------- simchain ---------------------
# ---------------------------------------------------

AddProg ("simchain", 1, """\
The program 'simchain' generates a chain complex of a simplicial polyhedron.
All simplices and their faces are generators of the chain groups, and the
formulas for the boundaries of these generators are also computed.""",
"creates a chain complex of a simplicial polyhedron",
"simchainX", "simchainA", "cubsimchainO", "|", "-d", "|",
"--quiet", "--help", "--log",)

Args ["simchainX"] = Argument (label = "Set of simplices X:",
description = """\
Choses a file which contains a set of simplices which define a polyhedron
whose chain complex is to be computed. Each simplex is defined as an n-tuple
of integers. Missing lower-dimensional faces are automatically added.""",
readfile = 1, always = 1,)

Args ["simchainA"] = Argument (label = "Set of simplices A:",
description = """\
Choses a file which contains the second simplicial polyhedron in the pair
(X,A) whose relative chain complex is to be computed. Also, missing faces
of higher-dimensional simplices are added. This argument is optional.""",
readfile = 1, always = 1,)


# ---------------------------------------------------
# --------------------- cubdiff ---------------------
# ---------------------------------------------------

AddProg ("cubdiff", 1, """\
The program 'cubdiff' computes the difference of sets of full cubes,
or sets of general cubical cells. The result contains those cells
listed in the first file which are not listed in the second file.""",
"computes the difference of sets of cubes",
"diffX", "diffY", "diffZ", "|", "diff-x", "diff-y", "|",
"--quiet", "--help", "--log",)

Args ["diffX"] = Argument (label = "Set of cubes X:", description = """\
Chooses a file that contains the set of full cubes or cubical cells X.
The program computes Z = X \\ Y. The computation is done on a combinatorial
basis, only cubes in Y identical with those in X are removed.""",
readfile = 1, always = 1)

Args ["diffY"] = Argument (label = "Set of cubes Y:", description = """\
Chooses a file that contains the set of full cubes or cubical cells Y.
The program computes Z = X \\ Y. The computation is done on a combinatorial
basis, only cubes in Y identical with those in X are removed.""",
readfile = 1, always = 1)

Args ["diffZ"] = Argument (label = "Set of cubes Z:", description = """\
Chooses a file that contains the set of full cubes or cubical cells Z.
The program computes Z = X \\ Y. The computation is done on a combinatorial
basis, only cubes in Y identical with those in X are removed.""",
writefile = 1, always = 1)

Args ["diff-x"] = Argument (label = "Keep the set X in the memory",
description = """\
Makes the program keep the set X in the memory and scan the set Y.
This is beneficial if the set X is smaller than the set Y, but may result
in saving the elements to Z in a different order than they appear in X.""",
command = "-x",)

Args ["diff-y"] = Argument (label = "Keep the set Y in the memory",
description = """\
Makes the program keep the set Y in the memory and scan the set X. This is
beneficial if the set Y is smaller than the set X. The order of elements
saved to Z is the same as they appear in X. This option is the default.""",
command = "-y",)


# ---------------------------------------------------
# --------------------- cubslice --------------------
# ---------------------------------------------------

AddProg ("cubslice", 1, """\
The program 'cubslice' splits a set of full cubes into subsets which are
contained within slices of the space that lie between hyperplanes which
are arranged so that the number of cubes in each slice is similar.""",
"splits a sets of cubes into slices",
"sliceI", "sliceO", "|", "TODO", "|",
"--quiet", "--help", "--log",)

Args ["sliceI"] = Argument (label = "Input file name:", description = """\
Chooses a file that contains the set of full cubes which is going to be
divided into slices. The thickness of the slices is automatically
computed so that the number of cubes in each slice is similar.""",
readfile = 1, always = 1)

Args ["sliceO"] = Argument (label = "Prefix of slices:", description = """\
Chooses a file prefix for the file names to write the slices to.
The consecutive number of each slice is appended, starting with 1,
and the extension ".cub" is added to each file name that is written.""",
writefile = 1, always = 1)


# ---------------------------------------------------
# -------------------- psetconn ---------------------
# ---------------------------------------------------

AddProg ("psetconn", 1, """\
The program 'psetconn' computes the connected components
of a full cubical set. Each component is saved to a different
file. The output file names are automatically generated.""",
"splits a full cubical set into connected components",
"connI", "connO", "|",
"--quiet", "--help", "--log",)

Args ["connI"] = Argument (label = "Input cubical set:", description = """\
Choses an input file with the text definition of a full cubical set whose
connected components are to be computed. The cubes are considered adjacent
if they have a common face of any dimension (including a vertex).""",
readfile = 1, always = 1)

Args ["connO"] = Argument (label = "Output file prefix:", description = """\
Defines a prefix for file names to write the connected components to.
Each component is saved to a file whose name consists of the prefix,
an integer that counts the components, and the suffix '.cub'.""",
writefile = 1, always = 1)


# ---------------------------------------------------
# -------------------- psetglue ---------------------
# ---------------------------------------------------

AddProg ("psetglue", 1, """\
The program 'psetglue' computes the union of several lists of points or full
cubical sets. It reads the sets into memory and writes one set that contains
a list of all the points or full cubes that appear in those sets.""",
"glues full cubical sets together",
"glueI", "glueO", "glue-p", "|",
"--quiet", "--help", "--log",)

Args ["glueI"] = Argument (label = "Input file(s):", description = """\
Choses input files with lists of points or full cubes represented in the
form of n-tuples of integer numbers in parentheses, e.g., (1,-12,3).
Note that the range of integers is limited to short int: [-32768,32767].""",
readfile = 1, multiple = 1, always = 1,)

Args ["glueO"] = Argument (label = "Output file:", description = """\
Defines the output file name to which the union of the sets of points or full
cubes will be written. Each point is written only once. The order of points
corresponds to the order in which they are read from the input files.""",
writefile = 1, always = 1,)

Args ["glue-p"] = Argument (label = "Pause before writing the output set",
description = """\
Makes a pause and waits for the user to press the Enter key after having read
all the input files, but before writing the output file. This function may be
useful if one wants to check how much memory is used by the program.""",
command = "-p",)


# ---------------------------------------------------
# --------------------- more... ---------------------
# ---------------------------------------------------

AddProg ("[more...]", 1, """\
More program descriptions are on their way.
Please, download this program after a few weeks
and you should see more... hopefully :)""",
"more programs coming soon",
"TODO",)


# ---------------------------------------------------
# ---------------- common arguments -----------------
# ---------------------------------------------------

Args ["--help"] = Argument (label = "Show help info", description = """\
Displays brief information on the program's usage to the screen.
No computations are done even if other arguments are selected.
If the output is too large, try adding "|more" to the command line.""",
command = "--help", short = 1,)

Args ["--quiet"] = Argument (label = "Quiet", description = """\
Runs the program in the 'quiet' mode which means that no the messages
are displayed to the standard output stream. However, if logging to a file
is turned on, then all these messages are stored in the file instead.""",
command = "--quiet", short = 1,)

Args ["--log"] = Argument (label = "Save log to a file:", description = """\
Saves the output of the program to a text file.
Enter the name of the file in the box provided. Any previous contents
of the file will be overwritten. Progess indicators are not saved.""",
command = "--log", writefile = 1,)

Args ["TODO"] = Argument (label = "\n\n\n<<< UNDER CONSTRUCTION >>>\n\n\n",
description = """\
The description of the command-line arguments of this program is currently
under construction. Please, click the 'Show help info' button and run the
program for a brief description. We apologize for the inconvenience.""",
always = 1,)

Args ["|"] = Argument (separator = 1)


# ---------------------------------------------------
# ---------------------- main -----------------------
# ---------------------------------------------------

rootWindow = Tk ()
chompChooser = ChompChooser (master = rootWindow)
chompChooser.master.title ("CHomP Programs Chooser")
#chompChooser.master.title ("CHomP Programs Chooser - UNDER CONSTRUCTION")
chompChooser.mainloop ()


