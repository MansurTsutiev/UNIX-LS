/*
 Mansur Tsutiev
 CSCI 49366 - 01
 LS Project (Options: l,a,o,n,r,s,t,x,1,g,m,p,F,R)
 */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/xattr.h>

//FUNCTION PROTOTYPES:
void print_no_option(struct dirent *dirp, DIR *dp);
void print_with_option_l(struct dirent *dirp, DIR *dp);
void print_with_option_a(struct dirent *dirp, DIR *dp);
void print_with_option_o(struct dirent *dirp, DIR *dp);
void print_with_option_n(struct dirent *dirp, DIR *dp);
void print_with_option_r(struct dirent *dirp, DIR *dp);
void print_with_option_s(struct dirent *dirp, DIR *dp);
void print_with_option_t(struct dirent *dirp, DIR *dp);
void print_with_option_x(struct dirent *dirp, DIR *dp);
void print_with_option_1(struct dirent *dirp, DIR *dp);
void print_with_option_g(struct dirent *dirp, DIR *dp);
void print_with_option_m(struct dirent *dirp, DIR *dp);
void print_with_option_p(struct dirent *dirp, DIR *dp);
void print_with_option_F(struct dirent *dirp, DIR *dp);
void print_with_option_R(struct dirent *dirp, DIR *dp);
int compare_function(const void *a,const void *b);
void padding(int size);
void generate_files();


struct Box
{
	char *d_name;
	long int time;
};

//*************************************** MAIN: ****************************************
int main(int argc, char *argv[])
{
	char path_name_buffer[200];
	if (getcwd(path_name_buffer, 200) == NULL)    //get absolute path of pwd
		printf("Error: Could not get absolute path of cwd.\n");

	DIR *dp;                              //ptr to DIR structure
	dp = opendir(path_name_buffer);       //returns ptr to DIR struct

	if (dp == NULL)                       //check for error
		printf("Error, can't open the file: %s", path_name_buffer);
	
	struct dirent *dirp = NULL;                  //ptr to a structure of one directory entry
	
	if (argc == 1)								 //check options:
	{
		print_no_option(dirp, dp);
	}
	else if (strcmp(argv[1], "-l") == 0)
	{
		print_with_option_l(dirp, dp);
	}
	else if (strcmp(argv[1], "-a") == 0)
	{
		print_with_option_a(dirp, dp);
	}
	else if (strcmp(argv[1], "-o") == 0)
	{
		print_with_option_o(dirp, dp);
	}
	else if (strcmp(argv[1], "-n") == 0)
	{
		print_with_option_n(dirp, dp);
	}
	else if (strcmp(argv[1], "-r") == 0)
	{
		print_with_option_r(dirp, dp);
	}
	else if (strcmp(argv[1], "-s") == 0)
	{
		print_with_option_s(dirp, dp);
	}
	else if (strcmp(argv[1], "-t") == 0)
	{
		print_with_option_t(dirp, dp);
	}
	else if (strcmp(argv[1], "-x") == 0)
	{
		print_with_option_x(dirp, dp);
	}
	else if (strcmp(argv[1], "-1") == 0)
	{
		print_with_option_1(dirp, dp);
	}
	else if (strcmp(argv[1], "-g") == 0)
	{
		print_with_option_g(dirp, dp);
	}
	else if (strcmp(argv[1], "-m") == 0)
	{
		print_with_option_m(dirp, dp);
	}
	else if (strcmp(argv[1], "-p") == 0)
	{
		print_with_option_p(dirp, dp);
	}
	else if (strcmp(argv[1], "-F") == 0)
	{
		print_with_option_F(dirp, dp);
	}
	else if (strcmp(argv[1], "-R") == 0)
	{
		print_with_option_R(dirp, dp);
	}
	else if (strcmp(argv[1], "generate") == 0)
	{
		generate_files();
	}
	
	return 0;
}
//********************************* PRINT OPTION 1: 'no option' ********************************************
void print_no_option(struct dirent *dirp, DIR *dp)
{
	char array[100][100];
	int i = 0;
	
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')				//filter out hidden files
		{
			strcpy(array[i], dirp->d_name);		//store each name in array
			i++;//grabs one too many
		}
	}
	
	int actual_elements = i;					//save i before modifing it
	
	if (i % 3 == 1)								//even out the array into sections of 3
	{
		strcpy(array[i], " ");
		strcpy(array[i+1], " ");
		i = i + 2;
	}
	else if (i % 3 == 2)
	{
		strcpy(array[i], " ");
		i = i + 1;
	}

	if (actual_elements >= 9)			//FILES: 9-up
	{
		for (int a = 0; a < i/3; a++)											//loop the array of names
		{
			printf("%s", array[a]);												//1st column
			padding(40 - strlen(array[a]));										//padding
			
			printf("%s", array[a+(i/3)]);										//2nd column
			padding(40 - strlen(array[a+(i/3)]));	//padding
			
			printf("%s", array[a+((i/3) * 2)]);									//3rd column
			printf("\n");
		}
	}
	else								//FILES: 1-8
	{
		for (int a = 0; a < i-1; a++)
		{
			printf("%s\n", array[a]);											//just 1 column
		}
	}
}
//********************************** PRINT OPTION 2: '-l' ******************************************

