#ifndef _EXPORT_QUESTION_TEST_H_
#define _EXPORT_QUESTION_TEST_H_

#include "../export_macro.h"


class ExportInterfaces;

// C++ 

class EXPORT ExportQuestionTest
{
public:
    ExportQuestionTest();
    ~ExportQuestionTest();



private:
    //std::shared_ptr<>
};


// C
extern "C" EXPORT ExportInterfaces*  GetInterfaces();


// Abstract Interfaces
extern "C" EXPORT ExportInterfaces* __stdcall CreateInterfaces();


#endif