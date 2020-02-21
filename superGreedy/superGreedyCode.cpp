#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

const int MAXN = 112345;

struct  book {
	
	int id, score;
	
	book () {}
	book (int _id, int _score) {
		id = _id;
		score = _score;
	}
	
};

struct library {
	
	int id;
	int n_books, n_days, total_day;
	int feito = 0;
	library () {}
	library (int nb, int nd, int bpd) {
		n_books = nb;
		n_days = nd;
		total_day = bpd;
	}
	
	bool operator < (library& other) const {
		return (n_days < other.n_days);
	}
	
};

pair<bool, int> vis[MAXN];
book livro[MAXN];
vector<library> lib;
vector<int> freeDay;
vector<int> conteudoLib[MAXN];

int B, L, D;

bool bookCmp(int& i, int& j){
	return (livro[i].score > livro[j].score);
}

void solve(){
	
	for(int i = 0 ; i < L ; i ++)
		sort(conteudoLib[i].begin(), conteudoLib[i].end(), bookCmp);
	
	sort(lib.begin(), lib.end());
	
	int currDay = 0;
	for(int i = 0 ; i < L ; i ++){
		freeDay[i] = currDay + lib[i].n_days;
		currDay = freeDay[i];
	}
	
	for(int i = 0 ; i < D ; i ++){
		
		for(int j = 0 ; j < L ; j ++){
			
			if(i >= freeDay[j]){
				
				int cnt = 0;
				
				for(int k = lib[j].feito ; k < lib[j].n_books ; k ++){
					if(vis[conteudoLib[lib[j].id][k]].first){
						continue;
					}
					cnt++;
					vis[conteudoLib[lib[j].id][k]].first = true;
					vis[conteudoLib[lib[j].id][k]].second = lib[j].id;
					if(cnt == lib[j].total_day) break;
				}
				
				lib[j].feito += cnt;
			}
		}
	}
	
	return;
}

void print_ans(){
	
	int bibCount = 0;
	
	vector<int> credits[L];
	
	for(int i = 0 ; i < L ; i ++){
		if(lib[i].feito != 0) bibCount++;
		for(int j = 0 ; j < lib[i].n_books ; j ++){
			if(vis[conteudoLib[lib[i].id][j]].second == lib[i].id){
				credits[lib[i].id].push_back(conteudoLib[lib[i].id][j]);
			}
		}
	}
	
	cout << bibCount << endl;
	for(int i = 0 ; i < L ; i ++){
		if(!credits[i].size()) continue;
		cout << i << " " << credits[i].size() << endl;
		for(int j = 0 ; j < credits[i].size() ; j ++){
			cout << credits[i][j] << " ";
		}
		cout << endl;
	}
	
	return;
}

int main() {
	
	for(int i = 0 ; i < MAXN ; i ++)
		vis[i].second = -1;
	
	cin >> B >> L >> D;
	
	lib.resize(L);
	freeDay.resize(L);
	
	for(int i = 0 ; i < B ; i ++) {
		cin >> livro[i].score;
		livro[i].id = i;
	}

	for(int i = 0; i < L; i++) {
		
		cin >> lib[i].n_books >> lib[i].n_days >> lib[i].total_day;
		lib[i].id = i;
		
		int inputBook;
		for(int j = 0;  j < lib[i].n_books; j++){
			cin >> inputBook;
			conteudoLib[i].push_back(inputBook);
		}
		
	}
	
	solve();
	print_ans();
	
	return 0;
}
