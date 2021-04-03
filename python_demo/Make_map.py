import cv2
import codecs
import numpy as np

map_pic=cv2.imread("test.png")
pic,g,r=cv2.split(map_pic)


def merge(i,j):
    global pic
    all_pix=0
    for x in range(i,i+2):
        for y in range(j,j+2):
            all_pix+=pic[x,y]
    
    for x in range(i,i+2):
        for y in range(j,j+2):
            pic[x,y]=all_pix/4


for i in range(1000-1):
    for j in range(1000-1):
        if(i%2==0 and j%2==0):
            merge(i,j)
            
cv2.imshow("1",pic)
cv2.waitKey(1)

f=codecs.open("Map.txt","w","utf-8")
for i in range(1000):
    for j in range(1000):
        f.write(str(pic[i,j])+" ")
    f.write("\n")
f.close()
print("done")

f=codecs.open("Map.txt","r","utf-8")

I=np.zeros((1000,1000),dtype=np.uint8)
xx=0
yy=0
for line in f:
    line=line.split(" ")[:-1]
    for item in line:
        #print(xx," ",yy)
        I[xx,yy]=int(item)
        yy+=1
    yy=0
    xx+=1
cv2.imshow("123",I)
cv2.waitKey(1)
        
