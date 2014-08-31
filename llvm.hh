#pragma once

#include <boost/spirit/include/qi.hpp>

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/PassManager.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Scalar.h>

#include <iostream>

#include "ast.hh"

using namespace llvm;

namespace qi = boost::spirit::qi;

class codegen {
public:
    codegen();

    Value* operator()(qi::info::nil) const {}
    Value* operator()(int n);
    Value* operator()(std::string const& s);
    Value* operator()(ast::expression const& expr);
    Value* operator()(ast::ifexpr const& expr);
    Value* operator()(ast::binary_op const& expr);
    Value* operator()(ast::unary_op const& expr);
    Value* operator()(ast::call const& call);
    Value* operator()(ast::definition const& def);

    Value *errorv(const char *msg) { std::cerr << msg << std::endl; return 0; }
    void dump() { _module->dump(); }
public:
    typedef Value* result_type;
private:
    Module* _module;
    LLVMContext* _context;
    IRBuilder<> _builder;
    ExecutionEngine* _engine;
    FunctionPassManager* _optimizer;
    std::map<std::string, Value*> _named_values;
};
