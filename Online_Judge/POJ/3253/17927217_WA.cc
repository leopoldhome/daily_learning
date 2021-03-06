#include <stdio.h>
#include <algorithm>
#include <functional>
#include <queue>
using namespace std;
int main() {
	int n;
	while (scanf("%d", &n) != EOF) {
		int *length = new int[n];
		for (int i = 0; i < n; ++i)
			scanf("%d", &length[i]);
		long long cost = 0;
		priority_queue<int, vector<int>, greater<int> > pq;
		for (int i = 0; i < n; ++i)
			pq.push(length[i]);
		while (pq.size() > 1) {
			int top1 = pq.top();
			pq.pop();
			int top2 = pq.top();
			pq.pop();
			cost += top1 + top2;
			pq.push(top1 + top2);
		}
		printf("%d", cost);
	}
	return 0;
}