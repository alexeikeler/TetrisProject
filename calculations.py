import math
import matplotlib.pyplot as plt
import matplotlib.ticker as tck
import sys

plt.style.use("seaborn-v0_8-whitegrid")



class Point:
    
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __str__(self):
        return f"({self.x}, {self.y})"

    def __eq__(self, other):
        if isinstance(other, Point):
            return self.x == other.x and self.y == other.y
        return False

    def __hash__(self):
        return hash((self.x, self.y))

def rotate(points, rc, angle):
    
    rotated = []
    for p in points:
        x = rc.x + (p.x - rc.x) * math.cos(math.radians(-angle)) - (p.y - rc.y) * math.sin(math.radians(-angle))
        y = rc.y + (p.x - rc.x) * math.sin(math.radians(-angle)) + (p.y - rc.y) * math.cos(math.radians(-angle))
        rotated.append(Point(x, y))

    return rotated


def draw(points, axis):
    for p in points:
        axis.plot(p.x, p.y, 'o', color="black")


def main():
    #  ###
    #    #
    figure1 = [Point(1, 2), Point(2, 2), Point(3, 2), Point(3, 1)]
    center1 = figure1[1]

    # # #
    #
    figure2 = [Point(1, 1), Point(1, 2), Point(2, 2), Point(3, 2)]
    center2 = figure2[2]

    #  ##
    # ##
    figure3 = [Point(1, 1), Point(2, 1), Point(2, 2), Point(3, 2)]
    center3 = figure3[2]

    # ###
    #  #
    figure4 = [Point(1, 2), Point(2, 2), Point(2, 1), Point(3, 2)]
    center4 = figure4[1]
    
    # ##
    #  ##
    figure5 = [Point(1, 2), Point(2, 2), Point(2, 1), Point(3, 1)]
    center5 = figure5[2]

    if(len(sys.argv) != 2):
        print("Error. Usage: python3 n, n interger from 1 to 4.")
        exit(1)

    index = int(sys.argv[1]) - 1    

    figures_and_centers = [
        (figure1, center1), 
        (figure2, center2),
        (figure3, center3),
        (figure4, center4),
        (figure5, center5)
    ]

    points, center = figures_and_centers[index]

    fig, axs = plt.subplots(2, 2, dpi=100)
    fig.suptitle("Rotated figures")
    
    axs_list = []
    angle = 0

    for i in range(2):
        for j in range(2):
            axs[i, j].xaxis.set_major_locator(tck.MultipleLocator())
            axs[i, j].yaxis.set_major_locator(tck.MultipleLocator())
            axs[i, j].set_title(f'Axis [{i}, {j}], angle {angle}')
            angle += 90
            axs_list.append(axs[i, j])

    #angles = [0, 90, 180, 270]
    length = 4

    for i in range(length):
        if i == 0:
            angle = 0
        else:
            angle = 90
        points = rotate(points, center, angle)
        draw(points, axs_list[i])

    # for i in range(4):
    #     print(f"p = {points[i]} ----> {rotated[i]}")
    #     ax1.plot(points[i].x, points[i].y, 'o', color='black')
    #     ax2.plot(rotated[i].x, rotated[i].y, 'x', color='red')

    plt.show()

if __name__ == "__main__":
    main()