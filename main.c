#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFAULT_MAX_ITER 100
#define EPSILON 0.001
#define True 1
#define False 0
#define DEBUG 1

void print_error(){
    printf("An Error Has Occurred");
}

void print_invalid_input(){
    printf("Invalid Input!");
}

int *get_numerical_args(){

}
//returns **squared** distance between two vectors
double distance_sq(double *vector, double *other_vector, int d){
    double res = 0;
    for (int i = 0; i < d; i++){
        res+= (vector[i] - other_vector[i]) * (vector[i] - other_vector[i]);
    }
    return res;
}


//returns the index of closest centroid in centroids to vector
int  closest_clust(double *vector, double **centroids, int k, int d){
    double curr_min = distance_sq(vector, centroids[0], d);
    int curr_res = 0;
    for (int i = 1; i < k; i++){
        double dist = distance_sq(vector, centroids[i], d);
        if (dist < curr_min){
            curr_min = dist;
            curr_res = i;
        }
    }
    return curr_res;
}

// change to_vector into sum of vector and to_vector
void add_vec(double *vector, double *to_vector, int d){
    for (int i = 0; i < d; i++){
        to_vector[i] += vector[i];
    }
    return;
}

// adds vector to cumulative sum in array sums and increases num of vectors by 1 in clust_sizes array
void assign_vec(double *vector, double **clust_sums,int *sizes, int index, int d){
    sizes[index]= sizes[index]+ 1;
    add_vec(vector, clust_sums[index], d);
    return;
}

//calculate the mean vector for all vectors currently in the cluster
double *calc_mean(double *sum_vector, int num_vectors, int d){
    double *result = calloc(d, sizeof (double ));
    for(int i = 0; i < d; i++){
        result[i] = sum_vector[i]/ num_vectors;
    }
    return result;
}

// checks whether the distance between to vectors is less than constant epsilon
int is_converged(double *new_centroid, double *old_centroid, int d){
    double dist = distance_sq(new_centroid, old_centroid, d);
    return (dist < EPSILON*EPSILON);
}

// resets the table of cumulative clusters sums and size to zero
void reset(double **sums, int *sizes, int k, int d){
    for (int i = 0; i < k; i++){
        sizes[i] = 0;
        for (int j = 0; j < d; j++){
            sums[i][j] = 0;
        }
    }
    return;
}

int isStrNumeric(char *str){
    if (str== NULL || str[0] == 0){
        return False;
    }
    if (str[0] == '0' && (str[1] != 0)){
        return False;
    }
    for (int i = 0; str[i] != 0; i++){
        if (str[i] < '0' || str[i]> '9'){
            return False;
        }
    }
    return True;
}

int getProgramArgs(int argc, char *argv[], int *k, int *max_iter, char **in_file_path, char **out_file_path){
    if (argc < 4 || argc > 5){
        print_invalid_input();
        return False;
    }
    if (!isStrNumeric(argv[1])){
        print_invalid_input();
        return False;
    }
    *k = atoi(argv[1]);
    int max_iter_provided = (argc== 5);
    if (max_iter_provided && !isStrNumeric(argv[2])){
        print_invalid_input();
        return False;
    }
    *max_iter = max_iter_provided ? atoi(argv[2]) : DEFAULT_MAX_ITER;
    *in_file_path = argv[2 + max_iter_provided];
    *out_file_path = argv[3 + max_iter_provided];
    return True;
}
/*
 * This function gets a string of filename and returns the number of chars in the first line
 */
int getLineSize(char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        if (DEBUG == 1){
            printf("in 'getLineSize', fp is null");
        }
        print_error();
        return False;
    }
    int count = 0;
    char c = fgetc(fp);
    while (c != EOF && c!= '\n'){
        c = fgetc(fp);
        count++;
    }
    fclose(fp);
    return count;
}

int getNumOfLines(char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        if (DEBUG == 1){
            printf("in 'getNumOfLines', fp is null\n");
        }
        print_error();
        return False;
    }
    int count = 0;
    int c = 0;
    while ( ( c = fgetc( fp ) ) != EOF )
    {
        count+= (c == '\n'); /* check if c equals '\n' */
    }
    fclose(fp);
    return count;
}

