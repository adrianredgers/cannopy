
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <NumerLib.h>
#include <ANNLib.h>


using namespace std;
using namespace cannopy;

namespace py = pybind11;

UIntType add(UIntType a, UIntType b) {
    return a + b;
}

FloatType maxi(const std::vector<FloatType> &v, FloatType w) {
    FloatType maxVal = w;

    for(FloatType f:v) {
        if(f>maxVal) {
            maxVal=f;
        }
    }
    return maxVal;
}


PYBIND11_MODULE(cannopy, m) {
    m.def("add", &add, "add stuff");
    m.def("max", &maxi, "max", py::arg("list"), py::arg("initMax") );
    m.def("fact", &factorial, "calc factorial of n", py::arg("n"));
    py::class_<RAMNeuron>(m, "RAM")
            .def(py::init<int &>())
            .def("reset", &RAMNeuron::reset)
            .def("set", &RAMNeuron::set)
            .def("lookup", &RAMNeuron::lookup);
}

