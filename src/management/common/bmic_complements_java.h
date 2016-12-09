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
    int64_t init;
    int64_t committed;
    int64_t max;
    int64_t used;
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

static inline void bmic_java_info_cleanup(bmic_java_info_t info) {
    gru_dealloc_string((char **) &info.name);
    gru_dealloc_string((char **) &info.version);
    gru_dealloc_string((char **) &info.jvm_package_version);
};

typedef struct bmic_java_os_info_t_ {
    const char *name;
    const char *arch;
    const char *version;
    
    int64_t mem_total;
    int64_t mem_free;
    
    int64_t swap_total;
    int64_t swap_free;
    int64_t swap_committed;
    
    double load_average;
    double process_cpu_load;
    double system_cpu_load;
    int64_t process_cpu_time;
    
    int64_t open_fd;
    int64_t max_fd;
    
    uint32_t cpus;
} bmic_java_os_info_t;

static inline void bmic_java_os_info_cleanup(bmic_java_os_info_t info) {
    gru_dealloc_string((char **) &info.name);
    gru_dealloc_string((char **) &info.arch);
    gru_dealloc_string((char **) &info.version);
}

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

