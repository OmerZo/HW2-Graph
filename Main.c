#include <stdio.h>
#include <stdlib.h>

void GreedyColoring();

typedef struct interval
{
	int x, y, color, name, degree;
} Interval;

//typedef struct vertex
//{
//	int x, y;
//} Vertex;

typedef struct node
{
	Interval* inter;
	struct node* next;
} Node;









void main()
{
	GreedyColoring();
}



void GreedyColoring()
{
	int k, i, j;
	printf("Please input k\n");
	scanf_s("%d", &k);
	Node** arr = (Node**)malloc(sizeof(Node*) * k);


	printf("You will now be asked to insert a family of %d intervals:\n", k);
	for (i = 0; i < k; i++)
	{
		Interval* inter = (Interval*)malloc(sizeof(Interval));
		Node* node = (Node*)malloc(sizeof(Node));
		node->inter = inter;
		node->next = NULL;

		printf("%dth Interval: ", i + 1);
		scanf_s("%d %d", &inter->x, &inter->y);
		inter->color = 0;
		inter->degree = 0;
		inter->name = i + 1;
		arr[i] = node;
	}

	printf("The Intervals family is:\n");
	Interval* current = arr[0]->inter;
	Interval* temp = NULL;
	printf("%d[%d,%d]", current->name, current->x, current->y);

	for (j = 1; j < k; j++)
	{
		temp = arr[j]->inter;
		if (temp->y > current->x&& current->y > temp->x)
		{
			Interval* inter = (Interval*)malloc(sizeof(Interval));
			Node* node = (Node*)malloc(sizeof(Node));
			inter->x = temp->x;
			inter->y = temp->y;
			inter->name = temp->name;
			inter->color = 0;
			node->inter = inter;
			node->next = arr[0]->next;
			arr[0]->next = node;
			current->degree++;
		}
	}

	for (i = 1; i < k; i++)
	{
		current = arr[i]->inter;
		printf(" ,%d[%d,%d]", current->name, current->x, current->y);
		for (j = 0; j < k; j++)
		{
			if (j != i)
			{
				temp = arr[j]->inter;
				if (temp->y > current->x&& current->y > temp->x)
				{
					Interval* inter = (Interval*)malloc(sizeof(Interval));
					Node* node = (Node*)malloc(sizeof(Node));
					inter->x = temp->x;
					inter->y = temp->y;
					inter->name = temp->name;
					inter->color = 0;
					node->inter = inter;
					node->next = arr[i]->next;
					arr[i]->next = node;
					current->degree++;
				}
			}
		}
	}
	printf("\n\n");

	//print arr of lists
	Node* node = NULL;
	Interval* inter = NULL;
	for (i = 0; i < k; i++)
	{
		node = arr[i];
		inter = node->inter;
		printf("%d [%d,%d]", inter->name, inter->x, inter->y);
		node = node->next;
		while (node != NULL)
		{
			inter = node->inter;
			printf(" --> %d [%d,%d]", inter->name, inter->x, inter->y);
			node = node->next;
		}
		printf("\n");
	}


	node = arr[0];
	inter = node->inter;
	int* used_color = NULL;
	inter->color = 1;
	for (i = 1; i < k; i++)
	{
		node = arr[i];
		used_color = (int*)calloc(sizeof(int), node->inter->degree + 1);
		node = node->next;
		while (node != NULL)
		{

			inter = node->inter;
			if(arr[inter->name - 1]->inter->color != 0)
				used_color[arr[inter->name - 1]->inter->color - 1] = 1;
			node = node->next;
		}


		for (j = 0; j < arr[i]->inter->degree + 1; j++)
		{
			if (used_color[j] == 0)
			{
				arr[i]->inter->color = j + 1;
				break;
			}
		}
		free(used_color);
	}



	//print
	node = NULL;
	inter = NULL;
	int maxDeg = 0, minDeg = k, maxColor = 0, sumDeg = 0;
	for (i = 0; i < k; i++)
	{
		node = arr[i];
		inter = node->inter;
		printf("\n%d [%d,%d] color = %d", inter->name, inter->x, inter->y, inter->color);
		sumDeg += inter->degree;
		maxDeg = (maxDeg > inter->degree) ? maxDeg : inter->degree;
		minDeg = (minDeg < inter->degree) ? minDeg : inter->degree;
		maxColor = (maxColor > inter->color) ? maxColor : inter->color;
		node = node->next;
		//while (node != NULL)
		//{
		//	inter = node->inter;
		//	printf(" --> %d [%d,%d]", inter->name, inter->x, inter->y);
		//	node = node->next;
		//}
	}
	printf("\nG Edges = %d\n", sumDeg / 2);
	printf("Maximum Degree of G = %d\n", maxDeg);
	printf("Minumum Degree of G = %d\n", minDeg);
	printf("Chromatic Number of G = %d\n", maxColor);
	printf("G's Comlement Edges = %d\n", ((k* (k - 1)) / 2) - (sumDeg / 2));
	printf("Minimum Degree of G's Comlement = %d\n", k - minDeg - 1);
	printf("Minumum Degree of G's Comlement = %d\n", k - maxDeg - 1);

}