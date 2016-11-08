//
// Created by CIJhn on 10/31/2016.
//

#ifndef MACHINEINSTRUCTIONSSIMULATOR_ASYNCEXECUTOR_H
#define MACHINEINSTRUCTIONSSIMULATOR_ASYNCEXECUTOR_H

#include <exception>
#include <functional>

namespace mis {
    struct Executor {
        using Task = std::function<void(void)>;

        virtual void execute(Task &task)=0;

        static Executor *newBlockExecutor();
    };

    struct AsyncExecutor : public Executor {
        template<typename T>
        using Job = std::function<T(void)>;

        template<typename E>
        struct Listener {
            virtual void onEvent(E &event)=0;
        };

        template<typename T>
        struct Future {
            virtual T *get()=0;

            virtual bool isDone()=0;

            virtual void addListener(Listener <T> &listener)=0;

            virtual void atFailureListener(Listener<std::exception> listener)=0;
        };

        Future<void> &&submit(Task task);

        template<typename T>
        Future<T> &&submit(Job<T> job);

    private:
        int numberOfThread;
    };
}

#endif //MACHINEINSTRUCTIONSSIMULATOR_ASYNCEXECUTOR_H
