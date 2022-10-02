#pragma once

#include "location.hh"

struct Locator {
    yy::location location_;

    Locator (yy::location loc) : location_ (loc) {}
    Locator () = default;
    
    virtual ~Locator () = default;
};

// Locator::~Locator () = default;