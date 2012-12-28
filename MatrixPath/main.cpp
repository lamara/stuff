/*
 * File:   main.cpp
 * Author: owner
 *
 * Created on December 27, 2012, 1:37 AM
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
using namespace std;

/*
 *
 */

static int numcalls = 0;

void loadMatrix(vector< vector<int> >& vector)
{
    ifstream file("matrix.txt");
    string line;
    string tempVal;
    int num;
    for (int row = 0; row < vector.size(); ++row)
    {
        //first we have to read the line that corresponds
        //to each row
        getline(file, line);
        istringstream stream(line);
        for (int col = 0; col < vector[0].size(); ++col)
        {
            //then we read the line using
            //comma as a delimiter and then
            //convert the gotten value
            //to an int
            getline(stream, tempVal, ',');
            num = atoi(tempVal.c_str());
            vector[row][col] = num;
        }
    }
    file.close();
}

void shortestPath(vector<int> &values, vector< vector<int> > const &matrix,
                  int stepsLeft, int prevSum, int row, int col)
{
    numcalls++;
    if (stepsLeft == 0)
    {
        prevSum += matrix[row][col];
        values.push_back(prevSum);
        return;
    }
    //number of possible moves (stepsLeft)
    //is determined by row size minus
    //one + col size minus one
    stepsLeft--;
    prevSum += matrix[row][col];
    if (row == matrix.size() - 1)
    {
        //have reached the farthest right we can, so can only branch downwards
        shortestPath(values, matrix, stepsLeft, prevSum, row, col + 1);
        return;
    }
    else if (col == matrix[0].size() - 1)
    {
        //have reached the farthest down we can, so can only branch right
        shortestPath(values, matrix, stepsLeft, prevSum, row + 1, col);
        return;
    }
    else
    {
        //are in the middle of the matrix, so can branch both ways
        shortestPath(values, matrix, stepsLeft, prevSum, row, col + 1);

        shortestPath(values, matrix, stepsLeft, prevSum, row + 1, col);
        return;
    }
}

//instead of brute forcing, creates an row/col identical matrix that
//holds the value of the shortest path to each individual point,
//so we can look up the shortest path directly adjacent to the
//location we are looking at currently
int fastShortestPath(vector< vector<int> > &grid, vector< vector<int> > const &matrix,
                    int targetRow, int targetCol)
{
    //following two for loops handle the two edge case paths
    for (int row = 1; row < grid.size(); ++row)
    {
        grid[row][0] = grid[row - 1][0] + matrix[row][0];
    }

    for (int col = 1; col < grid[0].size(); ++col)
    {
        grid[0][col] = grid[0][col - 1] + matrix[0][col];
    }


    //fills out the rest of the grid, giving
    //each location the value of the shortest path to it
    for (int row = 1; row < grid.size(); ++row)
    {
        for (int col = 1; col < grid[0].size(); ++col)
        {
            grid[row][col] = min(grid[row - 1][col], grid[row][col-1]) + matrix[row][col];
        }

    }
    return grid[targetRow - 1][targetCol - 1];
}


int main(int argc, char** argv) {
    vector< vector<int> > matrix;
    matrix.resize(80, vector<int>(80));
    loadMatrix(matrix);

    vector< vector<int> > grid;
    grid.resize(80, vector<int>(80));
    grid[0][0] = matrix[0][0];

    cout << fastShortestPath(grid, matrix, 80, 80);


    return 0;
}

