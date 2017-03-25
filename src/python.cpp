#include <boost/python.hpp>
#include <boost/python/exception_translator.hpp>
#include <boost/python/numpy.hpp>
#include "proc.h"
#include "gil.h"
#include "except.h"
#include "bitmap.h"


using namespace boost::python;


class BitmapWrapper : public Bitmap
                      , public wrapper<Bitmap>
{
    public:
        BitmapWrapper(std::string filepath)
            : Bitmap(filepath)
        {
            // nop
        }

        numpy::ndarray data_as_ndarray() const
        {
            tuple shape, strides;
            numpy::dtype data_type = numpy::dtype::get_builtin<uint8_t>();
            shape = make_tuple(_height,
                               _width,
                               3);
            strides = make_tuple(_width * 3 * sizeof(uint8_t),
                                 3 * sizeof(uint8_t),
                                 sizeof(uint8_t));
            return numpy::from_data(_data,
                                    data_type,
                                    shape,
                                    strides,
                                    object());
        }

        std::string info() const
        {
            ScopedPythonGILLock gil_lock;

            if (override f = this->get_override("info"))
                return f();
            else
                return Bitmap::info();
        }
};


void translate_FileError(FileError const & e)
{
    PyErr_SetString(PyExc_OSError, "FileError");
}


BOOST_PYTHON_MODULE(pymycpp)
{
    PyEval_InitThreads();

    numpy::initialize();

    register_exception_translator<FileError>(&translate_FileError);

    class_<Proc, boost::noncopyable>( "Proc", init<>() )
        .def("start", &Proc::start)
        .def("stop", &Proc::stop)
        ;

    class_<BitmapWrapper, boost::noncopyable>( "Bitmap", init<std::string>() )
        .def("get_width", &BitmapWrapper::get_width)
        .def("get_height", &BitmapWrapper::get_height)
        .def("save", &BitmapWrapper::save)
        .def("data", &BitmapWrapper::data_as_ndarray)
        .def("info", &BitmapWrapper::info)
        ;
}
