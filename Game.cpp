/*
                                                      O
                                                O    / \    O
                                            O  / \  /   \  / \  O
                                             \/   \/     \/   \/
                                              \_ _ _ _ _ _ _ _/
                                               \_ _ _ _ _ _ _/
                                                 VIVEKISGREAT

                                            Code by Vivek Parashar
                    linkedin  -> vivek parashar (https://www.linkedin.com/in/vivek-parashar-developer/)
*/

#include<bits/stdc++.h>
using namespace std;

#define fi                          first
#define se                          second
#define ll                          long long
#define pb                          push_back
#define po                          pop_back
#define mp                          make_pair
#define mod                         1000000007
#define ull                         unsigned long long
#define vll                         vector<long long>
#define endl                        "\n"
#define test                        int t;  cin>>t; while(t--)
#define vpll                        vector<pair<long long, long long> >
#define all(v)                      v.begin(), v.end()
#define prec(n)                     fixed<<setprecision(n)
#define bits(n)                     __builtin_popcount(n)
#define input(v)                    loop(i, 0, n)  cin>>v[i];
#define print_no                    cout<<"NO"<<endl;
#define print_yes                   cout<<"YES"<<endl;
#define sort_dec(v)                 sort(all(v), greater<int>());
#define sort_inc(v)                 sort(all(v));
#define vivek_is_gr8                ios_base::sync_with_stdio(false) , cin.tie(NULL);
#define loop(i, m, n)               for(int i = m; i<n; i++)
#define loop_dec(i, m, n)           for(int i = m; i>=n; i--)

#define MAP_ROWS 6
#define MAP_COLUMNS 12

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
enum land_type {
    GRASS,
    WATER,
    PATH_START,
    PATH_END,
    PATH_UP,
    PATH_RIGHT,
    PATH_DOWN,
    PATH_LEFT,
    TELEPORTER
};

enum entity {
    EMPTY,
    ENEMY,
    BASIC_TOWER,
    POWER_TOWER,
    FORTIFIED_TOWER,
};

struct tile {
    enum land_type land;
    enum entity entity;

    int n_enemies;
};

/** initilise variables globally so that we dont have to pass them through functions **/

struct tile maps[MAP_ROWS][MAP_COLUMNS];
int lives, money, enemy_start;
pair<int, int> s_point, e_point;
vector<pair<int, int> > path_vec;
map<pair<int, int>, int> tower_index;

////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void initialise_map();
void print_map();
void print_tile(struct tile tile, int entity_print);
void intialise_s_e_point();
void intialise_lake();
bool lake_dim_checker(pair<int, int>, pair<int, int>);
void add_path();
void add_enemy(string);
void add_tower(string);
void move_enemy(string);
void upgrade_tower(string);
void attack_enemy(string);
void rain_tiles(string);


int main(void) {

    #ifndef ONLINE_JUDGE
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif


    initialise_map();

    cin>>lives>>money;

    cin>>s_point.fi>>s_point.se>>e_point.fi>>e_point.se;
    cin>>enemy_start;

    cout<<"Starting Lives: "<<lives<<endl<<"Starting Money($): "<<money<<endl;
    cout<<"Start Point: "<<s_point.fi<<" "<<s_point.se<<endl<<"End Point: "<<e_point.fi<<" "<<e_point.se<<endl;
    cout<<"Initial Enemies: "<<enemy_start<<endl<<endl;

    intialise_s_e_point();
    print_map();


    /** creating lake then print the map **/
    intialise_lake();

    /** adding path to the map **/

    add_path();

    while(true){
        string s;
        getline(cin, s);
        if(s.size() == 1) break;
        if(s[0] == 'e') add_enemy(s);
        if(s[0] == 't') add_tower(s);
        if(s[0] == 'm') move_enemy(s);
        if(s[0] == 'u') upgrade_tower(s);
        if(s[0] == 'a') attack_enemy(s);
        if(s[0] == 'r') rain_tiles(s);
    }

    cout<<"Enter Command: [CTRL+D]"<<endl;
    cout<<"Game Over!"<<endl;

    return 0;
}

/** Adding Rain Tiles on map**/

void rain_tiles(string s){
    vector<int> temp;
    int total = 0;
    loop(i, 2, s.size()){
        if(s[i] == ' '){
            temp.pb(total);
            total = 0;
            continue;
        }

        total *= 10;
        total += (s[i] - '0');
    }

    temp.pb(total);
    total = 0;


}

/** Attacking the enemies **/

