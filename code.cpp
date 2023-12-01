#include <iostream>
#include <fstream>
#include <chrono> //计算运行时间需要的。
#include <vector>  
#include <cmath>
#include <iomanip>
using namespace std;
using namespace chrono;

struct point
{
    int x, y, z;
}; //结构体，用于存储三维空间中一个点的坐标。

double distance(point a, point b){ //函数 distance 计算两个点 a 和 b 之间的欧氏距离。
    return sqrt(double((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z))); 
}
//函数 quickSort 实现了快速排序算法，将数组 a 按照点的 x 值进行排序。
void quickSort(vector<point>& a, int l, int r){
	int p = a[(l+r)/2].x;
	int i = l, j = r;
	while (i < j){
		while (a[i].x < p){
			i++;
		}
		while (a[j].x > p){
			j--;
		}
		if (i <= j){
			swap(a[i],a[j]);
			i++;
			j--;
		}
	}
	if (i < r){
		quickSort(a, i, r);
	}
	if (l < j){
		quickSort(a, l, j);
	}
}

//找最短距离，使用分治法和递归。
double shortest(vector<point> a){
    int size = a.size();
    //如果某一数组只有小于等于9个点，进行计算它们之间的距离。
    if (size <= 9){ 
        double min_dist = INFINITY;
        for (int i = 0; i < a.size(); i++){
            for (int j = i+1; j < a.size(); j++){
                double dis = distance(a[i], a[j]);
                if(dis < min_dist){
                    return min_dist; //取该数组里面的点之间的最短距离。
                }
            }
        }
    }

    //分治法，把当前的部分分成其他两个更小的部分 （直到每一部分的点的个数小于等于三）。
    int middle = size/2;
    vector<point> left_part(a.begin(),a.begin()+middle);
    vector<point> right_part(a.begin()+middle, a.end());
    //函数递归调用自身，在每个子数组上寻找最短距离，然后比较两个最短距离。
    double d_min_left = shortest(left_part); 
    double d_min_right = shortest(right_part);
    double d_min = min(d_min_left,d_min_right);

    //在靠近两个数组的分割点的区域中的点，寻找所有两点有x值的差距小于d_min，因为它们之间的距离有可能小于d_min，即最短距离。
    int middle_x = a[middle].x;
    vector<point> cut_area_points;
    for (int i = middle - 25; i < middle + 25; i++){ 
        if (i >= 0 && i < size){
            double x_dis = abs(a[i].x - middle_x);
            if(x_dis < d_min){
                cut_area_points.push_back(a[i]);
            }
        }
    }
    //计算靠近两个数组的分割点的区域内的最短距离。
    double min_dist_cut = INFINITY;
    for (int i = 0; i < cut_area_points.size(); i++ ){
        for (int j = i+1; j < cut_area_points.size(); j++){
            double d_min_of_cut_points = distance(cut_area_points[i],cut_area_points[j]);
            if (d_min_of_cut_points < min_dist_cut){
                min_dist_cut = d_min_of_cut_points;
            }
        }
    }

    return min(min_dist_cut,d_min); //最后，函数返回数组中点之间的最小距离。
}


int main() {     
    int lines_of_doc = -1; 
    string name; cin >> name; //输入文件的名字。
    int size;
    vector<point> po;

    
    auto start = system_clock::now(); //计算代码运行时间
    //从文件输入数据。
    ifstream fileInput(name);
    while (!fileInput.eof()){
        if (lines_of_doc == -1){
            fileInput >> size;
            lines_of_doc++; 
        }
        else{
            int a,b,c;
            fileInput >> a >> b >> c;
            po.push_back({a,b,c});
            lines_of_doc++; 
        }
    } 
    cout << "Lines: " << size << endl;

    //计算最短距离，并输出结果。
    quickSort(po, 0, size-1);  
    double result = shortest(po);
    cout << fixed << setprecision(6) << result << endl;
    
    //输出代码运行时间。
    auto end   = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout <<  "Spent " 
        << double(duration.count()) * microseconds::period::num / microseconds::period::den 
        << " seconds" << endl; 
    return 0; 
} 