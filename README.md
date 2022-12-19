![screenshot](https://github.com/CetinFurkan/MaaS_General_Challenge/blob/main/ss_convex.png)

# MaaS General Challenge
### Eliminate overlapped convex hulls
Created by Furkan Çetin on 18/12/2022
### Algorithm:
- Load all point data from a json file
- Build polygons of convex hull set of points
- Compare all of the polygons with each one only once 
-- Check if bounding box of polygons collides
-- If so, check if lines of the polygons collides
-- Create a new polygon of intersection and calculate area of it
-- Find ratio of this area over all polygon area
-- If a polygon has more than %50 of its own area overlapped, mark it to remove later
- Go over all of the polygons and remove the marked ones
- Export the polygons into a new json file

### Project Building Steps:
(Tested with Ubuntu 19.06, Visual Studio Code 1.74.1) ([Installing Visual Studio Code](https://code.visualstudio.com/docs/setup/linux))
- [Download](https://github.com/CetinFurkan/MaaS_General_Challenge.git) repository folder into local drive and unzip
- Run **Visual Studio Code**
- Select **File > Open Folder** and browse the repository folder
- Select **Run > Run** without Debugging
- Now you will get an output json file and console output on Visual Studio Code Terminal

### BONUS: Processing Visualisation Steps:
In order to see point data and the polygons, I created a Java application with [Processing4](https://processing.org/download)
- Install **Processing** and run it
- Select **File > Open** and browse the repository folder
- Find the **test_convex_hull_polygons.pde** file in **test_convex_hull_polygons** folder
- Click **Sketch > Run** 