void attack_enemy(string s){
    int time = s[2] - '0';
    int count = 0;

    for(auto it : tower_index){
        int a = it.fi.fi, b = it.fi.se, c = it.se, d = (c == 3 ? 2 : 1);

        loop(times, 0, time){
            loop(i, max(a-d, 0), min(12, a+d+1)){
                loop(j, max(b-d, 0), min(12, b+d+1)){
                    loop(k, 0, c){
                        if(maps[i][j].entity == ENEMY){
                            maps[i][j].n_enemies -= 1;
                            count++;

                            if(maps[i][j].n_enemies <= 0){
                                maps[i][j].n_enemies = 0;
                                maps[i][j].entity = EMPTY;
                            }
                        }
                    }
                }
            }
        }
    }

    cout<<count<<" enemies destroyed!"<<endl;

    print_map();
}

/** upgrading the tower**/

void upgrade_tower(string s){
    int row = s[2]-'0', col = s[4]-'0';

    if(row >= 6 || col >= 12) cout<<"Error: Upgrade target is out-of-bounds."<<endl;
    else if((maps[row][col].entity == EMPTY) || (maps[row][col].entity == ENEMY))    cout<<"Error: Upgrade target contains no tower entity."<<endl;
    else if(maps[row][col].entity == FORTIFIED_TOWER) cout<<"Error: Tower cannot be upgraded further."<<endl;
    else if(((maps[row][col].entity == BASIC_TOWER) && (money < 300)) || ((maps[row][col].entity == POWER_TOWER) && (money<500 )))    cout<<"Error: Insufficient Funds."<<endl;
    else{
        if(maps[row][col].entity == BASIC_TOWER){
            maps[row][col].entity = POWER_TOWER;
            money -= 300;
        }else{
            maps[row][col].entity = FORTIFIED_TOWER;
            money -= 500;
        }

        tower_index[{row, col}]++;
        cout<<"Upgrade Successful!"<<endl;
    }

    print_map();
}

/** moving of the enemy is done by this function **/

void move_enemy(string s){
    int move = s[2]-'0';
    int count = 0;

    loop_dec(i, path_vec.size()-1, 0){
        int a1 = path_vec[i].fi;
        int a2 = path_vec[i].se;

        if(maps[a1][a2].entity == ENEMY){
            if((i+move) >= path_vec.size()){
                count += maps[a1][a2].n_enemies;
            }else{
                int a3 = path_vec[i+move].fi;
                int a4 = path_vec[i+move].se;

                maps[a3][a4].entity = ENEMY;

                if(maps[a3][a4].n_enemies > 0) maps[a3][a4].n_enemies += maps[a1][a2].n_enemies;
                else maps[a3][a4].n_enemies = maps[a1][a2].n_enemies;

            }

            maps[a1][a2].entity = EMPTY;
            maps[a1][a2].n_enemies = 0;
        }
    }

    cout<<count<<" enemies reached the end!"<<endl;

    print_map();
}

/** adding tower in the map**/
void add_tower(string s){
    int f = s[2]-'0', sec = s[4]-'0';
    if((money >= 200) && f<=5 && sec <= 11 ){
        if((maps[f][sec].land == GRASS) && (maps[f][sec].entity == EMPTY)){
            tower_index[{f, sec}] = 1;
            maps[f][sec].entity = BASIC_TOWER;
            money -= 200;
            cout<<"Tower successfully created!"<<endl;
        }else{
            cout<<"Error: Tower creation unsuccessful. Make sure you have at least $200 and that the tower is placed on a grass block with no entity."<<endl;
        }
    }else{
        cout<<"Error: Tower creation unsuccessful. Make sure you have at least $200 and that the tower is placed on a grass block with no entity."<<endl;
    }

    print_map();
}

/** adding enemies to the starting point**/

void add_enemy(string s){
    int count = 0;
    loop(i, 2, s.size()){
        count *=10;
        count += (s[i]-'0');
    }

    maps[s_point.fi][s_point.se].entity = ENEMY;
    maps[s_point.fi][s_point.se].n_enemies += count;

    cout<<"ENEMY Added"<<endl;

    print_map();
}

/** adding path to the map**/