//Displays the long format listing
void print_with_option_l(struct dirent *dirp, DIR *dp)
{
	struct stat stat_buf;
	char list[20];
	
	while ((dirp = readdir(dp)) != NULL)						//read each directory
	{
		
		if (stat(dirp->d_name, &stat_buf) < 0)					//get stat on the file
		{
			printf("lstat() error\n");
			exit(0);
		}
		
		if (dirp->d_name[0] != '.')								//filter out hidden files
		{
			printf( (S_ISDIR(stat_buf.st_mode)) ? "d" : "-");	//print permissions in rwx format
			printf( (stat_buf.st_mode & S_IRUSR) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWUSR) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXUSR) ? "x" : "-");
			printf( (stat_buf.st_mode & S_IRGRP) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWGRP) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXGRP) ? "x" : "-");
			printf( (stat_buf.st_mode & S_IROTH) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWOTH) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXOTH) ? "x" : "-");
			
			if (listxattr(dirp->d_name, list, 20, XATTR_NOFOLLOW) != 0)	//print extended attribute flag
				printf("@\t");
			else
				printf("\t");
			
			printf("%d\t", stat_buf.st_nlink);					//number of hard links
			
			
			struct passwd *pw = getpwuid(stat_buf.st_uid);		//username
			printf("%s\t", pw->pw_name);
			
			struct group *grp = getgrgid(pw->pw_gid);			//group name
			printf("%s\t", grp->gr_name);
			
			printf("%lld\t", stat_buf.st_size);					//print size of file
			
			//TIME:
			char time[300];
			struct tm *time_struct;
			time_struct = localtime(&stat_buf.st_mtime);		//breaks down raw time value into time_struct
			strftime(time, 200, "%b %d %H:%M", time_struct);	//formats time_struct, stores result in time
			printf("%s\t", time);
			
			printf("%s\n",dirp->d_name);						//print directory name

		}
	}
}
//********************************** PRINT OPTION 3: '-a' *******************************************

//Include directory entries whose names begin with a dot
void print_with_option_a(struct dirent *dirp, DIR *dp)
{
	char array[100][100];
	int i = 0;
	
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{

		strcpy(array[i], dirp->d_name);		//store each name in array
		i++;//grabs one too many

	}
	
	int actual_elements = i;					//save i before modifing it
	if (i % 3 == 1)
	{
		strcpy(array[i], " ");
		strcpy(array[i+1], " ");
		i = i + 2;
	}
	else if (i % 3 == 2)
	{
		strcpy(array[i], " ");
		i = i + 1;
	}
	
	if (actual_elements >= 9)			//FILES: 9-up
	{
		for (int a = 0; a < i/3; a++)											//loop the array of names
		{
			printf("%s", array[a]);												//1st column
			padding(40 - strlen(array[a]));										//padding
			
			printf("%s", array[a+(i/3)]);										//2nd column
			padding(40 - strlen(array[a+(i/3)]));								//padding
			
			printf("%s", array[a+((i/3) * 2)]);									//3rd column
			printf("\n");
		}
	}
	else								//FILES: 1-8
	{
		for (int a = 0; a < i-1; a++)
		{
			printf("%s\n", array[a]);											//just 1 column
		}
	}

}
//********************************** PRINT OPTION 4: '-o' *******************************************

