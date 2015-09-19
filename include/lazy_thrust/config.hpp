/*
*  Copyright 2015 Nikolaos Kyriazis
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#pragma once

//  allow thrust to use decltype for result_of
#ifndef __cpp_lib_result_of_sfinae
#   define __cpp_lib_result_of_sfinae
#endif

//  helpful auto return type deduction for one-liners
#define RETURN(x) -> decltype(x) { return x; }