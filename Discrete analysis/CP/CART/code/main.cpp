// learn 	--input file1 	--output file2 	--height int h
// classify --input file2	--output file3 	--data file4
// Time complexity should be O( n*m*h*log(n) )
// I suppose that it is O(   )

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <map>
#include "DataRepresent.h"
#include "BinarSearchTree.h"

const int AMORTIZATION = 1000;

double Summand(int nk,int a){
    if( nk == 0){
        return 0;
    }
    double answer = (double)nk*(log2(nk) - log2(a))/a;
    return answer;
}

void DivByEntropy(double &minEntropy,const std::vector<DataRepresent> &nums,const std::vector<int> &order,int startPos,int endPos,int dimension,double &splitLine,int &ifBiggerVal, int &ifSmallerVal,int &index){
    
    double current;
    double entropyValueNew = INFINITY;
    int zeros = 0;
    int units = 0;
    int leftZeroAmount = 0;
    int leftUnitsAmount = 0;

    std::map<int,int> itCount;
    int maximum = 0;

    for(int i = startPos; i <= endPos; ++i){
        if( nums[order[i]].value == false ){
            ++zeros;
        }
        else{
            ++units;
        }
        int cnt = ++itCount[ nums[order[i] ].space[dimension] ];
        if( cnt > maximum ){
            maximum = cnt;
        }
    }
    int totalAmount = units + zeros;
    if( maximum == totalAmount){
        minEntropy = INFINITY;
        return;
    }
    // [startPos,endPos]
    for(int i = startPos; i < endPos; ++i){

        current = nums[order[i]].space[dimension];
        int bnd = itCount.find(current)->second;
        for( int count = 0; count < bnd ; ++count){
            if( nums[order[i + count]].value == false){
                ++leftZeroAmount;
            }
            else{
                ++leftUnitsAmount;
            }
        }
        i = i + bnd - 1;
        if( i == endPos){
            return;
        }
        int rightUnitsAmount = units - leftUnitsAmount;//can be 0
        int rightZeroAmount = zeros - leftZeroAmount;//can be 0
        int leftAmount = leftZeroAmount + leftUnitsAmount;//never can be 0
        int rightAmount = rightZeroAmount + rightUnitsAmount;//never can be 0
        
        double leftEntropy;
        double rightEntropy;
        leftEntropy = Summand(leftZeroAmount,leftAmount) + Summand(leftUnitsAmount,leftAmount);
        if( leftEntropy != 0 ){
            leftEntropy *= -1;
        }
        rightEntropy = Summand(rightZeroAmount,rightAmount) + Summand(rightUnitsAmount,rightAmount);
        if( rightEntropy != 0 ){
            rightEntropy *= -1;
        }
        entropyValueNew = ( leftEntropy*leftAmount + rightEntropy*rightAmount )/totalAmount;
        if ( entropyValueNew <= minEntropy ){
            minEntropy = entropyValueNew;
            index = i;
            splitLine = current;
            ifBiggerVal = AMORTIZATION * rightUnitsAmount / rightAmount;
            ifSmallerVal = AMORTIZATION * leftUnitsAmount / leftAmount;
        }
    }
    return;

}


