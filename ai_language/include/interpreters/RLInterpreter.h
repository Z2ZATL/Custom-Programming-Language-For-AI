// interpreters/BaseInterpreter.h
#ifndef BASEINTERPRETER_H
#define BASEINTERPRETER_H

class BaseInterpreter {
public:
  virtual ~BaseInterpreter() {}
  virtual void interpret() = 0;
};

#endif


// connectors/BaseConnector.h
#ifndef BASECONNECTOR_H
#define BASECONNECTOR_H

class BaseConnector {
public:
  virtual ~BaseConnector() {}
  virtual void connect() = 0;
};

#endif


// main.cpp
#include "interpreters/BaseInterpreter.h"
#include "connectors/BaseConnector.h"
#include <iostream>

class MyInterpreter : public BaseInterpreter {
public:
  void interpret() override {
    std::cout << "Interpreting...\n";
  }
};

class MyConnector : public BaseConnector {
public:
  void connect() override {
    std::cout << "Connecting...\n";
  }
};


int main() {
  MyInterpreter interpreter;
  MyConnector connector;

  interpreter.interpret();
  connector.connect();

  return 0;
}