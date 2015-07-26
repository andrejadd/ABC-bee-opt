import pso

from Tkinter import *
import tkSimpleDialog
import Pmw

OPTMOVESTYLE_SELECTION = ["randomOptMove", "linearOptMove", "randomLinearOptMove", "randomDistanceOptMove"]
UPDATESTYLE_SELECTION = ["updatePeriodic", "updateOnGoal"]
DETECTIONMETHOD_SELECTION = ["upToDate", "gbestDetectChange", "scoutDetectChange"]
RESPONSEMETHOD_SELECTION = ["noResponse", "applyScout", "randomize10", "randomize10reset", \
	"randomize16reset", "randomize22reset", "subSwarmsTemporalMerge", "subSwarmsAdaptiveMerge"]



class ParameterWindow:
    win = None    
    pso = None
    e1 = None
    e2 = None
    menu1 = None
    menu2 = None
    menu3 = None
    menu4 = None
    moveFrequency = 0
    moveDistance = 0.0

    def show(self):
        self.win.lift()

    def apply(self):
    	#self.pso.optFunction.set_moveFrequency(int(self.e1.get()))
    	#self.pso.optFunction.set_moveDistance(float(self.e2.get()))
        #self.pso.optFunction.set_optMoveStyle(self.menu1.index(self.menu1.getvalue()))
        #self.pso.optFunction.set_updateStyle(self.menu2.index(self.menu2.getvalue()))
        #self.pso.set_detectionMethod(self.menu3.index(self.menu3.getvalue()))
        #self.pso.set_responseMethod(self.menu4.index(self.menu4.getvalue()))
		self.pso.setDynamicParameters(int(self.e1.get()), float(self.e2.get()), self.menu1.index(self.menu1.getvalue()),\
			self.menu2.index(self.menu2.getvalue()), self.menu3.index(self.menu3.getvalue()), \
			self.menu4.index(self.menu4.getvalue()))
		self.win.destroy()
    	

    def __init__(self, master, pso_in):
        self.win = Toplevel()
        self.pso = pso_in
        
        label = Label(self.win, text="MoveFrequency")
        label.grid(row=0)
        self.moveFrequency = self.pso.optFunction.get_moveFrequency()
        self.e1 = Entry(self.win)
        self.e1.insert(END,str(self.moveFrequency))
        self.e1.grid(row=0, column=1)

        label = Label(self.win, text="MoveDistance")
        label.grid(row=1)
        self.moveDistance = self.pso.optFunction.get_moveDistance()
        self.e2 = Entry(self.win)
        self.e2.insert(END,str(self.moveDistance))
        self.e2.grid(row=1, column=1)

#        self.l1 = Listbox(self.win, selectmode=SINGLE, height=2 )
#        for item in OPTMOVESTYLE_SELECTION:
#            self.l1.insert(END, item)
#        self.l1.select_set(self.optFunction.get_optMoveStyle())
#        self.l1.activate(self.optFunction.get_optMoveStyle())
#        self.l1.grid(row=2, column=1)

        self.menu1 = Pmw.OptionMenu (self.win,
                                     labelpos = 'w',
                                     label_text = 'OptimumMoveStyle:',
                                     items = OPTMOVESTYLE_SELECTION,
                                     menubutton_width = 15
                                     )
        self.menu1.setvalue(OPTMOVESTYLE_SELECTION[self.pso.optFunction.get_optMoveStyle()])
        self.menu1.grid(row=2, columnspan=2)

        self.menu2 = Pmw.OptionMenu (self.win,
                                     labelpos = 'w',
                                     label_text = 'UpdateStyle:',
                                     items = UPDATESTYLE_SELECTION,
                                     menubutton_width = 15
                                     )
        self.menu2.setvalue(UPDATESTYLE_SELECTION[self.pso.optFunction.get_updateStyle()])
        self.menu2.grid(row=3, columnspan=2)

        self.menu3 = Pmw.OptionMenu (self.win,
                                     labelpos = 'w',
                                     label_text = 'DetectionMethod:',
                                     items = DETECTIONMETHOD_SELECTION,
                                     menubutton_width = 15
                                     )
        self.menu3.setvalue(DETECTIONMETHOD_SELECTION[self.pso.get_detectionMethod()])
        self.menu3.grid(row=4, columnspan=2)

        self.menu4 = Pmw.OptionMenu (self.win,
                                     labelpos = 'w',
                                     label_text = 'ResponseMethod:',
                                     items = RESPONSEMETHOD_SELECTION,
                                     menubutton_width = 15
                                     )
        self.menu4.setvalue(RESPONSEMETHOD_SELECTION[self.pso.get_responseMethod()])
        self.menu4.grid(row=5, columnspan=2)


        # make s row of buttons
        buttons = Pmw.ButtonBox(self.win)
        buttons.grid(row= 6, columnspan=2, padx=10, pady=10)
        buttons.add('Apply',       command=self.apply)
        buttons.add('Cancel',       command=self.win.destroy)

