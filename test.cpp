#include <queue>
#include  <utility>
#include  <iostream>


int	main(int argc, char *argv[])
{
	std::pair <int, int> p1(1, 456);
	std::pair <int, int> p2(2, 422);
	std::pair <int, int> p3(3, 874);
	std::pair <int, int> p4(4, 32);

	std::priority_queue <std::pair<int, int> > pq;

	pq.push(p1);
	pq.push(p3);
	pq.push(p4);
	pq.push(p2);

	while (!pq.empty())
	{
		pq[p1].first = 0;
		std::cout << pq.top().first << " " << pq.top().second << std::endl;

		pq.pop();
	}
}
