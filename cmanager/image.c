/***************************************************************************
    revision             : $Id: image.c,v 1.1.1.1 2003-01-06 15:52:22 tellini Exp $
	copyright            : (C) 1999-2003 by Simone Tellini
	email                : tellini@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef NOT_USED
/// Include
#include <stdlib.h>
#include <stdio.h>

#define INTUI_V36_NAMES_ONLY
#define ASL_V38_NAMES_ONLY
#define CATCOMP_NUMBERS

#include <exec/types.h>                 // exec
#include <exec/memory.h>
#include <exec/libraries.h>
#include <exec/execbase.h>
#include <intuition/intuition.h>        // intuition
#include <intuition/screens.h>
#include <graphics/gfx.h>               // graphics
#include <graphics/modeid.h>
#include <libraries/mui.h>              // libraries
#include <clib/exec_protos.h>           // protos
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/alib_protos.h>
#include <clib/utility_protos.h>
#include <pragmas/exec_pragmas.h>       // pragmas
#include <pragmas/dos_pragmas.h>
#include <pragmas/intuition_pragmas.h>
#include <pragmas/graphics_pragmas.h>
#include <pragmas/muimaster_pragmas.h>
#include <pragmas/utility_pragmas.h>

#include <mymacros.h>

#include "CManager.h"
#include "mui.h"
#include "image.h"

#include "Library/CManager_protos.h"
#include "Library/CManager_lib_lib.h"
///
/// Prototypes
static ULONG    ImageGad_New( struct IClass *, Object *, struct opSet * );
static ULONG    ImageGad_Dispose( struct IClass *, Object *, Msg );
static ULONG    ImageGad_AskMinMax( struct IClass *, Object *, struct MUIP_AskMinMax * );
static ULONG    ImageGad_Draw( struct IClass *, Object *, struct MUIP_Draw * );
static ULONG    ImageGad_Set( struct IClass *, Object *, struct opSet * );
static ULONG    ImageGad_Setup( struct IClass *, Object *, Msg );
static ULONG    ImageGad_Cleanup( struct IClass *, Object *, Msg );

static void     FreeImage( Object *, struct ImageGadData * );

static UBYTE   *PlanarToChunky( struct CMImage * );
static void     Remap( struct CMImage *, Object *, struct ImageGadData * );
///
/// Data
struct MUI_CustomClass *CL_ImageGad;
///


/// ImageGad_Dispatcher
ULONG SAVEDS ASM ImageGad_Dispatcher( REG( a0 ) struct IClass *cl, REG( a2 ) Object *obj, REG( a1 ) Msg msg )
{
    switch( msg->MethodID ) {

        case OM_NEW:
            return( ImageGad_New( cl, obj, (APTR)msg ));

        case OM_SET:
            return( ImageGad_Set( cl, obj, (APTR)msg ));

        case OM_DISPOSE:
            return( ImageGad_Dispose( cl, obj, msg ));

        case MUIM_Setup:
            return( ImageGad_Setup( cl, obj, (APTR)msg ));

        case MUIM_Cleanup:
            return( ImageGad_Cleanup( cl, obj, (APTR)msg ));

        case MUIM_AskMinMax:
            return( ImageGad_AskMinMax( cl, obj, (APTR)msg ));

        case MUIM_Draw:
            return( ImageGad_Draw( cl, obj, (APTR)msg ));
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// ImageGad_New
static ULONG ImageGad_New( struct IClass *cl, Object *obj, struct opSet *msg )
{

    if( obj = ( Object * )DoSuperNew( cl, obj,
                                      TAG_MORE, msg->ops_AttrList )) {

        struct ImageGadData    *data = INST_DATA( cl, obj );

        data->UsedPens  = 0;
        data->AllocPens = FALSE;
        data->Pens      = NULL;
        data->BitMap    = NULL;

        return(( ULONG )obj );
    }

    return( 0 );
}
///
/// ImageGad_Set
static ULONG ImageGad_Set( struct IClass *cl, Object *obj, struct opSet *msg )
{
    struct ImageGadData    *data = INST_DATA( cl, obj );
    struct TagItem         *tags, *tag;


    for( tags = msg->ops_AttrList; tag = NextTagItem( &tags ); ) {

        switch( tag->ti_Tag ) {

            case MUIA_ImageGad_Image:

                FreeImage( obj, data );

                Remap( data->Image = ( struct CMImage * )tag->ti_Data, obj, data );

                MUI_Redraw( obj, MADF_DRAWUPDATE );
                break;
        }
    }

    return( DoSuperMethodA( cl, obj, (Msg) msg ));
}
///
/// ImageGad_Setup
static ULONG ImageGad_Setup( struct IClass *cl, Object *obj, Msg msg )
{
    struct ImageGadData    *data = INST_DATA( cl, obj );

    if(!( DoSuperMethodA( cl, obj, msg )))
        return( FALSE );

    if( data->Pens && data->AllocPens ) {
        ULONG   i;

        for( i = 0; i < data->UsedPens; i++ ) {
            TEXT    pen[28];

            sprintf( pen, "r%08lx,%08lx,%08lx",
                     ((ULONG *)data->Image->Colors )[( i * 3 ) + 0 ],
                     ((ULONG *)data->Image->Colors )[( i * 3 ) + 1 ],
                     ((ULONG *)data->Image->Colors )[( i * 3 ) + 2 ]);

            data->Pens[ i ] = MUI_ObtainPen( muiRenderInfo( obj ), (APTR)pen, 0 );
        }

        data->AllocPens = FALSE;
    }

    return( TRUE );
}
///
/// ImageGad_Cleanup
static ULONG ImageGad_Cleanup( struct IClass *cl, Object *obj, Msg msg )
{
    struct ImageGadData    *data = INST_DATA( cl, obj );

    if( data->Pens && ( data->AllocPens == FALSE )) {
        ULONG   i;

        for( i = 0; i < data->UsedPens; i++ )
            MUI_ReleasePen( muiRenderInfo( obj ), data->Pens[ i ]);

        data->AllocPens = TRUE;
    }

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// ImageGad_Dispose
static ULONG ImageGad_Dispose( struct IClass *cl, Object *obj, Msg msg )
{
    struct ImageGadData    *data = INST_DATA( cl, obj );

    FreeImage( obj, data );

    return( DoSuperMethodA( cl, obj, msg ));
}
///
/// ImageGad_AskMinMax
static ULONG ImageGad_AskMinMax( struct IClass *cl, Object *obj, struct MUIP_AskMinMax *msg )
{
    struct ImageGadData    *data = INST_DATA( cl, obj );

    DoSuperMethodA( cl, obj, (Msg) msg );

    msg->MinMaxInfo->MinWidth  += IMG_WIDTH;
    msg->MinMaxInfo->DefWidth  += IMG_WIDTH;
    msg->MinMaxInfo->MaxWidth  += IMG_WIDTH;

    msg->MinMaxInfo->MinHeight += IMG_HEIGHT;
    msg->MinMaxInfo->DefHeight += IMG_HEIGHT;
    msg->MinMaxInfo->MaxHeight += IMG_HEIGHT;

    return( 0 );
}
///
/// ImageGad_Draw
static ULONG ImageGad_Draw( struct IClass *cl, Object *obj, struct MUIP_Draw *msg )
{
    struct ImageGadData    *data = INST_DATA( cl, obj );

    DoSuperMethodA( cl, obj, (Msg) msg );

    if(( msg->flags & ( MADF_DRAWUPDATE | MADF_DRAWOBJECT )) && data->BitMap ) {

        BltBitMapRastPort( data->BitMap, 0, 0, _rp( obj ),
                           _mleft( obj ), _mtop( obj ), IMG_WIDTH, IMG_HEIGHT, 0xC0 );
    }

    return( 0 );
}
///

/// FreeImage
static void FreeImage( Object *obj, struct ImageGadData *data )
{
    if( data->Pens && ( data->AllocPens == FALSE )) {
        ULONG   i;

        for( i = 0; i < data->UsedPens; i++ )
            MUI_ReleasePen( muiRenderInfo( obj ), data->Pens[ i ]);
    }

    data->AllocPens = TRUE;

    FreeVec( data->Pens );

    CM_DeleteBitMap( data->BitMap );

    data->UsedPens = 0;
    data->Pens     = NULL;
    data->BitMap   = NULL;
}
///

#define MINTERM_ONE         ABC | ABNC | ANBC | ANBNC | NABC | NABNC | NANBC | NANBNC
#define MINTERM_COPY        ABC | ABNC | NABC | NABNC
#define MINTERM_NOT_C       ABNC | ANBNC | NABNC | NANBNC
#define MINTERM_B_AND_C     ABC | NABC
#define MINTERM_NOT_B_AND_C ANBC | NANBC
#define MINTERM_B_OR_C      ABC | ABNC | NABC | NABNC | ANBC | NANBC

/// Remap
static void Remap( struct CMImage *Image, Object *obj, struct ImageGadData *data )
{
//    UBYTE  *ChunkyImg;

    if( Image == NULL )
        return;

/*    if( ChunkyImg = PlanarToChunky( Image )) {
        UBYTE  *usedcolors;

        if( usedcolors = AllocVec(( 1 << Image->BitMap->Depth ) * sizeof( *usedcolors ), MEMF_ANY ))*/ {
            ULONG   i;
//            UBYTE  *ptr;

//            ptr = ChunkyImg;

            data->UsedPens = 1 << Image->BitMap->Depth;

            /*  Build an array with the used pens   */
/*            for( i = 0; i < IMG_WIDTH * IMG_HEIGHT; i++ ) {
                UBYTE   c;
                ULONG   n;
                BOOL    add;

                c   = *ptr++;
                add = TRUE;

                for( n = 0; n < data->UsedPens; n++ )
                    if( c == usedcolors[ n ]) {
                        add = FALSE;
                        break;
                    }

                if( add )
                    usedcolors[ data->UsedPens++ ] = c;
            }
*/
            if( data->Pens = AllocVec( data->UsedPens * sizeof( ULONG ), MEMF_ANY )) {

                /*  Alloc the pens  */
                for( i = 0; i < data->UsedPens; i++ ) {
                    TEXT    pen[28];

                    sprintf( pen, "r%08lx,%08lx,%08lx",
                             ((ULONG *)Image->Colors )[( /*usedcolors[*/ i /*]*/ * 3 ) + 0 ],
                             ((ULONG *)Image->Colors )[( /*usedcolors[*/ i /*]*/ * 3 ) + 1 ],
                             ((ULONG *)Image->Colors )[( /*usedcolors[*/ i /*]*/ * 3 ) + 2 ]);

                    data->Pens[ i ] = MUI_ObtainPen( muiRenderInfo( obj ), (APTR)pen, 0 );
                }

                data->AllocPens = FALSE;

                if( data->BitMap = CM_CreateBitMap( IMG_WIDTH, IMG_HEIGHT, Image->BitMap->Depth, BMF_CLEAR, _screen( obj )->RastPort.BitMap )) {
                    struct BitMap  *single;

                    if( single = CM_CreateBitMap( IMG_WIDTH, IMG_HEIGHT, 1, 0, NULL )) {
                        struct BitMap   full;
                        LONG            mask = ( 1 << Image->BitMap->Depth ) - 1;

                        InitBitMap( &full, Image->BitMap->Depth, IMG_WIDTH, IMG_HEIGHT );

                        for( i = 0; i < full.Depth; i++ )
                            full.Planes[ i ] = single->Planes[ 0 ];

                        /* Run down the colours. */
                        for( i = 1; i <= mask; i++ ) {

                            /* Set the single plane bitmap to all 1's. */
                            BltBitMap( single, 0, 0, single, 0, 0, IMG_WIDTH, IMG_HEIGHT, MINTERM_ONE, 1, NULL );

                            /* Isolate the pixels to match the colour specified in `i'. */
                            BltBitMap( Image->BitMap, 0, 0, &full, 0, 0, IMG_WIDTH, IMG_HEIGHT, MINTERM_B_AND_C, i, NULL );

                            if( mask ^ i )
                                BltBitMap( Image->BitMap, 0, 0, &full, 0, 0, IMG_WIDTH, IMG_HEIGHT, MINTERM_NOT_B_AND_C, mask ^ i, NULL );

                            /* Set the pixels in the destination bitmap, use the designated colour. */
                            BltBitMap( &full, 0, 0, data->BitMap, 0, 0, IMG_WIDTH, IMG_HEIGHT, MINTERM_B_OR_C, data->Pens[ i - 1 ], NULL );
                        }

                        CM_DeleteBitMap( single );
                    }

/*                    UBYTE   planes;
                    ULONG   offy;

                    ptr    = ChunkyImg;
                    planes = Image->BitMap->Depth;
                    offy   = 0;

                    for( i = 0; i < IMG_HEIGHT; i++ ) {
                        ULONG   y, offx = 0;
                        UBYTE   mask = 0x80;

                        for( y = 0; y < IMG_WIDTH; y++ ) {
                            UBYTE   oldpen, bit;
                            ULONG   newpen, offset;

                            oldpen = 0;

                            while( usedcolors[ oldpen ] != *ptr )
                                ++oldpen;

                            ptr++;

                            newpen = MUIPEN( data->Pens[ oldpen ]);
                            offset = offx + offy;

                            for( bit = 0; bit < planes; bit++ ) {

                                if( newpen & 0x01 )
                                    data->BitMap->Planes[ bit ][ offset ] |= mask;

                                newpen >>= 1;
                            }

                            mask >>= 1;

                            if( mask == 0 ) {
                                mask  = 0x80;
                                offx += 1;
                            }
                        }

                        offy += data->BitMap->BytesPerRow;
                    }
*/
                }
            }
/*
            FreeVec( usedcolors );
        }

        FreeVec( ChunkyImg );
*/    }
}
///
/// PlanarToChunky
static UBYTE *PlanarToChunky( struct CMImage *Img )
{
    UBYTE  *chunky;

    if( chunky = AllocVec( IMG_WIDTH * IMG_HEIGHT * sizeof( UBYTE ), MEMF_ANY )) {
        ULONG   y, offy = 0;
        UBYTE  *dest;

        dest = chunky;

        for( y = 0; y < IMG_HEIGHT; y++ ) {
            ULONG   x, offx = 0;
            UBYTE   mask = (1 << 7), bit = 7;

            for( x = 0; x < IMG_WIDTH; x++ ) {
                UBYTE   chunk;
                UBYTE   d;
                ULONG   offset;

                chunk  = 0;
                offset = offy + offx;

                for( d = 0; d < Img->BitMap->Depth; d++ )
                    chunk |= (( Img->BitMap->Planes[ d ][ offset ] & mask ) >> bit ) << d;

                *dest++ = chunk;

                mask >>= 1;
                bit   -= 1;

                if( mask == 0 ) {
                    offx += 1;
                    mask  = (1 << 7);
                    bit   = 7;
                }
            }

            offy += Img->BitMap->BytesPerRow;
        }
    }

    return( chunky );
}
///
#endif
