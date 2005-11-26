
#ifndef LIBRARIES_STFAXPHONEBOOK_H
#define LIBRARIES_STFAXPHONEBOOK_H

#if defined(__GNUC__)
# pragma pack(2)
#endif

/*  Phonebook Entry                 */

struct PBEntry {
        struct Node     Node;
        TEXT            Number[ 60 ];
        TEXT            Name[ 60 ];
        TEXT            OldNote[ 100 ];
        TEXT            LetterHead[ 256 ];
        TEXT            Signature[ 256 ];
        TEXT            Attn[ 60 ];
        TEXT            Phone[ 60 ];
        TEXT            Greeting[ 256 ];
        TEXT            Address[ 256 ];
        TEXT            City[ 80 ];
        TEXT            ZIP[ 20 ];
        TEXT            Country[ 80 ];
        TEXT            Mobile[ 80 ];
        TEXT            Company[ 80 ];
        ULONG           ScheduleTime;       // default schedule time
        TEXT            EMail[ 80 ];
        TEXT            Note[ 2048 ];
};

/*  Types                           */

#define PBT_ENTRY       0
#define PBT_GROUP       1
#define PBT_MEMBER      2   // group member

/*  Flags (stored in Node.ln_Pri)   */

#define PB_SELECTED     (1 << 0)
#define PB_USEHEAD      (1 << 1)
#define PB_USESIGN      (1 << 2)
#define PB_MAKECOVER    (1 << 3)
#define PB_EXTENDED     (1 << 4)
#define PB_EDITING      (1 << 5)

/*  Group                           */

struct PBGroup {
        struct Node     Node;
        struct MinList  Members;
        TEXT            Name[ 60 ];
};

struct PBGMember {
        struct Node     Node;
        struct PBEntry *Entry;
};

#if defined(__GNUC__)
# pragma pack()
#endif

#endif /* LIBRARIES_STFAXPHONEBOOK_H */
