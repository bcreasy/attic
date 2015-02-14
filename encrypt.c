#include <stdio.h>
#include <crypt.h>
#include <ctype.h>

int main(void)
{
	FILE *infile, *outfile;
	char p, q;
	int temp1, temp2;

	infile=fopen("/root/C/blah", "r");
	outfile=fopen("/root/C/blah.out", "w");

	do
	{
		p=fgetc(infile);
		q=fgetc(infile);

		if(p==EOF || q==EOF)
			break;
		p=toupper(p);
		q=toupper(q);

		if(p==' ')
			p=0;
		else
			p=p-'A'+1;

		if(q==' ')
			p=0;
		else
			q=q-'A'+1;

		temp1=((10*p)+(3*q)%27);
		temp2=((3*p)+(10*q)%27);

		p=temp1;
		q=temp2;

		if(p==0)
			p=' ';
		else
			p=p+'A'-1;

		if(q==0)
			q=' ';
		else
			q=q+'A'-1;

		fprintf(outfile, "%c%c", p, q);
	} while(1);

	printf("\n\nThats all folks...\n");
	
	exit(0);
}
