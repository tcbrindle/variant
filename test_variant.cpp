#include "variant"
#include <assert.h>
#include <string>
#include <vector>
#include <memory>

namespace se=std::experimental;

void initial_is_empty(){
    se::variant<int> v;
    assert(v.empty());
}

void empty_index_is_neg_one(){
    se::variant<int> v;
    assert(v.index()==-1);
}

void get_empty_t_works_for_empty(){
    se::variant<int> v;

    se::empty_t& e1=se::get<se::empty_t>(v);
    se::empty_t& e2=se::get<se::empty_t>(v);
    assert(&e1==&e2);
}

void can_construct_first_type(){
    se::variant<int> v(42);
    assert(v.index()==0);
}

void can_get_value_of_first_type(){
    se::variant<int> v(42);
    int& i=se::get<int>(v);
    assert(i==42);
}

void can_construct_second_type(){
    se::variant<int,std::string> v(std::string("hello"));
    assert(v.index()==1);
    std::string& s=se::get<std::string>(v);
    assert(s=="hello");
}

void can_move_variant(){
    se::variant<int,std::string> v(std::string("hello"));
    se::variant<int,std::string> v2(std::move(v));
    assert(v2.index()==1);
    assert(v.index()==-1);
    std::string& s=se::get<std::string>(v2);
    assert(s=="hello");
}

void can_copy_variant(){
    se::variant<int,std::string> v(std::string("hello"));
    se::variant<int,std::string> v2(v);
    assert(v2.index()==1);
    assert(v.index()==1);
    std::string& s=se::get<std::string>(v);
    assert(s=="hello");
    std::string& s2=se::get<std::string>(v2);
    assert(s2=="hello");
    assert(&s!=&s2);
}

void can_copy_const_variant(){
    se::variant<int,std::string> const v(std::string("hello"));
    se::variant<int,std::string> v2(v);
    assert(v2.index()==1);
    assert(v.index()==1);
    std::string const& s=se::get<std::string>(v);
    assert(s=="hello");
    std::string& s2=se::get<std::string>(v2);
    assert(s2=="hello");
    assert(&s!=&s2);
}

void construction_from_lvalue(){
    std::vector<int> vec(42);
    se::variant<std::vector<int>> v(vec);
    assert(vec.size()==42);
    assert(v.index()==0);
    std::vector<int>& vec2=se::get<std::vector<int>>(v);
    assert(&vec2!=&vec);
    assert(vec2.size()==42);
}

void construction_from_const_lvalue(){
    std::vector<int> const vec(42);
    se::variant<std::vector<int>> v(vec);
    assert(vec.size()==42);
    assert(v.index()==0);
    std::vector<int>& vec2=se::get<std::vector<int>>(v);
    assert(&vec2!=&vec);
    assert(vec2.size()==42);
}

void move_construction_with_move_only_types(){
    std::unique_ptr<int> ui(new int(42));
    se::variant<std::unique_ptr<int>> v(std::move(ui));
    assert(v.index()==0);
    std::unique_ptr<int>& p2=se::get<std::unique_ptr<int>>(v);
    assert(p2);
    assert(*p2==42);
    
    se::variant<std::unique_ptr<int>> v2(std::move(v));
    assert(v.index()==-1);
    assert(v2.index()==0);
    std::unique_ptr<int>& p3=se::get<std::unique_ptr<int>>(v2);
    assert(p3);
    assert(*p3==42);
}

int main(){
    initial_is_empty();
    empty_index_is_neg_one();
    get_empty_t_works_for_empty();
    can_construct_first_type();
    can_get_value_of_first_type();
    can_construct_second_type();
    can_move_variant();
    can_copy_variant();
    can_copy_const_variant();
    construction_from_lvalue();
    construction_from_const_lvalue();
    move_construction_with_move_only_types();
}
