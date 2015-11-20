

#ifndef CAN_CFG_H_
#define CAN_CFG_H_

#if !(((CAN_SW_MAJOR_VERSION == 2) && (CAN_SW_MINOR_VERSION == 0)) )
#error Can: Configuration file expected BSW module version to be 2.0.X*
#endif

// Number of controller configs
#define CAN_CTRL_CONFIG_CNT				1

#define CAN_DEV_ERROR_DETECT			STD_ON
#define CAN_VERSION_INFO_API			STD_OFF
#define CAN_MULTIPLEXED_TRANSMISSION	STD_OFF  // Not supported
#define CAN_WAKEUP_SUPPORT				STD_OFF  // Not supported
#define CAN_HW_TRANSMIT_CANCELLATION	STD_OFF  // Not supported

#define CAN_BAUDRATE_CNT				4
#define CAN_MESSAGE_TYPE_CNT			3

typedef enum {
	CAN_CTRL_1 = 0,
	CAN_CONTROLLER_CNT = 1
} CanControllerIdType;

typedef enum {
	VCU_CANID = 1,
	SCU_CANID = 2,
	TCU_CANID = 3
} Can_ECUIdType;

 typedef enum {
     CAN_ID_TYPE_EXTENDED,
     CAN_ID_TYPE_MIXED,
     CAN_ID_TYPE_STANDARD
 } Can_IdTypeType;

typedef enum {
	CAN_OBJECT_TYPE_RECEIVE,
	CAN_OBJECT_TYPE_TRANSMIT
} Can_ObjectTypeType;

 typedef enum {
      CAN_ARC_HANDLE_TYPE_BASIC,
      CAN_ARC_HANDLE_TYPE_FULL
  } Can_Arc_HohType;
  
#define CAN_CONTROLLER_CanController (CanControllerIdType)0
  
#define CAN_TX (Can_HwHandleType)0
#define NUM_OF_HTHS (Can_HwHandleType)1

#define CAN_RX (Can_HwHandleType)0
#define NUM_OF_HRHS (Can_HwHandleType)1

typedef struct Can_Callback {
    void (*CancelTxConfirmation)( const Can_PduType *);
    void (*RxIndication)( uint8 ,Can_IdType ,uint8 , const uint8 * );
    void (*ControllerBusOff)(uint8);
    void (*TxConfirmation)(PduIdType);
    void (*ControllerWakeup)(uint8);
    void (*Arc_Error)(uint8,Can_Arc_ErrorType);
} Can_CallbackType;

typedef struct {
	//	Specifies the InstanceId of this module instance. If only one instance is
	//	present it shall have the Id 0
	int CanIndex;
} Can_GeneralType;

typedef uint32_t Can_FilterMaskType;

typedef struct Can_HardwareObjectStruct {
	// Specifies the type (Full-CAN or Basic-CAN) of a hardware object.
	Can_Arc_HohType CanHandleType;

	// Specifies whether the IdValue is of type - standard identifier - extended
	// identifier - mixed mode ImplementationType: Can_IdType
	Can_IdTypeType CanIdType;

	//	Specifies (together with the filter mask) the identifiers range that passes
	//	the hardware filter.
	uint32 CanIdValue;

	//	Holds the handle ID of HRH or HTH. The value of this parameter is unique
	//	in a given CAN Driver, and it should start with 0 and continue without any
	//	gaps. The HRH and HTH Ids are defined under two different name-spaces.
	//	Example: HRH0-0, HRH1-1, HTH0-2, HTH1-3
	uint32 CanObjectId;

	// Specifies if the HardwareObject is used as Transmit or as Receive object
	Can_ObjectTypeType CanObjectType;

	// Reference to the filter mask that is used for hardware filtering togerther
	// with the CAN_ID_VALUE
	Can_FilterMaskType *CanFilterMaskRef;

	// A "1" in this mask tells the driver that that HW Message Box should be
	// occupied by this Hoh. A "1" in bit 31(ppc) occupies Mb 0 in HW.
	uint32 Can_Arc_MbMask;

	// End Of List. Set to TRUE is this is the last object in the list.
	boolean Can_Arc_EOL;
} Can_HardwareObjectType;
 
 typedef enum {
	CAN_ARC_PROCESS_TYPE_INTERRUPT,
	CAN_ARC_PROCESS_TYPE_POLLING
} Can_Arc_ProcessType;
 

typedef struct {

	//	Enables / disables API Can_MainFunction_BusOff() for handling busoff
	//	events in polling mode.
	// INTERRUPT or POLLING
	Can_Arc_ProcessType CanBusOffProcessing;

	// Supported baudrates
	uint32 CanControllerSupportedBaudRates[CAN_BAUDRATE_CNT];

	// Specifies the baudrate of the controller in kbps.
	uint32 CanControllerBaudRateId;

	//	This parameter provides the controller ID which is unique in a given CAN
	//	Driver. The value for this parameter starts with 0 and continue without any
	//	gaps.
	CanControllerIdType CanControllerId;

	// This id is used to filter messages to the correct recipient ECU
	// (for the filters to work, it should be smaller than MCP2515_MAX_ECU_CANID)
	Can_ECUIdType CanECUId;

	// Defines if a CAN controller is used in the configuration.
	boolean CanControllerActivation;

	// Specifies the buadrate of the controller in kbps.
	uint32 CanControllerBaudRate;

	//	Enables / disables API Can_MainFunction_Read() for handling PDU
	//	reception events in polling mode.
	Can_Arc_ProcessType CanRxProcessing;

	//	Enables / disables API Can_MainFunction_Write() for handling PDU
	//	transmission events in polling mode.
	Can_Arc_ProcessType CanTxProcessing;

	//	Enables / disables API Can_MainFunction_Wakeup() for handling wakeup
	//	events in polling mode.
	Can_Arc_ProcessType CanWakeupProcessing;

	//	Reference to the CPU clock configuration, which is set in the MCU driver
	//	configuration
	uint32 CanCpuClockRef;

	//	This parameter contains a reference to the Wakeup Source for this
	//	controller as defined in the ECU State Manager. Implementation Type:
	//	reference to EcuM_WakeupSourceType
	uint32 CanWakeupSourceRef;

	// List of Hoh id's that belong to this controller
	const Can_HardwareObjectType  *Can_Arc_Hoh;

	boolean Can_Arc_Loopback;

	// Set this to use the fifo
	boolean Can_Arc_Fifo;

	Can_IdTableType CanMsgIdTable[CAN_MESSAGE_TYPE_CNT];
} Can_ControllerConfigType;

typedef struct {
	const Can_ControllerConfigType *CanController;
	
	// Callbacks( Extension )
	const Can_CallbackType *CanCallbacks;

} Can_ConfigSetType;


typedef struct {
	// This is the multiple configuration set container for CAN Driver
	// Multiplicity 1..*
	const Can_ConfigSetType	 *CanConfigSet;
	// This container contains the parameters related each CAN
	// Driver Unit.
	// Multiplicity 1..*
	const Can_GeneralType	 *CanGeneral;
} Can_ConfigType;


extern const Can_ConfigType CanConfigData;
extern const Can_ControllerConfigType CanControllerConfigData[];
extern const Can_ConfigSetType Can_ConfigSet;


#endif /*CAN_CFG_H_*/

