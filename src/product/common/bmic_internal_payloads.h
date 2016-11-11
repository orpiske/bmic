/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bmic_internal_payloads.h
 * Author: opiske
 *
 * Created on November 11, 2016, 11:18 AM
 */

#ifndef BMIC_INTERNAL_PAYLOADS_H
#define BMIC_INTERNAL_PAYLOADS_H

/**
 * Wrappers for internal usage of this subcomponent
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bmic_payload_add_attr_t_ {
    gru_list_t *list;
    gru_status_t *status;
} bmic_payload_add_attr_t;


#ifdef __cplusplus
}
#endif

#endif /* BMIC_INTERNAL_PAYLOADS_H */

