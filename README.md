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

Dependencies:
----

Runtime/Compilation:
* cmake
* gcc or clang
* [gru](https://github.com/orpiske/gru)

Usage Example:
----

Discovery:

```
$ ./bmic-client discovery -u admin -p admin -s localhost                                                          
The product name is artemis
The API version is 1.x.x
The product version is: 1.4.0
```

Displaying Core Capabilities:
```
$ ./bmic-client capabilities --read-all -u admin -p admin -s localhost
The product name is artemis
The API version is 1.x.x
The product version is: 1.4.0

  NAME                                MODE  TYPE            DESCRIPTION
- ManagementAddress                   ro    string          management address of this server
 \_ jms.queue.activemq.management
- JournalMinFiles                     ro    int             number of journal files to pre-create
 \_ 2
- Clustered                           ro    boolean         whether this server is clustered
 \_ false
- JournalCompactMinFiles              ro    int             minimal number of journal files before compacting
 \_ 10
- FailoverOnServerShutdown            rw    boolean         if clients failover on a server shutdown
 \_ false
- PersistDeliveryCountBeforeDelivery  ro    boolean         whether delivery count is persisted before messages are delivered to the consumers
 \_ false
- MessageExpiryThreadPriority         ro    long            priority of the thread used to scan message expiration
 \_ 3
- ConnectionTTLOverride               ro    long            connection time to live
 \_ -1
- JournalCompactPercentage            ro    int             percentage of live data before compacting the journal
 \_ 30
- PersistenceEnabled                  ro    boolean         whether this server is using persistence and store data
 \_ true
- CreateBindingsDir                   ro    boolean         whether the bindings directory is created on this server startu
 \_ true
- ConnectionCount                     ro    int             number of clients connected to this server
 \_ 0
- Version                             ro    string          server's version
 \_ 1.4.0
- JournalDirectory                    ro    string          file system directory used to store journal log
 \_ ./data/journal
- UptimeMillis                        ro    long            uptime of this server in milliseconds
 \_ 322921
- ManagementNotificationAddress       ro    string          management notification address of this server
 \_ activemq.notifications
- DivertNames                         ro    string          names of the diverts deployed on this server
 \_  (null)
- JournalSyncNonTransactional         ro    boolean         whether the journal is synchronized when receiving non-transactional datar
 \_ true
- TotalConnectionCount                ro    long            number of clients which have connected to this server since it was started
 \_ 0
- SharedStore                         ro    boolean         whether this server shares its data store with a corresponding live or backup serve
 \_ false
- QueueNames                          ro    string          names of the queues created on this server

 \_ test.performance.queue
 \_ jms.queue.DLQ
 \_ jms.queue.ExpiryQueue
- PagingDirectory                     ro    string          file system directory used to store paging files
 \_ ./data/paging
- LargeMessagesDirectory              ro    string          file system directory used to store large messages
 \_ ./data/large-messages
- JournalBufferSize                   ro    int             size of the internal buffer on the journal
 \_ 501760
- SecurityInvalidationInterval        ro    long            interval time (in milliseconds) to invalidate security credentials
 \_ 10000
- MessageCounterSamplePeriod          rw    long            sample period (in milliseconds) to take message counter snapshot
 \_ 10000
- TotalMessagesAdded                  ro    long            number of messages sent to this server since it was started
 \_ 0
- TotalConsumerCount                  ro    long            number of consumers consuming messages from all the queues on this server
 \_ 0
- BridgeNames                         ro    string          names of the bridges deployed on this server
 \_  (null)
- WildcardRoutingEnabled              ro    boolean         whether wildcard routing is supported by this server
 \_ true
- PersistIDCache                      ro    boolean         whether message ID cache is persisted
 \_ true
- AsyncConnectionExecutionEnabled     ro    boolean         whether code coming from connection is executed asynchronously or not
 \_ true
- Started                             ro    boolean         whether this server is started
 \_ true
- TotalMessageCount                   ro    long            number of messages in all queues on the server
 \_ 0
- CreateJournalDir                    ro    boolean         whether the journal directory is created on this server startup
 \_ true
- Backup                              ro    boolean         whether this server is a backup
 \_ false
- AddressNames                        ro    string          addresses created on this server

 \_ jms.queue.DLQ
 \_ jms.queue.ExpiryQueue
 \_ test.performance.queue
- TotalMessagesAcknowledged           ro    long            number of messages acknowledged from all the queues on this server since it was started
 \_ 0
- JournalBufferTimeout                ro    int             timeout (in nanoseconds) used to flush internal buffers on the journal
 \_ 5564000
- IncomingInterceptorClassNames       ro    string          list of interceptors used by this server for incoming messages
 \_  (null)
Unable to read the capability ReplicaSync (wrong cap, maybe?): (null)
- ConnectorsAsJSON                    ro    string          connectors configured for this server using JSON serialization
 \_ []
- JournalFileSize                     ro    int             size (in bytes) of each journal files
 \_ 10485760
- TransactionTimeout                  ro    long            timeout (in milliseconds) after which transactions is removed from the resource manager after it was created
 \_ 300000
- MessageCounterEnabled               ro    boolean         whether message counter is enabled for this server
 \_ false
- OutgoingInterceptorClassNames       ro    string          list of interceptors used by this server for outgoing messages
 \_  (null)
- IDCacheSize                         ro    int             size of the cache for pre-creating message IDs
 \_ 20000
- Uptime                              ro    string          uptime of this server
 \_ 5 minutes
- JournalSyncTransactional            ro    boolean         whether the journal is synchronized when receiving transactional data
 \_ true
- ScheduledThreadPoolMaxSize          ro    int             maximum number of threads in the scheduled thread pool
 \_ 5
- TransactionTimeoutScanPeriod        ro    long            frequency (in milliseconds)  to scan transactions to detect which transactions have timed out
 \_ 1000
- SecurityEnabled                     ro    boolean         whether security is enabled for this server
 \_ true
- BindingsDirectory                   ro    string          file system directory used to store bindings
 \_ ./data/bindings
- Connectors                          ro    object          connectors configured for this server
 \_  (null)
- JournalMaxIO                        ro    int             maximum number of write requests that can be in the AIO queue at any given time
 \_ 1
- MessageExpiryScanPeriod             ro    long            frequency (in milliseconds)  to scan messages to detect which messages have expired
 \_ 30000
- MessageCounterMaxDayCount           rw    int             maximum number of days kept in memory for message counter
 \_ 10
- JournalType                         ro    string          type of journal used by this server
 \_ NIO
- ThreadPoolMaxSize                   ro    int             maximum number of threads in the thread pool
 \_ 30

```

You can also read individual capabilities/attributes: 

```
$ ./bmic-client queue -r QueueSize -n test.performance.queue -u admin -p admin -s [hostname]
Product name is activemq
API version is 5.x.x
The product version is: 5.13.3
 \_ 1
```

License
----

The code is licensed under Apache License v2
