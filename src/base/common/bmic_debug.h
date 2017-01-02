/*
 * Copyright 2017 Otavio Piske <angusyoung@gmail.com>.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * File:   bmic_debug.h
 * Author: Otavio Piske <angusyoung@gmail.com>
 *
 * Created on January 2, 2017, 1:22 PM
 */

#ifndef BMIC_DEBUG_H
#define BMIC_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(BMIC_DEBUG) && BMIC_DEBUG >=2
 #define bmic_trace(message, ...) { logger_t btlogger = gru_logger_get(); btlogger(TRACE, message, __VA_ARGS__) }
#else
 #define bmic_trace(level, message, ...)
#endif




#ifdef __cplusplus
}
#endif

#endif /* BMIC_DEBUG_H */

