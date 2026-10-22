#include<stdio.h>
#include"sqstack.h"


static int compute(sqstack *snum,int op)
{
	datatype n1,n2,result;

	if(st_pop(snum,&n2) != 0)
		return -1;

	if(st_pop(snum,&n1) != 0)
		return -1;

	switch(op)
	{
		case '+':
			result = n1 + n2;
			break;
		case '-':
			result = n1 - n2;
			break;
		case '*':
			result = n1 * n2;
			break;
		case '/':
			if(n2 ==0)
				return -1;

			result =n1/n2;
			break;
		default:
			return -1;
	}

	if(st_push(snum,&result) !=0)
		return -1;

	return 0;
}

static int get_pri(int op)
{
	switch(op)
	{
	case '(':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	
	default:
		return -1;
	}
}

static int deal_op(sqstack *snum, sqstack *sop,int op)
{
	datatype old_op;
	datatype new_op = op;

	if(get_pri(op) < 0)
		return -1;

	if(op == '(')
		return st_push(sop,&new_op);

	while(!st_isempty(sop))
	{
		if(st_top(sop,&old_op) != 0)
			return -1;

		if(old_op == '(' || get_pri(op) > get_pri(old_op))
			break;

		if(st_pop(sop,&old_op) != 0)
			return -1;
		
		if(compute(snum,old_op) != 0)
			return -1;
	}

	return st_push(sop,&new_op);
}

static int deal_bracket(sqstack *snum,sqstack *sop)
{
	datatype old_op;

	while(!st_isempty(sop))
	{
		if(st_top(sop,&old_op)!=0)
			return -1;

		if(old_op == '(')
		{
			st_pop(sop,&old_op);
			return 0;
		}

		if(st_pop(sop,&old_op)!=0)
			return -1;

		if(compute(snum,old_op)!=0)
			return -1;
	}

	return -1;
}

static void print_state(sqstack *snum,sqstack *sop)
{
	int i;

	printf("Number stack: ");

	if(st_isempty(snum))
	{
		printf("<empty>");
	}
	else
	{
		for(i=0;i<=snum->top;i++)
		{
			printf("%d ",snum->data[i]);
		}
	}
	
	printf("\n");

	printf("Operator stack:");

	if(st_isempty(sop))
	{
		printf("<empty");
	}
	else
	{
		for(i=0;i<=sop->top;i++)
		{
			printf("%c ",(char)sop->data[i]);
		}
	}

	printf("\n");
}


int main(void)
{
	char str[] ="(11+3)*2-5";
	int i =0;
	int value =0;
	int flag =0;
	
	datatype old_op;
	datatype result;
	
	
	sqstack *snum =st_create();
	sqstack *sop =st_create();

	if(snum == NULL || sop == NULL)
		goto error;
	
	while(str[i] != '\0')
	{
		if(str[i]>='0' &&str[i]<='9')
		{
			value =value *10 + (str[i] - '0');
			flag =1;
		}
		else
		{
			if(flag)
			{
				if(st_push(snum,&value)!=0)
					goto error;

				value = 0;
				flag =0;
			}

			if(str[i] ==')')
			{
				if(deal_bracket(snum,sop)!=0)
					goto error;
			}
			else
			{
				if(deal_op(snum,sop,str[i])!=0)
					goto error;
			}
		}
	
		//printf("\nCurrent character:%c\n",str[i]);

		//print_state(snum,sop);

		//printf("value = %d,flag = %d\n",value,flag);
		
		i++;
	}

	if(flag)
	{
		if(st_push(snum,&value)!=0)
			goto error;
	}
	
	while(!st_isempty(sop))
	{
		if(st_pop(sop,&old_op)!=0)
			goto error;
	
		if(compute(snum,old_op)!=0)
			goto error;
	}

	if(st_pop(snum,&result)!=0)
		goto error;
	if(!st_isempty(snum))
		goto error;
	printf("%s = %d\n",str,result);

	st_destroy(snum);
	st_destroy(sop);

	return 0;

error:
    fprintf(stderr,
            "ERROR: i=%d, char=%d, snum_top=%d, sop_top=%d\n",
            i,
            (unsigned char)str[i],
            snum ? snum->top : -99,
            sop ? sop->top : -99);

    fprintf(stderr, "Calculation failed\n");
	st_destroy(snum);
	st_destroy(sop);
	    return 1;


}

