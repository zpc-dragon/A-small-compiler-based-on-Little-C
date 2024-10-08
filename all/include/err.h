// err.h
#ifndef ERR_H
#define ERR_H
#include <exception>
#include <string>
bool NoError = true;//是否有错误
enum class ErrorCode {
    FileNotFound,
    ExprError,
    DeclareError,
    ExecuteError,
    IFError,
    WhileError,
    ConditonError,
    ProgramError,

};
class BaseException : public std::exception { // 基础异常类
    protected:
    ErrorCode code;
    std::string message;

    public:
    BaseException(ErrorCode code, const std::string& message) : code(code), message(message) {}

    const char* what() const throw() override {
        return message.c_str();
    }

    ErrorCode getCode() const {
        return code;
    }
};
class FileException : public BaseException {    // 文件异常类
    public:
    FileException(const std::string& path) : BaseException(ErrorCode::FileNotFound, "file not found at " + path) {}
};
class  ExprException : public BaseException { // 表达式异常类
    public:
    ExprException(const std::string& message) : BaseException(ErrorCode::ExprError, message) {}
};
class DeclareException : public BaseException {   // 声明异常类
    public:
    DeclareException(const std::string& message) : BaseException(ErrorCode::DeclareError, message) {}
};
class ExecuteException : public BaseException {  // 执行异常类
    public:
    ExecuteException(const std::string& message) : BaseException(ErrorCode::ExecuteError, message) {}
};
class IFException : public BaseException {  // IF异常类
    public:
    IFException(const std::string& message) : BaseException(ErrorCode::IFError, message) {}
};
class WhileException : public BaseException {   // While异常类
    public:
    WhileException(const std::string& message) : BaseException(ErrorCode::WhileError, message) {}
};
class ConditionException : public BaseException {   // 条件异常类
    public:
    ConditionException(const std::string& message) : BaseException(ErrorCode::ConditonError, message) {}
};
class ProgramException : public BaseException {    // 程序异常类
    public:
    ProgramException(const std::string& message) : BaseException(ErrorCode::ProgramError, message) {}
};
void throwError(ErrorCode code, const std::string& message) {
    NoError = false;
    switch (code) {
        case ErrorCode::FileNotFound:
            throw FileException(message);
            break;
        case ErrorCode::ExprError:
            throw ExprException(message);
            break;
        case ErrorCode::DeclareError:
            throw DeclareException(message);
            break;
        case ErrorCode::ExecuteError:
            throw ExecuteException(message);
            break;
        case ErrorCode::IFError:
            throw IFException(message);
            break;
        case ErrorCode::WhileError:
            throw WhileException(message);
            break;
        case ErrorCode::ConditonError:
            throw ConditionException(message);
            break;
        case ErrorCode::ProgramError:
            throw ProgramException(message);
            break;
        default:
            break;
    }
}
#endif // ERR_H
