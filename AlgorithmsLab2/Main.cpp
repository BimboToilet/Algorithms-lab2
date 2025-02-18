#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <memory>
#include <random>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
struct Node {
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;
	int val;
	int mod;

	Node(int val): left(), right(), val(val), mod(0) { }

	Node(std::shared_ptr<Node> l, std::shared_ptr<Node> r)
	{	
		left = l;
		right = r;
		mod = 0;
		val = INT_MIN;
		if (left != nullptr) {
			val = std::max(left->val,val);
		}
		if (right != nullptr) {
			val = std::max(right->val, val);
		}
	}
};
std::shared_ptr<Node> modify(std::shared_ptr<Node> root, int lefttree, int righttree, int left, int right, int change) {
	std::shared_ptr<Node> changednode(new Node(*root));
	if (left > right)
		return root;
	if (left == lefttree && righttree == right){
		changednode->mod += change;
		return changednode;
	}
	else {
		int medium = (lefttree + righttree) / 2;
		if (root->left!= nullptr) {
			changednode->left = modify(root->left, lefttree, medium, left, std::min(right, medium), change);
		}
		if (root->right!= nullptr) {
			changednode->right = modify(root->right, medium + 1, righttree, std::max(left, medium + 1), right, change);
		}
		return changednode;
	}
}
std::shared_ptr<Node> buildtree(std::vector<int>& arr, int lefttree, int righttree) {
	if (lefttree == righttree) {
		return std::shared_ptr<Node> (new Node(arr[righttree]));
	}
	int medium = (lefttree + righttree) / 2;
	return std::shared_ptr<Node>  (new Node(buildtree(arr, lefttree, medium),buildtree(arr, medium + 1, righttree)));
}
int findpoint(std::shared_ptr<Node> root, int lefttree, int righttree, int i) {
	int ans = 0;
	if (lefttree == righttree) {
		return root->mod;
	}
	int medium = (lefttree + righttree) / 2;
	if (i > medium) {
		ans+= findpoint(root->right, medium + 1, righttree, i);
	}
	else {
		ans+= findpoint(root->left, lefttree, medium, i);
	}
	ans += root->mod;
	return ans;
}
int algo_enumeration(int x, int y, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& rectangles) {
	int counter = 0;
	for (auto pair : rectangles) {
		if (x >= pair.first.first && x <= pair.second.first && y >= pair.first.second && y <= pair.second.second) {
			counter++;
		}
	}
	return counter;
}
int algo_on_map(int x, int y, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& rectangles) {
	std::vector<int> newx;
	std::vector<int> newy;
	for (auto pair: rectangles) {
		newx.push_back(pair.first.first);
		newx.push_back(pair.second.first);
		newy.push_back(pair.first.second);
		newy.push_back(pair.second.second);

		newx.push_back(pair.second.first+1);
		newy.push_back(pair.second.second + 1);

		newx.push_back(pair.first.first-1);
		newy.push_back(pair.first.second-1);
	}
	std::sort(newx.begin(),newx.end());
	std::sort(newy.begin(), newy.end());
	newx.erase(std::unique(newx.begin(),newx.end()),newx.end());
	newy.erase(std::unique(newy.begin(), newy.end()), newy.end());
	std::vector<std::vector<int>> matrix (newy.size(), std::vector<int> (newx.size(),0));
	for (auto rec:rectangles) {
		int firstX = std::find(newx.begin(), newx.end(), rec.first.first) - newx.begin();
		int firstY = std::find(newy.begin(), newy.end(), rec.first.second) - newy.begin();
		int secondX = std::find(newx.begin(), newx.end(), rec.second.first) - newx.begin();
		int secondY = std::find(newy.begin(), newy.end(), rec.second.second) - newy.begin();
		for (int i = firstX; i < secondX + 1;i++) {
			for (int g = firstY; g < secondY + 1; g++) {
				matrix[g][i]++;
			}
		}
	}
	return matrix[std::min((int)(std::lower_bound(newy.begin(), newy.end(), y) - newy.begin()),(int)(newy.size()-1))][std::min((int)(std::lower_bound(newx.begin(), newx.end(), x) - newx.begin()),(int)(newx.size()-1))];
}
int algo_on_map_zip(int x, int y, std::vector<std::vector<int>>& matrix, std::vector<int>& newx, std::vector<int>& newy) {
	return matrix[x][y];
}
int algo_on_tree_zip(int x, int y, std::vector<int>& newx, std::vector<int>& newy, std::vector<std::shared_ptr<Node>>& trees) {
	return findpoint(trees[x], 0, newy.size() - 1, y);
}
int algo_on_tree(int x, int y, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>>& rectangles) {
	std::vector<int> newx;
	std::vector<int> newy;
	std::vector<std::vector<int>> boundaries;
	for (auto pair : rectangles) {
		newx.push_back(pair.first.first);
		newx.push_back(pair.second.first);
		newy.push_back(pair.first.second);
		newy.push_back(pair.second.second);

		newx.push_back(pair.second.first + 1);
		newy.push_back(pair.second.second + 1);

		newx.push_back(pair.first.first - 1);
		newy.push_back(pair.first.second - 1);
	}
	std::sort(newx.begin(), newx.end());
	std::sort(newy.begin(), newy.end());
	newx.erase(std::unique(newx.begin(), newx.end()), newx.end());
	newy.erase(std::unique(newy.begin(), newy.end()), newy.end());
	for (auto pair: rectangles) {
		int firstX = std::find(newx.begin(), newx.end(), pair.first.first) - newx.begin();
		int firstY = std::find(newy.begin(), newy.end(), pair.first.second) - newy.begin();
		int secondX = std::find(newx.begin(), newx.end(), pair.second.first) - newx.begin();
		int secondY = std::find(newy.begin(), newy.end(), pair.second.second) - newy.begin();
		boundaries.push_back({ 0,firstY,secondY,firstX,0 });
		boundaries.push_back({ 1,firstY,secondY,secondX + 1,0 });
	}
	
	std::vector<std::shared_ptr<Node>> trees(newx.size());
	std::vector<int> start(newy.size(),0);
	trees[0] = buildtree(start,0,start.size() - 1);
	for (int i = 1; i < newx.size(); i++) {
		trees[i] = trees[i - 1];
		int flag = 0;
		for (auto edge : boundaries) {
			if (edge[4] != 1 && i == edge[3] && edge[0] == 0) {
				edge[4] = 1;
				if (flag==0) {
					trees[i] = modify(trees[i - 1], 0, newy.size() - 1, edge[1], edge[2], 1);
					flag = 1;
				}
				else {
					trees[i] = modify(trees[i], 0, newy.size() - 1, edge[1], edge[2], 1);
				}
			}
			if (edge[4] != 1 && i == edge[3] && edge[0] == 1) {
				edge[4] = 1;
				if (flag == 0) {
					trees[i] = modify(trees[i - 1], 0, newy.size() - 1, edge[1], edge[2], -1);
					flag = 1;
				}
				else {
					trees[i] = modify(trees[i], 0, newy.size() - 1, edge[1], edge[2], -1);
				}
			}
		}
	}
	return findpoint(trees[std::min((int)(std::lower_bound(newx.begin(), newx.end(), x) - newx.begin()), (int)(newx.size() - 1))], 0, newy.size() - 1, std::min((int)(std::lower_bound(newy.begin(), newy.end(), y) - newy.begin()), (int)(newy.size() - 1)));
}
int main() {
	srand(time(0));
	std::mt19937 x(0);
	std::mt19937 y(0);
	int n = 1000;
	for (int i = 1; i < n+1;i+=100) {
		std::uniform_int_distribution<int> varx(0, 10 * i);
		std::uniform_int_distribution<int> vary(0, 10 * i);
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> rectangles;
		for (int j = 0; j < i;j++) {
			rectangles.push_back({ {10 * j,10 * j},{10 * (2 * i - j),10 * (2 * i - j)} });
		}
		std::vector<int> newx;
		std::vector<int> newy;
		std::vector<std::vector<int>> boundaries;
		for (auto pair : rectangles) {
			newx.push_back(pair.first.first);
			newx.push_back(pair.second.first);
			newy.push_back(pair.first.second);
			newy.push_back(pair.second.second);

			newx.push_back(pair.second.first + 1);
			newy.push_back(pair.second.second + 1);

			newx.push_back(pair.first.first - 1);
			newy.push_back(pair.first.second - 1);
		}
		std::sort(newx.begin(), newx.end());
		std::sort(newy.begin(), newy.end());
		newx.erase(std::unique(newx.begin(), newx.end()), newx.end());
		newy.erase(std::unique(newy.begin(), newy.end()), newy.end());

		std::vector<std::vector<int>> matrix(newy.size(), std::vector<int>(newx.size(), 0));
		for (auto rec : rectangles) {
			int firstX = std::find(newx.begin(), newx.end(), rec.first.first) - newx.begin();
			int firstY = std::find(newy.begin(), newy.end(), rec.first.second) - newy.begin();
			int secondX = std::find(newx.begin(), newx.end(), rec.second.first) - newx.begin();
			int secondY = std::find(newy.begin(), newy.end(), rec.second.second) - newy.begin();
			boundaries.push_back({ 0,firstY,secondY,firstX,0 });
			boundaries.push_back({ 1,firstY,secondY,secondX + 1,0 });
			for (int i = firstX; i < secondX + 1; i++) {
				for (int g = firstY; g < secondY + 1; g++) {
					matrix[g][i]++;
				}
			}
		}

		std::vector<std::shared_ptr<Node>> trees(newx.size());
		std::vector<int> start(newy.size(), 0);
		trees[0] = buildtree(start, 0, start.size() - 1);
		for (int i = 1; i < newx.size(); i++) {
			trees[i] = trees[i - 1];
			int flag = 0;
			for (auto edge : boundaries) {
				if (edge[4] != 1 && i == edge[3] && edge[0] == 0) {
					edge[4] = 1;
					if (flag == 0) {
						trees[i] = modify(trees[i - 1], 0, newy.size() - 1, edge[1], edge[2], 1);
						flag = 1;
					}
					else {
						trees[i] = modify(trees[i], 0, newy.size() - 1, edge[1], edge[2], 1);
					}
				}
				if (edge[4] != 1 && i == edge[3] && edge[0] == 1) {
					edge[4] = 1;
					if (flag == 0) {
						trees[i] = modify(trees[i - 1], 0, newy.size() - 1, edge[1], edge[2], -1);
						flag = 1;
					}
					else {
						trees[i] = modify(trees[i], 0, newy.size() - 1, edge[1], edge[2], -1);
					}
				}
			}
		}
		long double average1 = 0;
		long double average2 = 0;
		long double average3 = 0;
		for (int t = 0; t < 10; t++) {

			int alx = varx(x);
			int aly = vary(y);

			auto StartTime = Clock::now();
			algo_enumeration(alx, aly, rectangles);
			auto EndTime = Clock::now();
			average1 += std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime).count();


			StartTime = Clock::now();
			int conx = std::min((int)(std::lower_bound(newy.begin(), newy.end(), aly) - newy.begin()), (int)(newy.size() - 1));
			int cony = std::min((int)(std::lower_bound(newx.begin(), newx.end(), alx) - newx.begin()), (int)(newx.size() - 1));
			//algo_on_map(alx, aly, rectangles);
			algo_on_map_zip(conx,cony,matrix,newx,newy);
			EndTime = Clock::now();
			average2 += std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime).count();


			StartTime = Clock::now();
			conx = std::min((int)(std::lower_bound(newx.begin(), newx.end(), alx) - newx.begin()), (int)(newx.size() - 1));
			cony = std::min((int)(std::lower_bound(newy.begin(), newy.end(), aly) - newy.begin()), (int)(newy.size() - 1));
			//algo_on_tree(alx, aly, rectangles);
			algo_on_tree_zip(conx, cony, newx, newy, trees);
			EndTime = Clock::now();
			average3 += std::chrono::duration_cast<std::chrono::microseconds>(EndTime - StartTime).count();
		}
		average1 /= 10;
		average2 /= 10;
		average3 /= 10;
		std::cout << average1 << ' ' << average2 << ' ' << average3 << std::endl;
	}
	return 0;
}