void add_path(){
    int ind_i = s_point.fi, ind_j = s_point.se;
    cin.ignore(1,'\n');
    string a;   getline(cin, a);

    loop(i, 0, a.size()){
        if(a[i] == ' ') continue;
        if((a[i] == 'r') || (a[i] == 'l') || (a[i] == 'u') || (a[i] == 'd')){
            path_vec.pb({ind_i, ind_j});
            if(a[i] == 'r')  maps[ind_i][ind_j++].land = PATH_RIGHT;
            else if(a[i] == 'l') maps[ind_i][ind_j--].land = PATH_LEFT;
            else if(a[i] == 'u') maps[ind_i--][ind_j].land = PATH_UP;
            else    maps[ind_i++][ind_j].land = PATH_DOWN;
        }else   break;
    }

    print_map();
}

/** here we are initialise the starting and ending point of the map + giving the no. of enemies at starting position **/

void intialise_s_e_point(){
    maps[s_point.fi][s_point.se].land = PATH_START;
    maps[e_point.fi][e_point.se].land = PATH_END;

    if(enemy_start > 0 && enemy_start <=999){
        maps[s_point.fi][s_point.se].entity = ENEMY;
        maps[s_point.fi][s_point.se].n_enemies = enemy_start;
    }
}


/** creating lake then print the map **/

void intialise_lake(){
    cout<<endl<<endl<<endl;
    pair<int, int> lake_start, lake_dim;
    cin>>lake_start.fi>>lake_start.se>>lake_dim.fi>>lake_dim.se;

    cout<<"Enter Lake: "<<lake_start.fi<<" "<<lake_start.se<<" "<<lake_dim.fi<<" "<<lake_dim.se<<endl;

    if(lake_dim_checker(lake_start, lake_dim)){
        loop(i, lake_start.fi, lake_start.fi+lake_dim.fi){
            loop(j, lake_start.se, lake_start.se+lake_dim.se){
                maps[i][j].land = WATER;
            }
        }

        cout<<"LAKE ADDED"<<endl;
    }else{
        cout<<"Error: Lake out of bounds, ignoring..."<<endl<<endl;
    }

    print_map();
    cout<<endl;
}

bool lake_dim_checker(pair<int, int> start, pair<int, int> dim){
    if(start.fi < 0 || start.se < 0 || dim.fi<0 || dim.se<0) return 0;
    if(((start.fi + dim.fi-1) >= 6) || ((start.se + dim.se-1) >= 12))  return 0;

    if((start.fi <= s_point.fi) && ((start.fi+dim.fi) > s_point.fi) && (start.se <= s_point.se) && ((start.se+dim.se) > s_point.se)) return 0;
    if((start.fi <= e_point.fi) && ((start.fi+dim.fi) > e_point.fi) && (start.se <= e_point.se) && ((start.se+dim.se) > e_point.se)) return 0;

    return 1;
}

/** Initialises map tiles to contain GRASS land and EMPTY entity. **/

void initialise_map() {
    for (int row = 0; row < MAP_ROWS; ++row) {
        for (int col = 0; col < MAP_COLUMNS; ++col) {
            maps[row][col].land = GRASS;
            maps[row][col].entity = EMPTY;
            maps[row][col].n_enemies = 0;
        }
    }
}

void print_map() {
    cout<<"Lives: "<<lives<<"  Money: $"<<money<<endl;
    for (int row = 0; row < MAP_ROWS * 2; ++row) {
        for (int col = 0; col < MAP_COLUMNS; ++col) {
            print_tile(maps[row / 2][col], row % 2);
        }
        cout<<endl;
    }

    cout<<endl;
}

void print_tile(struct tile tile, int land_print) {
    if (land_print) {
        if (tile.land == GRASS) cout<<" . ";
        else if (tile.land == WATER) cout<<" ~ ";
        else if (tile.land == PATH_START) cout<<" S ";
        else if (tile.land == PATH_END) cout<<" E ";
        else if (tile.land == PATH_UP) cout<<" ^ ";
        else if (tile.land == PATH_RIGHT) cout<<" > ";
        else if (tile.land == PATH_DOWN) cout<<" v ";
        else if (tile.land == PATH_LEFT) cout<<" < ";
        else if (tile.land == TELEPORTER) cout<<"( )";
        else cout<<" ? ";
    } else {
        if (tile.entity == EMPTY)cout<<"   ";
        else if (tile.entity == ENEMY) cout<<setw(3) << setfill('0')<<tile.n_enemies;
        else if (tile.entity == BASIC_TOWER) cout<<"[B]";
        else if (tile.entity == POWER_TOWER)cout<<"[P]";
        else if (tile.entity == FORTIFIED_TOWER) cout<<"[F]";
        else cout<<" ? ";
    }
}

