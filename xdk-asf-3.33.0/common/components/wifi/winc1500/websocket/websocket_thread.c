/*
 * Copyright (c) 2014 Putilov Andrey
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>

//#include "ws_server.h"
#include "websocket.h"
#include "socket/include/socket.h"

#define WS_PORT 8088
#define BUF_LEN 10000
//#define PACKET_DUMP

uint8_t gBuffer[BUF_LEN];
//extern WS_SERVER_DATA ws_serverData;
SOCKET ActiveclientSocket = 0;

bool StreamActive = false;

void error(const char *msg);
int safeSend(SOCKET clientSocket, uint8_t *buffer, int32_t bufferSize);
bool clientWorker(SOCKET clientSocket, void *pvMsg,uint8_t *puc);
void my_memset(uint8_t *puc, uint8_t val, uint32_t size);
void my_memcpy(uint8_t *puc1, uint8_t *puc2, uint32_t size);


void my_memset(uint8_t *puc, uint8_t val, uint32_t size)
{
	uint32_t i;
	for(i=0; i<size; i++)
	{
		*puc++ = val;
	}	
}

void my_memcpy(uint8_t *puc1, uint8_t *puc2, uint32_t size)
{
	uint32_t i;
	for(i=0; i<size; i++)
	{
		*puc1++ = *puc2++;
	}	
}

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int safeSend(SOCKET clientSocket, uint8_t *buffer, int32_t bufferSize)
{
//    int32_t written;

	buffer[bufferSize] = 0;
    printf("WS: out packet:\r\n%s",buffer);

    //while (bufferSize > NET_PRES_SocketWriteIsReady(clientSocket, bufferSize, 0));

    //written = send(clientSocket, buffer, bufferSize, 0); //MR:
	send(clientSocket, buffer, bufferSize, 0); //MR:

	//if (written == -1)
	//{
		//close(clientSocket); //MR:
		//printf("send failed");
		//return EXIT_FAILURE;
	//}
    //if (written != bufferSize)
    //{
        //close(clientSocket); //MR:
        //printf("written not all bytes");
//
        //return EXIT_FAILURE;
    //}

    return EXIT_SUCCESS;
}

bool clientWorker(SOCKET clientSocket, void *pvMsg,uint8_t *puc)
{    
    static size_t readedLength = 0;
    static size_t frameSize = BUF_LEN;
    static enum wsState state = WS_STATE_OPENING;
    static uint8_t *data = NULL;
    static size_t dataSize = 0;
    static enum wsFrameType frameType = WS_INCOMPLETE_FRAME;
    static struct handshake hs;
    static int value;
	static bool clientWorkerInit = true;
    
	if(clientWorkerInit==true)
	{
		printf("WS: Init\r\n");
		clientWorkerInit = false;
		my_memset(gBuffer, 0, BUF_LEN);
		readedLength = 0;
		frameSize = BUF_LEN;
		state = WS_STATE_OPENING;
		data = NULL;
		dataSize = 0;
		frameType = WS_INCOMPLETE_FRAME;	
		nullHandshake(&hs);
		ActiveclientSocket = clientSocket;
	}
	
	printf("WS: Process\r\n");

#define prepareBuffer frameSize = BUF_LEN; my_memset(gBuffer, 0, BUF_LEN);
#define initNewFrame frameType = WS_INCOMPLETE_FRAME; readedLength = 0; my_memset(gBuffer, 0, BUF_LEN);

    //while (frameType == WS_INCOMPLETE_FRAME)
    //{
        //----------------------------------------------------------------------
        // !!! Blocking for Incoming Data !!!
        //while (NET_PRES_SocketReadIsReady(clientSocket) == 0);
        //----------------------------------------------------------------------
		tstrSocketRecvMsg *pstrRecv = (tstrSocketRecvMsg *)pvMsg;
		printf("WS: Size %d\r\n",pstrRecv->s16BufferSize);
		my_memcpy(gBuffer + readedLength, puc,pstrRecv->s16BufferSize );		
		//recv(clientSocket, gBuffer + readedLength, BUF_LEN - readedLength, 0); //0; //recv(clientSocket, gBuffer+readedLength, BUF_LEN-readedLength, 0); //MR:
        printf("WS: Taken\r\n");
		gBuffer[pstrRecv->s16BufferSize]=0;
		printf("%s\r\n",gBuffer);

        ssize_t readed = pstrRecv->s16BufferSize;
		if (!readed)
        {
            close(clientSocket); //MR:
            printf("recv failed");
            return 1;
        }

        readedLength += readed;
		gBuffer[readed]=0;
		printf("in packet:\r\n%s",gBuffer);
        assert(readedLength <= BUF_LEN);

        if (state == WS_STATE_OPENING)
        {
            frameType = wsParseHandshake(gBuffer, readedLength, &hs);
        }
        else
        {
            frameType = wsParseInputFrame(gBuffer, readedLength, &data, &dataSize);
        }

        if ((frameType == WS_INCOMPLETE_FRAME && readedLength == BUF_LEN) || frameType == WS_ERROR_FRAME)
        {
            if (frameType == WS_INCOMPLETE_FRAME)
                printf("buffer too small\r\n");
            else
                printf("error in incoming frame\r\n");

            if (state == WS_STATE_OPENING)
            {
                prepareBuffer;
                frameSize = sprintf((char *) gBuffer,
                        "HTTP/1.1 400 Bad Request\r\n"
                        "%s%s\r\n\r\n",
                        versionField,
                        version);
                safeSend(clientSocket, gBuffer, frameSize);
                goto CATCH;
            }
            else
            {
                prepareBuffer;
                wsMakeFrame(NULL, 0, gBuffer, &frameSize, WS_CLOSING_FRAME);
                if (safeSend(clientSocket, gBuffer, frameSize) == EXIT_FAILURE)
                    goto CATCH;
                state = WS_STATE_CLOSING;
                initNewFrame;
            }
        }

        if (state == WS_STATE_OPENING)
        {
            //assert(frameType == WS_OPENING_FRAME);
            if (frameType == WS_OPENING_FRAME)
            {
                // if resource is right, generate answer handshake and send it
                if (strcmp(hs.resource, "/echo") != 0)
                {
                    frameSize = sprintf((char *) gBuffer, "HTTP/1.1 404 Not Found\r\n\r\n");
                    safeSend(clientSocket, gBuffer, frameSize);
                    goto CATCH;
                }

                prepareBuffer;
                wsGetHandshakeAnswer(&hs, gBuffer, &frameSize);
                freeHandshake(&hs);
                if (safeSend(clientSocket, gBuffer, frameSize) == EXIT_FAILURE)
                    goto CATCH;
                state = WS_STATE_NORMAL;
                initNewFrame;
            }
        }
        else
        {
            if (frameType == WS_CLOSING_FRAME)
            {
                if (state == WS_STATE_CLOSING)
                {
                    goto CATCH;
                }
                else
                {
                    prepareBuffer;
                    wsMakeFrame(NULL, 0, gBuffer, &frameSize, WS_CLOSING_FRAME);
                    safeSend(clientSocket, gBuffer, frameSize);
                    goto CATCH;
                }
            }
            else if (frameType == WS_TEXT_FRAME)
            {
                uint8_t *recievedString = NULL;
                recievedString = malloc(dataSize + 1);
                assert(recievedString);
                my_memcpy(recievedString, data, dataSize);
                recievedString[ dataSize ] = 0;

                //--------------------------------------------------------------
                // Command Parser
                if (strstr((const char *) recievedString, "StartStream"))
                {
                    printf("Stream Started\n\r");
                    StreamActive = true;
                }
                else if (strstr((const char *) recievedString, "StopStream"))
                {
                    printf("Stream Stopped\n\r");
                    StreamActive = false;
                }
                else if (strstr((const char *) recievedString, "SetDelay"))
                {
                    value = atoi((const char *) (recievedString + 9));
                    printf("Set Streamer Task Delay: %d\n\r", value);
                    //ws_serverData.delay = value;
                }
                else if (strstr((const char *) recievedString, "reset"))
                {
                    printf("Reset\n\r");
                    //vTaskDelay(1000);
                    //SYS_RESET_SoftwareReset();
                }
                //--------------------------------------------------------------

                prepareBuffer;
                wsMakeFrame(recievedString, dataSize, gBuffer, &frameSize, WS_TEXT_FRAME);
                free(recievedString);
                if (safeSend(clientSocket, gBuffer, frameSize) == EXIT_FAILURE) goto CATCH;
                initNewFrame;

            }
        }
//    } // read/write cycle

	if(frameType != WS_INCOMPLETE_FRAME)
	{
		goto CATCH;
	}

	return 0;

CATCH:
	clientWorkerInit = true;
	close(clientSocket); //MR:

	return 1;
}

void wsSendDataCallback(const uint8_t *DataString, size_t DataLength)
{
    size_t StreamFrameSize = BUF_LEN;
    static uint8_t gBufferStream[BUF_LEN];

    if (StreamActive == true)
    {
        memset(gBufferStream, 0, BUF_LEN);
        wsMakeFrame(DataString, DataLength, gBufferStream, &StreamFrameSize, WS_TEXT_FRAME);
        safeSend(ActiveclientSocket, gBufferStream, StreamFrameSize);
    }

}
