#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

void delete_edge(int *matrix[], int point_1, int point_2){
    if(point_1 > point_2){
        matrix[point_2][point_1] = 0;
    } else if(point_1 < point_2){
        matrix[point_1][point_2] = 0;
    }
}

void restore_edge(int *matrix[], int point_1, int point_2){
    if(point_1 > point_2){
        matrix[point_2][point_1] = 1;
    } else if(point_1 < point_2){
        matrix[point_1][point_2] = 1;
    }
}

void depth_search(int *matrix[], int point, int dot, int color[]){
    int i;
    color[dot] = 1;
    for(i=1;i<=point;i++){
        if((color[i] == 0) && ((matrix[dot][i] == 1) || (matrix[i][dot] == 1))){
            depth_search(matrix, point, i, color);
        }
    }
}

int degrees_counter(int *matrix[], int point, int dot){
    int i, j;
    int degrees[point];
    int degree = 0;
    for(j=1;j<=point;j++){
    for(i=1;i<=point;i++){
        if(matrix[i][j] == 1){
            degree++;
        }
        if(matrix[j][i] == 1){
            degree++;
        }
    }
    degrees[j] = degree;
    degree = 0;
    }
    return degrees[dot];
}

int bridge(int *matrix[], int point, int point_1, int point_2, int color[]){
    int i;
    int flag = 1;

    delete_edge(matrix, point_1, point_2);
    depth_search(matrix, point, point_1, color);
    restore_edge(matrix, point_1, point_2);

    for(i=1;i<=point;i++){
        if(color[i] == 0){
            flag = 0;           // bridge
        }
    }

    if(flag == 0){
        return 1;
    } else if(flag == 1){
        return 0;
    }
}

int adjacent_point(int *matrix[], int point, int dot, int points_2[]){
    int i;
    int point_2;
    int flag = 0;
    for(i=1;i<=point;i++){
        if(points_2[i] > 0){
            flag = 1;
        }
    }
    if(flag == 1){
        for(i=1;i<=point;i++){
            if(points_2[i] != 0){
                point_2 = points_2[i];
                points_2[i] = 0;
                break;
            }
        }
        return point_2;
    } else if(flag == 0){
        printf("\nThere is no adjacent points\n");
        return 0;
    }
}

void Fleury(int *matrix[], int point, int dot, int first){
    int i;
    int point_2 = 0;
    int conn;
    int points_2[point];
    int color[point];
    int flag = 1;

    for(i=1;i<=point;i++){
        if(color[i] != 2){
            color[i] = 0;
        }
    }
    for(i=1;i<=point;i++){
        if(degrees_counter(matrix, point, i) == 0){
            color[i] = 2;
        }
    }

    for(i=1;i<=point;i++){
        if(color[i] != 2){
            flag = 0;
        }
    }

    printf("\nInput point = %d\n", dot);
    if(flag == 1){
        printf("\nEND OF FLEURY's ALGORITHM\n");
        if(first == dot){
            printf("\nEULERIAN GRAPH\n");
        } else if(first != dot){
            printf("\nNOT AN EULERIAN GRAPH\nTHERE IS NO EULERIAN CYCLE\n");
        }
        return;
    }

    for(i=1;i<=point;i++){
        points_2[i] = 0;
    }
    for(i=1;i<=point;i++){
        if((matrix[dot][i] == 1) || (matrix[i][dot] == 1)){
            points_2[i] = i;
        }
    }

    if(degrees_counter(matrix, point, dot) == 0){
        printf("\ncase 4\nTHERE IS NO EULERIAN CYCLE\n");
        return;
    }

    point_2 = adjacent_point(matrix, point, dot, points_2);
    if(point_2 == 0){
        printf("\nNOT AN EULERIAN GRAPH\nTHERE IS NO EULERIAN CYCLE\n");
        return;
    }

    conn = bridge(matrix, point, dot, point_2, color);
    printf("\nconn = %d\n", conn);

    if(conn == 0){
        printf("\ncase 1\n");
        delete_edge(matrix, dot, point_2);
        Fleury(matrix, point, point_2, first);
    } else if((conn == 1) && (degrees_counter(matrix, point, dot) > 1)){
        while(conn == 1){
            printf("\ncase 2\n");
            point_2 = adjacent_point(matrix, point, dot, points_2);
            if(point_2 != 0){
                conn = bridge(matrix, point, dot, point_2, color);
            } else if(point_2 == 0){
                printf("\nNOT AN EULERIAN GRAPH\nTHERE IS NO EULERIAN CYCLE\n");
                return;
            }
        }
        delete_edge(matrix, dot, point_2);
        Fleury(matrix, point, point_2, first);
    } else if((conn == 1) && (degrees_counter(matrix, point, dot) == 1)){
        printf("\ncase 3\n");
        delete_edge(matrix, dot, point_2);
        Fleury(matrix, point, point_2, first);
    }
}

int main(){

    int i, j, point;
    printf("Size of matrix:\n");
    scanf("%d", &point);
    int **matrix;
    matrix = (int**)malloc(point * sizeof(int*));
    for(i=1;i<=point;i++){
        matrix[i] = (int*)malloc(point * sizeof(int));
    }

    printf("Input matrix:\n");
    for(i=1;i<=point;i++){
    for(j=1;j<=point;j++){
        scanf("%d", &matrix[i][j]);
        }
        }
    printf("Matrix: \n");
    for(i=1;i<=point;i++){
    for(j=1;j<=point;j++){
        printf("%d\t", matrix[i][j]);
    }
        printf("\n");
    }
    printf("\n");

    for(i=1;i<=point;i++){
    for(j=1;j<=point;j++){
        if(matrix[i][j] == 1){
            printf("%d -- ", i);
            printf("%d;\n", j);
        }
    }
    }

    int dot = 0;
    srand(time(NULL));
    dot = (rand() % point) + 1;
                                    // Connection check
    int first;
    first = dot;
    int flag = 1;
    int color[point];
    for(i=1;i<=point;i++){
        color[i] = 0;
    }
    depth_search(matrix, point, dot, color);
    for(i=1;i<=point;i++){
        if(color[i] == 0){
            flag = 0;
        }
    }
    if(flag == 0){
        printf("GRAPH DISCONNECTED; THERE IS NO EULERIAN CYCLE");
    } else if(flag == 1){
        Fleury(matrix, point, dot, first);
    }
    free(*matrix);

return 0;
}
