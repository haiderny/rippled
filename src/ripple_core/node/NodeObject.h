//------------------------------------------------------------------------------
/*
    Copyright (c) 2011-2013, OpenCoin, Inc.
*/
//==============================================================================

#ifndef RIPPLE_CORE_NODE_NODEOBJECT_H_INCLUDED
#define RIPPLE_CORE_NODE_NODEOBJECT_H_INCLUDED

/** The types of node objects.
*/
enum NodeObjectType
{
    hotUNKNOWN = 0,
    hotLEDGER = 1,
    hotTRANSACTION = 2,
    hotACCOUNT_NODE = 3,
    hotTRANSACTION_NODE = 4
};

/** A blob of data with associated metadata, referenced by hash.

    The metadata includes the following:

    - Type of the blob
    - The ledger index in which it appears
    - The SHA 256 hash

    @note No checking is performed to make sure the hash matches the data.
    @see SHAMap
*/
class NodeObject : public CountedObject <NodeObject>
{
public:
    static char const* getCountedObjectName () { return "NodeObject"; }

    enum
    {
        /** Size of the fixed keys, in bytes.

            We use a 256-bit hash for the keys.

            @see NodeObject
        */
        keyBytes = 32,
    };

    /** The type used to hold the hash.

        The hahes are fixed size, SHA256.

        @note The key size can be retrieved with `Hash::sizeInBytes`
    */
    typedef UnsignedInteger <32> Hash;

    // Please use this one. For a reference use Ptr const&
    typedef boost::shared_ptr <NodeObject> Ptr;

    // These are DEPRECATED, type names are capitalized.
    typedef boost::shared_ptr <NodeObject> pointer;
    typedef pointer const& ref;

private:
    // This hack is used to make the constructor effectively private
    // except for when we use it in the call to make_shared.
    // There's no portable way to make make_shared<> a friend work.
    struct PrivateAccess { };
public:
    // This constructor is private, use createObject instead.
    NodeObject (NodeObjectType type,
                LedgerIndex ledgerIndex,
                Blob& data,
                uint256 const& hash,
                PrivateAccess);

    /** Create an object from fields.

        The caller's variable is modified during this call. The
        underlying storage for the Blob is taken over by the NodeObject.

        @param type The type of object.
        @param ledgerIndex The ledger in which this object appears.
        @param data A buffer containing the payload. The caller's variable
                    is overwritten.
        @param hash The 256-bit hash of the payload data.
    */
    static Ptr createObject (NodeObjectType type,
                             LedgerIndex ledgerIndex,
                             Blob& data,
                             uint256 const& hash);

    /** Retrieve the type of this object.
    */
    NodeObjectType getType () const;

    /** Retrieve the hash metadata.
    */
    uint256 const& getHash () const;

    /** Retrieve the ledger index in which this object appears.
    */
    // VFALCO TODO rename to getLedgerIndex or getLedgerId
    LedgerIndex getIndex () const;

    /** Retrieve the binary data.
    */
    Blob const& getData () const;

    /** See if this object has the same data as another object.
    */
    bool isCloneOf (NodeObject::Ptr const& other) const;

    /** Binary function that satisfies the strict-weak-ordering requirement.

        This compares the hashes of both objects and returns true if
        the first hash is considered to go before the second.

        @see std::sort
    */
    struct LessThan
    {
        inline bool operator() (NodeObject::Ptr const& lhs, NodeObject::Ptr const& rhs) const noexcept
        {
            return lhs->getHash () < rhs->getHash ();
        }
    };

private:
    NodeObjectType mType;
    uint256 mHash;
    LedgerIndex mLedgerIndex;
    Blob mData;
};

#endif