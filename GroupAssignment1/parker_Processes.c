#include "header.h"
void forwardQueryToServer(char *employeeName, char *jobTitle, char *status);
int searchFile(char *fname, char *employeeName, char *jobTitle, char *status);

int main()
{
    // this code will eventually get moved into an overall client function

    pid_t pid;
    pid = fork(); // creates a child process

    if (pid == 0) // child process
    {
        manager(); // asking user for input query

        exit(0);
    }
    else if (pid > 0) // parent process
    {
        // wait(NULL);
        assistant(); // fetching query results for the Manager process
    }
    else
    {
        printf("fork error");
        exit(1);
    }
    return 0;
}


/*
---------------------------------------------------------
The main Manager function is in charge of getting user input 
queries.

Params: none
Return: void
*/
void manager()
{
    struct Query query;              // stores query information
    struct Query *queryPtr = &query; // pointer to query information

    printf("Enter an employee name.\n"); // gets and stores employee name into Query struct
    fgets(queryPtr->employeeName, EMPLOYEENAME_LEN, stdin);
    strtok(queryPtr->employeeName, "\n");

    printf("Enter a job title.\n"); // gets and stores job title into Query struct
    fgets(queryPtr->jobTitle, JOBTITLE_LEN, stdin);
    strtok(queryPtr->jobTitle, "\n");

    printf("Enter a status.\n"); // gets and stores status into Query struct
    fgets(queryPtr->status, STATUS_LEN, stdin);
    strtok(queryPtr->status, "\n");

    pipeSend(queryPtr->employeeName, queryPtr->jobTitle, queryPtr->status); // sends the query to the assistant
}

/*
---------------------------------------------------------
The main Assistant function is in charge of getting user input 
queries from the manager. The Assistant then searches a local
cache file and prints the record. If the record isn't found,
then the Assistant then forwards the request to the Server and waits
for the response back from the Server with the retrieved query
information.

Params: none
Return: void
*/
void assistant()
{
    struct Query query;                                                                                                      // holds user query
    query = pipeReceive();                                                                                                   // assistant receives query from Manager
    printf("\nRECEIVED:\nEmployee Name: %s\nJob Title: %s\nStatus: %s\n", query.employeeName, query.jobTitle, query.status); // Print the read message

    FILE *f;                        // file pointer
    f = fopen("History.txt", "a+"); // opens file for appending

    char fname[] = "History.txt"; // name of file to search

    if (searchFile(fname, query.employeeName, query.jobTitle, query.status) != 0) // a match was found
    {
        // PRINT EMPLOYEE INFORMATION FROM THE HISTORY FILE TO THE TERMINAL
    }
    else // a match wasn't found
    {
        // NEED TO MAKE SOCKET CONNECTION TO SERVER TO SEND QUERY

        // THEN RECEIVE DATA FROM SERVER AND PRINT TO NEW TERMINAL

        // THEN WRITE EMPLOYEE TO NEW EMPLOYEE TO HISTORY FILE

        forwardQueryToServer(query.employeeName, query.jobTitle, query.status);
    }
}

/*
---------------------------------------------------------
Searches an input file for a specific employee's name, 
job title, and status.

Params: pointers to the file name, and pointers to the employee name, job title, and status of the query in question
Return: an int representing the number of matches found in the file
*/
int searchFile(char *fname, char *employeeName, char *jobTitle, char *status)
{
    FILE *f;             // file pointer
    int line_num = 1;    // keeps track of the line number
    int find_result = 0; // number of matches found
    char temp[512];      // stores the current read line

    if ((f = fopen(fname, "r")) == NULL) // opens file for reading
    {
        return (-1);
    }

    while (fgets(temp, 512, f) != NULL) // goes through file line by line
    {
        if ((strstr(temp, employeeName)) != NULL && (strstr(temp, jobTitle)) != NULL && (strstr(temp, status)) != NULL) // searches for the specific employee attributes
        {
            printf("A match found on line: %d\n", line_num);
            printf("\n%s\n", temp);
            find_result++;
        }
        line_num++;
    }

    if (find_result == 0) // no results found
    {
        printf("\nSorry, couldn't find a match.\n");
    }

    if (f)
    {
        fclose(f); //Close the file if still open.
    }
    return find_result;
}