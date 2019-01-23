#ifndef UTILS_EXCEPTIONS_HPP
#define UTILS_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace utils::exceptions {

    // logic_error	domain_error	invalid_argument
    // length_error	out_of_range	runtime_error
    // range_error	overflow_error	underflow_error

    /**
     *	\brief
     *		Generic exception with message.
     *	\param	msg
     *		The message to be shown when encountering this exception.
     */
    class Exception : public std::exception {
        private:
            std::string _msg;
        public:
            explicit Exception(const std::string& name, const std::string& msg)
                : _msg(name + ": " + msg) {}

            explicit Exception(const std::string& msg)
                : Exception("Exception", msg) {}

            virtual ~Exception() {}

            virtual const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
                return this->getMessage().c_str();
            }

            virtual inline const std::string& getMessage() const noexcept {
                return this->_msg;
            }
    };

    /**
     *	\brief
     *		Out of bounds exception with message and index.
     *	\param	idx
     *		The index that caused the out of bounds.
     */
    class OutOfBoundsException : public Exception  {
        public:
            OutOfBoundsException(const int& idx)
                : Exception("OutOfBoundsException",
                            "Index " + std::to_string(idx) + " was out of bounds.")
            {}
    };

    /**
     *	\brief
     *		Nullpointer exception with message.
     *	\param	msg
     *		The null reference.
     */
    class NullPointerException : public Exception  {
        public:
            NullPointerException(const std::string& msg)
                : Exception("NullPointerException",
                            "Reference " + msg + " not set to an object.")
            {}
    };

    /**
     *	\brief
     *		Divide by zero exception with message.
     *	\param	msg
     *		The index that caused the out of bounds.
     */
    class DivideByZeroException : public Exception  {
        public:
            DivideByZeroException(const std::string& msg)
                : Exception("DivideByZeroException",
                            "Tried to devide by zero in " + msg)
            {}
    };

    /**
     *	\brief
     *		Casting exception with message.
     *	\param	src
     *		The object to be casted.
     *	\param	dest
     *		The object to cast to.
     */
    class CastingException : public Exception  {
        public:
            CastingException(const std::string& src, const std::string& dest)
                : Exception("CastingException",
                            "Cannot cast \"" + src + "\" to object of type \""
                            + dest + "\"!")
            {}
    };

    /**
     *	\brief
     *		File read exception.
     *	\param	name
     *		The name of the file to be read.
     */
    class FileReadException : public Exception  {
        public:
            FileReadException(const std::string& name)
                : Exception("FileReadException",
                            "Cannot read from file: " + name)
            {}
    };

    /**
     *	\brief
     *		File write exception.
     *	\param	name
     *		The name of the file to be written.
     */
    class FileWriteException : public Exception  {
        public:
            FileWriteException(const std::string& name)
                : Exception("FileWriteException",
                            "Cannot write to file: " + name)
            {}
    };

    /**
     *	\brief
     *		Unexpected file extension exception.
     *	\param	ext
     *		The extension of a file that was not expected.
     */
    class UnexpectedExtension : public Exception  {
        public:
            UnexpectedExtension(std::string ext)
                : Exception("UnexpectedExtension", ext)
            {}
    };
}

#endif // UTILS_EXCEPTIONS_HPP
