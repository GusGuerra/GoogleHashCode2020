#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>
using namespace std;

const int MAXN = 112345;

struct book {
	
	int id, score;
	
	book () {}
	book (int _id, int _score) {
		id = _id;
		score = _score;
	}
	
};

struct library {
	
	public:
		int id;
		int bundle = -1;
		int bookAmount, buildingTime, booksPerDay;
		int done = 0;
		int totalScore = 0;

		library () {}
		library (int nb, int nd, int bpd) {
			bookAmount = nb;
			buildingTime = nd;
			booksPerDay = bpd;
		}
		double getSpeed(){
			double sp = (double)totalScore/(double)bookAmount;
			sp *= (double)booksPerDay;
			return sp;
		}
	
};

class libCmp {
	
	public:
		bool operator () (library a, library b){

			double AGrade = (a.getSpeed()*a.getSpeed())*(1.0/(double)(a.buildingTime));
			double BGrade = (b.getSpeed()*b.getSpeed())*(1.0/(double)(b.buildingTime));
			double bonusA = (1.0/(double)(a.buildingTime));
			double bonusB = (1.0/(double)(b.buildingTime));

			AGrade += bonusA*bonusA;
			BGrade += bonusB*bonusB;

			// A comes before B if AGrade > BGrade
			// Graph can be seen on GeoGebra: z = (1/y)*x^2 + (1/y)^2
			return (AGrade > BGrade);
		}
};

int B, L, D;
int bundleCnt = 0;
pair<bool, int> vis[MAXN];
book livro[MAXN];
vector<library> lib;
priority_queue <library, vector<library>, libCmp> box[MAXN];
vector<int> freeDay;
vector<int> libContent[MAXN];

bool bookCmp(int i, int j){
	// sort books by score
	return (livro[i].score > livro[j].score);
}

void bookSolve(){
	
	for(int i = 0 ; i < L ; i ++)
		sort(libContent[i].begin(), libContent[i].end(), bookCmp);
	
	return;
}

void merge(int i, int j){
	
	if(lib[i].bundle < 0 && lib[j].bundle >= 0){
		lib[i].bundle = lib[j].bundle;
		return;
	}
	
	if(lib[j].bundle < 0 && lib[i].bundle >= 0){
		lib[j].bundle = lib[i].bundle;
		return;
	}
	
	if(lib[j].bundle >= 0 && lib[i].bundle >= 0){
		return;
	}
	
	lib[i].bundle = lib[j].bundle = bundleCnt++;
	
	return;
}

void libSolve(){
	
	for(int i = 0 ; i < L ; i ++) // sorting by index so I can compare using two pointers
		sort(libContent[i].begin(), libContent[i].end());
	
	for(int i = 0 ; i < L ; i ++){
		for(int j = i + 1 ; j < L ; j ++){
			
			int ip = 0, jp = 0;
			int cnt = 0;
			
			// counting how many books libraries i and j have in common
			while(ip != lib[i].bookAmount && jp != lib[j].bookAmount){
				if(libContent[i][ip] == libContent[j][jp]){
					cnt ++;
					ip ++;
					jp ++;
				} else if(libContent[i][ip] > libContent[j][jp]){
					jp++;
				} else {
					ip++;
				}
			}
			
			// merging condition will change
			if((cnt == lib[i].bookAmount) || (cnt == lib[j].bookAmount)){
				merge(i, j);
			}
		}
	}
	
	for(int i = 0 ; i < L ; i ++){
		int& belong = lib[i].bundle;
		if(belong < 0) belong = bundleCnt++;
		box[belong].push(lib[i]);
	}
	
	return;
}

void computeAns(){
	
	vector<bool> seenBundle (bundleCnt, false);
	int seenCnt = 0;
	int currentDay = 0;
	
	for(int i = 0 ; ; i ++){
		
		if(currentDay >= D) return; // deadline
		int index = (i % bundleCnt);
		
		if(seenBundle[index]) continue;
		if(box[index].empty()){
			seenBundle[index] = true;
			seenCnt++;
		}
		
		if(seenCnt == bundleCnt) return;
		
		// Solving for first library in this group
		// compute its answer until deadline, then discard
		library currLib = box[index].top();
		box[index].pop();
		
		int available = currentDay + currLib.buildingTime;
		currentDay = available;
		
		for(int j = 0 ; j < (D - available) ; j ++){
			
			int bookLimit = currLib.booksPerDay * (D - available);
			for(int k = 0 ; k < min(bookLimit, currLib.bookAmount) ; k ++){
				
				int bookToScan = libContent[currLib.id][k];
				if(vis[bookToScan].first) continue;
				
				vis[bookToScan] = {true, currLib.id};
				lib[currLib.id].done++;
			}
		}
	}
	
	return;
}

void printAns(){
	
	vector<int> credits[L];
	
	for(int i = 0 ; i < L ; i ++){
		for(int j = 0 ; j < lib[i].bookAmount ; j ++){
			if(vis[libContent[lib[i].id][j]].second == lib[i].id){
				credits[lib[i].id].push_back(libContent[lib[i].id][j]);
			}
		}
	}
	
	int bibCnt = ansOrder.size();
	for(int i = 0 ; i < ansOrder.size() ; i ++)
		if(!credits[ansOrder[i]].size()) --bibCnt;
	
	cout << bibCnt << endl;
	
	for(int i = 0 ; i < ansOrder.size() ; i ++){
		
		int ansLib = ansOrder[i];
		if(!credits[ansLib].size()) continue;
		cout << ansLib << " " << credits[ansLib].size() << endl;
		
		for(int j = 0 ; j < credits[ansLib].size() ; j ++){
			cout << credits[ansLib][j] << " ";
		}
		cout << endl;
	}
	
	return;
}

void readInput(){
	
	cin >> B >> L >> D;
	
	lib.resize(L);
	freeDay.resize(L);
	
	for(int i = 0 ; i < B ; i ++) {
		cin >> livro[i].score;
		livro[i].id = i;
	}
	
	for(int i = 0; i < L; i++) {
		
		cin >> lib[i].bookAmount >> lib[i].buildingTime >> lib[i].booksPerDay;
		lib[i].id = i;
		
		int inputBook;
		for(int j = 0;  j < lib[i].bookAmount; j++){
			cin >> inputBook;
			libContent[i].push_back(inputBook);
		}
	}
	
	return;
}

void setup(){
	for(int i = 0 ; i < MAXN ; i ++)
		vis[i].second = -1;
	return;
}

int main() {
	
	setup();
	readInput();
	
	libSolve();
	// group libraries into similar bundles
	// so I can treat them as completely distinct
	
	bookSolve();
	// for now, only sort the books by descending score
	
	computeAns();
	printAns();
	
	return 0;
}