int *getLinesLengths(int num_lines, char *filename){
    int *linesLengths = (int *)calloc(num_lines,sizeof(int));
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        if (DEBUG == 1){
            printf("in 'getLinesLengths', fp is null\n");
        }
        print_error();
        return NULL;
    }
    if (linesLengths == NULL){
        if (DEBUG == 1){
            printf("in 'getLinesLengths', linesLengths is null\n");
        }
        fclose(fp);
        print_error();
        return NULL;
    }
    for(int i = 0; i < num_lines; i++){
        char c = 0;
        int line_len = 0;
        while((c = fgetc(fp)) != EOF){
            if (c == '\n'){
                linesLengths[i] = line_len;
               // printf("line length = %d \n", line_len);
                line_len = 0;
            }
            else{ line_len++;}
        }
    }
    return linesLengths;
}

/* returns number of numbers in first line of file */
int get_dimension(char *in_file_path, int line_size){
    FILE *fp = fopen(in_file_path, "r");
    if (fp == NULL){
        if (DEBUG == 1){
            printf("in 'get_dimension', fp is null\n");
        }
        print_error();
        return False;
    }
    char *line = (char  *) calloc(line_size, sizeof (char));
    if (line == NULL){
        if (DEBUG){
            printf("in 'get_dimension', line is null\n");
        }
        print_error();
        return False;
    }
    if(fgets(line, line_size, fp) == NULL){
        fclose(fp);
        if (DEBUG == 1){
            printf("in 'get_dimension', fgets == null\n");
        }
        print_invalid_input();
        free(line);
        return False;
    }
    int counter = 1;
    for(int i = 0; line[i] != '\0'; i++ ){
        if(line[i] == ','){
            counter++;
        }
    }
    fclose(fp);
    free(line);
    return counter;
}

/* turns string of comma seperated numbers to array of doubles */
double *str_to_vec(char *line,int line_len, int d){
    double *res = (double *) calloc(d, sizeof(double));
    char *line_copy = (char *) calloc(line_len, sizeof(char ));
    strcpy(line_copy, line);
    if(res == NULL){
        if (DEBUG == 1){
            printf("in 'str_to_vec', res is null\n");
        }
        print_error();
        return NULL;
    }
    const char s[2] = ",";
    char *token = NULL;

    /* get the first token */
    token = strtok(line_copy, s);
    int i = 0;
    /* walk through other tokens */
    while( token != NULL ) {
        res[i] = strtod(token, NULL);
        i++;
        token = strtok(NULL, s);
    }
    free(line_copy);
    return res;
}

int readData(char *in_file_path, double ***vectors, int *N, int *d){
    FILE *fp = fopen(in_file_path, "r");
    if (fp == NULL){
        if (DEBUG == 1){
            printf("in 'readData', fp is null");
        }
        print_error();
        return False;
    }
    int line_size = getLineSize(in_file_path);
    *N = getNumOfLines(in_file_path);
    *vectors = (double **)calloc(*N, sizeof(double  *));
    *d = get_dimension(in_file_path, line_size);
    if( *N == 0 || line_size == 0 || *d == 0){
        if (DEBUG == 1){
            printf("in 'readData', invalid input\n");
        }
        print_invalid_input();
        fclose(fp);
        return False;
    }
    if (*vectors == NULL){
        if (DEBUG == 1){
            printf("in 'readData', *vectors == NULL");
        }
        print_error();
        fclose(fp);
        return False;
    }
    char *line = (char  *) calloc(line_size, sizeof (char));
    if (line == NULL){
        if (DEBUG){
            printf("in 'get_dimension', line is null\n");
        }
        print_error();
        return False;
    }
    int *lines_length = getLinesLengths(*N, in_file_path);
    for(int i=0; (fgets(line, lines_length[i], fp)) != NULL; i++){
        double *vec = str_to_vec(line, lines_length[i], *d);
        printf("%s\n", line);
        free(line);
        printf("free successedded");
        line =  i < ((*N) - 1) ? ((char *) calloc(line, lines_length[i+1] )) : NULL;
       // printf("%c", line[0]);
        //free(line);
        //line = (char  *) calloc(lines_length[i+1], sizeof (char));
       // line =   ((char *) realloc(line,  * sizeof (char))) ;
        printf("dhhdh");
        if(vec == NULL){
            if (DEBUG == 1){
                printf("in 'readData', vec==NULL");
            }
            print_error();
            fclose(fp);
            free(lines_length);
            free(line);
            return False;
        }
        *vectors[i] = vec;
    }
    fclose(fp);
    free(lines_length);
    free(line);
    return True;
}
//
//char str[80] = "This is - www.tutorialspoint.com - website";
//const char s[2] = "-";
//char *token;
//
///* get the first token */
//token = strtok(str, s);
//
///* walk through other tokens */
//while( token != NULL ) {
//printf( " %s\n", token );
//
//token = strtok(NULL, s);
//}


