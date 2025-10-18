/* taken from uOFW */
#pragma once

#include <psptypes.h>

#define SCE_KERNEL_MAX_MODULE_SEGMENT   (4) /** The maximum number of segments a module can have. */

/** The SceModule structure represents a loaded module in memory. */
typedef struct _SceModule {
    /** Pointer to the next registered module. Modules are connected via a linked list. */
	struct _SceModule *next; //0
    /** The attributes of a module. One or more of ::SceModuleAttribute and ::SceModulePrivilegeLevel. */
	u16 attribute; //4
    /** 
     * The version of the module. Consists of a major and minor part. There can be several modules 
     * loaded with the same name and version.
     */
	u8 version[2]; //6
    /** The module's name. There can be several modules loaded with the same name. */
	char modName[27]; //8
    /** String terminator (always '\0'). */
	char terminal; //35
    /** 
     * The status of the module. Contains information whether the module has been started, stopped, 
     * is a user module, etc.
     */
	u16 status; //36
    /** Reserved. */
	u16 padding; //38
    /** A secondary ID for the module. */
	SceUID secId; //40
    /** The module's UID. */
	SceUID modId; //44
    /** The thread ID of a user module. */
	SceUID userModThid; //48
    /** The ID of the memory block belonging to the module. */
	SceUID moduleBlockId; //52
    /** The ID of the TEXT segment's memory partition. */
	SceUID mpIdText; //56
    /** The ID of the DATA segment's memory partition. */
	SceUID mpIdData; //60
    /** Pointer to the first resident library entry table of the module. */
	void *entTop; //64
    /** The size of all resident library entry tables of the module. */
	SceSize entSize; //68
    /** Pointer to the first stub library entry table of the module. */
	void *stubTop; //72
    /** The size of all stub library entry tables of the module. */
	SceSize stubSize; //76
    /** 
     * A pointer to the (required) module's start entry function. This function is executed during 
     * the module's startup. 
     */
	void * moduleStart; //80
    /** 
     * A pointer to the (required) module's stop entry function. This function is executed during 
     * the module's stopping phase. 
     */
	void * moduleStop; //84
    /** 
     * A pointer to a module's Bootstart entry function. This function is probably executed after 
     * a reboot. 
     */
	void * moduleBootstart; //88
    /** 
     * A pointer to a module's rebootBefore entry function. This function is probably executed 
     * before a reboot. 
     */
    void * moduleRebootBefore; //92
    /** 
     * A pointer to a module's rebootPhase entry function. This function is probably executed 
     * during a reboot. 
     */
    void * moduleRebootPhase; //96
    /** 
     * The entry address of the module. It is the offset from the start of the TEXT segment to the 
     * program's entry point. 
     */
	u32 entryAddr; //100
    /** Contains the offset from the start of the TEXT segment of the program's GP register value. */
	u32 gpValue; //104
    /** The start address of the TEXT segment. */
	u32 textAddr; //108
    /** The size of the TEXT segment. */
	SceSize textSize; //112
    /** The size of the DATA segment. */
	SceSize dataSize; //116
    /** The size of the BSS segment. */
	SceSize bssSize; //120
    /** The number of segments the module consists of. */
	u8 	nSegments; //124
    /** Reserved. */
	u8	padding2[3]; //125
    /** An array containing the start address of each segment. */
	u32 segmentAddr[SCE_KERNEL_MAX_MODULE_SEGMENT]; //128
    /** An array containing the size of each segment. */
	SceSize segmentSize[SCE_KERNEL_MAX_MODULE_SEGMENT]; //144
    /** An array containing the alignment information of each segment. */
    u32 segmentAlign[SCE_KERNEL_MAX_MODULE_SEGMENT]; //160
    /** The priority of the module start thread. */
	s32 moduleStartThreadPriority; //176
    /** The stack size of the module start thread. */
	SceSize moduleStartThreadStacksize; //180
    /** The attributes of the module start thread. */
	SceUInt moduleStartThreadAttr; //184
    /** The priority of the module stop thread. */
	s32 moduleStopThreadPriority; //188
    /** The stack size of the module stop thread. */
	SceSize moduleStopThreadStacksize; //192
    /** The attributes of the module stop thread. */
	SceUInt moduleStopThreadAttr; //196
    /** The priority of the module reboot before thread. */
	s32 moduleRebootBeforeThreadPriority; //200
    /** The stack size of the module reboot before thread. */
	SceSize moduleRebootBeforeThreadStacksize; //204
    /** The attributes of the module reboot before thread. */
	SceUInt moduleRebootBeforeThreadAttr; //208
    /** The value of the coprocessor 0's count register when the module is created. */
	u32 countRegVal; //212
    /** The segment checksum of the module's segments. */
    u32 segmentChecksum; //216
    /** TEXT segment checksum of the module. */
    u32 textSegmentChecksum; //220
    /** Unknown. */
    u32 unk224; //224
} _SceModule; //size = 228
