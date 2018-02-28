#include <stdio.h>
#include <malloc.h>

struct student_records{
	int id;
	char* firstname;
	char* lastname;
	float gpa;
	char* major;
	int mark_as_delete;//0 not delete, 1 delete
	struct student_records* next_student_records;
};

int compare(char *s1,char *s2){
	for(;*s1==*s2;++s1,++s2)
		if(*s1=='\0')
			return 0;
	return -1;
}
int length(char* s)
{
	int len = 0;
	while(1==1)
	{
		if((*s)!='\0')
		{
			len = len + 1;
			s++;
		}
		else
		{
			return len;
		}
	}
}
int is_letter(char s)
{
	if (s<65 || s>122)
		return 0;
	if (s>90 && s<97)
		return 0;
	return 1;
}

int is_lower(char s)
{
	if (s<97 || s>122)
		return 0;
	return 1;
}

int is_upper(char s)
{
	if (s<65 || s>90)
		return 0;
	return 1;
}

int re_name(char *s)
{
	if (is_letter(*s))
	{
		if (is_lower(*s))
			*s = *s - 32;
	}
	else
		return 0;
	s++;
	while (*s != '\0')
	{
		if (is_letter(*s))
		{
			if (is_upper(*s))
				*s = *s +32;
		}
		else
			return 0;
		s++;
	}
	return 1;
}

int re_major(char *s)
{
	if (length(s) != 3)
		return 0;
	while (*s != '\0')
	{
		if (is_letter(*s))
		{
			if (is_lower(*s))
				*s = *s - 32;
		}
		else
			return 0;
		s++;
	}
	return 1;
}

// -1 input error
int handle(int id, char* firstname, char* lastname, float gpa, char* major)
{
	if (id <= 0 || gpa<1.0 || gpa > 4.0)
		return -1;
	if (length(firstname)<3 || length(firstname)>10 || length(lastname)<3 || length(lastname)>10 || length(major) != 3)
		return -1;
	int f1 = re_name(firstname);
	int f2 = re_name(lastname);
	int f3 = re_major(major);
	if (f1==0 || f2==0 || f3==0)
		return -1;
	return 1;
}