//Displays the long format listing, but excludes group name.
void print_with_option_o(struct dirent *dirp, DIR *dp)
{
	struct stat stat_buf;
	char list[20];
	
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (stat(dirp->d_name, &stat_buf) < 0)	//get stat on the file
		{
			printf("lstat() error\n");
			exit(0);
		}
		
		if (dirp->d_name[0] != '.')								//filter out hidden files
		{
			printf( (S_ISDIR(stat_buf.st_mode)) ? "d" : "-");	//print permissions in rwx format
			printf( (stat_buf.st_mode & S_IRUSR) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWUSR) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXUSR) ? "x" : "-");
			printf( (stat_buf.st_mode & S_IRGRP) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWGRP) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXGRP) ? "x" : "-");
			printf( (stat_buf.st_mode & S_IROTH) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWOTH) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXOTH) ? "x" : "-");
			
			if (listxattr(dirp->d_name, list, 20, XATTR_NOFOLLOW) != 0)	//print extended attribute flag
				printf("@\t");
			else
				printf("\t");
			
			printf("%d\t", stat_buf.st_nlink);				//number of hard links
			
			struct passwd *pw = getpwuid(stat_buf.st_uid);	//username
			printf("%s\t", pw->pw_name);
			
			printf("%lld\t", stat_buf.st_size);				//print size of file
			
			//TIME:
			char time[300];
			struct tm *time_struct;
			time_struct = localtime(&stat_buf.st_mtime);		//breaks down raw time value into time_struct
			strftime(time, 200, "%b %d %H:%M", time_struct);	//formats time_struct, stores result in time
			printf("%s\t", time);
			
			printf("%s\n",dirp->d_name);						//print directory name
		}
	}
}
//********************************** PRINT OPTION 5: '-n' *******************************************

//Displays the long format listing, with GID and UID numbers
void print_with_option_n(struct dirent *dirp, DIR *dp)
{
	struct stat stat_buf;
	char list[20];
	
	while ((dirp = readdir(dp)) != NULL)						//read each directory
	{
		
		if (stat(dirp->d_name, &stat_buf) < 0)					//get stat on the file
		{
			printf("lstat() error\n");
			exit(0);
		}
		
		if (dirp->d_name[0] != '.')								//filter out hidden files
		{
			printf( (S_ISDIR(stat_buf.st_mode)) ? "d" : "-");	//print permissions in rwx format
			printf( (stat_buf.st_mode & S_IRUSR) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWUSR) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXUSR) ? "x" : "-");
			printf( (stat_buf.st_mode & S_IRGRP) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWGRP) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXGRP) ? "x" : "-");
			printf( (stat_buf.st_mode & S_IROTH) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWOTH) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXOTH) ? "x" : "-");
			
			if (listxattr(dirp->d_name, list, 20, XATTR_NOFOLLOW) != 0)	//print extended attribute flag
				printf("@\t");
			else
				printf("\t");
			
			printf("%d\t", stat_buf.st_nlink);					//number of hard links
			
			printf("%d\t", stat_buf.st_uid);					//username
			
			printf("%d\t", stat_buf.st_gid);					//group name
			
			printf("%lld\t", stat_buf.st_size);					//print size of file
			
			//TIME:
			char time[300];
			struct tm *time_struct;
			time_struct = localtime(&stat_buf.st_mtime);		//breaks down raw time value into time_struct
			strftime(time, 200, "%b %d %H:%M", time_struct);	//formats time_struct, stores result in time
			printf("%s\t", time);
			
			printf("%s\n",dirp->d_name);						//print directory name
		}
	}
}
//********************************** PRINT OPTION 6: '-r' *******************************************

