// copyright @ batam
#ifndef FILTER_H
#define FILTER_H

#include <iostream>
#include <numeric>
#include <deque>
#include <stdexcept>
#include <vector>

//moving average filter
template <typename A>
class moving_average_filter
{
    std::deque<A> buffer; //data yang ingin diinput
    uint16_t len; //total data yang ingin diinput
    A out; //output data

    public:
    moving_average_filter(uint16_t len) : len(len) {} 

    A update (A input) { //function to calculate the average
        if (buffer.size() >= len){ 
            buffer.pop_front(); 
        } 
        buffer.push_back(input);
        out = std::accumulate(buffer.begin(), buffer.end(), 0.0) / buffer.size(); 
        return out; 
    }

    A get_out() {return out;}
};

//median filter
template <typename T>
class median_filter
{
    std::deque<T> buffer;
    uint16_t len;
    T out;

    public:
    median_filter(uint16_t length) : len(length) {}

    T update(T input) {
        if (buffer.size() >= len) {
            buffer.pop_front();
        }
        buffer.push_back(input);
        std::sort(buffer.begin(), buffer.end());
        // buffer 5 data only
        out = buffer[buffer.size()/2];
        return out;
    }

    T get_out() {return out;}

};
#endif