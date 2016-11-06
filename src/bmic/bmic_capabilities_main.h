/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bmic_capabilities_main.h
 * Author: opiske
 *
 * Created on November 6, 2016, 7:36 PM
 */

#ifndef BMIC_CAPABILITIES_MAIN_H
#define BMIC_CAPABILITIES_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "base/transport/bmic_endpoint.h"
#include "base/transport/bmic_endpoint_http.h"
#include "base/format/bmic_json.h"


#include "management/common/bmic_product.h"
#include "management/common/bmic_discovery_hint.h"
#include "management/discovery/bmic_discovery.h"
#include "product/bmic_product_register.h"

#include "base/common/bmic_regex.h"

#define OPT_MAX_STR_SIZE 255

#ifdef __cplusplus
extern "C" {
#endif
    
int capabilities_main(int argc, char **argv);


#ifdef __cplusplus
}
#endif

#endif /* BMIC_CAPABILITIES_MAIN_H */

