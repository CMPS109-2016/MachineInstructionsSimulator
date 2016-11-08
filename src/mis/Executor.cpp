//
// Created by CIJhn on 11/7/2016.
//

#include "mis/Executor.h"

namespace mis {
    class BlockedExecutor : public Executor {
    public:
        virtual void execute(Task &task) override {
            task();
        }
    };

    Executor *Executor::newBlockExecutor() {
        return new BlockedExecutor();
    }

//    template<typename T>
//    class CompletedFuture : public AsyncExecutor::Future<T> {
//    private:
//        T value;
//    public:
//        virtual T *get() override {
//            return value;
//        }
//
//        virtual bool isDone() override {
//            return true;
//        }
//
//        virtual void addListener(AsyncExecutor::Listener <T> &listener) override {
//            listener.onEvent(value);
//        }
//
//        virtual void atFailureListener(AsyncExecutor::Listener<std::exception> listener) override {
//        }
//    };
//
//    template<typename T>
//    class FailedFuture : public AsyncExecutor::Future<T> {
//    private:
//        std::exception exception;
//    public:
//        virtual T *get() override {
//            return nullptr;
//        }
//
//        virtual bool isDone() override {
//            return true;
//        }
//
//        virtual void addListener(AsyncExecutor::Listener<T> listener) override {
//        }
//
//        virtual void atFailureListener(AsyncExecutor::Listener<std::exception> listener) override {
//            listener.onEvent(exception);
//        }
//    };
}
