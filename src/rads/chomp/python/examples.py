"""
This is the CHomP Examples Chooser, an interactive application which
helps browsing and running the examples available in the CHomP package.

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

Started on July 26, 2010 (from chooser.py). Last revision: August 28, 2010.
"""

# ---------------------------------------------------
# -------------------- overture ---------------------
# ---------------------------------------------------

from Tkinter import *
#from Tix import *
#from Tkconstants import *
from tkFileDialog import *
from tkMessageBox import *
import tkFont
import os
import string


# ---------------------------------------------------
# ------------------ CHomP Chooser ------------------
# ---------------------------------------------------

class ChompChooser (Frame):
	"The main application class of the CHomP Examples chooser program."

	def __setPath (self, dirname = None):
		"Sets the directory in which the examples are located."
		# if no directory name was given, try a few defaults
		if not dirname:
			if self.path.get () == "":
				self.__setPath ("examples")
			if self.path.get () == "":
				self.__setPath (os.curdir)
			if self.path.get () == "":
				self.__setPath (os.pardir + os.sep +
					"examples")
			if self.path.get () == "":
				self.__setPath (os.pardir + os.sep +
					"chomp" + os.sep + "examples")
			return

		# verify if the directory exists
		if not os.path.exists (dirname):
			return

		# check if there exists at least one example
		files = os.listdir (dirname)
		for filename in files:
			if string.find (filename, ".txt") < 0:
				continue
			if os.path.isfile (os.path.join (dirname, filename)):
				self.path.set (dirname)
				break

		# determine a list of scripts in that directory
		self.Scripts = {}
		if os.name == 'posix':
			ScriptExtension = 'sh'
		elif os.name == 'nt':
			ScriptExtension = 'bat'
		else:
			ScriptExtension = ''
		for filename in files:
			try:
				basename, extension = \
					string.rsplit (filename, '.')
			except:
				basename = filename
				extension = ''
			if not self.Scripts.has_key (basename):
				self.Scripts [basename] = None
			if extension == ScriptExtension:
				self.Scripts [basename] = filename

		# generate a new list of examples available in that directory
		self.Examples = []
		self.Descriptions = {}
		for filename in files:
			try:
				basename, extension = \
					string.rsplit (filename, '.')
			except:
				continue
			if not extension == 'txt':
				continue
			if not self.Scripts [basename]:
				continue
			fullfilename = os.path.join (dirname, filename)
			if not os.path.isfile (fullfilename):
				continue
			try:
				txtfile = open (fullfilename, "rU")
				self.Descriptions [basename] = \
					txtfile.read ()
				txtfile.close ()
			except:
				continue
			self.Examples.append (basename)
		self.Examples.sort ()

		# add the dummy entry "none"
		if not len (self.Examples):
			self.Examples. append ('[None]')
			self.Descriptions ['[None]'] = \
				"There are no examples " + \
				"in the provided directory."
			self.Scripts ['[None]'] = None

		# create new arrays of examples
		self.basicList = []
		self.fullList = []
		for prog in range (len (self.Examples)):
			if self.BasicList.has_key (self.Examples [prog]):
				self.basicList.append (self.Examples [prog])
			self.fullList.append (self.Examples [prog])

	def __changePath (self):
		dirWin = Toplevel ()
		dirWin.title ("Change Path")
		dirWin.grid ()
		dirWin.columnconfigure (0, weight = 1)
		dirWin.columnconfigure (1, weight = 1)
		dirLabel = Label (dirWin,
			text = "Enter the path to CHomP examples:")
		dirLabel.grid (columnspan = 2)
		path = StringVar ()
		path.set (self.path.get ())
		dirEnter = Entry (dirWin, textvariable = path)
		dirEnter.focus_set ()
		dirEnter.grid (sticky = E+W, columnspan = 2)
		dirOK = Button (dirWin, text = "OK", command = lambda:
			(dirWin.destroy (), self.__setPath (path.get ()),
			self.__setProgList ()))
		dirOK.grid (row = 2, column = 1)
		dirCancel = Button (dirWin, text = "Cancel",
			command = lambda: dirWin.destroy ())
		dirCancel.grid (row = 2, column = 0)
		dirWin.bind ("<Return>", lambda event: dirOK.invoke ())
		dirWin.bind ("<Escape>", lambda event: dirCancel.invoke ())

	def __setProgList (self):
		"Creates or changes the list of examples in the listbox."
		# remember of fix a program that is selected
		sel = self.progs.curselection ()
		if len (sel) == 1:
			selected = self.proglist [int (sel [0])]
		elif self.curProg != '':
			selected = self.curProg

		# determine a list of examples to include in the list
		if self.full.get () or not len (self.basicList):
			self.proglist = self.fullList
		else:
			self.proglist = self.basicList

		# add the examples to the listbox
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
			defindex = 0
		self.progs.selection_set (first = defindex)
		self.progs.activate (index = defindex)
		if not self.curProg == self.proglist [defindex]:
			self.curProg = self.proglist [defindex]
			self.__showDescription (self.curProg)
		self.progs.see (index = defindex)

	def __showDescriptionLabel (self, contents):
		"Shows the provided description."
		self.description.set (contents)

	def __showDescriptionText (self, contents):
		"Shows the provided description."
		self.desc.delete ('1.0', 'end')
		self.desc.insert ('1.0', contents)

	def __showDescription (self, name):
		"Shows the description of the given example."
		if (self.Descriptions.has_key (name)):
			contents = self.Descriptions [name]
		else:
			contents = ''
		if labelDesc:
			self.__showDescriptionLabel (contents)
		else:
			self.__showDescriptionText (contents)

	def __setProgEvent (self, event):
		"Sets the program frame as chosen from the listbox."
		sel = self.progs.curselection ()
		progNumber = int (sel [0])
		prog = self.proglist [progNumber]
		if prog == self.curProg:
			self.progs.see (index = progNumber)
			return
		self.__showDescription (prog)
		self.progs.focus_set ()
		self.progs.see (index = progNumber)
		self.curProg = prog

	def __runProg (self, name):
		"Runs the selected example."
		if not self.Scripts [name]:
			return
		self.master.withdraw ()
		prevPath = os.getcwd ()
		os.chdir (self.path.get ())
		cmd = os.curdir + os.sep + self.Scripts [name]
		print "Running '%s':" % name
	#	print "-" * 65
		os.system (cmd)
		print "-" * 65
		os.chdir (prevPath)
		self.master.deiconify ()
		self.progs.focus_set ()

	def __runSelectedProg (self):
		"Runs the first example that is selected in the list."
		sel = self.progs.curselection ()
		progNumber = int (sel [0])
		prog = self.proglist [progNumber]
		self.__runProg (prog)

	def __init__ (self, master = None):
		Frame.__init__ (self, master)

		# prepare an empty array of all the examples
		self.Examples = []

		# prepare an empty dictionary of the corresponding scripts
		self.Scripts = {}

		# prepare an empty dictionary of example descriptions
		self.Descriptions = {}

		# prepare a dictionary of basic examples
		self.BasicList = {}
		for prog in range (len (BasicExamples)):
			self.BasicList [BasicExamples [prog]] = 1

		# prepare empty lists for the CHomP examples
		self.basicList = []
		self.fullList = []

		# determine the path for the CHomP examples
		self.path = StringVar ()
		self.__setPath ()

		# prepare a variable for selecting the full list of examples
		self.full = IntVar ()
		self.full.set (0)

		# make the top window resizeable
		top = self.winfo_toplevel ()
		top.rowconfigure (0, weight = 1)
		top.columnconfigure (0, weight = 1)

		# make this window resizeable
		self.grid (sticky = N+S+E+W)
		self.rowconfigure (0, weight = 1)
		self.rowconfigure (1, weight = 0)
		self.rowconfigure (2, weight = 0)
		self.rowconfigure (3, weight = 0)
		self.rowconfigure (4, weight = 0)
		self.columnconfigure (0, weight = 0)
		self.columnconfigure (1, weight = 0)
		self.columnconfigure (2, weight = 1)
		self.columnconfigure (3, weight = 0)

		# create the area for the description of each example
		if labelDesc:
			# create a label for the description
			self.description = StringVar ()
			self.description.set ('')
			self.desc = Label (self, width = 80,
				textvariable = self.description,
				background = "#AAAAFF", justify = LEFT,
				font = ("Courier", "10"))
			self.desc.grid (row = 0, column = 2,
				rowspan = 4, columnspan = 2,
				sticky = N+S+E+W)
		else:
			# create a text field for the description
			self.desc = Text (self, background = "#AAAAFF",
				width = 80, height = 22, takefocus = 0,
				font = ("Courier", "10"))
			self.desc.grid (row = 0, column = 2, rowspan = 4,
				sticky = N+S+E+W)

			# create a vertical scrollbar for the description
			self.descVert = Scrollbar (self,
				orient = VERTICAL, background = "#CCFFCC")
			self.descVert["command"] = self.desc.yview
			self.desc["yscrollcommand"] = self.descVert.set
			self.descVert.grid (row = 0, column = 3,
				rowspan = 4, sticky = N+S)

			# create a horizontal scrollbar for the description
		#	self.descHoriz = Scrollbar (self,
		#		orient = HORIZONTAL, background = "#CCFFCC")
		#	self.descHoriz["command"] = self.desc.xview
		#	self.desc["xscrollcommand"] = self.descHoriz.set
		#	self.descHoriz.grid (row = 3, column = 2,
		#		sticky = W+E)

		# create the bottom line with brief program information
		self.info = Label (self, justify = LEFT, text =
			"CHomP Examples Chooser. Ver. 0.01. " +
			"Copyright (C) 2007-2010 by Pawel Pilarczyk. " +
			"All rights reserved.")
		self.info.grid (row = 4, column = 0, columnspan = 4,
			sticky = N+S+E+W)

		# create a list box for choosing programs
		self.progs = Listbox (self, width = 16, height = 10,
			background = "#CCFFCC")
		self.curProg = ''
		self.__setProgList ()
		self.progs.grid (row = 0, column = 0, sticky = N+S+E+W)
		self.progs.bind ("<Any-ButtonRelease>", self.__setProgEvent)
		self.progs.bind ("<Any-Button>", self.__setProgEvent)
		self.progs.bind ("<Any-KeyPress>", self.__setProgEvent)
		self.progs.bind ("<Any-KeyRelease>", self.__setProgEvent)
		self.progs.bind ("<FocusIn>", self.__setProgEvent)
		self.progs.bind ("<Return>", lambda event:
			self.__runSelectedProg ())
		self.progs.bind ("<Double-Button-1>", lambda event:
			self.__runSelectedProg ())
		self.progs.focus_set ()

		# create a checkbox for the full list of programs
		self.fullchg = Checkbutton (self, text = "Show full list   ",
			background = "#CCFFCC", activebackground = "#CCFFCC",
			variable = self.full, command = self.__setProgList)
		self.fullchg.grid (row = 1, column = 0, columnspan = 2,
			sticky = N+S+W+E)

		# create a scrollbar for the list of programs
	#	self.progScroll = Scrollbar (self, orient = VERTICAL,
	#		background = "#CCFFCC")
	#	self.progScroll["command"] = self.progs.yview
	#	self.progs["yscrollcommand"] = self.progScroll.set
	#	self.progScroll.grid (row = 0, column = 1, sticky = N+S)

		# create a button for changing the path
		self.pathbutton = Button (self, text = "[change path]",
			command = self.__changePath)
		self.pathbutton.grid (row = 2, column = 0,
			columnspan = 2, sticky = N+S+W+E)

		# create the "RUN" button
		self.run = Button (self, text = " RUN ",
			command = lambda: self.__runProg (self.curProg),
			background = "#FF7777",
			activebackground = "#FF7777")
		self.run.grid (row = 3, column = 0, columnspan = 2,
			sticky = N+S+W+E)

		# set some global keys
		self.bind_all ("<Escape>", lambda event: self.quit ())


# ---------------------------------------------------
# ----------------------- data ----------------------
# ---------------------------------------------------

# an array of basic examples (to be changed without notice)
BasicExamples = ["bing", "circle", "dim10", "kleinbot", "maze",
	"probl", "projplan", "qexample", "relative", "repeller", "simklein",
	"simtorus", "torus", "torus2", "wind3", "wrapped"]

# choose the "Label" widget (as opposed to "Text") for descriptions
labelDesc = 1


# ---------------------------------------------------
# ---------------------- main -----------------------
# ---------------------------------------------------

rootWindow = Tk ()
chompChooser = ChompChooser (master = rootWindow)
chompChooser.master.title ("CHomP Examples")
chompChooser.mainloop ()


