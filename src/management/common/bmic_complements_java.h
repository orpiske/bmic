/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bmic_complements_java.h
 * Author: opiske
 *
 * Created on December 5, 2016, 10:49 AM
 */

#ifndef BMIC_COMPLEMENTS_JAVA_H
#define BMIC_COMPLEMENTS_JAVA_H

#ifdef __cplusplus
extern "C" {
#endif
    

typedef struct bmic_java_mem_info_t_ {
    uint64_t init;
    uint64_t committed;
    uint64_t max;
    uint64_t used;
} bmic_java_mem_info_t;

typedef enum bmic_java_memory_model_t_ {
    /**
     * Memory model for Java 6 and 7 (ie.: no metaspace)
     */
    JAVA_LEGACY,
            
    /**
     * Memory model for Java 8
     */
    JAVA_MODERN,
} bmic_java_memory_model_t;

typedef struct bmic_java_info_t_ {
    const char *name;
    const char *version;
    const char *jvm_package_version;
    bmic_java_memory_model_t memory_model;
} bmic_java_info_t;

typedef struct bmic_java_os_info_t_ {
    const char *name;
    const char *arch;
    const char *version;
    
    uint64_t mem_total;
    uint64_t mem_free;
    
    uint64_t swap_total;
    uint64_t swap_free;
    uint64_t swap_committed;
    
    double load_average;
    double process_cpu_load;
    double system_cpu_load;
    uint64_t process_cpu_time;
    
    uint64_t open_fd;
    uint64_t max_fd;
    
    uint32_t cpus;
} bmic_java_os_info_t;

typedef bmic_java_info_t (*bmic_complement_api_java_info_fn)(bmic_handle_t *handle,
        gru_status_t *status);

typedef bmic_java_mem_info_t (*bmic_complement_api_java_mem_info_fn)(bmic_handle_t *handle,
        gru_status_t *status);

typedef bmic_java_os_info_t (*bmic_complement_api_java_os_info_fn)(bmic_handle_t *handle,
        gru_status_t *status);

typedef struct bmic_complements_java_api_t_ {
    bmic_complement_api_java_info_fn java_info;
    bmic_complement_api_java_mem_info_fn eden_info;
    bmic_complement_api_java_mem_info_fn survivor_info;
    // aka PS Old Gen
    bmic_complement_api_java_mem_info_fn tenured_info;
    
    bmic_complement_api_java_mem_info_fn code_cache_info;
    
    // Only if memory model == JAVA_MODERN
    bmic_complement_api_java_mem_info_fn metaspace_info;
    
    // Only if memory model == JAVA_LEGACY
    bmic_complement_api_java_mem_info_fn permgen_info;
    
    bmic_complement_api_java_os_info_fn os_info;
    
} bmic_complements_java_api_t;


#ifdef __cplusplus
}
#endif

#endif /* BMIC_COMPLEMENTS_JAVA_H */

