
try:
    import OpenGL.GLUT as GLUT
    import OpenGL.GL as GL
    import OpenGL.GLU as GLU
except ImportError:
    raise ImportError("Need working versions of PyOpenGL with GL,GLU,and GLUT")

import os

class gl_window(object):

    """ General Class object to create an openGL window. Window can be
    either 2 or 3 dimensional. Creates orthographic projection with
    some sensible settings, and the best I could do for lighting. The
    class takes a function render_func which is used to update the
    display (using its appropriate arguments). This function is
    wrapped around a function used to do the translation and rotation
    and zooming appropriately. These actions are completed using the
    mouse. Left click rotates. Right click translates and the scroll
    wheel zooms. The window has several commands. Pressing the 'r' key
    will reset the display to the initial configuration. Pressing 'q'
    exits the window. pressing 's' will save the current image to the
    supplied directory with the supplied (or infered)
    extension. Finally, 'l' turns lights on and off. This only makes a
    difference if your drawing function draws objects which reflect
    light. This is included because it is slightly faster. I currently
    also include a lot of antialiasing and smoothing to make things
    look good, if it runs slow (it shouldn't) you can turn them off
    in render_func.

    Caveat: OpenGL actually blocks until the rendering loop is exited.
    This means that nothing else can run while the window is up, until
    the window is destroyed. Unfortunately if this window is spawned
    by an interpreter, there is no clean way to kill the OpenGL engine
    in such that you can open another openGL window in the same
    interpreter.  I've hacked around this by having the class
    immediately spawn a child process and run the rest of the window
    in that child process. This way you get your interpreter back
    immediately, and if running from a shell your shell back. The only
    disadvantage is that if you fork from an interpreter that is
    already bloated with memory, it could bog your machine down a bit.

    Paramters:
    ----------
    dim: int
       : Either 2 or 3 for the projection
    width: int
         : Initial window width
    height: int
          : Initial window height.
    title : String
          : Window Title
    refresh: Int
           : Number of milliseconds between refreshes
             of the window using render_func
    background_color: tuple of floats
                    : background color in RGBA format
    render_func: function
               : The function to be called at each time the screen is
                 refreshed. This should always be constructed so
                 it's first argument is window, which is a reference
                 to the openGL window, this way your rendering can
                 interact with the current window object.
    rf_args : tuple
            : arguments to be passed to render_func
    rf_kwargs : dict
              : keyword argments to be pass to render func
    save_file : string
              : Location and filename for saving. It will save files as
                savefile[i] where i increments with each save, if an
                extension is included it will attempt infer the filetype
                to save it as, otherwise it will default to 'pdf'
    save_file_type: string
                  : Format to save file in.
    """


    def __init__(self,
                 dim=3,
                 width=800,
                 height=800,
                 title="",
                 refresh=15,
                 background_color = (0.,0.,0.,0.),
                 render_func=lambda window: None,
                 rf_args=(),
                 rf_kwargs = {},
                 save_file='./',
                 save_file_type=None):
        
        pid = os.fork() # Hack to get interpetor back
        if not pid==0:
            return None
        self.mouse_down = False
        self.mouse_old = [0., 0.]
        self.rotate = [0., 0., 0.]
        self.translate = [0., 0., 0.]
        self.scale = 1.0
        self.dim=dim

        # grab the file extension if its there.
        split_path_ext = os.path.splitext(save_file)
        self.save_file = split_path_ext[0]
        if save_file_type is None:
            self.save_file_ext = split_path_ext[1]
            if self.save_file_ext == '':
                self.save_file_ext = '.png'
        else:
            self.save_file_ext = save_file_type
        self.save_count = 0

        self.width = width
        self.height = height
        self.lights = False
        
         

        GLUT.glutInit()

        # Some window attributes
        GLUT.glutInitDisplayMode(GLUT.GLUT_RGBA | GLUT.GLUT_DOUBLE)
        GLUT.glutInitWindowSize(self.width, self.height)
        GLUT.glutInitWindowPosition(0, 0)
        self.win = GLUT.glutCreateWindow(title)

        # OpenGL callbacks
        GLUT.glutDisplayFunc(self.draw(render_func,*rf_args,**rf_kwargs))
        GLUT.glutKeyboardFunc(self.on_key)
        GLUT.glutMouseFunc(self.on_click)
        GLUT.glutMotionFunc(self.on_mouse_motion)
        try:
            GLUT.glutCloseFunc(self.on_exit)
        # JJB - 6/6/12 -- 
        except GLUT.error.NullFunctionError:
            GLUT.glutWMCloseFunc(self.on_exit)
        GLUT.glutTimerFunc(refresh, self.timer, refresh)

        self.glinit()
        # Clean the slate
        GL.glClearColor(*background_color)
        # and start
        GLUT.glutMainLoop()

        

    def glinit(self):
        #set up 2d or 3d viewport in nice orthographic projection
        GL.glViewport(0, 0, self.width, self.height)
        GL.glMatrixMode(GL.GL_PROJECTION)
        GL.glLoadIdentity()
        if self.dim==3:
            GL.glOrtho(-1.,1.,-1,1.,-1000.,1000.)
            GL.glMatrixMode(GL.GL_MODELVIEW)
        else:
            GL.glMatrixMode(GL.GL_PROJECTION)
            GL.glLoadIdentity()
            GLU.gluOrtho2D(-1.,1.,-1.,1.)
        #smooth the crap out of everything
        GL.glEnable(GL.GL_POINT_SMOOTH)
        GL.glEnable(GL.GL_LINE_SMOOTH)
        GL.glEnable(GL.GL_POLYGON_SMOOTH)
        GL.glEnable(GL.GL_BLEND)
        GL.glBlendFunc(GL.GL_SRC_ALPHA,GL.GL_ONE_MINUS_SRC_ALPHA)
        GL.glHint(GL.GL_LINE_SMOOTH_HINT,GL.GL_NICEST)
        GL.glHint(GL.GL_POINT_SMOOTH_HINT,GL.GL_NICEST)
        GL.glHint(GL.GL_POLYGON_SMOOTH_HINT,GL.GL_NICEST)


    # Timer callback
    def timer(self, t):
        GLUT.glutTimerFunc(t, self.timer, t)
        GLUT.glutPostRedisplay()

    # Kill the window and the child process that spawned it.
    def on_exit(x):
        os._exit(0)

    # handles some keys.
    def on_key(self, *args):
        ESCAPE = '\033'
        if args[0] == ESCAPE or args[0] == 'q':
            os._exit(0)
        elif args[0] == 'r':
            self.rotate = [0., 0., 0.]
            self.translate = [0., 0., 0.]
            self.scale = 1.0
        elif args[0] == 'l':
            if self.lights:
                self.lights = False
            else:
                self.lights = True
        elif args[0] == 's':
            try:
                from PIL import Image
            except:
                try:
                    import Image
                except:
                    raise ImportError("Cannot Find appropriate Image Library, for Saving")
            vp = GL.glGetIntegerv(GL.GL_VIEWPORT)
            pixel_array = GL.glReadPixels(0,0,vp[2],vp[3],GL.GL_RGB,GL.GL_UNSIGNED_BYTE)

            pilImage = Image.fromstring(mode="RGB",size=(vp[3],vp[2]),data=pixel_array)
            pilImage = pilImage.transpose(Image.FLIP_TOP_BOTTOM)
            pilImage.save(self.save_file + str(self.save_count) + '.png')
            self.save_count += 1 
            
    # Handle the clicks, and scale or update as necessary
    def on_click(self, button, state, x, y):
        if state == GLUT.GLUT_DOWN:
            self.mouse_down = True
            self.button = button
            if self.button == 3 or self.button == 5:
                self.scale *= 1.1
            if self.button == 4 or self.button == 6:
                self.scale *= 0.9
        else:
            self.mouse_down = False
        self.mouse_old[0] = x
        self.mouse_old[1] = y

    # Figure out rotation and translation amounts
    def on_mouse_motion(self, x, y):
        dx = x - self.mouse_old[0]
        dy = y - self.mouse_old[1]
        if self.mouse_down and self.button == 0: #left button
            if self.dim == 3:
                self.rotate[0] += dy * .2
                self.rotate[1] += dx * .2
            else:
                #This fixes the rotation in 2d so we always rotate
                # the way the user drags
                vp = GL.glGetIntegerv(GL.GL_VIEWPORT)
                if x > vp[2]/2:
                    self.rotate[0] -= dy * .2
                else:
                    self.rotate[1] += dy * .2
                if y > vp[3]/2:
                    self.rotate[1] += dx * .2
                else:
                    self.rotate[1] -= dx * .2
        elif self.mouse_down and self.button == 2: #right button
            self.translate[0] += dx * .001
            self.translate[1] -= dy * .001
        self.mouse_old[0] = x
        self.mouse_old[1] = y

    ###END GL CALLBACKS


    def draw(self,f,*args,**kwargs):
        def draw_func():
            #Clear the current buffer
            GL.glFlush()
            GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT)
            # Render stuff
            f(self,*args,**kwargs)
            # Load the identity matrix
            GL.glMatrixMode(GL.GL_MODELVIEW)
            GL.glLoadIdentity()

            #Transform the view in the appropriate way
            if self.dim == 3:
                GL.glTranslatef(0.,0.,-2)
                GL.glTranslatef(self.translate[0],
                                self.translate[1],
                                self.translate[2])
                GL.glRotatef(self.rotate[0], 1, 0, 0)
                GL.glRotatef(self.rotate[1], 0, 1, 0) 

            else:
                GL.glTranslatef(0.,0.,0.)
                GL.glTranslatef(self.translate[0], self.translate[1],0.0)
                GL.glRotatef(self.rotate[0], 0, 0, 1)
                GL.glRotatef(self.rotate[1], 0, 0, 1) 

            # Scale
            GL.glScale(self.scale,self.scale,self.scale)
            #More niceness
            GL.glShadeModel(GL.GL_SMOOTH)
            # Lights if they are there
            if self.lights:
                GL.glLightfv(GL.GL_LIGHT0,GL.GL_POSITION, (1.,1.,1.))
                GL.glLightfv(GL.GL_LIGHT0, GL.GL_AMBIENT, (0.,0.,0.,.5))
                GL.glLightfv(GL.GL_LIGHT0, GL.GL_DIFFUSE, (.4,.4,.4,.4))
                GL.glLightfv(GL.GL_LIGHT0, GL.GL_SPECULAR,(.4,.4,.4,.4))
                GL.glLightModelfv(GL.GL_LIGHT_MODEL_AMBIENT,(.2,.2,.2,1.0))
                GL.glEnable(GL.GL_LIGHT0)
                GL.glEnable(GL.GL_COLOR_MATERIAL)
                GL.glColorMaterial(GL.GL_FRONT_AND_BACK,GL.GL_AMBIENT_AND_DIFFUSE)
                GL.glMaterialfv(GL.GL_FRONT_AND_BACK,GL.GL_SPECULAR,(1.,1.,1.,1.))
                GL.glMaterialfv(GL.GL_FRONT_AND_BACK,GL.GL_EMISSION,(0.,0.,0.,1.))
            # Swap out the new rendering.
            GLUT.glutSwapBuffers()
        return draw_func

    
