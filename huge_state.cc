#include "huge_state/huge_state.h"
#include <stdint.h>
#include <vector>

namespace kautil{
namespace huge_state{


struct info{
    intptr_t * cur = 0;
    intptr_t begin = 0;
    intptr_t end = 0;
    intptr_t step = 0;
};


struct huge_state{
    using arr_type = std::vector<info>;
    arr_type itrs;
    bool __next(int i){
        if(i  < itrs.size()){
            auto & cur = itrs.at(i);
            if(*cur.cur >= cur.end){
                *cur.cur = cur.begin;
                return __next(i + 1);
            }else{
                *(cur.cur)+=cur.step;
                for(auto itr : itrs) if(*itr.cur >= itr.end)  return __next(0) ;
                return true;
            }
        }else{
            return false;
        }
    }
//    void dbg(){
//        for(auto itr :itrs) if(itr.cur == itr.end) return;
//        std::stringstream ss;
//        for(auto itr :itrs)ss << itr.cur << " ";
//        assert(__set.insert(ss.str()).second);
//        std::cout << __set.size() << " : " << ss.str() << std::endl;
//    }
};



HugeState::HugeState() : m(new huge_state()){}
HugeState::~HugeState(){ if(m){ delete m;m = nullptr; }}

void HugeState::add_with_step(void ** cursor, const void *begin, const  void *end, unsigned int step /*= sizeof(intptr_t)*/){
    m->itrs.resize(m->itrs.size() + 1);
    m->itrs.back().cur = (intptr_t*)cursor;
    *m->itrs.back().cur = (intptr_t)begin;
    m->itrs.back().begin = (intptr_t)begin;
    m->itrs.back().end = (intptr_t)end;
    m->itrs.back().step = step;
}

void HugeState::add_with_count(void ** cursor, const void *begin, const  void *end, const unsigned long long int &n) {
    add_with_step(cursor, begin, end, (((intptr_t) end) - ((intptr_t) begin)) / static_cast<intptr_t>(n));
}

bool HugeState::next(){ return m->__next(0); }

} //namespace state{
} //namespace kautil{




int tmain_kautil_kautil_huge_state_static(int(*__printf)(const char * , ...)){

    struct self_defined{ int i;const char s[4]; };
    auto a  = std::initializer_list<const char *>{"a1","a2","a3"};
    auto b  = std::vector<int>{1,2,3};
    auto c  = std::vector<int64_t>{10,20,30};
    auto d  = std::vector<self_defined>{{.i=0,.s="abc"},{.i=1,.s="efg"}};
    auto range = std::pair<void*,void*>{(void*)125,(void*)(125*2)};
    auto range_step = 10;

    auto a_cur = (void*) nullptr;
    auto b_cur = (void*) nullptr;
    auto c_cur = (void*) nullptr;
    auto d_cur = (void*) nullptr;
    auto r_cur = (void*) nullptr;

    int cnt = 0;
    {
        kautil::huge_state::HugeState x2;
        x2.add_with_step(&a_cur,a.begin()+1, a.end()-1,sizeof(intptr_t));
        x2.add_with_step(&b_cur,(b.begin()+1).base(), (b.end()-1).base(), sizeof(decltype(b)::value_type));
        x2.add_with_count(&c_cur,&*c.data(), &*c.end(), c.size());
        x2.add_with_count(&d_cur,d.begin().base(), d.end().base(), d.size());
        x2.add_with_step(&r_cur,range.first, range.second, range_step);
        for(;;){

            __printf(
                "%s %d %lld %d %s %lld\n"
                ,*(char **)a_cur
                ,*(int*)b_cur
                ,*(int64_t*)c_cur
                ,((self_defined*)d_cur)->i
                ,((self_defined*)d_cur)->s
                ,(int64_t)r_cur);
            if(!x2.next()) break;
        }
    }
    return 0;
}
