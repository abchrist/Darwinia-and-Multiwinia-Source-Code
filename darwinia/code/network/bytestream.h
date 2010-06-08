
/*
 * BYTE STREAM
 *
 * Code designed to write basic data types
 * into a char * byte stream
 * for Network transmission via UDP
 *
 */

#ifndef _included_bytestream_h
#define _included_bytestream_h



#define READ_INT(_stream)                   *((int*)_stream); _stream += sizeof(int);
#define READ_FLOAT(_stream)                 *((float*)_stream); _stream += sizeof(float);
#define READ_SIGNED_CHAR(_stream)			*((signed char*)_stream); _stream += sizeof(signed char);
#define READ_UNSIGNED_CHAR(_stream)         *((unsigned char*)_stream); _stream += sizeof(unsigned char);
#define READ_UNSIGNED_SHORT(_stream)        *((unsigned short*)_stream); _stream += sizeof(unsigned short);
#define READ_WORLDOBJECTID(_stream)         *((WorldObjectId*)_stream); _stream += sizeof(WorldObjectId);


#define WRITE_INT(_stream, _val)            *((int*)_stream) = _val; _stream += sizeof(int);
#define WRITE_FLOAT(_stream, _val)          *((float*)_stream) = _val; _stream += sizeof(float);
#define WRITE_SIGNED_CHAR(_stream, _val)    *((signed char*)_stream) = _val; _stream += sizeof(signed char);
#define WRITE_UNSIGNED_CHAR(_stream, _val)  *((unsigned char*)_stream) = _val; _stream += sizeof(unsigned char);
#define WRITE_UNSIGNED_SHORT(_stream, _val) *((unsigned short*)_stream) = _val; _stream += sizeof(unsigned short);
#define WRITE_WORLDOBJECTID(_stream, _val)  *((WorldObjectId*)_stream) = _val; _stream += sizeof(WorldObjectId);




#endif
