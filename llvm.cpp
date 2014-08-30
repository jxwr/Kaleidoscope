#include <boost/variant/apply_visitor.hpp>
#include <boost/spirit/include/qi.hpp>
#include <llvm/Analysis/Verifier.h>

#include <iostream>

#include "llvm.hh"

namespace qi = boost::spirit::qi;

codegen::codegen() : _builder(getGlobalContext()) {
    _module = new Module("top", getGlobalContext());
}

Value* codegen::operator()(int n) { 
    return ConstantInt::get(getGlobalContext(), APInt(64, n));
}

Value* codegen::operator()(std::string const& s) { 
    auto v = _named_values[s];
    return v ? v : errorv("Unknown variable name");
}

Value* codegen::operator()(ast::expression const& ast) {
    boost::apply_visitor(*this, ast.expr);
}

Value* codegen::operator()(ast::binary_op const& expr) {
    auto L = boost::apply_visitor(*this, expr.left.expr);
    auto R = boost::apply_visitor(*this, expr.right.expr);

    switch (expr.op) {
    case '+':
        return _builder.CreateAdd(L, R, "addtmp");;
    case '-':
        return _builder.CreateSub(L, R, "subtmp");
    case '*':
        return _builder.CreateMul(L, R, "multmp");
    }
    return errorv("invalid binary operator");
}

Value* codegen::operator()(ast::unary_op const& expr) {
    boost::apply_visitor(*this, expr.subject.expr);
}

Value* codegen::operator()(ast::call const& call) {
    auto callee = _module->getFunction(call.get<0>());
    if (callee == nullptr) {
        return errorv("Unknown function referenced");
    }

    std::vector<Value*> args;
    for (auto arg : call.get<1>()) {
        args.push_back(boost::apply_visitor(*this, arg.expr));
    }

    return _builder.CreateCall(callee, args, "calltmp");
}

Value* codegen::operator()(ast::definition const& def) {
    auto proto = def.get<0>();
    auto body = def.get<1>();
    auto name = proto.get<0>();
    auto args = proto.get<1>();

    // gen prototype
    std::vector<Type*> ints(args.size(), Type::getInt64Ty(getGlobalContext()));
    auto ft = FunctionType::get(Type::getInt64Ty(getGlobalContext()), ints, false);
    auto fun = Function::Create(ft, Function::ExternalLinkage, name, _module);

    // naming args
    unsigned idx = 0;
    _named_values.clear();
    for (auto iter = fun->arg_begin(); idx != args.size(); iter++, idx++) {
        iter->setName(args[idx]);
        _named_values[args[idx]] = iter;
    }

    // gen body
    auto bb = BasicBlock::Create(getGlobalContext(), "entry", fun);
    _builder.SetInsertPoint(bb);

    auto ret = boost::apply_visitor(*this, body.expr);
    _builder.CreateRet(ret);
    verifyFunction(*fun);

    return nullptr;
}