//Reverse the order of the sort
void print_with_option_r(struct dirent *dirp, DIR *dp)
{
	char array[100][100];
	int i = 0;
	
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')				//filter out hidden files
		{
			strcpy(array[i], dirp->d_name);		//store each name in array
			i++;//grabs one too many
		}
	}
	
	char temp[100];
	int x = i - 1, y = 0;
	while (x > y)								//reverse the array
	{
		strcpy(temp, array[x]);
		strcpy(array[x], array[y]);
		strcpy(array[y], temp);
		x--;
		y++;
	}
	
	int actual_elements = i;					//save i before modifing it
	
	if (i % 3 == 1)								//even out the array into sections of 3
	{
		strcpy(array[i], " ");
		strcpy(array[i+1], " ");
		i = i + 2;
	}
	else if (i % 3 == 2)
	{
		strcpy(array[i], " ");
		i = i + 1;
	}
	
	if (actual_elements >= 9)			//FILES: 9-up
	{
		for (int a = 0; a < i/3; a++)											//loop the array of names
		{
			printf("%s", array[a]);												//1st column
			padding(40 - strlen(array[a]));										//padding
			
			printf("%s", array[a+(i/3)]);										//2nd column
			padding(40 - strlen(array[a+(i/3)]));								//padding
			
			printf("%s", array[a+((i/3) * 2)]);									//3rd column
			printf("\n");
		}
	}
	else								//FILES: 1-8
	{
		for (int a = 0; a < i-1; a++)
		{
			printf("%s\n", array[a]);											//just 1 column
		}
	}
}
//********************************** PRINT OPTION 7: '-s' *******************************************

//Display the number of file system blocks actually used by each file
void print_with_option_s(struct dirent *dirp, DIR *dp)
{
	char array[100][100];
	int i = 0;
	struct stat stat_buf;
	
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')					//filter out hidden files
		{
			if (stat(dirp->d_name, &stat_buf) < 0)	//get stat on the file
			{
				printf("lstat() error\n");
				exit(0);
			}
			
			char result[100];
			sprintf(result, "%lld ", stat_buf.st_blocks);	//convert # of sys. blocks to string
			strcat(result, dirp->d_name);					//concatenate #blocks + file_name
			
			strcpy(array[i], result);			//store each string in array
			i++;//grabs one too many
			
			
		}
	}
	
	int actual_elements = i;					//save i before modifing it
	
	if (i % 3 == 1)								//even out the array into sections of 3
	{
		strcpy(array[i], " ");
		strcpy(array[i+1], " ");
		i = i + 2;
	}
	else if (i % 3 == 2)
	{
		strcpy(array[i], " ");
		i = i + 1;
	}
	
	if (actual_elements >= 9)			//FILES: 9-up
	{
		for (int a = 0; a < i/3; a++)											//loop the array of names
		{
			printf("%s", array[a]);												//1st column
			padding(40 - strlen(array[a]));										//padding
			
			printf("%s", array[a+(i/3)]);										//2nd column
			padding(40 - strlen(array[a+(i/3)]));								//padding
			
			printf("%s", array[a+((i/3) * 2)]);									//3rd column
			printf("\n");
		}
	}
	else								//FILES: 1-8
	{
		for (int a = 0; a < i-1; a++)
		{
			printf("%lld ", stat_buf.st_blocks);
			printf("%s\n", array[a]);											//just 1 column
		}
	}
}
//********************************** PRINT OPTION 8: '-t' *******************************************

