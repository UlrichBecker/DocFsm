#!/bin/bash
docfsm --std c++11 $1 | dot -Tpdf -o ${1%.*}.pdf
