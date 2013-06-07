import networkx as nx
try:
    import matplotlib.colors as colors
    import matplotlib.cm as cm
except:
    raise ImportError("Need Matplotlib for colors")
try:
    import OpenGL.GL as GL
    import OpenGL.GLU as GLU
    import OpenGL.GLUT as GLUT
except:
    raise ImportError("Could not load OpenGL, perhaps PyOpenGL and associated libraries are not installed")

import gl_windows as glw

def draw_opengl(G,
                pos=None,
                nodelist=None,
                node_size=20,
                node_color=(1.,0.,0.,1.),
                node_cmap=cm.jet,
                node_border_size=3,
                node_border_color=(0.,0.,0.,1.),
                node_border_cmap=cm.jet,
                edgelist=None,
                edge_color=(0.,0.,0.,1.),
                edge_cmap=cm.jet,
                edge_style='-',
                edge_thickness=1.0,
                with_arrows=True,
                arrow_size=20,
                with_node_labels=True,
                node_labels=None,
                node_label_font='Helvetica',
                node_font_size=10,
                node_label_color=(0.,0.,0.,1.),
                node_label_cmap=cm.jet,
                with_edge_labels=False,
                edge_labels=None,
                edge_label_font='Helvetica',
                edge_font_size=10,
                edge_label_color=(0.,0.,0.,1.),
                edge_label_cmap=cm.jet,
                gl_fancy_nodes=False,
                gl_fancy_arrows=False,
                gl_window_width=800,
                gl_window_height=800,
                gl_window_title="NetworkX Graph",
                gl_refresh=15,
                gl_background_color=(1.,1.,1.,1.),
                save_file='./',
                save_file_type=None,
                rescale_pos=True,
                dim=3):

    """An openGL drawing function for networkx graphs. Works like the
    current matplotlib networkx graph drawing, with one feature missing
    and 1 feature added a couple improved. Descriptions of all the
    paramters are as follows.

    Actions are completed using the mouse. Left click rotates. Right
    click translates and the scroll wheel zooms. The window has
    several commands. Pressing the 'r' key will reset the display to
    the initial configuration. Pressing 'q' exits the window. pressing
    's' will save the current image to the supplied directory with the
    supplied (or infered) extension. Finally, 'l' turns lights on and
    off. This only makes a difference if your drawing function draws
    objects which reflect light. This is included because it is
    slightly faster. I currently also include a lot of antialiasing
    and smoothing to make things look good, if it runs slow (it
    shouldn't) you can turn them off

    Parameters:
    -----------
    G: networkX Graph
     : Graph to draw!
    pos: dict keyed by node of tuples
       : a dictionary of node positions, keyed by node, tuples can
         be either 2 or 3 dimensional, if none is provided this is
         calculated ahead of time
    nodelist: list
            : list of nodes to draw, if None draws all nodes
    node_size: Integer or list
             : If an integer all nodes are drawn the same size,
               If it's a list it is assumed each number corresponds
               to each node in nodelists size. Must be the same lenght
               as node list. The units are in pixels. This is the node
               radius
    node_color: str, tuple,float or list of any.
              : any of the matplotlib color options really, as with node
                size if its a list it must be the same length as nodelist.
                All colors are converted to an RGBA tuple using matplotlib
                color functions. If a float is provided node_cmap is used
                to convert to RGBA. Strings can be HTML colors or HEX Codes
    node_cmap : function: float -> 4 tuple (float)
              : function to map float values onto color for nodes
    node_border_* :
                  : Same as node values, controls how the border looks
    edgelist: list
            : List of edges to draw
    edge_[color,cmap]: Same as node_[color,cmap]
    edge_thickness: numeric or list of numeric
                  : line thickness in points, parsed the same as other
                    node and edge properties
    edge_style: str or list of str
              : can be '-' for solid, '-.' for dash dot,'--' for dashed
                and ':' for dotted, as per matplotlib
    with_arrows: bool
               : Draw arrows, but only if the graph is directed
    arrow_size : numeric, or list of numeric
               : Size to draw arrows, in points.
    with_node_labels: Bool
                    : Whether to draw node labels
    node_labels: list of str
               : A list of node labels, must be the same length as node
               list. If None and with_node_labels is true, uses node keys
               as labels
    node_label_font: 'Helvetica' or 'Times'
                   : Currently openGL only supports 'Helvetica' or 'Times
                   for text.
    node_font_size: 10,12,18,24
                  : Currently opengl only supports 'Helvetica' with size
                    10,12, or 18, and Times with 10 or 24
    node_label_c* : Same as other colors
    edge_labels : Same as node labels
    gl_fancy_nodes: Bool
                  : Draws nodes as spheres if True. Can slow down refresh
                  rendering. borders are ignored, renders as normal points
                  if false
    gl_fancy_arrows: Bool
                   : Draw arrows as cones if True, or pyramids if False
    gl_window_height: int
                    : Initial window height
    gl_window_width: int
                   : Initial window width
    gl_window_title: string
                   : Window Title
    gl_refresh: int
              : Number of milliseconds to wait before redraw
    save_file : str
              : file prefix to save files
    save_file_type: str
                  : save file type, if None, will be inferred from
                  save_file, or default to pdf
    rescale_pos: Bool
               : The current drawing assumes everything is between (-1,1)^d
                the spring layout calculates positions between (0,1)^d,
                the graphviz seems to do something larger. This rescales
                and center everything about 0
    dim: 2 or 3
       : Dimension to draw the graph in.

    """
    
    if pos is None:
        pos=nx.drawing.spring_layout(G,dim=dim)
    if rescale_pos:
        max_pos = {}
        min_pos = {}
        for d in range(dim):
            max_pos[d] = max([pos[n][d] for n in pos])
            min_pos[d] = min([pos[n][d] for n in pos])

        for n in pos:
            pos[n] = tuple([1.9*((pos[n][d]-min_pos[d])/(max_pos[d]-min_pos[d])-.5) for d in range(dim)])
    
    if nodelist is None:
        nodelist = G.nodes()

    node_size = parse_numeric_value(node_size,len(nodelist),"Node Size")
    node_color = parse_color_value(node_color,
                                   len(nodelist),
                                   node_cmap,
                                   "Node Color")
    node_border_size = parse_numeric_value(node_border_size,
                                           len(nodelist),
                                           "Node Border Size")
    node_border_color = parse_color_value(node_border_color,
                                          len(nodelist),
                                          node_border_cmap,
                                          "Node Border Color")
    if edgelist is None:
        edgelist = G.edges()

    edge_color = parse_color_value(edge_color,
                                   len(edgelist),
                                   edge_cmap,
                                   "Edge Color")
    edge_thickness = parse_numeric_value(edge_thickness,
                                         len(edgelist),
                                         "Edge Thickness")
    edge_style = parse_line_stipple(edge_style,
                                      len(edgelist),
                                      "Edge Style")
    if not G.is_directed():
        with_arrows=False
    
    if with_arrows:
        arrow_size = parse_numeric_value(arrow_size,
                                         len(edgelist),
                                         "Arrow Size")
        arrow_points = calc_arrow_points(pos,
                                         edgelist,
                                         dict(zip(nodelist,node_size)),
                                         gl_window_width,
                                         gl_window_height)
    else:
        arrow_size = None
        arrow_points = None
        
    if with_node_labels and node_labels is None:
        node_labels = map(str,nodelist)


        node_label_color = parse_color_value(node_label_color,
                                             len(node_labels),
                                             node_label_cmap,
                                             "Node Label Color")
        node_label_font = parse_font_type_size(node_label_font,
                                               node_font_size,
                                               len(nodelist),
                                               "Node font and Size")
    if with_edge_labels and edge_labels is None:
        edge_labels = map(str,edgelist)
        edge_label_color = parse_color_value(edge_label_color,
                                             len(edge_labels),
                                             edge_label_cmap,
                                             "Edge Label Color")
        edge_label_font = parse_font_type_size(edge_label_font,
                                               edge_font_size,
                                               len(edgelist),
                                               "Edge font and Size")

    glw.gl_window(dim=dim,
                  background_color=gl_background_color,
                  refresh=gl_refresh,
                  height=gl_window_height,
                  width=gl_window_width,
                  title=gl_window_title,
                  save_file=save_file,
                  save_file_type=save_file_type,
                  render_func=graph_rend_func,
                  rf_args=(pos,
                           nodelist,
                           node_size,
                           node_color,
                           node_border_size,
                           node_border_color,
                           gl_fancy_nodes,
                           edgelist,
                           edge_color,
                           edge_style,
                           edge_thickness,
                           with_arrows,
                           arrow_points,
                           arrow_size,
                           gl_fancy_arrows,
                           with_node_labels,
                           node_labels,
                           node_label_font,
                           node_label_color,
                           with_edge_labels,
                           edge_labels,
                           edge_label_font,
                           edge_label_color))