//Sort by time modified (most recently modified first)
void print_with_option_t(struct dirent *dirp, DIR *dp)
{
	struct stat stat_buf;
	
	struct Box array[100];
	char array_of_strings[100][100];
	int i = 0;
	
	while ((dirp = readdir(dp)) != NULL)						//read each directory
	{
		if (stat(dirp->d_name, &stat_buf) < 0)					//get stat on the file
		{
			printf("lstat() error\n");
			exit(0);
		}
		
		if (dirp->d_name[0] != '.')								//filter out hidden files
		{
			struct Box object;									//package the box with dir name and time
			object.d_name = dirp->d_name;
			object.time = stat_buf.st_mtime;
			array[i] = object;									//add to array
			i++;
		}
	}
	
	qsort(array, i, sizeof(struct Box), compare_function);		//sort array by time
	
	//convert array of Boxes to array of strings
	for (int a = 0; a < i; a++)
	{
		strcpy(array_of_strings[a], array[a].d_name);
	}
	
	//formating function
	
	int actual_elements = i;					//save i before modifing it
	
	if (i % 3 == 1)								//even out the array into sections of 3
	{
		strcpy(array_of_strings[i], " ");
		strcpy(array_of_strings[i+1], " ");
		i = i + 2;
	}
	else if (i % 3 == 2)
	{
		strcpy(array_of_strings[i], " ");
		i = i + 1;
	}
	
	if (actual_elements >= 9)			//FILES: 9-up
	{
		for (int a = 0; a < i/3; a++)											//loop the array of names
		{
			printf("%s", array_of_strings[a]);												//1st column
			padding(40 - strlen(array_of_strings[a]));										//padding
			
			printf("%s", array_of_strings[a+(i/3)]);										//2nd column
			padding(40 - strlen(array_of_strings[a+(i/3)]));								//padding
			
			printf("%s", array_of_strings[a+((i/3) * 2)]);									//3rd column
			printf("\n");
		}
	}
	else								//FILES: 1-8
	{
		for (int a = 0; a < i-1; a++)
		{
			printf("%s\n", array_of_strings[a]);											//just 1 column
		}
	}

	
	

}
//********************************** PRINT OPTION 9: '-x' *******************************************

//Multi-column output sorted across the page rather than down the page.
void print_with_option_x(struct dirent *dirp, DIR *dp)
{
	int counter = 1;
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')					//filter out hidden files
		{
			if (counter < 3)						//1st and 2nd column
			{
				printf("%s", dirp->d_name);
				padding(40 - strlen(dirp->d_name));
				counter++;
			}
			else									//3rd column
			{
				printf("%s\n", dirp->d_name);
				counter = 1;
			}
			
		}
	}

}

//********************************** PRINT OPTION 10: '-1' *******************************************

//Force output to be one entry per line.
void print_with_option_1(struct dirent *dirp, DIR *dp)
{
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')				//filter out hidden files
		{
			printf("%s\n",dirp->d_name);		//print each file_name
		}
	}
}

//********************************** PRINT OPTION 11: '-g' *******************************************

//List in long format as in -l, except that the owner is not printed.
void print_with_option_g(struct dirent *dirp, DIR *dp)
{
	struct stat stat_buf;
	char list[20];
	
	while ((dirp = readdir(dp)) != NULL)						//read each directory
	{
		
		if (stat(dirp->d_name, &stat_buf) < 0)					//get stat on the file
		{
			printf("lstat() error\n");
			exit(0);
		}
		
		if (dirp->d_name[0] != '.')								//filter out hidden files
		{
			printf( (S_ISDIR(stat_buf.st_mode)) ? "d" : "-");	//print permissions in rwx format
			printf( (stat_buf.st_mode & S_IRUSR) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWUSR) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXUSR) ? "x" : "-");
			printf( (stat_buf.st_mode & S_IRGRP) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWGRP) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXGRP) ? "x" : "-");
			printf( (stat_buf.st_mode & S_IROTH) ? "r" : "-");
			printf( (stat_buf.st_mode & S_IWOTH) ? "w" : "-");
			printf( (stat_buf.st_mode & S_IXOTH) ? "x" : "-");
			
			if (listxattr(dirp->d_name, list, 20, XATTR_NOFOLLOW) != 0)	//print extended attribute flag
				printf("@\t");
			else
				printf("\t");
			

			
			printf("%d\t", stat_buf.st_nlink);					//number of hard links
			
			
			struct passwd *pw = getpwuid(stat_buf.st_uid);		//username
			
			struct group *grp = getgrgid(pw->pw_gid);			//group name
			printf("%s\t", grp->gr_name);
			
			printf("%lld\t", stat_buf.st_size);					//print size of file
			
			//TIME:
			char time[300];
			struct tm *time_struct;
			time_struct = localtime(&stat_buf.st_mtime);		//breaks down raw time value into time_struct
			strftime(time, 200, "%b %d %H:%M", time_struct);	//formats time_struct, stores result in time
			printf("%s\t", time);
			
			printf("%s\n",dirp->d_name);						//print directory name
			
		}
	}

}

