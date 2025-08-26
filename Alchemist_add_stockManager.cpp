#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

// PotionRecipe 클래스: 재료 목록을 vector<string>으로 변경
class PotionRecipe {
public:
    string potionName;
    vector<string> ingredients; // 단일 재료에서 재료 '목록'으로 변경

    // 생성자: 재료 목록을 받아 초기화하도록 수정
    PotionRecipe(const string& name, const vector<string>& ingredients)
        : potionName(name), ingredients(ingredients) {}
};

// AlchemyWorkshop 클래스: 레시피 목록을 관리
class AlchemyWorkshop {
private:
    vector<PotionRecipe> recipes;

public:
    // addRecipe 메서드: 재료 목록(vector)을 매개변수로 받도록 수정
    void addRecipe(const string& name, const vector<string>& ingredients) {
        recipes.push_back(PotionRecipe(name, ingredients));
        cout << ">> 새로운 레시피 '" << name << "'이(가) 추가되었습니다." << endl;
    }

    // 모든 레시피 출력 메서드
    void displayAllRecipes() const {
        if (recipes.empty()) {
            cout << "아직 등록된 레시피가 없습니다." << endl;
            return;
        }

        cout << "\n--- [ 전체 레시피 목록 ] ---" << endl;
        for (size_t i = 0; i < recipes.size(); ++i) {
            cout << "- 물약 이름: " << recipes[i].potionName << endl;
            cout << "  > 필요 재료: ";
            
            // 재료 목록을 순회하며 출력
            for (size_t j = 0; j < recipes[i].ingredients.size(); ++j) {
                cout << recipes[i].ingredients[j];
                // 마지막 재료가 아니면 쉼표로 구분
                if (j < recipes[i].ingredients.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
        cout << "---------------------------\n";
    }

    const PotionRecipe* searchRecipeByName(const string& name) const {
        auto it = find_if(recipes.cbegin(), recipes.cend(),
                               [&](const PotionRecipe& r){ return r.potionName == name; });
        return (it!=recipes.cend())?&(*it):nullptr;
    }

    vector<const PotionRecipe*> searchRecipeByIngredient(const string &ingredient) const {
        vector<const PotionRecipe*> ret;
        
        for(const auto& i : recipes){
            if(find(i.ingredients.begin(),i.ingredients.end(), ingredient) != i.ingredients.end()){
                ret.push_back(&i);
            }
        }

        return ret;
    }
};

class StockManager{
private:
    map<string,int> potionStock;
    const int MAX_STOCK = 3;

public:
    //레시피가 공방에 추가될 때, 해당 물약은 자동으로 3개의 초기 재고
    void initializeStock(string potionName){
        potionStock[potionName] = MAX_STOCK;
    }

    bool hasPotion(string potionName){
        return potionStock.find(potionName)!=potionStock.end();
    }

    int potionCount(string potionName){
        auto it = potionStock.find(potionName);
        return (it==potionStock.end()) ? -1 : it->second;       
    }

    // 물약 이름으로 재고를 검색하고, 재고가 1개 이상인 경우 모험가에게 지급할 수 있습니다.
    bool dispensePotion(string potionName){   
        if(hasPotion(potionName) && potionCount(potionName)){ 
            cout << potionName << "을 1개 지급하였습니다\n";
            potionStock[potionName]--;
            return 1;
        }
        else {
            cout << potionName << "이 없습니다.\n";
            return 0;
        }
    }

    //레시피의 재료로 관련 물약들의 재고를 검색하고, 재고가 1개 이상인 경우 지급할 수 있어야 합니다.
    bool dispensePotionByintgredient(const string& ingredientName, const AlchemyWorkshop& workshop){   
        // map 순회
        vector<const PotionRecipe*> candidate = workshop.searchRecipeByIngredient(ingredientName);
        if(candidate.empty()){
            cout << "해당 재료로 만들 수 있는 물약이 없습니다.\n";
            return 0;
        }
        
        cout << "* 해당 재료가 들어가는 포션 목록\n";
        for(const auto c : candidate){
            if(hasPotion(c->potionName) && potionCount(c->potionName)>0){
                cout << "- " << c->potionName << " : " << potionCount(c->potionName)<< '\n';
            }
        }
        return 1;
    }

    // 공병 반환
    void returnPotion(string potionName){
        if(hasPotion(potionName)){ 
            if(potionStock[potionName]>=MAX_STOCK) cout << "저장할 공간이 부족합니다.\n";
            else {
                cout << potionName << "의 공병이 1개 반환되었습니다.\n";
                potionStock[potionName]++;
                cout << "현재 " << potionName << "의 남은 개수는 " << potionStock[potionName] << "개 입니다.\n";
            }
        }
    }

    // 재고 확인
    int getStock(string potionName){
        if(hasPotion(potionName)){
            return potionStock[potionName];
        }
        else {
            cout << "취급하지 않는 물건입니다.\n";
            return -1;
        }
    }
};

int main() {
    AlchemyWorkshop myWorkshop;
    StockManager stockManager;

    while (true) {
        cout << "⚗️ 연금술 공방 관리 시스템" << endl;
        cout << "1. 레시피 추가" << endl;
        cout << "2. 모든 레시피 출력" << endl;
        cout << "3. 레시피 검색(포션 입력)" << endl;
        cout << "4. 포션 검색(재료 입력)" << endl;
        cout << "5. 창고 열기" << endl;
        cout << "6. 종료" << endl;
        cout << "선택: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            string name;
            cout << "물약 이름: ";
            cin.ignore(10000, '\n');
            getline(cin, name);

            // 여러 재료를 입력받기 위한 로직
            vector<string> ingredients_input;
            string ingredient;
            cout << "필요한 재료들을 입력하세요. (입력 완료 시 '끝' 입력)" << endl;
            
            while (true) {
                cout << "재료 입력: ";
                getline(cin, ingredient);
                
                // 사용자가 '끝'을 입력하면 재료 입력 종료
                if (ingredient == "끝") {
                    break;
                }
                ingredients_input.push_back(ingredient);
            }

            // 입력받은 재료가 하나 이상 있을 때만 레시피 추가
            if (!ingredients_input.empty()) {
                myWorkshop.addRecipe(name, ingredients_input);
                stockManager.initializeStock(name); // 레시피 추가시 3개로 초기화
            } else {
                cout << ">> 재료가 입력되지 않아 레시피 추가를 취소합니다." << endl;
            }

        } 
        else if (choice == 2) {
            myWorkshop.displayAllRecipes();

        } 
        else if (choice == 3) {
            cout << "---------------------------\n";
            cout << "검색하려는 물약 이름을 입력하세요.\n";
            string s; cin >> s;
            auto search = myWorkshop.searchRecipeByName(s);
            if(search) {
                cout << "물약 " << search->potionName << "의 재료\n";
                for(auto i = 0; i < search->ingredients.size() ;++i){
                    cout << i+1 << ") " << search->ingredients[i] << '\n';
                }
            } else {
                cout << "물약을 찾을 수 없습니다.\n";
            }
            cout << "---------------------------\n";
        } 
        else if (choice == 4) {
            cout << "---------------------------\n";
            cout << "검색하려는 재료 이름을 입력하세요.\n";

            string s; cin >> s;

            cout << "재료에 맞는 물약을 찾기 시작합니다.\n";
            vector<const PotionRecipe*> list = myWorkshop.searchRecipeByIngredient(s);
            cout << "검색 완료했습니다.\n";
            if(!list.empty()){
                cout << "재료 " << s << "로 만들 수 있는 물약들\n";
                for(int i = 0 ; i < list.size(); i++){
                    cout << i+1 << ") " << list[i]->potionName<<'\n';
                }
            }
            else cout << "재료에 맞는 물약을 찾을 수 없습니다.\n";
            cout << "---------------------------\n";
        } 
        else if(choice == 5){
            // 창고 열기
            // 특정 물약의 현재 재고 수량을 알 수 있어야 합니다.
            while(true){
                cout << "---------------------------\n";
                cout << "💲 물약 창고" << endl;
                cout << "1. 재고 확인" << endl;
                cout << "2. 재고 검색" << endl;
                cout << "3. 공병 반환" << endl;
                cout << "4. 창고 나가기" << endl;
                cout << "선택 : ";
                
                int num; cin >> num;
                if (cin.fail()) {
                    cout << "잘못된 입력입니다." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    continue;
                }
                cout << "\n";
                if(num==1){
                    cout << "재고를 확인하실 상품명을 입력하세요.\n";
                    string s; cin >> s;
                    if(stockManager.hasPotion(s)){
                        cout << "물약 " << s << "는 " << stockManager.potionCount(s) << "개 입니다.\n";
                    }
                    else cout << "존재하지 않는 물약입니다.\n";
                }
                else if(num == 2) {
                    cout << "검색 방법을 선택하세요.\n";
                    cout << "1. 물약명 검색\n";
                    cout << "2. 재료명 검색\n";
                    cout << "입력 : ";
                    int se; cin >> se;
                    if(se==1) {
                        cout << "물약명을 입력하세요\n";
                        cout << "입력 : ";
                        string s; cin >> s;
                        if(stockManager.hasPotion(s)){
                            if(stockManager.potionCount(s)){
                                cout << "현재 " << s << "는 " << stockManager.potionCount(s) << "개 있습니다.\n";
                                cout << "모험가에게 지급하시겠습니까? (Y / N) : ";
                                char str; cin >> str;
                                cout << '\n';
                                if(str=='y' || str=='Y') stockManager.dispensePotion(s);
                                else if (str == 'n' || str=='N') cout << "취소하였습니다. \n";
                                else cout << "잘못된 입력입니다.\n";
                            }
                            else cout << s << " 물약은 재고가 없습니다.\n";
                        }
                        else cout << "해당 물약은 없습니다.\n";
                    }
                    else if (se == 2) {
                        cout << "재료명을 입력하세요\n";
                        cout << "입력 : ";
                        string s; cin >> s;
                        
                        if(stockManager.dispensePotionByintgredient(s, myWorkshop)){
                            cout << "모험가에게 지급하시겠습니까? (Y / N) : ";
                            char str; cin >> str;
                            cout << '\n';
                            if(str=='y' || str=='Y') {
                                cout << "지급하실 물약을 입력하세요. : ";
                                string dispence; cin >> dispence;
                                stockManager.dispensePotion(dispence);
                            }
                            else if (str == 'n' || str=='N') cout << "취소하였습니다. \n";
                            else cout << "잘못된 입력입니다.\n";
                        }
                    }
                    else cout << "잘못된 입력입니다.\n";
                }
                else if (num == 3){
                    // 공병 반환
                    cout << "반환 하실 공병의 포션명을 입력하세요.\n";
                    string s; cin >> s;
                    stockManager.returnPotion(s);
                }
                else if (num==4){
                    cout << "창고를 나갑니다.\n";
                    break;
                }
                else cout << "잘못된 입력입니다.\n";
            }
            // 물약 이름으로 재고를 검색하고, 재고가 1개 이상인 경우 모험가에게 지급할 수 있습니다.
            // 레시피의 재료로 관련 물약들의 재고를 검색하고, 재고가 1개 이상인 경우 지급할 수 있어야 합니다.
            // 공병 반환
        }   
        else if (choice == 6) {
            cout << "공방 문을 닫습니다..." << endl;
            break;

        } 
        else {
            cout << "잘못된 선택입니다. 다시 시도하세요." << endl;
        }
    }

    return 0;
}