void BuildCART(TTree* ptr,std::vector<DataRepresent> &nums,std::vector<int> &order,int startPos,int endPos,int &hight){

    if ( startPos < endPos ){

        double minEntropy = INFINITY;
        int index = startPos;
        int dimension;
        double line = 10;
        int ifSmallerVal = 0;
        int ifBiggerVal = 0;
        
        double bestEntropy = INFINITY;
        int bestIndex = index;
        int bestDimension;
        double bestLine;
        int bestIfBiggerVal;
        int bestIfSmallerVal;

        for(int i = 0; i < m && ( bestEntropy != 0 ) ; ++i){
            
            dimension = i;
            sort(order.begin() + startPos,order.begin() + endPos,[&](int lhs,int rhs){ return nums[lhs].space[dimension] < nums[rhs].space[dimension]; });
            DivByEntropy(minEntropy,nums,order,startPos,endPos,dimension,line,ifBiggerVal,ifSmallerVal,index);
            if( minEntropy < bestEntropy ){
                bestIndex = index;
                bestEntropy = minEntropy;
                bestDimension = dimension;
                bestLine = line;
                bestIfBiggerVal = ifBiggerVal;
                bestIfSmallerVal = ifSmallerVal;
            }
            
        }

        ptr->coordinate = bestDimension;
        ptr->position = bestLine;
        ptr->ifBiggerValue = bestIfBiggerVal;
        ptr->ifSmallerValue = bestIfSmallerVal;
        ptr->left = nullptr;
        ptr->right = nullptr;

        if( startPos < bestIndex && bestIndex < endPos ){
            ++hight;
            if( hight < hightBound){
                TTree * left = nullptr;
                TTree * right = nullptr;
                if( bestIfBiggerVal != 1000 && bestIfBiggerVal != 0){
                    right = (TTree *)malloc(sizeof(TTree));
                }
                if( bestIfSmallerVal != 1000 && bestIfSmallerVal != 0){
                    left = (TTree *)malloc(sizeof(TTree));
                }
                
                sort(order.begin() + startPos,order.begin() + endPos,[&](int lhs,int rhs){ return nums[lhs].space[bestDimension] < nums[rhs].space[bestDimension]; });
                
                // std::vector<DataRepresent> numsRight(endPos - bestIndex);
                // std::vector<int> orderRight(endPos - bestIndex);
                // std::vector<DataRepresent> numsLeft(bestIndex + 1);
                // std::vector<int> orderLeft(bestIndex + 1);

                // for(int i = 0; i <= bestIndex ; ++i){
                //     numsLeft[i] = nums[order[i]];
                //     orderLeft[i] = i;
                // }

                // for(int i = bestIndex + 1, k = 0; i <= endPos; ++i, ++k){
                //     numsRight[k] = nums[order[i]];
                //     orderRight[k] = k;
                // }

                //nums.erase(nums.begin(), nums.end() );
                //order.erase(order.begin(),order.end());
                ptr->left = left;
                ptr->right = right;
                int leftHight = hight;
                int rightHight = hight;
                if( bestIfBiggerVal != 1000 && bestIfBiggerVal != 0){
                    BuildCART(right,nums,order,bestIndex + 1,endPos,rightHight);
                }
                if( bestIfSmallerVal != 1000 && bestIfSmallerVal != 0){
                    BuildCART(left,nums,order,0,bestIndex,leftHight);
                }
            }
        }

    }
    else{
        free(ptr);
        ptr = nullptr;
        std::cout << "Error startPos >= endPos\n" << startPos << " " << endPos << "\n";
    }

    return;
}


int SearchValue(std::vector<double> &answer,TTree *node){

    //Because if left branch exist 
    //then right branch exist to
    TTree * tmp = node;
    if( tmp != nullptr){

        if( answer[tmp->coordinate] <= tmp->position ){
            if(tmp->left != nullptr){
                SearchValue(answer,tmp->left);
            }
            else{
                return tmp->ifSmallerValue;
            }
        }
        else{
            if( tmp->right != nullptr){
                SearchValue(answer,tmp->right);
            }
            else{
                return tmp->ifBiggerValue;
            }
        }

    }

}

void PrintTree(TTree *node){

    if (node != nullptr){

        PrintTree(node->left);
             
        std::cout << node->coordinate << "\t" << node->position << "\t" << node->ifBiggerValue << "\t" << node->ifSmallerValue <<"\n";

        PrintTree(node->right);
    }

}

void SerializeTree(TTree *node,std::ofstream& DTfout){
	if( DTfout.is_open() ){

		if( node != nullptr ){
		
            if( node->left != nullptr){
                DTfout << 1 << " ";
                SerializeTree(node->left,DTfout);
            }
            else{
                DTfout << 0 << " ";
            }
            
            DTfout << *node << " ";
            
            if( node->right != nullptr){
                DTfout << 1 << " ";
                SerializeTree(node->right,DTfout);
            }
            else{
                DTfout << 0 << " ";
            }
            (void*) node;
            free( node );
        }
        
	}
    else{
        std::cout << "Can't open file in SerializeTree function\n";
    }
	
	
}

