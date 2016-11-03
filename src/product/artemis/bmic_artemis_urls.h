/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bmic_artemis_urls.h
 * Author: opiske
 *
 * Created on November 3, 2016, 2:51 PM
 */

#ifndef BMIC_ARTEMIS_URLS_H
#define BMIC_ARTEMIS_URLS_H

/**
 * Base URLs must not have the '/' char at the end
 */
#define ARTEMIS_BASE_URL_HINT_URL "%s/jolokia"
#define ARTEMIS_BASE_URL_HINT_ADDRESSING "http://%s:%i/jolokia"
#define ARTEMIS_PRODUCT_INFO_PATH "read/org.apache.activemq.artemis:brokerName=\"0.0.0.0\",module=Core,serviceType=Server,type=Broker/Version"

#endif /* BMIC_ARTEMIS_URLS_H */

