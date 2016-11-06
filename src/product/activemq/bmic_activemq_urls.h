/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   bmic_activemq_urls.h
 * Author: opiske
 *
 * Created on November 3, 2016, 2:44 PM
 */

#ifndef BMIC_ACTIVEMQ_URLS_H
#define BMIC_ACTIVEMQ_URLS_H

/**
 * Base URLs must not have the '/' char at the end
 */
#define ACTIVEMQ_BASE_URL_HINT_URL "%s/api/jolokia"
#define ACTIVEMQ_BASE_URL_HINT_ADDRESSING "http://%s:%i/api/jolokia"
#define ACTIVEMQ_PRODUCT_INFO_PATH "read/org.apache.activemq:brokerName=localhost,type=Broker/BrokerVersion"
#define ACTIVEMQ_PRODUCT_CAPABILITIES "list/org.apache.activemq"

#define ACTIVEMQ_CAPABILITIES_KEY_REGEX "(brokerName=)([a-z0-9\\.])*(,type=Broker)"
#define ACTIVEMQ_CORE_CAP_ATTRIBUTES "(brokerName=)([a-z0-9\\.])*(,type=Broker\\/attr$)"

#endif /* BMIC_ACTIVEMQ_URLS_H */

