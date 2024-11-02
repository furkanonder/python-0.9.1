#include "sc_global.h"

void xPrintNum(TscOperand Num)
{
    printf(" %ld",(long)Num);
}

void xPrintFlags(TscOperand Flags)
{
	long x = (long)Flags;
    x = x & 0x0000000F;

    if (x == H_EXTRA) {
      	printf(" h_extra");
    if (x == H_SIZE) {
     	printf(" h_size");
    }
    if (x == H_OFFSET) {
    	printf(" h_offset");
    }
    if (x == H_PORT) {
      	printf(" h_port");
    }
    if (x == H_PRIV) {
      	printf(" h_priv");
    }
    if (x == PSEUDOFIELD) {
      	printf(" h_port and h_priv");
    }
    x = (long)Flags;
    if (x & NOSIGN) {
      	printf(" unsigned");
    }
    if (x & INT32) {
      	printf(" int32");
    }
}

void xPrintCode(TscOpcode Opcode)
{
	switch (Opcode) {
		case BufSize:
      		printf("BufSize");
    		break;
	    case Trans:
            printf("Trans");
    		break;
	    case TTupleS:
        	printf("TTupleS");
    		break;
	    case Unpack:
            printf("Unpack");
    		break;
	    case AilWord:
            printf("AilWord");
    		break;
	    case ListS:
            printf("ListS");
    		break;
	    case StringS:
            printf("StringS");
    		break;
	    case PutFS:
            printf("PutFS");
    		break;
	    case TStringSeq:
            printf("TStringSeq");
    		break;
	    case TStringSlt:
        	printf("TStringSlt");
    		break;
	    case PutVS:
        	printf("PutVS");
    		break;
	    case TListSeq:
            printf("TListSeq");
    		break;
	    case TListSlt:
            printf("TListSlt");
    		break;
	    case LoopPut:
            printf("LoopPut");
    		break;
	    case EndLoop:
            printf("EndLoop");
    		break;
	    case PutI:
            printf("PutI");
    		break;
	    case PutC:
            printf("PutC");
    		break;
	    case Dup:
            printf("Dup");
    		break;
	    case Pop:
            printf("Pop");
    		break;
	    case Align:
            printf("Align");
    		break;
	    case Pack:
            printf("Pack");
    		break;
	    case GetVS:
            printf("GetVS");
    		break;
	    case LoopGet:
            printf("LoopGet");
    		break;
	    case GetFS:
            printf("GetFS");
    		break;
	    case GetI:
            printf("GetI");
    		break;
	    case GetC:
        	printf("GetC");
    		break;
	    case PushI:
             printf("PushI");
    		break;
	    case MarshTC:
            printf("MarshTC");
    		break;
	    case UnMarshTC:
            printf("UnMarshTC");
    		break;
	    case Equal:
            printf("Equal");
    		break;
	    default:
            printf("Unknown opcode %04x",(int)Opcode);
    		break;
    }
}