def parse_numeric_value(x,n,description):
    #Takes a numeric value or list, sees makes a list of it repeated or
    # determines if it is long enough
    if type(x) is int or type(x) is float:
        return [x]*n
    else:
        if not len(x)==n:
            raise RuntimeError("Dimensions of " + description + " do not match")
        else:
            return x

def parse_color_value(c,n,cmap,description):
    # Converts any number of color values into a list of RGBA tuples
    tc = type(c)
    if tc is tuple or tc is str:
        return [colors.colorConverter.to_rgba(c)]*n
    elif tc is float:
        return [cmap(c)]*n
    else:
        if not len(c)==n:
            raise RuntimeError("Dimensions of " + description + " do not match")
        else:
            tc = type(c[0])
            if tc is tuple or tc is str:
                return map(colors.colorConverter.to_rgba,c)
            elif tc is float:
                return map(cmap,c)
            else:
                return c

def parse_line_stipple(s,n,description):
    # Stipple parsing for openGL
    def string_match(s):
        if s == '-':
            return (1,0xFFFF)
        elif s == '--':
            return (1,0x000F)
        elif s == ':':
            return (1,0x0101)
        elif s == '-.':
            return (1,0xC204)
    if type(s) is str:
        return [string_match(s)]*n
    else:
        if not len(s) == n:
            raise RuntimeError("Dimensions of " + description + " do not match")
        else:
            if type(s[0]) is str:
                return map(string_match,s)
            else:
                return s

