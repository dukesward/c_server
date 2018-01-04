#include "config.h"

void printConfig(struct F_Config *c)
{
	printf("[%d] %s = %s\n", c->index, c->key, c->value);

	if(c->next != NULL)
		printConfig(c->next);
}

void loadConfig()
{
	struct F_Config *c = malloc(sizeof(struct F_Config));
    // initialize members of c
    c->key = NULL;
    c->value = NULL;
    c->index = -1;
    c->next = NULL;

    struct F_State *s = malloc(sizeof(struct F_State));

    s->filename = _CONFIG_;
    s->f = 0;
    s->s = IDLE;
    s->first_ln = c;

    readConfig(s);
    // printConfig(s->first_ln);

    char *key = "port";
    char *value = getConfig(s->first_ln, key);

    if(value == NULL)
    	printf("Config value not found for key [%s]\n", key);
    else
    	printf("Config value for key [%s] is [%s]\n", key, value);
}

void readConfig(struct F_State *s)
{
	FILE *f;
	printf("Reading config from: %s\n", s->filename);
	// open the config for reading text
	f = fopen(s->filename, "rt");

	if(f == 0)
		perror("Cannot open file");
	else
		readConfigBuf(f, s->first_ln);

	// close the file after reading
	fclose(f);
}

void readConfigBuf(FILE *f, struct F_Config *c)
{
	char *buf, *key, *value;
	int ch, buf_s, pos, line_n, has_key, is_cmt;
	// set initial buff size
	buf_s = BUF_S;
	buf = (char *) malloc(buf_s * sizeof(char));
	// indicates the num of chars read
	pos = 0;
	// indicates the current line number
	line_n = 0;
	// indicates if key has been located for each config
	has_key = 0;
	// indicates if the current line is a comment
	is_cmt = 0;

	while(ch != EOF)
	{
		// get one char each time
		ch = fgetc(f);
		// these chars indicating an end of line
		if(ch==EOF || ch=='\n' || ch=='\r')
		{
			// terminate buffer
			buf[pos] = 0;
			// need to throw the line into the config if not '\r'
			if(strlen(buf) > 0) fillConfig(c, key, value, 0);
			// refresh the position index for new line
			pos = 0;
			// refresh the comment indicator
			is_cmt = 0;
			// reset buf size to default and refresh the buffer
			buf_s = BUF_S;
			buf = (char *) malloc(buf_s * sizeof(char));
			// refrech key indicator
			has_key = 0;
		}
		else
		{
			if(pos >= buf_s - 1)
			{
				// if buf is full, realloc more mem into it
				buf_s += BUF_S;
				buf = (char *) realloc(buf, buf_s * sizeof(char));
			}

			if(ch == '#')
			{
				if(pos == 0)
				{
					// if the first char is #, the whole line is comment
					is_cmt = 1;
				}
			}
			else if(ch!=' ' && is_cmt!=1)
			{
				if(has_key==0 && ch=='=')
				{
					key = (char *) malloc(strlen(buf) * sizeof(char));
					key = buf;
					// reset buf size to accommodate for value
					buf_s = BUF_S;
					buf = (char *) malloc(buf_s * sizeof(char));
					// found key
					has_key = 1;
					pos = 0;
				}
				else
				{
					// keep building the current line
					buf[pos] = ch;
					pos++;
				}
			}

			value = (char *) malloc(strlen(buf) * sizeof(char));
			value = buf;
		}
	}
}

void fillConfig(struct F_Config *c, char *key, char *value, int i)
{
	// if the index is -1, fill the config, other wise allocate mem for the next
	if(c->index < 0)
	{
		// printf("Size of line: %zu\n", strlen(line));
		c->key = (char *) malloc((strlen(key) + 1) * sizeof(char));
		strcpy(c->key, key);

		c->value = (char *) malloc((strlen(value) + 1) * sizeof(char));
		strcpy(c->value, value);

		c->index = i;
	}
	else
	{
		int j = c->index + 1;
		if(c->next == NULL)
		{
			struct F_Config *next = malloc(sizeof(struct F_Config));
			// initialize members of next
			next->key = NULL;
			next->value = NULL;
			next->index = -1;
			next->next = NULL;
			c->next = next;
		}
		// the next line should always have an index from the current increased by 1
		fillConfig(c->next, key, value, j);
	}
}

void processConfig(char *c)
{
	int i, buf_s, has_key, pos;
	char *buf, *key, *value;
	// indicates if we already found the key
	has_key = 0;
	pos = 0;

	buf_s = BUF_S;
	buf = (char *) malloc(buf_s * sizeof(char));

	for(i=0; i<strlen(c); i++)
	{
		if(i >= buf_s - 1)
		{
			buf_s += BUF_S;
			buf = (char *) realloc(buf, buf_s * sizeof(char));
		}

		if(c[i] != ' ')
		{
			if(has_key==0 && c[i]=='=') 
			{
				key = (char *) malloc(strlen(buf) * sizeof(char));
				key = buf;
				buf_s = BUF_S;
				buf = (char *) malloc(buf_s * sizeof(char));
				pos = 0;
			}
			else
			{
				buf[pos] = c[i];
				pos++;
			}
		}
	}

	value = (char *) malloc(strlen(buf) * sizeof(char));
	value = buf;
	printf("Key: %s\n", key);
	printf("Value: %s\n", value);
}

char *getConfig(struct F_Config *c, char *key)
{
	int dif;
	char *ret;
	// compare the key with the config key
	dif = strcmp(key, c->key);

	if(dif == 0)
	{
		// if the key matches, return the value of the current config
		ret = (char *) malloc((strlen(c->value) + 1) * sizeof(char));
		strcpy(ret, c->value);
	}
	else if(c->next != NULL)
	{
		// if more configs, keep searching
		ret = getConfig(c->next, key);
	}
	else
	{
		// if no matches and no more configs, return NULL
		ret = NULL;
	}
}
///////