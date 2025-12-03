#ifndef BASICS_H
#define BASICS_H

typedef struct {
    char* data;
    int data_size;
} StringData;

typedef struct {
    int size;
    StringData **string;
} StringDataArray;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point* pt;
    int counter;
    int capacity;
} SpacePoints;

char* create_char_array(int basic_size);
void task1_lowdiff(int weight);
void task1_meddiff(int* nums_array, int array_size);
SpacePoints* create_space_points(int basic_size);
void add_point_to_space(SpacePoints* sppt, int x, int y);
float transfer(Point pt1, Point pt2);

#endif