class ParameterWindow_new:
    win = None    
    param = None
    e1 = None
    e2 = None
    menu1 = None
    menu2 = None
    menu3 = None
    menu4 = None
    moveFrequency = 0
    moveDistance = 0.0

    def show(self):
        self.win.lift()

    def apply(self):
    	self.pso.optFunction.set_moveFrequency(int(self.e1.get()))
    	self.pso.optFunction.set_moveDistance(float(self.e2.get()))
        #self.optFunction.set_optMoveStyle(int(self.l1.curselection()[0]))
        self.pso.optFunction.set_optMoveStyle(self.menu1.index(self.menu1.getvalue()))
        self.pso.optFunction.set_updateStyle(self.menu2.index(self.menu2.getvalue()))
        self.pso.set_detectionMethod(self.menu3.index(self.menu3.getvalue()))
        self.pso.set_responseMethod(self.menu4.index(self.menu4.getvalue()))
        self.win.destroy()
    	

    def __init__(self, master, psoParam):
        self.win = Toplevel()
        self.param = psoParam
        
        label = Label(self.win, text="MoveFrequency")
        label.grid(row=0)
        self.moveFrequency = self.pso.optFunction.get_moveFrequency()
        self.e1 = Entry(self.win)
        self.e1.insert(END,str(self.moveFrequency))
        self.e1.grid(row=0, column=1)

        label = Label(self.win, text="MoveDistance")
        label.grid(row=1)
        self.moveDistance = self.pso.optFunction.get_moveDistance()
        self.e2 = Entry(self.win)
        self.e2.insert(END,str(self.moveDistance))
        self.e2.grid(row=1, column=1)

#        self.l1 = Listbox(self.win, selectmode=SINGLE, height=2 )
#        for item in OPTMOVESTYLE_SELECTION:
#            self.l1.insert(END, item)
#        self.l1.select_set(self.optFunction.get_optMoveStyle())
#        self.l1.activate(self.optFunction.get_optMoveStyle())
#        self.l1.grid(row=2, column=1)

        self.menu1 = Pmw.OptionMenu (self.win,
                                     labelpos = 'w',
                                     label_text = 'OptimumMoveStyle:',
                                     items = OPTMOVESTYLE_SELECTION,
                                     menubutton_width = 15
                                     )
        self.menu1.setvalue(OPTMOVESTYLE_SELECTION[self.pso.optFunction.get_optMoveStyle()])
        self.menu1.grid(row=2, columnspan=2)

        self.menu2 = Pmw.OptionMenu (self.win,
                                     labelpos = 'w',
                                     label_text = 'UpdateStyle:',
                                     items = UPDATESTYLE_SELECTION,
                                     menubutton_width = 15
                                     )
        self.menu2.setvalue(UPDATESTYLE_SELECTION[self.pso.optFunction.get_updateStyle()])
        self.menu2.grid(row=3, columnspan=2)

        self.menu3 = Pmw.OptionMenu (self.win,
                                     labelpos = 'w',
                                     label_text = 'DetectionMethod:',
                                     items = DETECTIONMETHOD_SELECTION,
                                     menubutton_width = 15
                                     )
        self.menu3.setvalue(DETECTIONMETHOD_SELECTION[self.pso.get_detectionMethod()])
        self.menu3.grid(row=4, columnspan=2)

        self.menu4 = Pmw.OptionMenu (self.win,
                                     labelpos = 'w',
                                     label_text = 'ResponseMethod:',
                                     items = RESPONSEMETHOD_SELECTION,
                                     menubutton_width = 15
                                     )
        self.menu4.setvalue(RESPONSEMETHOD_SELECTION[self.pso.get_responseMethod()])
        self.menu4.grid(row=5, columnspan=2)


        # make s row of buttons
        buttons = Pmw.ButtonBox(self.win)
        buttons.grid(row= 6, columnspan=2, padx=10, pady=10)
        buttons.add('Apply',       command=self.apply)
        buttons.add('Cancel',       command=self.win.destroy)

