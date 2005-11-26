#ifndef _MACROS_H
#define _MACROS_H

/****************************************************************************/

#ifdef __MORPHOS__
#define SAVEDS   __saveds
#define REGARGS  __regargs
#define STDARGS  __attribute__((stkparm))
#define INLINE   __inline
#define REG(x,p) p __asm( #x )
#define REGARRAY
#define ASM

#define M_HOOK(n,y,z) \
    static LONG n##_GATE(void); \
    static LONG n##_GATE2(struct Hook *hook,y,z); \
    static struct EmulLibEntry n = {TRAP_LIB,0,(void (*)(void))n##_GATE}; \
    static LONG n##_GATE(void) {return (n##_GATE2((void *)REG_A0,(void *)REG_A2,(void *)REG_A1));} \
    static struct Hook n##_hook = {{0,0},(void *)&n}; \
    static LONG n##_GATE2(struct Hook *hook, y, z)

#define M_DISP(n) static ULONG _##n(void)
#define M_DISPSTART \
    struct IClass *cl = (struct IClass *)REG_A0; \
    Object        *obj = (Object *)REG_A2; \
    Msg            msg  = (Msg)REG_A1;
#define M_DISPEND(n) static struct EmulLibEntry n = {TRAP_LIB,0,(void *)&_##n};
#define DISP(n) ((APTR)&n)

#define copymem(to,from,len) CopyMem((APTR)(from),(APTR)(to),(ULONG)(len))

#else /*__MORPHOS__ */

#define SAVEDS   __saveds
#define ASM      __asm
#define REGARGS  __regargs
#define STDARGS  __stdargs
#define INLINE   __inline
#define REG(x,p) register __ ## x p
#define REGARRAY register

#define max(a,b) __builtin_max(a,b)
#define min(a,b) __builtin_min(a,b)

#define M_HOOK(n,y,z) \
    static LONG SAVEDS ASM n##_func(REG(a0,struct Hook *hook),REG(a2,y),REG(a1,z)); \
    static struct Hook n##_hook = {0,0,(HOOKFUNC)n##_func}; \
    static LONG ASM SAVEDS n##_func(REG(a0,struct Hook *hook),REG(a2,y),REG(a1,z))

#define M_DISP(n) static ULONG ASM SAVEDS n(REG(a0,struct IClass *cl),REG(a2,Object *obj),REG(a1,Msg msg))
#define M_DISPSTART
#define M_DISPEND(n)
#define DISP(n) (n)

#define copymem(to,from,len) memcpy((APTR)(to),(APTR)(from),(ULONG)(len))

#endif /*__MORPHOS__ */

#ifndef NODE
#define NODE(a) ((struct Node *)(a))
#endif /* NODE */

#ifndef MINNODE
#define MINNODE(a) ((struct MinNode *)(a))
#endif /* MINNODE */

#ifndef LIST
#define LIST(a) ((struct List *)(a))
#endif /* LIST */

#ifndef MINLIST
#define MINLIST(a) ((struct MinList *)(a))
#endif /* MINLIST */

#ifndef MESSAGE
#define MESSAGE(m) ((struct Message *)(m))
#endif /* MESSAGE */

#undef NEWLIST
#define NEWLIST(l) (LIST(l)->lh_Head = NODE(&LIST(l)->lh_Tail), \
                    LIST(l)->lh_Tail = NULL, \
                    LIST(l)->lh_TailPred = NODE(&LIST(l)->lh_Head))

#ifndef QUICKNEWLIST
#define QUICKNEWLIST(l) (LIST(l)->lh_Head = NODE(&LIST(l)->lh_Tail), \
                         LIST(l)->lh_TailPred = NODE(&LIST(l)->lh_Head))
#endif /* QUICKNEWLIST */

#undef ADDTAIL
#define ADDTAIL(l,n) AddTail(LIST(l),NODE(n))

#ifndef PORT
#define PORT(port) ((struct MsgPort *)(port))
#endif /* PORT */

#undef INITPORT
#define INITPORT(p,s) (PORT(p)->mp_Flags = PA_SIGNAL, \
                       PORT(p)->mp_SigBit = (UBYTE)(s), \
                       PORT(p)->mp_SigTask = FindTask(NULL), \
                       NEWLIST(&(PORT(p)->mp_MsgList)))

#ifndef QUICKINITPORT
#define QUICKINITPORT(p,s,t) (PORT(p)->mp_Flags = PA_SIGNAL, \
                              PORT(p)->mp_SigBit = (UBYTE)(s), \
                              PORT(p)->mp_SigTask = (t), \
                              QUICKNEWLIST(&(PORT(p)->mp_MsgList)))
#endif /* QUICKINITPORT */

#ifndef INITMESSAGE
#define INITMESSAGE(m,p,l) (MESSAGE(m)->mn_Node.ln_Type = NT_MESSAGE, \
                            MESSAGE(m)->mn_ReplyPort = PORT(p), \
                            MESSAGE(m)->mn_Length = ((UWORD)l))
#endif /* INITMESSAGE */

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif /* MAKE_ID */

#ifndef BOOLSAME
#define BOOLSAME(a,b) (((a) ? TRUE : FALSE)==((b) ? TRUE : FALSE))
#endif /* BOOLSAME */

#ifndef RAWIDTH
#define RAWIDTH(w) ((((UWORD)(w))+15)>>3 & 0xFFFE)
#endif /* RAWIDTH */

#ifndef superset
#define superset(cl,obj,tag,val) SetSuperAttrs(cl,obj,tag,val,TAG_DONE)
#endif /* superset */

#ifndef superget
#define superget(cl,obj,tag,storage) DoSuperMethod(cl,obj,OM_GET,tag,storage)
#endif /* superget */

#define MUIVER20 20

#ifndef IDCMP_MOUSEOBJECT
#define IDCMP_MOUSEOBJECT 0x40000000
#endif /* IDCMP_MOUSEOBJECT */

#define IDSSIZE(a) (sizeof(a)/sizeof(ULONG))

/****************************************************************************/

#endif /* _MACROS_H */

