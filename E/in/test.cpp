#include<bits/stdc++.h>
using namespace std;

struct info{
    int weight;
    int parent;
};
struct item{
    int value;
    int weight;
};


vector<vector<int>>dp;
vector<item>elements, temp;
double theta;
set<pair<int, int>>s;
int n, w; 
int opt(int i, int v)
{
    
    if(v <= 0) return 0;
    if(i == 0) return 1e9;
    if(dp[i][v] != 1e9) return dp[i][v];
    int without =  opt(i-1, v);
    int with = opt(i-1, v-elements[i].value) + elements[i].weight;
    return dp[i][v] = min(with, without);
}
int max_elem, tot_value, original_answer;

string format_float(double x)
{
    string s = to_string(x);
    if(s.find('.') != string::npos){
        while(!s.empty() && s.back() == '0') 
            s.pop_back();        
        if(s.back() == '.') s += '0';
    }
    return s;
}


bool report(double epsilon)
{
    
    theta = epsilon * max_elem / 2 / n;
    // cout << "Rounded Instance with Eps: " << fixed << format_float(epsilon) << "\n";
    // cout << "Theta: " << fixed << setprecision(16) <<  theta << "\n";
    tot_value = 0;
    for(int i = 1; i <= n; i++) {
        double x = elements[i].value/theta;
        elements[i].value = (int)(x);
        elements[i].value++;
        tot_value += elements[i].value;
    }
    // for(auto x : elements) cout << "(" << x.value << ", " << x.weight << ")\n";
    for(auto &x : dp) x.clear();
    dp.clear();
    dp.resize(n + 1);
    for(int i = 0; i <= n; i++) dp[i].resize(tot_value + 1, 1e9);    
    for(int i = 0; i <= n; i++) dp[i][0] = 0;
    // for(int i = 0; i <= n; i++){
    //     for(int j = 0; j <= tot_value; j++){
    //         cout << dp[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    for(int i = 1; i <= n; i++){
        for(int v = 1; v <= tot_value; v++){
            dp[i][v] = dp[i-1][v];
            if(v-elements[i].value >= 0){
                dp[i][v] = min(dp[i][v], dp[i-1][v-elements[i].value] + elements[i].weight);
            }
        }
    }
    int ans = 0, idx = 0;
    for(int v = tot_value; v >= 0; v--){
        if(dp[n][v] <= w){
            ans = v;
            idx = v;            
            break;
        }
    }
    // cout << "Answer of reduced instance: " << ans << "\n";
    double achieved_answer = ans * theta;
    // cout << "Answer of reduced instance multiplied by theta: " 
    //      << fixed << setprecision(12) << achieved_answer << "\n";
    // cout << "Indices: ";
    int rounded_up_answer = 0, weight = 0;
    for(int i = n; i >= 1; i--){
        int x = dp[i-1][idx];
        int y = 1e9;
        if(idx - elements[i].value >= 0)  y = dp[i-1][idx - elements[i].value];
        if(y + elements[i].weight == dp[i][idx]){
            // cout << i << " ";
            idx -= elements[i].value;
            rounded_up_answer += temp[i].value;
            weight += temp[i].weight;
        }
    }
    // cout << "\nAnswer of original instance (rounded up): " << rounded_up_answer << "\n";
    // cout << "Used Weight: " << weight << "\n";
    double ratio = 1.0 * original_answer/rounded_up_answer;
    // cout << "Ratio: ";
    // if(ratio == 1.0) 
    //     cout << "1.0\n\n";
    // else
    //     cout << fixed << setprecision(16) << ratio << "\n\n";
    cout << format_float(epsilon) << "->" << ratio << ", ";
    return ratio > (1 + 0.7*epsilon);
}

void original_instance()
{
    dp.resize(n + 1);
    for(int i = 0; i <= n; i++) dp[i].resize(tot_value + 1, 1e9);
    for(int i = 0; i <= n; i++) dp[i][0] = 0;
    for(int i = 1; i <= n; i++){
        for(int v = 1; v <= tot_value; v++){
            dp[i][v] = dp[i-1][v];
            if(v-elements[i].value >= 0){
                dp[i][v] = min(dp[i][v], dp[i-1][v-elements[i].value] + elements[i].weight);
            }
        }
    }
    int ans = 0, idx = 0;
    for(int v = tot_value; v >= 0; v--){
        if(dp[n][v] <= w){
            ans = v;
            idx = v;            
            break;
        }
    }
    // cout << "Original Instance: \nAnswer: " << ans << "\n";
    int weight = 0;
    vector<int>indices;
    for(int i = n; i >= 1; i--){
        int x = dp[i-1][idx];
        int y = 1e9;
        if(idx - elements[i].value >= 0)  y = dp[i-1][idx - elements[i].value];
        if(y + elements[i].weight == dp[i][idx]){            
            indices.push_back(i);
            idx -= elements[i].value;
            weight += elements[i].weight;
        }
    }
    // cout << "Used Weight: " << weight << "\n";
    // cout << "Indices: ";
    // for(auto x : indices) cout << x << " ";
    original_answer = ans;
    // cout << "\n\n";
}
typedef long long ll;
int main()
{   
    // 10^4932    
    long double n; cin >> n;
    long double t = 1;
    // for(int i = 0; i < n; i++) t *= 10;
    t = n;
    cout << "t = " << t << "\n";
    long double p = -1,l = 1, r = t;
    ll term = 4;
    // cin >> term;
    term *= 40;
    for(int i = 0; i < 30000; i++){
        long double mid = (l + r) / 2;
        double x = ceil(mid / t * term);
        if(x > term-1){
            r = mid;
            p = mid;
        }else{
            l = mid; 
        }
    } 
    cout << fixed << setprecision(20) << "p = " << p << "\n";
    long double d = t / (p + 1);
    cout << fixed << setprecision(20) << d << "\n";




    return 0;
}