//********************************** PRINT OPTION 12: '-m' *******************************************

//Stream output format; list files across the page, separated by commas.
void print_with_option_m(struct dirent *dirp, DIR *dp)
{
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')					//filter out hidden files
		{
				printf("%s, ", dirp->d_name);
		}
	}
	printf("\n");
}

//********************************** PRINT OPTION 13: '-p' *******************************************

//Display a slash (‘/’) immediately after each pathname that is a directory.
void print_with_option_p(struct dirent *dirp, DIR *dp)
{
	struct stat stat_buf;
	char array[100][100];
	int i = 0;
	
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')					//filter out hidden files
		{
			if (stat(dirp->d_name, &stat_buf) < 0)	//get stat on the file
			{
				printf("lstat() error\n");
				exit(0);
			}
			if (S_ISDIR(stat_buf.st_mode))			//check if it's directory
				strcat(dirp->d_name, "/");			//add '/' to file_name
				
			strcpy(array[i], dirp->d_name);		//store each name in array
			
			i++;//grabs one too many
		}
	}
	
	int actual_elements = i;					//save i before modifing it
	
	if (i % 3 == 1)								//even out the array into sections of 3
	{
		strcpy(array[i], " ");
		strcpy(array[i+1], " ");
		i = i + 2;
	}
	else if (i % 3 == 2)
	{
		strcpy(array[i], " ");
		i = i + 1;
	}
	
	if (actual_elements >= 9)			//FILES: 9-up
	{
		for (int a = 0; a < i/3; a++)											//loop the array of names
		{
			printf("%s", array[a]);												//1st column
			padding(40 - strlen(array[a]));										//padding
			
			printf("%s", array[a+(i/3)]);										//2nd column
			padding(40 - strlen(array[a+(i/3)]));	//padding
			
			printf("%s", array[a+((i/3) * 2)]);									//3rd column
			printf("\n");
		}
	}
	else								//FILES: 1-8
	{
		for (int a = 0; a < i-1; a++)
		{
			printf("%s\n", array[a]);											//just 1 column
		}
	}

}


//********************************** PRINT OPTION 14: 'F' *******************************************

//append indicator (one of */=@|) to entries
void print_with_option_F(struct dirent *dirp, DIR *dp)
{
	char array[100][100];
	int i = 0;
	struct stat stat_buf;
	
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')				//filter out hidden files
		{
			if (lstat(dirp->d_name, &stat_buf) < 0)					//get stat on the file
			{
				printf("lstat() error\n");
				exit(0);
			}
			
			if (S_ISDIR(stat_buf.st_mode))	strcat(dirp->d_name, "/");	//is a directory
			if (S_ISLNK(stat_buf.st_mode))	strcat(dirp->d_name, "@");	//is a symlink
			if (S_ISFIFO(stat_buf.st_mode)) strcat(dirp->d_name, "|");	//is a named pipe
			if (S_ISSOCK(stat_buf.st_mode)) strcat(dirp->d_name, "=");	//is a socket
			
			//test if executable (ugly)				not a dir						not a symlink
			if ((stat_buf.st_mode & S_IXUSR) && (!S_ISDIR(stat_buf.st_mode)) && (!S_ISLNK(stat_buf.st_mode)))	//is executable file
				strcat(dirp->d_name, "*");
			
			strcpy(array[i], dirp->d_name);		//store each name in array
			i++;//grabs one too many
		}
	}
	
	int actual_elements = i;					//save i before modifing it
	
	if (i % 3 == 1)								//even out the array into sections of 3
	{
		strcpy(array[i], " ");
		strcpy(array[i+1], " ");
		i = i + 2;
	}
	else if (i % 3 == 2)
	{
		strcpy(array[i], " ");
		i = i + 1;
	}
	
	if (actual_elements >= 9)			//FILES: 9-up
	{
		for (int a = 0; a < i/3; a++)											//loop the array of names
		{
			printf("%s", array[a]);												//1st column
			padding(40 - strlen(array[a]));										//padding
			
			printf("%s", array[a+(i/3)]);										//2nd column
			padding(40 - strlen(array[a+(i/3)]));	//padding
			
			printf("%s", array[a+((i/3) * 2)]);									//3rd column
			printf("\n");
		}
	}
	else								//FILES: 1-8
	{
		for (int a = 0; a < i-1; a++)
		{
			printf("%s\n", array[a]);											//just 1 column
		}
	}

}