def test_2d_render_func(window):
    GL.glEnable(GL.GL_POINT_SMOOTH)
    GL.glEnable(GL.GL_LINE_SMOOTH)
    GL.glEnable(GL.GL_LINE_STIPPLE)
    points = [(0.,0.),(.25,.25),(.25,-.25),(-.25,.25),(-.25,-.25)]
    GL.glColor(0.,1.,0.)
    GL.glPointSize(20)
    GL.glBegin(GL.GL_POINTS)
    for p in points:
        GL.glVertex(*p)
    GL.glEnd()
    GL.glLineStipple(1,0x0101)
    GL.glBegin(GL.GL_LINES)
    for p in points:
        for q in points:
            GL.glVertex(*p)
            GL.glVertex(*q)
    GL.glEnd()
    return

def test_3d_render_func(window):
    GL.glEnable(GL.GL_POINT_SMOOTH)
    GL.glColor(0.,1.,0.)
    GL.glPointSize(20)
    GL.glBegin(GL.GL_POINTS)
    GL.glVertex(0.,0.)
    GL.glVertex(.25,.25,.25)
    GL.glVertex(.25,.25,-.25)
    GL.glVertex(.25,-.25,.25)
    GL.glVertex(.25,-.25,-.25)
    GL.glVertex(-.25,.25,.25)
    GL.glVertex(-.25,.25,-.25)
    GL.glVertex(-.25,-.25,.25)
    GL.glVertex(-.25,-.25,-.25)
    GL.glEnd()
    return

def test_2d():
    gl_window(dim=2,background_color=(0.,0.,0.,0.),render_func=test_2d_render_func)

def test_3d():
    gl_window(dim=3,render_func=test_3d_render_func)

if __name__ == "__main__":
    test_3d()
    test_2d()
