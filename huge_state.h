

#ifndef KAUTIL_HUGESTATE_HUGE_STATE_CC
#define KAUTIL_HUGESTATE_HUGE_STATE_CC


namespace kautil{
namespace huge_state{

struct huge_state;
struct HugeState final {
    HugeState();
    virtual ~HugeState();
    virtual void add_with_step(void **cursor, const void *begin, const  void *end, unsigned int step);
    virtual void add_with_count(void **cursor, const void* begin, const void* end, unsigned long long int const& n);
    virtual bool next();
private:
    huge_state * m=0;

}; // HugeState


} //namespace state{
} //namespace kautil{


#endif
