# -*- coding: utf-8 -*-
"""
Created on Thu Nov  6 18:54:15 2014

@author: sebastian
"""

import  pygame, math

class corridor_animation():
    # use draw_robot(x,y,rot) to update the position of the robot
    
    def __init__(self):
        
        pygame.init()

        # position in a x,y grid (zero is upper left, x is horizontal, y is vertical)
        self.nodes = {1:(1,1),2:(3,1),3:(5,1),4:(1,3),5:(3,3),6:(5,3),7:(1,5),8:(3,5),9:(5,5)}
        # edges connecting the nodes
        self.edges = ((1,2),(2,3),(4,5),(5, 6),(7,8),(8,9), (1, 4), (2, 5), (3, 6), (4, 7,), (5, 8), (6, 9))

        self.path = (1,2,3, 6, 5, 8)

        self.size = width, height = 800,800
        self.black = 0, 0, 0
        self.white = 255,255,255
        self.red = 255,0,0
        self.yellow = 255, 255, 0
        self.cw = 60 # corridorwidth
        self.pw = 3 # pathwidth
        self.grid_size = 2*self.cw
        self.myfont = pygame.font.SysFont("monospace", 15)

        self.nodes_pos = {node:(gridpos[0]*self.grid_size,gridpos[1]*self.grid_size) for node,gridpos in self.nodes.iteritems()}

        self.screen = pygame.display.set_mode(self.size)
        
        self.draw_robot(1, 1, 0)

       
    def draw_background(self):
    
        self.screen.fill(self.black)

        for node,pos in self.nodes_pos.iteritems():
          pygame.draw.rect(self.screen,self.white,(pos[0]-self.cw/2+1,pos[1]-self.cw/2+1,self.cw,self.cw))
        for edge in self.edges:
          pygame.draw.line(self.screen,self.white,self.nodes_pos[edge[0]],self.nodes_pos[edge[1]],self.cw)

        for node1,node2 in zip(self.path[:-1],self.path[1:]):
          pygame.draw.line(self.screen,self.red,self.nodes_pos[node1],self.nodes_pos[node2],self.pw)
         
        for node,pos in self.nodes_pos.iteritems(): 
          label = self.myfont.render(str(node), 1, self.black)
          self.screen.blit(label, pos)
          
        pygame.display.flip()
        
        
#    def draw_robot_grid(self, x, y, rot):
#        self.draw_background()
#        x = int(x)
#        y = int(y)
#        pygame.draw.circle(self.screen, self.yellow, [self.grid_size*x, self.grid_size*y], self.cw/3) 
#        pygame.draw.line(self.screen,self.black,[self.grid_size*x, self.grid_size*y],[self.grid_size*x+self.cw/3*math.cos(rot), self.grid_size*y+self.cw/3*math.sin(rot)],2)
#        pygame.display.flip()
#        return
        
    def draw_robot(self, x, y, rot):
        self.draw_background()
        x = int(x)
        y = int(y)
        pygame.draw.circle(self.screen, self.yellow, [x, y], self.cw/3) 
        pygame.draw.line(self.screen,self.black,[x, y],[x+self.cw/3*math.cos(rot), y+self.cw/3*math.sin(rot)],2)
        pygame.display.flip()
        return
    
    def get_node_position(self, node):
        return self.nodes_pos[node]
        
    def get_node_index(self):
        return [node for node in self.nodes]
#while 1:
# for event in pygame.event.get():
#   if event.type == pygame.QUIT: sys.exit()