int main(int argc, char** argv) {
	int error_mark=0; //0 normal; -1 input file not exist; -2 failed to parse file;
	char* inputfile;
	if (argc >= 2)
		inputfile = *(argv+1);
	else
	{
		printf("FAILED TO PARSE FILE\n");
		return 0;
	}
	FILE *file = NULL;
	file = fopen(inputfile, "r");
	struct student_records* head_record = NULL;
	struct student_records* tail_record = NULL;
	if (file == NULL)
	{
		printf("FAILED TO PARSE FILE\n");
		return 0;
	}
	else
	{
		char* ADD = "ADD";
		char* UPDATE = "UPDATE";
		char* DELETE = "DELETE";
		char* command;
		command = (char *)malloc(sizeof(char)*100);
		fscanf(file,"%s",command);
		while (!feof(file))
		{			
			int id;
			char* firstname;
			firstname = (char *)malloc(sizeof(char)*100);
			char* lastname;
			lastname = (char *)malloc(sizeof(char)*100);
			float gpa;
			char* major;
			major = (char *)malloc(sizeof(char)*100);
			if (compare(command,ADD)==0)
			{
				if (fscanf(file, "%d %s %s %f %s\n", &id, firstname, lastname, &gpa, major))
				{
					if (handle(id, firstname, lastname, gpa, major)==-1)
					{
						error_mark = -2;
						break;
					}
					struct student_records* temp = NULL;
					temp = malloc(sizeof(struct student_records));
					temp->id = id;
					temp->firstname = firstname;
					temp->lastname = lastname;
					temp->gpa = gpa;
					temp->major = major;
					temp->mark_as_delete = 0;
					temp->next_student_records = NULL;
					if(head_record == NULL)
					{
						head_record = temp;
						tail_record = temp;
					}
					else
					{
						if (temp->id < head_record->id)
						{
							temp->next_student_records = head_record;
							head_record = temp;
						}
						else
						{
							if (temp->id > tail_record->id)
							{
								tail_record->next_student_records = temp;
								tail_record = temp;
							}
							else
							{
								struct student_records* insert_temp = NULL;
								insert_temp = head_record;
								while (1==1)
								{
									if (insert_temp->id == temp-> id)
									{
										error_mark = -4;
										break;
									}
									if (insert_temp->next_student_records->id > temp->id)
									{
										temp->next_student_records = insert_temp->next_student_records;
										insert_temp->next_student_records = temp;
										break;
									}
									else
									{
										insert_temp = insert_temp->next_student_records;
									}
								}
								if (error_mark == -4)
									break;
							}
						}
					}
					fscanf(file, "%s", command);
				}
				else
				{
					error_mark = -2;
					break;
				}
			}
			else
			{
				if (compare(command,UPDATE)==0)
				{
					if (fscanf(file, "%d %s %s %f %s\n", &id, firstname, lastname, &gpa, major))
					{	
						struct student_records* temp = head_record;
						while(1==1)
						{
							if (temp->id != id)
							{
								if (temp->next_student_records == NULL)
								{
									error_mark = -3;
									break;
								}
								else
								{
									temp = temp->next_student_records;
								}
							}
							else
							{
								break;
							}
						}
						if (error_mark == -3)
							break;
						if (handle(id,firstname,lastname,gpa,major) == -1)
						{
							error_mark = -2;
							break;
						}
						temp->id = id;
						free(temp->firstname);
						temp->firstname = firstname;
						free(temp->lastname);
						temp->lastname = lastname;
						temp->gpa = gpa;
						free(temp->major);
						temp->major = major;
						fscanf(file, "%s", command);
					}
					else
					{
						error_mark = -2;
						break;
					}
				}
	
				else
				{
					if (compare(command,DELETE)==0)
					{
						if (fscanf(file,"%d\n",&id))
						{
							struct student_records* temp = head_record;
							if (temp->id == id)
							{
								free(temp->firstname);
								free(temp->lastname);
								free(temp->major);
								if (temp->next_student_records != NULL)
									head_record = temp->next_student_records;
								else
								{
									head_record = NULL;
									tail_record = NULL;
								}
								free(temp);
							}
							else
							{
								if (temp->next_student_records == NULL)
								{
									error_mark = -3;
									break;
								}
								while(1==1)
								{
									if (temp->next_student_records->id != id)
									{
										if (temp->next_student_records->next_student_records == NULL)
										{
											error_mark = -3;
											break;
										}
											else
										{
											temp = temp->next_student_records;
										}
									}
									else
									{
										break;
									}
								}
								if (error_mark == -3)
									break;
								free(temp->next_student_records->firstname);
								free(temp->next_student_records->lastname);
								free(temp->next_student_records->major);
								struct student_records* temp_1 = temp->next_student_records;
								temp->next_student_records = temp->next_student_records->next_student_records;
								if (temp->next_student_records == NULL)
									tail_record = temp;
								free(temp_1);
							}
						}
						fscanf(file,"%s", command);
					}
					else
					{
						error_mark = -2;
						break;
					}
				}
			}
		}
		fclose(file);
	}

	if (error_mark == -2)
	{
		printf("FAILED TO PARSE FILE\n");
		return -1;
	}
	if (error_mark == -3)
	{
		printf("STUDENT RECORD CANNOT BE DELETED NOR UPDATED\n");
		return -1;
	}
	if (error_mark == -4)
	{
		printf("ID NOT UNIQUE\n");
		return -1;
	}
	if (head_record == NULL)
		error_mark = -11;
	char* flagv = "-v";
	char* flagi = "-i";
	char* flagf = "-f";
	char* flagm = "-m";
	char* flago = "-o";
	char* flagg = "-g";
	int if_v = 0;
	int if_i = 0;
	int if_f = 0;
	int if_m = 0;
	int if_o = 0;
	int if_g = 0;
	char* arg_i;
	char* arg_f;
	char* arg_m;
	char* arg_o;
	
	int argv_posi = 2;
	if (argc == 2 || error_mark == -11)
	{
		if (argc == 2)
			printf("NO QUERY PROVIDED\n");
	}
	else
	{
		while (argv_posi < argc)
		{
			if (compare((*(argv+argv_posi)),flagv) == 0)
			{
				if_v = 1;
				if (if_i == 1 || if_f == 1 || if_m == 1)
				{
					error_mark = -5;
					break;
				}
			}
			if (compare((*(argv+argv_posi)),flagi) == 0)
			{
				argv_posi++;
				arg_i = *(argv+argv_posi);
				if_i = 1;
				if (if_v == 1)
				{
					error_mark = -5;
					break;
				}
			}
			if (compare((*(argv+argv_posi)),flagf) == 0)
			{
				argv_posi++;
				arg_f = *(argv+argv_posi);
				if_f = 1;
				if (if_v == 1)
				{
					error_mark = -5;
					break;
				}
			}
			if (compare((*(argv+argv_posi)),flagm) == 0)
			{
				argv_posi++;
				arg_m = *(argv+argv_posi);
				if_m = 1;
				if (if_v == 1)
				{
					error_mark = -5;
					break;
				}
			}
			if (compare((*(argv+argv_posi)),flago) == 0)
			{
				argv_posi++;
				arg_o = *(argv+argv_posi);
				if_o = 1;
			}
			if (compare((*(argv+argv_posi)),flagg) == 0)
			{
				if_g = 1;
			}
			argv_posi++;
		}
		if	(error_mark != -5)
		{
			if (if_i == 1)
			{
				int sum = 0;
				while ((*arg_i) != '\0')
				{
					sum = sum*10;
					if ((*arg_i) < 48 || (*arg_i) > 57)
					{
						error_mark = -12;
						break;
					}
					sum = sum + ((*arg_i) - '0');
					arg_i++;
				}
				if (error_mark != -12)
				{
					struct student_records* flag_temp = NULL;
					flag_temp = head_record;
					while (flag_temp!=NULL)
					{
						if (flag_temp->id != sum)
						{
							flag_temp->mark_as_delete = 1;
						}
						flag_temp = flag_temp->next_student_records;
					}
				}
			}
			if (if_f == 1)
			{
				if (re_name(arg_f) == 0 || length(arg_f) < 3 || length(arg_f) > 10)
					error_mark = -5;
				else
				{
					struct student_records* flag_temp = NULL;
					flag_temp = head_record;
					while (flag_temp!=NULL)
					{
						if (compare((flag_temp->lastname),arg_f)!=0)
						{
							flag_temp->mark_as_delete = 1;
						}
						flag_temp = flag_temp->next_student_records;
					}
				}
			}
			if (if_m == 1)
			{
				if (re_major(arg_m) == 0)
					error_mark = -5;
				else
				{
					struct student_records* flag_temp = NULL;
					flag_temp = head_record;
					while (flag_temp!=NULL)
					{
						if (compare((flag_temp->major),arg_m)!=0)
						{
							flag_temp->mark_as_delete = 1;
						}
						flag_temp = flag_temp->next_student_records;
					}
				}
			}
			if (error_mark == 0)
			{
				float ave_gpa = 0.0;
				float gpa_count = 0.0;
				if (if_o == 0)
				{
					int mark_print = 0;
					struct student_records* pt = NULL;
					pt = head_record;
					while(pt != NULL)
					{
						if (pt->mark_as_delete == 0)
						{
							if (if_g == 0)
							{
								printf("%d %s %s %.2f %s\n", pt->id, pt->firstname, pt->lastname, pt->gpa, pt->major);
							}
							else
							{
								ave_gpa = ave_gpa + pt->gpa;
								gpa_count = gpa_count + 1.0;
							}
							mark_print = 1;
						}
						pt = pt->next_student_records;
					}
					if (mark_print == 0)
						error_mark = -11;
					else
					{
						if (if_g == 1)
						{
							float result;
							result = ave_gpa / gpa_count;
							printf("%.2f\n", result);
						}
					}
				}
				else
				{	
					if (if_v == 1 || if_i == 1 || if_f == 1 || if_m == 1)
					{
						char* outputfile = NULL;
						outputfile = arg_o;
						FILE *testfile = NULL;
						FILE *outfile = NULL;
						testfile = fopen(outputfile, "r");
						if (testfile == NULL)
						{
							outfile = fopen(outputfile, "w");
						}
						else
						{
							printf("The file is exist, do you want to overridden? y for yse, n for no.");
							int over;
							over = getchar();
							if (over == 121 )
							{
								outfile = fopen(outputfile, "w");
							}
							else
							{
								error_mark = -8;
							}
							fclose(testfile);
						}
						if (error_mark != -8)
						{
							int mark_print = 0;
							struct student_records* pt = NULL;
							pt = head_record;
							while(pt != NULL)
							{
								if (if_g == 0)
								{
									if (pt->mark_as_delete == 0)
									{
										fprintf(outfile,"%d %s %s %.2f %s\n", pt->id, pt->firstname, pt->lastname, pt->gpa, pt->major);
										mark_print = 1;
									}
								}
								else
								{
									ave_gpa = ave_gpa + pt->gpa;
									gpa_count = gpa_count + 1.0;
								}
								pt = pt->next_student_records;
							}
							if (mark_print == 0)
								error_mark = -11;
							else
							{
								if (if_g == 1)
								{
									float result = ave_gpa / gpa_count;
									fprintf(outfile, "%.2f\n", result);
								}
							}
						fclose(outfile);
						}
					}
					else
					{
						error_mark = -5;
					}
				}
			}
		}
	}
	if (head_record != NULL)
	{
		free(head_record->firstname);
		free(head_record->lastname);
		free(head_record->major);
		if (head_record->next_student_records!=NULL)
		{
			struct student_records* temp = head_record->next_student_records;
			free(head_record);
			head_record = temp;
		}
		else
		{
			free(head_record);
		}
	}
	if (error_mark == -5)
	{
		printf("OTHER ERROR\n");
		return 0;
	}
	if (error_mark == -12)
	{
		printf("OTHER ERROR\n");
		return 0;
	}
	if (error_mark == -11)
	{
		printf("STUDENT RECORD NOT FOUND\n");
		return 0;
	}
	if (error_mark == -8)
	{
		printf("FILE EXISTS\n");
		return 0;
	}
	return 0;
}
