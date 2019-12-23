#include <stdio.h>
#include <stdlib.h>

void GreedyColoring();

typedef struct interval
{
	int x, y, color, name, degree;
} Interval;

typedef struct node
{
	Interval* inter;
	struct node* next;
} Node;


Node** getIntervals(int k)
{
	int i;
	Node** arr = NULL;
	Interval* inter = NULL;
	Node* node = NULL;

	arr = (Node**)malloc(sizeof(Node*) * k);

	printf("You will now be asked to insert a family of %d intervals:\n", k);
	for (i = 0; i < k; i++)
	{
		inter = (Interval*)malloc(sizeof(Interval));
		node = (Node*)malloc(sizeof(Node));
		node->inter = inter;
		node->next = NULL;

		printf("%dth Interval: ", i + 1);
		scanf_s("%d %d", &inter->x, &inter->y);
		inter->color = 0;
		inter->degree = 0;
		arr[i] = node;
	}
	return arr;
}

void printIntervals(Node** arr, int k)
{
	int i;
	Interval* current = NULL;

	printf("The Intervals family is:\n");
	current = arr[0]->inter;
	printf("[%d,%d]" , current->x, current->y);
	for (i = 1; i < k; i++)
	{
		current = arr[i]->inter;
		printf(",[%d,%d]" , current->x, current->y);
	}

}

void swap(Node* a, Node* b)
{
	Node t = *a;
	*a = *b;
	*b = t;
}

int partition(Node** arr, int low, int high)
{
	int i, j;
	Node* pivot = NULL;
	
	pivot = arr[high];
	i = (low - 1);

	for (j = low; j <= high - 1; j++)
	{
		if (arr[j]->inter->x < pivot->inter->x)
		{
			i++;
			swap(arr[i], arr[j]);
		}
	}
	swap(arr[i + 1], arr[high]);
	return (i + 1);
}

void quickSort(Node** arr, int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, low, high);
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

void nameIntervals(Node** arr, int k)
{
	int i;
	
	for (i = 0; i < k; i++)
	{
		arr[i]->inter->name = i + 1;
	}
}

void createNeighbors(Node** arr, int k)
{
	int i, j;
	Interval* current = NULL;
	Interval* temp = NULL;
	Interval* inter = NULL;
	Node* node = NULL;

	for (i = 0; i < k; i++)
	{
		current = arr[i]->inter;

		for (j = 0; j < k; j++)
		{
			if (i != j)
			{
				temp = arr[j]->inter;
				if ((temp->y > current->x) && (current->y > temp->x))
				{
					node = (Node*)malloc(sizeof(Node));
					inter = (Interval*)malloc(sizeof(Interval));
					inter->x = temp->x;
					inter->y = temp->y;
					inter->name = temp->name;
					inter->color = temp->color;
					node->inter = inter;
					node->next = arr[i]->next;
					arr[i]->next = node;
					current->degree++;
				}
			}
		}
	}
}

void colorIntervals(Node** arr, int k)
{
	int i, j;
	Interval* inter = NULL;
	Node* node = NULL;
	int* used_color = NULL;

	node = arr[0];
	inter = node->inter;
	inter->color = 1;
	for (i = 1; i < k; i++)
	{
		node = arr[i];
		used_color = (int*)calloc(sizeof(int), node->inter->degree + 1);
		node = node->next;
		while (node != NULL)
		{

			inter = node->inter;
			if (arr[inter->name - 1]->inter->color != 0)
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
}

int printInfo(Node** arr, int k)
{
	int i;
	int maxDeg = 0, minDeg = k, maxColor = 0, sumDeg = 0;
	Node* node = NULL;
	Interval* inter = NULL;

	for (i = 0; i < k; i++)
	{
		node = arr[i];
		inter = node->inter;
		sumDeg += inter->degree;
		maxDeg = (maxDeg > inter->degree) ? maxDeg : inter->degree;
		minDeg = (minDeg < inter->degree) ? minDeg : inter->degree;
		maxColor = (maxColor > inter->color) ? maxColor : inter->color;
	}

	printf("\nG Edges = %d\n", sumDeg / 2);
	printf("Maximum Degree of G = %d\n", maxDeg);
	printf("Minumum Degree of G = %d\n", minDeg);
	printf("Chromatic Number of G = %d\n", maxColor);
	printf("G's Complement Edges = %d\n", ((k * (k - 1)) / 2) - (sumDeg / 2));
	printf("Maximum Degree of G's Complement = %d\n", k - minDeg - 1);
	printf("Minumum Degree of G's Complement = %d\n", k - maxDeg - 1);

	return maxColor;
}

void printOptionalColoring(Node** arr, int k, int maxColor)
{
	int i, j, flag = 0;
	Interval* inter = NULL;
	int** optColor = (int**)malloc(sizeof(int*) * maxColor);
	for (i = 0; i < maxColor; i++)
	{
		optColor[i] = (int*)malloc(sizeof(int) * k);
	}

	for (i = 0; i < k; i++)
	{
		inter = arr[i]->inter;
		optColor[inter->color - 1][inter->name - 1] = 1;
	}

	printf("Optional Coloring: ");
	for (i = 0; i < maxColor; i++)
	{
		printf(i == 0 ? "{" : ", {");
		for (j = 0; j < k; j++)
		{
			if (optColor[i][j] == 1)
			{
				printf(flag == 0 ? "[%d,%d]" : ",[%d,%d]", arr[j]->inter->x, arr[j]->inter->y);
				flag = 1;
			}
		}
		flag = 0;
		printf("} = %d", i + 1);
		free(optColor[i]);
	}
	free(optColor);

}

void freeMemory(Node** arr, int k)
{
	int i;
	Node* node = NULL;
	Node* temp = NULL;

	for (i = 0; i < k; i++)
	{
		node = arr[i];
		while (node != NULL)
		{
			temp = node;
			free(node->inter);
			node = node->next;
			free(temp);
		}
	}

	free(arr);
}

void printAdjacencyList(Node** arr, int k)
{
	int i;
	Node* node = NULL;
	Interval* inter = NULL;

	for (i = 0; i < k; i++)
	{
		node = arr[i];
		inter = node->inter;

		printf("\n%d [%d,%d] color = %d", inter->name, inter->x, inter->y, inter->color);
		node = node->next;
		while (node != NULL)
		{
			inter = node->inter;
			printf(" ---> %d [%d,%d]", inter->name, inter->x, inter->y);
			node = node->next;
		}
	}
}

void main()
{
	GreedyColoring();
}


void GreedyColoring()
{
	int k, maxColor;
	Node** arr = NULL;
	printf("Please input k\n");
	scanf_s("%d", &k);


	arr = getIntervals(k);
	printIntervals(arr, k);
	quickSort(arr, 0, k - 1);
	nameIntervals(arr, k);
	createNeighbors(arr, k);
	colorIntervals(arr, k);
	maxColor = printInfo(arr, k);
	printOptionalColoring(arr, k, maxColor);
	printf("\n");
	//printAdjacencyList(arr, k);
	freeMemory(arr, k);
}