//********************************** PRINT OPTION 15: 'R' *******************************************

//Recursively list subdirectories encountered (not complete)
void print_with_option_R(struct dirent *dirp, DIR *dp)
{
	char array[100][100];
	char array_of_directories[100][100];
	int i = 0, index_for_dir_array = 0;
	struct stat stat_buf;
	
	
	while ((dirp = readdir(dp)) != NULL)  //read each directory
	{
		if (dirp->d_name[0] != '.')				//filter out hidden files
		{
			if (lstat(dirp->d_name, &stat_buf) < 0)					//get stat on the file
			{
				printf("lstat() error\n");
				exit(0);
			}
			
			if (S_ISDIR(stat_buf.st_mode))		//if directory, save it
			{
				strcpy(array_of_directories[index_for_dir_array], dirp->d_name);
				index_for_dir_array++;
			}
			
			strcpy(array[i], dirp->d_name);		//store each name in array
			i++;//grabs one too many
		}
	}
	
	int actual_elements = i;					//save i before modifing it
	
	if (i % 3 == 1)								//even out the array into sections of 3
	{
		strcpy(array[i], " ");
		strcpy(array[i+1], " ");
		i = i + 2;
	}
	else if (i % 3 == 2)
	{
		strcpy(array[i], " ");
		i = i + 1;
	}
	
	if (actual_elements >= 9)			//FILES: 9-up
	{
		for (int a = 0; a < i/3; a++)											//loop the array of names
		{
			printf("%s", array[a]);												//1st column
			padding(40 - strlen(array[a]));										//padding
			
			printf("%s", array[a+(i/3)]);										//2nd column
			padding(40 - strlen(array[a+(i/3)]));	//padding
			
			printf("%s", array[a+((i/3) * 2)]);									//3rd column
			printf("\n");
		}
	}
	else								//FILES: 1-8
	{
		for (int a = 0; a < i-1; a++)
		{
			printf("%s\n", array[a]);											//just 1 column
		}
	}
	
	//display saved directories and their files
	DIR *dirp1;
	struct dirent *dp1;
	for (int x = 0; x < index_for_dir_array; x++)
	{
		printf("\n\n%s/\n", array_of_directories[x]);
		if ((dirp1 = opendir(array_of_directories[x])) == NULL) {
			perror ("Cannot open .");
			exit (1);
		}
		while ((dp1 = readdir (dirp1)) != NULL)
		{
			if (dp1->d_name[0] != '.')				//filter out hidden files
			printf("%s\n", dp1->d_name);
		}
		printf("\n");
		
	}
	

}

//********************************** HELPER METHODS *******************************************

//comparator func for qsort:
int compare_function(const void *a,const void *b)
{
	struct Box *ptr1 = (struct Box*)a;		//cast void ptr to struct Box ptr
	struct Box *ptr2 = (struct Box*)b;
	
	long int left = ptr1->time;				//assign left and right value to compare
	long int right = ptr2->time;
	
	return right - left;					//0 = equal, <0 = right goes first, >0 = left goes first
}

//generate dummy files:
void generate_files()
{
	for (int i = 1; i <= 20; i++)
	{
		char num[10];
		char filename[20] = "file";
		sprintf(num, "%d.txt", i);			//convert # to string
		strcat(filename, num);				//concatenate
		FILE *fp;
		fp = fopen(filename, "w");
		fclose(fp);
	}
	for (int i = 1; i <= 5; i++)
	{
		char num[10];
		char directory_name[20] = "directory";
		sprintf(num, "%d", i);				//convert # to string
		strcat(directory_name, num);		//concatenate
		mkdir(directory_name, 0777);
	}
	//make symlinks
	symlink("file1.txt", "symbolic_link");
}

void padding(int size)
{
	for (int b = 0; b < size; b++) printf(" ");	//padding
}