def parse_font_type_size(f,s,n,description):
    # Font parsing for openGL
    def string_match(f,s):
        if f =='Helvetica':
            if s ==10:
                return GLUT.GLUT_BITMAP_HELVETICA_10
            elif s ==12:
                return GLUT.GLUT_BITMAP_HELVETICA_12
            elif s == 18:
                return GLUT.GLUT_BITMAP_HELVETICA_18
            else:
                raise RuntimeError("Cannot parse font " + f + ", size " + str(s))
        elif f =='Times':
            if s ==10:
                return GLUT.GLUT_BITMAP_TIMES_ROMAN_10
            elif s == 24:
                return GLUT.GLUT_BITMAP_TIMES_ROMAN_24
            else:
                raise RuntimeError("Cannot parse font " + f + ", size " + str(s))
    if type(f) is str:
        return [string_match(f,s)]*n
    else:
        if len(f) != len(s) != n:
            raise RuntimeError("Fonts and font size must match " + description + "Dimention")
        else:
            return map(string_match,f,s)

def calc_arrow_points(pos,edgelist,node_size_dict,width,height):
    #Pre calculates the arrow positions. Since we don't want them
    #rendered inside the ndoes, we have to back them off by an amount
    #equal to the node size. OpenGL also by default draws everything
    # at 0,0,0, so we have to find out how much we have to rotate and
    # translate the axis to put them in the correct place. This requires
    # some linear algebra, but happily we only ahve to do it once.
    from math import sqrt, acos, pi
    def window_project(p,width,height):
        pw = []
        pw.append( width * (p[0] + 1.)/2.)
        pw.append( height * (p[1]+ 1.)/2.)
        if len(p)==3:
            pw.append((p[2]+1.)/2)
        return tuple(pw)
    def window_unproject(pw,width,height):
        p = []
        p.append(2.*(pw[0]/width) - 1.)
        p.append(2.*(pw[1]/height) - 1.)
        if len(pw) ==3:
            p.append(2.*pw[2] - 1.)
        return tuple(p)
    def dot(p1,p2):
        s = 0
        for k in range(len(p2)):
            s += p1[k]*p2[k]
        return s 
    def normalize(p):
        v_len = sqrt(dot(p,p))
        return tuple(map(lambda x: x/v_len, p))

    arrow_points = []
    for (i,j) in edgelist:
        d = []
        for r in range(len(pos[i])):
            d.append(pos[i][r]-pos[j][r])
        d = tuple(d + [0])
        c = (0,0,1)
        pwi=window_project(pos[i],width,height)
        pwj=window_project(pos[j],width,height)
        nw = tuple([pwj[z] - pwi[z] for z in range(len(pwj))])
        nw = normalize(nw)
        arrow_stopw = tuple([pwj[z] - ((node_size_dict[j]/2.)*nw[z])
                             for z in range(len(nw))])
        arrow_stop = window_unproject(arrow_stopw,width,height)
        if len(arrow_stop) == 2:
            arrow_stop = tuple(list(arrow_stop) + [0])
        arrow_angle= acos(dot(normalize(d),c))*(180./pi)
        arrow_points.append((arrow_stop,arrow_angle))

    return arrow_points
        

