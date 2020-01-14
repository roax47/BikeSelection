#include<stdlib.h>     
#include<stdio.h>
class HumanQueue
{
private:
	int size;      
	int start;   
	int count; 
	int* que;    

public:
	HumanQueue(int s);
	bool isEmpty();
	void push(int hum);
	int pop();
	~HumanQueue();
};
HumanQueue::HumanQueue(int s)
{
	size = s;
	count = 0;
	start = 0;
	que = (int*)malloc(sizeof(int)*size);
}

bool HumanQueue::isEmpty()
{
	if (count == 0) return true;
	else return false;
}
void HumanQueue::push(int hum)
{
	int position = start + count;
	count++;
	if (position >= size - 1)
	{
		size *= 2;
		que = (int*)realloc(que, size*sizeof(int));
	}
	que[position] = hum;
}
int HumanQueue::pop()
{
	if (!isEmpty())
	{
		start++;
		count--;
		return que[start - 1];
	}
	return NULL;
}
HumanQueue::~HumanQueue()
{
	free(que);
}

bool DFS(int** humans, int* hum_match, int *bike_match, int *hum_dist, int *hum_checked,int x)
{
	hum_checked[x] = 1;
	int temp = 0;
	for (int j = 1; j < humans[x][0] + 1; j++)
	{
		if (bike_match[humans[x][j]] == -1)
		{
			//Skojarzenie roweru z cz³owiekiem
			bike_match[humans[x][j]] = x;
			hum_match[x] = humans[x][j];
			return true;
		}
		if (bike_match[humans[x][j]] != -1)
		{
			temp = bike_match[humans[x][j]];
			//Sprawdzenie czy inny cz³owiek nie by³ sprawdzany i jego odlegosc od obecnego == 1
			if (hum_checked[temp] == 0 && hum_dist[temp] == hum_dist[x] + 1)
			{
				if (DFS(humans, hum_match, bike_match, hum_dist, hum_checked, temp))
				{
					//Skojarzenie roweru z cz³owiekiem
					bike_match[humans[x][j]] = x;
					hum_match[x] = humans[x][j];
					return true;
				}
			}
		}
	}
	return false;
}
void BFS(HumanQueue* que, int** humans, int *bike_match, int *hum_dist)
{
	int current = 0;
	int new_human = 0;
	while (!que->isEmpty())
	{
		current = que->pop();
		for (int i = 1; i < humans[current][0] + 1; i++)
		{
			if (bike_match[humans[current][i]] != -1)
			{
				new_human = bike_match[humans[current][i]];
				//Je¿eli rower jest skojarzony z innym cz³owiekem a jego odleg³osc nie jest okreslona
				// to przypisz mu odleglosc obecnego cz³owieka + 1
				if (hum_dist[new_human] == -1)
				{
					hum_dist[new_human] = hum_dist[current] + 1;
					que->push(new_human);
				}
			}
		}
	}
	delete que;
}
int main()
{
	int size_hum = 0, size_bikes = 0, i = 0,j=1, temp = 0;
	int** humans;
	int* hum_match;
	int *bike_match;
	int *hum_dist;
	int *hum_checked;
	bool end=true;
	HumanQueue* que = nullptr;
	

	scanf("%d %d", &size_hum, &size_bikes);
	humans = new int*[size_hum];
	hum_match= new int[size_hum];
	hum_dist= new int[size_hum];
	hum_checked = new int[size_hum];
	size_bikes++;
	bike_match = new int[size_bikes];

	while (true)
	{
		scanf("%d ", &temp);
		humans[i] = new int[temp + 1];
		humans[i][0] = temp;
		while (j < temp +1)
		{
			scanf("%d", &humans[i][j]);
			j++;
		}
		i++;
		j = 1;
		if (i == size_hum) break;
	}

	for (int z = 0; z < size_hum; z++) hum_match[z] = -1;
	for (int k = 0; k < size_bikes; k++) bike_match[k] = -1;

	// algorytm Hopcrofta-Karpa na podstawie 
	// https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm
	// http://www.rafalnowak.pl/wiki/index.php?title=Algorytm_Hopcrofta-Karpa
	while (end)
	{
		end = false;
		for (int l = 0; l < size_hum; l++) hum_dist[l]= -1;
		que = new HumanQueue(50);
		for (int i = 0; i < size_hum; i++)
		{
			if (hum_match[i] == -1)
			{
				hum_dist[i] = 0;
				que->push(i);
			}
		}
		BFS(que, humans,bike_match,hum_dist);
		for (int z = 0; z < size_hum; z++) hum_checked[z] = 0;
		for (int i = 0; i < size_hum; i++)
		{
			if (hum_match[i] == -1)
			{
				if (DFS(humans, hum_match, bike_match, hum_dist, hum_checked, i)) end=true;
			}
		}
	}
	temp = 0;
	for (int i = 0; i < size_hum; i++)
	{
		if (hum_match[i] > 0) temp++;
	}
	printf("%d", temp);
	for (int i = 0; i < size_hum; i++) delete humans[i];
	delete humans;
	delete hum_match;
	delete bike_match;
	delete hum_dist;
	delete hum_checked;
    return 0;
}

