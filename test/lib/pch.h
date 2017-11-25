#pragma once

// Standard C++ Library
#include <memory>

// Googletest
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Windows
#include "..\..\targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace win32cpp {}
using namespace win32cpp;
using testing::Eq;
using testing::Ne;
