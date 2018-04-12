
#include <pybind11/pybind11.h>
#include <NumerLib.h>
#include <ANNLib.h>


namespace py = pybind11;
using namespace cannopy;

IntType add(IntType a, IntType b) {
    return a + b;
}


PYBIND11_MODULE(cannopy, m) {
    m.def("add", &add, "add stuff");
    m.def("fact", &factorial, "calc factorial of n", py::arg("n"));
//    m.def("RAM", &factorial, "calc factorial");
    py::class_<RAMNeuron>(m, "RAM")
            .def(py::init<int &>())
            .def("reset", &RAMNeuron::reset)
            .def("set", &RAMNeuron::set)
            .def("lookup", &RAMNeuron::lookup);
}

