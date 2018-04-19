
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>
#include <NumerLib.h>
#include <ANNLib.h>


using namespace std;
using namespace cannopy;

namespace py = pybind11;


PYBIND11_MODULE(cannopy, m) {
    m.def("fact", (FloatType (*)(UIntType)) &factorial, "calc factorial of n", py::arg("n"));
    py::class_<RAMNeuron>(m, "RAM")
            .def(py::init<UIntType &>(), py::arg("numInputs"))
            .def("reset", (void (RAMNeuron::*)()) &RAMNeuron::reset)
            .def("set", (void (RAMNeuron::*)(vector<UIntType> &, UIntType)) &RAMNeuron::set, py::arg("address"),
                 py::arg("value"))
            .def("lookup", (UIntType (RAMNeuron::*)(vector<UIntType> &)) &RAMNeuron::lookup);
    py::class_<Wisard>(m, "Wisard")
            .def(py::init<UIntType &, UIntType &, UIntType &>(), py::arg("retinaSize"), py::arg("coverage"),
                 py::arg("arity"))
            .def("reset", (void (Wisard::*)()) &Wisard::reset)
            .def("train", (void (Wisard::*)(vector<UIntType> &)) &Wisard::train, py::arg("retina"))
            .def("lookup", (UIntType (Wisard::*)(vector<UIntType> &)) &Wisard::lookup, py::arg("retina"));
}