def graph_rend_func(window,
                    pos,
                    nodelist,
                    node_size,
                    node_color,
                    node_border_size,
                    node_border_color,
                    fancy_nodes,
                    edgelist,
                    edge_color,
                    edge_style,
                    edge_thickness,
                    with_arrows,
                    arrow_points,
                    arrow_size,
                    fancy_arrow,
                    with_node_labels,
                    node_labels,
                    node_label_font,
                    node_label_color,
                    with_edge_labels,
                    edge_labels,
                    edge_label_font,
                    edge_label_color):
    # Does teh rendering grunt work, with the actual parsed values from
    # the calling function
    # Make everything look pretty...
    GL.glEnable(GL.GL_POINT_SMOOTH)
    GL.glEnable(GL.GL_LINE_SMOOTH)
    GL.glEnable(GL.GL_DEPTH_TEST)
    # Draw the edges
    draw_edges(pos,edgelist,edge_color,edge_style,edge_thickness)
    # Draw the arrows on top of the edges
    if with_arrows:
        draw_arrows(pos,
                    edgelist,
                    edge_color,
                    arrow_points,
                    arrow_size,
                    1/window.scale,
                    window.lights,
                    fancy_arrow)
    #Draw the nodes on top of those
    draw_nodes(pos,
               nodelist,
               node_color,
               node_size,
               node_border_size,
               node_border_color,
               1/window.scale,
               fancy_nodes,
               window.lights)
    #labels on top of everything
    if with_node_labels:
        draw_node_labels(pos,
                         nodelist,
                         node_labels,
                         node_label_font,
                         node_label_color)
    if with_edge_labels:
        draw_edge_labels(pos,
                         edgelist,
                         edge_labels,
                         edge_label_font,
                         edge_label_color)
    GL.glEnable(GL.GL_DEPTH_TEST)

def draw_edges(pos,edgelist,edge_color,edge_style,edge_thickness):
    GL.glEnable(GL.GL_LINE_STIPPLE)
    GL.glDisable(GL.GL_DEPTH_TEST)
    k = 0
    for (i,j) in edgelist:
        GL.glLineWidth(edge_thickness[k])
        GL.glLineStipple(*edge_style[k])
        GL.glBegin(GL.GL_LINES)
        GL.glColor(*tuple(edge_color[k]))
        GL.glVertex(*tuple(pos[i]))
        GL.glVertex(*tuple(pos[j]))
        GL.glEnd()
        k+=1
    GL.glEnable(GL.GL_DEPTH_TEST)

def draw_arrows(pos,
                edgelist,
                edge_color,
                arrow_points,
                arrow_size,
                scale,
                lights,
                fancy):

    GL.glDisable(GL.GL_DEPTH_TEST)
    if lights:
        GL.glEnable(GL.GL_LIGHTING)
    k = 0
    for (i,j) in edgelist:
        d = []
        for r in range(len(pos[i])):
            d.append(pos[i][r]-pos[j][r])
        d = tuple(d + [0])
        GL.glPushMatrix()

        GL.glTranslate(arrow_points[k][0][0],
                       arrow_points[k][0][1],
                       arrow_points[k][0][2])
        GL.glScale(scale,scale,scale)
        GL.glRotate(arrow_points[k][1],-d[1],d[0],0)

        GL.glColor(*edge_color[k])
        vp = GL.glGetIntegerv(GL.GL_VIEWPORT)
        arrow_size_scale = 2./(vp[2]+vp[3])
        if fancy:
            cone = GLU.gluNewQuadric()
            GLU.gluQuadricNormals(cone,GLU.GLU_SMOOTH)
            GLU.gluQuadricTexture(cone,GLU.GLU_TRUE)
            GLU.gluCylinder(cone,
                            0,
                            arrow_size_scale*arrow_size[k]/3.,
                            arrow_size_scale*arrow_size[k],
                            32,
                            32)
        else:
            s1 = arrow_size[k]*arrow_size_scale
            s2 = arrow_size_scale*arrow_size[k]/3.
            GL.glBegin(GL.GL_POLYGON);
            GL.glVertex3d(0, 0, 0);
            GL.glVertex3d(-s2, s2, s1);
            GL.glVertex3d(-s2, -s2, s1);

            GL.glVertex3d(0, 0, 0);
            GL.glVertex3d(-s2, s2, s1);
            GL.glVertex3d(s2, s2, s1);

            GL.glVertex3d(0, 0, 0);
            GL.glVertex3d(s2, s2, s1);
            GL.glVertex3d(s2, -s2, s1);

            GL.glVertex3d(0, 0, 0);
            GL.glVertex3d(s2, -s2, s1);
            GL.glVertex3d(-s2, -s2, s1);

            GL.glVertex3d(-s2, -s2, s1);
            GL.glVertex3d(s2, -s2, s1);
            GL.glVertex3d(s2, s2, s1);
            GL.glVertex3d(-s2, s2, s1);
            GL.glEnd();
        GL.glPopMatrix()
        k+=1
    if lights:
        GL.glDisable(GL.GL_LIGHTING)
    GL.glEnable(GL.GL_DEPTH_TEST)

