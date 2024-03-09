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


void report(double epsilon)
{
    
    theta = epsilon * max_elem / 2 / n;
    cout << "Rounded Instance with Eps: " << fixed << format_float(epsilon) << "\n";
    cout << "Theta: " << fixed << setprecision(16) <<  theta << "\n";
    std::cout.unsetf(std::ios::fixed);
    std::cout.precision(std::cout.precision());
    tot_value = 0;
    for(int i = 1; i <= n; i++) {
        // cout << elements[i].value << " ";
        double x = ceil(elements[i].value/theta);
        elements[i].value = x + (elements[i].value == max_elem);
        tot_value += elements[i].value;
        // cout << elements[i].value << "\n";
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
    cout << "Answer of reduced instance: " << ans << "\n";
    double achieved_answer = ans * theta;
    cout << "Answer of reduced instance multiplied by theta: " 
         << fixed << setprecision(12) << achieved_answer << "\n";
    cout << "Indices: ";
    int rounded_up_answer = 0, weight = 0;
    for(int i = n; i >= 1; i--){
        int x = dp[i-1][idx];
        int y = 1e9;
        if(idx - elements[i].value >= 0)  y = dp[i-1][idx - elements[i].value];
        if(y + elements[i].weight == dp[i][idx]){
            cout << i << " ";
            idx -= elements[i].value;
            rounded_up_answer += temp[i].value;
            weight += temp[i].weight;
        }
    }
    cout << "\nAnswer of original instance (rounded up): " << rounded_up_answer << "\n";
    cout << "Used Weight: " << weight << "\n";
    double ratio = 1.0 * original_answer/rounded_up_answer;
    cout << "Ratio: ";
    if(ratio == 1.0) 
        cout << "1.0\n\n";
    else
        cout << fixed << setprecision(16) << ratio << "\n\n";
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
    cout << "Original Instance: \nAnswer: " << ans << "\n";
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
    cout << "Used Weight: " << weight << "\n";
    cout << "Indices: ";
    for(auto x : indices) cout << x << " ";
    original_answer = ans;
    cout << "\n\n";
}

int main()
{
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    info dummy = {INT_MAX, -1};
    cin >> n>> w;
    elements.resize(n + 1);
    tot_value = max_elem = 0;
    for(int i = 1; i <= n; i++) {
        cin >> elements[i].value >> elements[i].weight;
        tot_value += elements[i].value;
        max_elem = max(max_elem, elements[i].value);
    }
    original_instance();
    temp = elements;
    report(0.5);
    elements = temp;
    report(0.2);
    elements = temp;
    report(0.1);
    elements = temp;
    report(0.05);
    // ratio = original/achiever_after_reduction (1 + 0.7epsilon)
    // 0.5 -> 1.35
    // 0.2 -> 1.14
    // 0.1 -> 1.07
    // 0.05-> 1.035
    return 0;
}