#include <iostream>
#include "./togglebit.hpp"
#include <atomic>
#include <string.h>
#include <functional>
#include <thread>
using namespace std;

#define __CAS64(A, B, C) __sync_bool_compare_and_swap(A, B, C)

namespace types {
struct uint128_t {
    uint64_t lo;
    uint64_t hi;
}
__attribute__ ((__aligned__(16)));
}

inline bool DCAS(volatile types::uint128_t * src, types::uint128_t cmp, types::uint128_t with) {

    bool result;
    asm volatile (
        "lock cmpxchg16b %1\n\t"
        "setz %0"       // on gcc6 and later, use a flag output constraint instead
        : "=q" ( result )
        , "+m" ( *src )
        , "+d" ( cmp.hi )
        , "+a" ( cmp.lo )
        : "c" ( with.hi )
        , "b" ( with.lo )
        : "cc", "memory" // compile-time memory barrier.  Omit if you want memory_order_relaxed compile-time ordering.
    );
    return result;
}



enum funcType
{
    addType = 1,
    removeType = 2,
    None = 0
};

template <class DS, class X>
struct func_array
{
    function<X(X)> add;
    function<X()> remove;
    function<DS *()> copy;
    function<void()> printDS;
};

template <class DS, class X>
func_array<DS, X> make_pointers(DS *ds)
{
    func_array<DS, X> funcs;
    funcs.add = bind(&DS::insert, ds, placeholders::_1);
    funcs.remove = bind(&DS::remove, ds);
    funcs.copy = bind(&DS::copy, ds);
    funcs.printDS = bind(&DS::print, ds);
    return funcs;
};

template <class X>
struct Request


{
    funcType func;
    X args;
    Request()
    {
        this->func = None;
        this->args = -1;
    }
    Request(funcType func, X args)
    {
        this->func = func;
        this->args = args;
    }
};

template <class DS , class X>
class Psim
{
    public:
        
        atomic<int> cas_suc = {0};
        atomic<int> cas_fail = {0};
        atomic<int> seq = {0};

        struct SimStruct
        {
            DS *object = nullptr;
            atomic<bool> *applied = nullptr;
            X *rvals = nullptr;
            func_array<DS,X> f_array;

            SimStruct(){};
            void InitStruct(DS *object,int nthreads)
            {
                this->applied = new atomic<bool>[nthreads];
                this->rvals = new X[nthreads];
                for(int i=0;i<nthreads;i++){
                    this->applied[i].store(false,memory_order_relaxed);
                    this->rvals[i] = -1;
                }
                this->object = object;
            };
        };

        struct SimStructSeq
        {
            SimStruct *state = new SimStruct();
            long long seq = 0;
        };

        atomic<bool> *toggles = nullptr;
        int nthreads = 0;
        SimStruct *St = {nullptr};
        Request<X> *announce_array = nullptr;
        SimStructSeq *curr_state = nullptr;
        int *local_index;

        Psim(DS *object, int nthreads, func_array<DS, X> funcs)
        {
            this->St = new SimStruct[nthreads * 2];

            for (int i = 0; i < nthreads*2; i++){
                this->St[i].InitStruct(object, nthreads);
            }


            this->nthreads = nthreads;
            this->toggles = new atomic<bool>[nthreads];
            this->local_index = new int[nthreads];
            
            this->announce_array = new Request<X>[nthreads];
            for(int i=0;i<this->nthreads;i++){
                this->local_index[i] = 0;
                this->toggles[i].store(false,memory_order_relaxed);
            }

            this->curr_state = new SimStructSeq();

            this->curr_state->state = &this->St[0];
            this->curr_state->seq = this->seq.load(memory_order_seq_cst);
            this->curr_state->state->f_array = make_pointers<DS, X>(object);
    }

    int PsimApplyOp(Request<X> req, int pid)
    {
        this->announce_array[pid] = req;
        this->toggles[pid].store(!this->toggles[pid].load(),memory_order_release);
        Attempt(pid);
        return this->curr_state->state->rvals[pid];
    }

    void Attempt(int pid)
    {
        atomic<bool> *local_toggles = new atomic<bool>[this->nthreads];
        for(int i=0;i<this->nthreads;i++){
            local_toggles[i].store(false,memory_order_release);
        }
        SimStructSeq *old_ptr;
        SimStructSeq *new_ptr = new SimStructSeq();

        for(int i=0;i<2;i++)
        {
            const int myIndex = (this->local_index[pid] == 0) ? 2*pid : 2*pid+1;
            old_ptr = this->curr_state;
            this->St[myIndex].object = old_ptr->state->f_array.copy();
            
            for(int i=0;i<this->nthreads;i++)
            {
                // cout<<St[myIndex].applied[i].load()<<endl;
                // cout<<this->curr_state->state->applied[i].load()<<endl;
                this->St[myIndex].applied[i].store(old_ptr->state->applied[i].load(),memory_order_release);
                local_toggles[i].store(this->toggles[i].load(),memory_order_release); 
            }

            if(this->curr_state != old_ptr) continue;

            this->St[myIndex].f_array = make_pointers<DS, X>(this->St[myIndex].object);
            for (int i = 0; i < nthreads; i++)
            {
                
                if (local_toggles[i].load()!= this->St[myIndex].applied[i].load())
                {
                    switch (this->announce_array[i].func)
                    {
                    case 0:
                        cout << "Request is NONE" << endl;
                        break;
                    case 1:
                        // cout<<"Request is ADD"<<endl;
                        this->St[myIndex].rvals[i] = this->St[myIndex].f_array.add(this->announce_array[i].args);
                        // cout<<"rval is "<< this->St[index][pid].rvals[i] <<endl;
                        break;
                    case 2:
                        this->St[myIndex].rvals[i] = this->St[myIndex].f_array.remove();
                        break;
                    default:
                        cout << "wrong number of function!" << endl;
                        break;
                    }
                }
            }

            for (int i = 0; i < this->nthreads; i++)
            {
                this->St[myIndex].applied[i].store(local_toggles[i].load(),memory_order_relaxed); 
            }

            new_ptr->state = &this->St[myIndex];
            // new_ptr->seq = this->seq.load(memory_order_seq_cst);
            new_ptr->seq = old_ptr->seq+1;

            types::uint128_t before = {(uint64_t)old_ptr->state, (uint64_t)old_ptr->seq};
            types::uint128_t after = {(uint64_t)new_ptr->state, (uint64_t)old_ptr->seq};

            if (DCAS((volatile types::uint128_t*)this->curr_state,before,after))
            {
                this->curr_state = new_ptr;
                // this->seq.fetch_add(1, memory_order_seq_cst);
                this->cas_suc.fetch_add(1, memory_order_relaxed);
                this->local_index[pid] = (this->local_index[pid] + 1) % 2;
                return;
            }
                else this->cas_fail.fetch_add(1, memory_order_relaxed);
            }
        }
};
