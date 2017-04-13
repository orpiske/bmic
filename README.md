BMIC: Broker Management Interface Client
============

Build Status
----
Linux Build Status: [![Linux Build Status](https://travis-ci.org/orpiske/bmic.svg?branch=master)](https://travis-ci.org/orpiske/bmic)

Coverity Scan: [![Coverity Scan Build Status](https://scan.coverity.com/projects/10840/badge.svg)](https://scan.coverity.com/projects/orpiske-bmic)

Introduction:
----

This project contains a library and a command-line client that allows you to
access the management interface for messaging brokers such as Apache ActiveMQ,
Apache Artemis, JBoss A-MQ 6 and JBoss A-MQ 7.

This is a work in progress.

Dependencies:
----

Runtime/Compilation:
* cmake
* gcc or clang
* [gru](https://github.com/orpiske/gru)
* [json-c](https://github.com/json-c/json-c)


Usage Example:
----

Discovery:

```
bmic discovery -u admin -p admin -s localhost
The product name is artemis
The API version is 1.x.x
The product version is: 1.4.0
```

Displaying Core Capabilities:
```
bmic capabilities --read-all -u admin -p admin -s localhost                          
The product name is artemis
The API version is 1.x.x
The product version is: 1.4.0

                         Capability Value                    
                  ManagementAddress jms.queue.activemq.management
                    JournalMinFiles 2
                          Clustered false
             JournalCompactMinFiles 10
           FailoverOnServerShutdown false
 PersistDeliveryCountBeforeDelivery false
        MessageExpiryThreadPriority 3
              ConnectionTTLOverride -1
           JournalCompactPercentage 30
                 PersistenceEnabled true
                  CreateBindingsDir true
                    ConnectionCount 0
                            Version 1.4.0                    
                   JournalDirectory ./data/journal           
                       UptimeMillis 6345586
      ManagementNotificationAddress activemq.notifications   
                        DivertNames (null)
        JournalSyncNonTransactional true
               TotalConnectionCount 0
                        SharedStore false
                         QueueNames 
                                    jms.queue.DLQ
                                    jms.queue.ExpiryQueue
                                    test.performance.queue
                    PagingDirectory ./data/paging            
             LargeMessagesDirectory ./data/large-messages    
                  JournalBufferSize 501760
       SecurityInvalidationInterval 10000
         MessageCounterSamplePeriod 10000
                 TotalMessagesAdded 0
                 TotalConsumerCount 0
                        BridgeNames (null)
             WildcardRoutingEnabled true
                     PersistIDCache true
    AsyncConnectionExecutionEnabled true
                            Started true
                  TotalMessageCount 0
                   CreateJournalDir true
                             Backup false
                       AddressNames 
                                    jms.queue.DLQ
                                    jms.queue.ExpiryQueue
                                    test.performance.queue
          TotalMessagesAcknowledged 0
               JournalBufferTimeout 5564000
      IncomingInterceptorClassNames (null)
                        ReplicaSync Unable to read
                   ConnectorsAsJSON Unable to read
                    JournalFileSize Unable to read
                 TransactionTimeout Unable to read
              MessageCounterEnabled Unable to read
      OutgoingInterceptorClassNames Unable to read
                        IDCacheSize Unable to read
                             Uptime Unable to read
           JournalSyncTransactional Unable to read
         ScheduledThreadPoolMaxSize Unable to read
       TransactionTimeoutScanPeriod Unable to read
                    SecurityEnabled Unable to read
                  BindingsDirectory Unable to read
                         Connectors Unable to read
                       JournalMaxIO Unable to read
            MessageExpiryScanPeriod Unable to read
          MessageCounterMaxDayCount Unable to read
                        JournalType Unable to read
                  ThreadPoolMaxSize Unable to read

```

You can also read individual capabilities/attributes: 
```
./bmic capabilities -r LargeMessagesDirectory -u admin -p admin -s localhost       
The product name is artemis
The API version is 1.x.x
The product version is: 1.4.0

                         Capability Value                    
             LargeMessagesDirectory ./data/large-messages
```

From queues too:
```
bmic queue -r -a MessageCount -n test.performance.queue -u admin -p admin -s localhost
The product name is artemis
The API version is 1.x.x
The product version is: 1.4.0
                       MessageCount 0
```

Listing queues:

```
bmic queue -l -u admin -p admin -s localhost
The product name is artemis
The API version is 1.x.x
The product version is: 1.4.0
Queue name: jms.queue.jms.queue.cli2.test.notcore
Queue name: jms.queue.DLQ
Queue name: jms.queue.ExpiryQueue
Queue name: test.performance.queue
Queue name: jms.queue.cli1.test.notcore
Queue name: jms.queue.cli2.test.notcore
```

Creating queues:
```
./bmic queue -c -n new.queue -u admin -p admin -s localhost
```

Deleting queues:
```
./bmic queue -d -n new.queue -u admin -p admin -s localhost
```


License
----

The code is licensed under Apache License v2
