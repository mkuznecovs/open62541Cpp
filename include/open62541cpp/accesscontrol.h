#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H
struct UA_AccessControl {
    void* context;
    void (*clear)(UA_AccessControl* ac);

    /* Supported login mechanisms. The server endpoints are created from here. */
    size_t userTokenPoliciesSize;
    UA_UserTokenPolicy* userTokenPolicies;

    /* Authenticate a session. The session context is attached to the session
     * and later passed into the node-based access control callbacks. The new
     * session is rejected if a StatusCode other than UA_STATUSCODE_GOOD is
     * returned. */
    UA_StatusCode (*activateSession)(UA_Server* server,
                                     UA_AccessControl* ac,
                                     const UA_EndpointDescription* endpointDescription,
                                     const UA_ByteString* secureChannelRemoteCertificate,
                                     const UA_NodeId* sessionId,
                                     const UA_ExtensionObject* userIdentityToken,
                                     void** sessionContext);

    /* Deauthenticate a session and cleanup */
    void (*closeSession)(UA_Server* server, UA_AccessControl* ac, const UA_NodeId* sessionId, void* sessionContext);

    /* Access control for all nodes*/
    UA_UInt32 (*getUserRightsMask)(UA_Server* server,
                                   UA_AccessControl* ac,
                                   const UA_NodeId* sessionId,
                                   void* sessionContext,
                                   const UA_NodeId* nodeId,
                                   void* nodeContext);

    /* Additional access control for variable nodes */
    UA_Byte (*getUserAccessLevel)(UA_Server* server,
                                  UA_AccessControl* ac,
                                  const UA_NodeId* sessionId,
                                  void* sessionContext,
                                  const UA_NodeId* nodeId,
                                  void* nodeContext);

    /* Additional access control for method nodes */
    UA_Boolean (*getUserExecutable)(UA_Server* server,
                                    UA_AccessControl* ac,
                                    const UA_NodeId* sessionId,
                                    void* sessionContext,
                                    const UA_NodeId* methodId,
                                    void* methodContext);

    /* Additional access control for calling a method node in the context of a
     * specific object */
    UA_Boolean (*getUserExecutableOnObject)(UA_Server* server,
                                            UA_AccessControl* ac,
                                            const UA_NodeId* sessionId,
                                            void* sessionContext,
                                            const UA_NodeId* methodId,
                                            void* methodContext,
                                            const UA_NodeId* objectId,
                                            void* objectContext);

    /* Allow adding a node */
    UA_Boolean (*allowAddNode)(UA_Server* server,
                               UA_AccessControl* ac,
                               const UA_NodeId* sessionId,
                               void* sessionContext,
                               const UA_AddNodesItem* item);

    /* Allow adding a reference */
    UA_Boolean (*allowAddReference)(UA_Server* server,
                                    UA_AccessControl* ac,
                                    const UA_NodeId* sessionId,
                                    void* sessionContext,
                                    const UA_AddReferencesItem* item);

    /* Allow deleting a node */
    UA_Boolean (*allowDeleteNode)(UA_Server* server,
                                  UA_AccessControl* ac,
                                  const UA_NodeId* sessionId,
                                  void* sessionContext,
                                  const UA_DeleteNodesItem* item);

    /* Allow deleting a reference */
    UA_Boolean (*allowDeleteReference)(UA_Server* server,
                                       UA_AccessControl* ac,
                                       const UA_NodeId* sessionId,
                                       void* sessionContext,
                                       const UA_DeleteReferencesItem* item);

    /* Allow browsing a node */
    UA_Boolean (*allowBrowseNode)(UA_Server* server,
                                  UA_AccessControl* ac,
                                  const UA_NodeId* sessionId,
                                  void* sessionContext,
                                  const UA_NodeId* nodeId,
                                  void* nodeContext);

#ifdef UA_ENABLE_SUBSCRIPTIONS
    /* Allow transfer of a subscription to another session. The Server shall
     * validate that the Client of that Session is operating on behalf of the
     * same user */
    UA_Boolean (*allowTransferSubscription)(UA_Server* server,
                                            UA_AccessControl* ac,
                                            const UA_NodeId* oldSessionId,
                                            void* oldSessionContext,
                                            const UA_NodeId* newSessionId,
                                            void* newSessionContext);
#endif

#ifdef UA_ENABLE_HISTORIZING
    /* Allow insert,replace,update of historical data */
    UA_Boolean (*allowHistoryUpdateUpdateData)(UA_Server* server,
                                               UA_AccessControl* ac,
                                               const UA_NodeId* sessionId,
                                               void* sessionContext,
                                               const UA_NodeId* nodeId,
                                               UA_PerformUpdateType performInsertReplace,
                                               const UA_DataValue* value);

    /* Allow delete of historical data */
    UA_Boolean (*allowHistoryUpdateDeleteRawModified)(UA_Server* server,
                                                      UA_AccessControl* ac,
                                                      const UA_NodeId* sessionId,
                                                      void* sessionContext,
                                                      const UA_NodeId* nodeId,
                                                      UA_DateTime startTimestamp,
                                                      UA_DateTime endTimestamp,
                                                      bool isDeleteModified);
#endif
};
#endif  // ACCESSCONTROL_H