int main(int argc, char *argv[]) {
    // to be replaced
//    int max_iter = 0;
//    int k = 0;
//    int N = 0;
//    int d = 0;
//    double **vectors= (double**)malloc(N * (sizeof (double*)));
//    double **centroids= (double**)malloc(k * (sizeof (double*)));
//    double **clust_sums= (double**)malloc(k * (sizeof (double*)));
//    int *clust_sizes= (int*)malloc(k * (sizeof (int)));
//# # get list of vectors:
//# ###vectors = read_data()
//# points = [(2, 10), (8,2), (6,1), (11,8), (15,7), (1,8), (12,6)]
//# #points = [[0,0], [1,0], [2,0]]
//# vectors = [Vector(list(i)) for i in points]
    int k = 0;
    int max_iter = 0;
    char *in_file = NULL;
    char *out_file = NULL;
    int res = getProgramArgs(argc, argv, &k, &max_iter, &in_file, &out_file);

    if (!res){
        return 1;
    }


//    printf("%d ", k);
//    printf("%d ", max_iter);
//    printf("%s ", in_file);
//    printf("%s ", out_file);
    double **vectors = NULL;
    int N = 0; //n = 7, d = 2 for example input
    int d = 0;
    printf("entering readData\n");
    if(readData(in_file, &vectors, &N, &d) == 0){
        if (DEBUG == 1){
            printf("readData ok!\n");
        }
        return 1;
    }
//    double vectors[][2] = {{2,10}, {8,2}, {6,1}, {11,8}, {15,7},
//                           {1,8}, {12,6}};
    double **centroids= (double**)calloc(k, (sizeof (double*)));
    for (int i = 0; i < k; i++){
        centroids[i] = (double*)calloc(k, (sizeof (double )));
    }
    for (int i = 0; i < k; i++){
        for (int j = 0; j< d; j++){
            centroids[i][j] = vectors[i][j];
        }
    }
    double **clust_sums= (double**)calloc(k,(sizeof (double*)));
    for (int i = 0; i < k; i++){
        clust_sums[i] = (double*)calloc(d, (sizeof (double )));
    }
    int *clust_sizes= (int*)calloc(k, (sizeof (int)));
    if (vectors == NULL || centroids == NULL || clust_sums == NULL || clust_sizes == NULL){
        print_error();
        return 1;
    }
    int converged = False;
    // main loop:
    for(int i = 0; (i < max_iter) && !converged; i++){
        converged = True;
        for (int j = 0; j < N; j++){
            int closest_clust_index = closest_clust(vectors[j], centroids, k, d);
            assign_vec(vectors[j], clust_sums, clust_sizes, closest_clust_index, d);
        }

        for (int j = 0; j < k; j++){
            double *new_centroid = calc_mean(clust_sums[j], clust_sizes[j], d);
            if(!is_converged(new_centroid, centroids[j], d)){
                converged = False;
            }
            centroids[j] = new_centroid;
        }
        printf("\n\nprinting results in iteration %d\n", i);
        for (int l=0; l<k; l++){
            for (int j = 0; j < d; j++){
                printf("%f,", centroids[l][j]);
            }
            printf("\n");
        }
        reset(clust_sums, clust_sizes, k, d);
    }

    printf("\n\nprinting results\n");
    for (int i=0; i<k; i++){
        for (int j = 0; j < d; j++){
            printf("%f,", centroids[i][j]);
        }
        printf("\n");
    }
    int n;
    // func that writes to files
    for (int i = 0; i < k; i++){
        free(centroids[i]);
        free(clust_sums[i]);
    }
    free(centroids);
    free(clust_sums);
    free(clust_sizes);
    for (int i = 0; i < N; i++){
        free(vectors[i]);
    }
    free(vectors);
    return 0;
}

// tasks:
// 1) check division by zero at first main loop iteration V
// 2) ensure valid output V
// 3) file-reading function
// 4) input validating
// 5) file-writing output