def draw_node_labels(pos,nodelist,node_labels,node_label_font,node_label_colors):
    GL.glDisable(GL.GL_DEPTH_TEST)
    GL.glColor(*node_label_colors[0]) #Seems to be a bug
    i = 0
    for n in nodelist:
        GL.glRasterPos(*pos[n])
        GL.glColor(*node_label_colors[i])
        # JJB - hack workaround
        try:
            GLUT.glutBitmapString(node_label_font[i],node_labels[i])
        except:
            pass
        i+=1
    GL.glEnable(GL.GL_DEPTH_TEST)

def draw_edge_labels(pos,edgelist,edge_labels,edge_label_font,edge_label_colors):
    GL.glDisable(GL.GL_DEPTH_TEST)
    GL.glColor(*edge_label_colors[0]) #Seems to be a bug
    k = 0
    for (i,j) in edgelist:
        position = []
        for d in range(len(pos[i])):
            position.append((pos[i][d]+pos[j][d])/2.)
        position = tuple(position)
        GL.glRasterPos(*position)
        GL.glColor(*edge_label_colors[k])
        try:
            GLUT.glutBitmapString(edge_label_font[k],edge_labels[k])
        except:
            pass
        k+=1
    GL.glEnable(GL.GL_DEPTH_TEST)
        
def draw_nodes(pos,
               nodelist,
               node_color,
               node_size,
               node_border_size,
               node_border_color,
               scale,
               fancy,
               lights):
    GL.glDisable(GL.GL_DEPTH_TEST)
    i = 0
    if fancy:
        vp = GL.glGetIntegerv(GL.GL_VIEWPORT)
        sphere_size_scale = 2./(vp[2]+vp[3])
    for n in nodelist:
        if fancy:
            if lights:
                GL.glEnable(GL.GL_LIGHTING)
            GL.glPushMatrix()
            if len(pos[n]) ==2:
                position = tuple(list(pos[n]) + [0.])
            else:
                position = pos[n]
            GL.glTranslate(*position)
            GL.glScale(scale,scale,scale)

            GL.glColor(*node_color[i])
            sphere = GLU.gluNewQuadric()
            GLU.gluQuadricNormals(sphere,GLU.GLU_SMOOTH)
            GLU.gluQuadricTexture(sphere,GLU.GLU_TRUE)
            GLU.gluSphere(sphere,
                          node_size[i]*sphere_size_scale,
                          32,
                          32)
            GL.glPopMatrix()
            if lights:
                GL.glDisable(GL.GL_LIGHTING)
        else:                   
            GL.glPointSize(node_size[i]+node_border_size[i])
            GL.glBegin(GL.GL_POINTS)
            GL.glColor(*node_border_color[i])
            GL.glVertex(*tuple(pos[n]))
            GL.glEnd()

            GL.glPointSize(node_size[i])
            GL.glBegin(GL.GL_POINTS)
            GL.glColor(*node_color[i])
            GL.glVertex(*tuple(pos[n]))
            GL.glEnd()
        i+=1
    GL.glEnable(GL.GL_DEPTH_TEST)


if __name__ == "__main__":
    # A few examples
    G = nx.barabasi_albert_graph(20,2)
    draw_opengl(G)
    G = nx.DiGraph(G)
    draw_opengl(G)
    G = nx.fast_gnp_random_graph(100,1/100.)
    ns = [(G.degree(n)+1)**2 for n in G.nodes()]
    draw_opengl(G,with_node_labels=False,node_size=ns)
    
