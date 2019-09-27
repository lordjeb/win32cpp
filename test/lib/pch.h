#pragma once

// Standard C++ Library
#include <memory>

// Googletest
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CODE_ANALYSIS_WARNINGS)
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#pragma warning(pop)
using namespace testing;

// Windows
#include "..\..\targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// win32cpp
#include "debug.h"
#include "error.h"
using namespace win32cpp;
