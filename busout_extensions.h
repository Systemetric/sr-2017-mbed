/* busout_extensions.h
 *
 * Copyright 2016 Josh Holland
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JDH_16_17_RASPI3_BUSOUT_EXTENSIONS_H_
#define JDH_16_17_RASPI3_BUSOUT_EXTENSIONS_H_

#include "mbed.h"

BusOut& operator &=(BusOut& bus, int v);
BusOut& operator |=(BusOut& bus, int v);
BusOut& operator ^=(BusOut& bus, int v);

#endif  // JDH_16_17_RASPI3_BUSOUT_EXTENSIONS_H_