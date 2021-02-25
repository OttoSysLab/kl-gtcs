#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <typeinfo>

int main()
{
    #pragma region 
    // printf("Fcuck!!!\n");
    // float f = 1003.1415967;
    // char buf_1[256];
    // char buf_2[256];
    // sprintf(buf_1,"%1.3f",f);
    // std::cout << buf_1 << std::endl;
    
    // snprintf(buf_2,sizeof(buf_2),"%.3f",f);
    // std::cout << buf_2 << std::endl;
    #pragma endregion
   
    std::string inttest = "1234";
    std::cout << typeid(inttest).name() << std::endl;
    std::cout << typeid(std::atoi(inttest.c_str())).name() << std::endl;
    std::cout << typeid(std::stoi(inttest)).name() << std::endl;
    std::cout << typeid((uint16_t)std::atoi(inttest.c_str())).name() << std::endl;
    std::cout << typeid((uint16_t)std::stoi(inttest)).name() << std::endl;
    return 0;
}
