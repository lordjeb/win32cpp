#pragma once

// Standard C++ Library
#include <memory>

// Googletest
#include <gtest/gtest.h>
#include <gmock/gmock.h>
using namespace testing;

// Windows
#include "..\..\targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// win32cpp
#include "debug.h"
#include "error.h"
using namespace win32cpp;