TTree * DeserializeTree(std::ifstream& DTfin){

	if( DTfin.is_open() ){
		
        int value;
        DTfin >> value;

        if ( value == 0){
            TTree * ptr = nullptr;
            return ptr;
        }
        if( value == 1){
            
            TTree * left = DeserializeTree(DTfin);
            
            TTree * ptr = (TTree *)malloc(sizeof(TTree));
            
            DTfin >> ptr->coordinate >> ptr->position >> ptr->ifSmallerValue >> ptr->ifBiggerValue ;
            
            TTree * right = DeserializeTree(DTfin);
            
            ptr->left = left;
            ptr->right = right;
            return ptr;
        }

	}
    else{
        std::cout << "Can't open file in DeserializeTree function\n";
    }
		
}


void CleanTree(TTree *node){

    if( node != nullptr ){
    
        if( node->left != nullptr){
            CleanTree(node->left);
        }
        
        if( node->right != nullptr){
            CleanTree(node->right);
        }

        (void*) node;
        free( node );
    }

}

std::ifstream fin;
std::ofstream fout;
std::ifstream fdata;

int main(int argc, char const * argv[]) {

    if ( argc <= 1 ){
        std::cout << "Inject in coommand line \n./program learn --input --output --hight\n";
        std::cout << "or\n";
        std::cout << "./program classify --input --output --data\n";
        return 1;
    }

    std::string arg = argv[1];
    bool okIn = false;

    std::string firstInput;
    std::string firstOutput;
    int hight;

    std::string secondInput;
    std::string secondData;
    std::string secondOutput;

    if ( arg == "learn" && argc == 5 ){
        okIn = true;
        firstInput = argv[2];
        firstOutput = argv[3];
        hight = atoi(argv[4]);
        fin.open(firstInput);
        fout.open(firstOutput);
        hightBound = hight;
    }
    if( arg == "classify" && argc == 5 ){
        okIn = true;
        secondInput = argv[2];
        secondOutput = argv[3];
        secondData = argv[4];
        fin.open(secondInput);
        fout.open(secondOutput);
        fdata.open(secondData);
    }
    
    if ( okIn == false ){
        std::cout << "Inject in coommand line \n./program learn --input --output --hight\n";
        std::cout << "or\n";
        std::cout << "./program classify --input --output --data\n";
        return 1;
    }
    if ( !fout ){
        std::cout << "Can't open file\n";
        return 1;
    }
    if ( !fin ){
        std::cout << "Can't open file\n";
        return 1;
    }

    if ( arg == "learn"){
        
        fin >> n >> m;
        std::vector<DataRepresent> nums(n);
        srand(2);
        std::vector<int> order(n);
        for(int i = 0; i < n; ++i){
            for(int k = 0; k < m; ++k){
                fin >> nums[i].space[k];
            }
            fin >> nums[i].value;
            order[i] = i;
        }
        TTree *myCart = (TTree *)malloc(sizeof(TTree));;
        hight = 0;
        BuildCART(myCart,nums,order,0,n - 1,hight);
        fout << 1 << " ";
        SerializeTree(myCart,fout);
    }

	if( arg == "classify"){
        
        fdata >> n >> m;
        std::vector<DataRepresent> nums(n);
        for(int i = 0; i < n; ++i){
            for(int k = 0; k < m; ++k){
                fdata >> nums[i].space[k];
            }
            nums[i].value = 2;
        }

        TTree *ptr = nullptr;
        ptr = DeserializeTree(fin);
        if( ptr == nullptr){
            std::cout << "Problem with tree\n";
        }

        for(int i = 0; i < n; ++i){
            int chisl = SearchValue(nums[i].space,ptr);
            double ans = (double)chisl / AMORTIZATION;
            fout << ans << "\n";
        }

        CleanTree(ptr);

    }

    return 0;
}
