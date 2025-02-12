/***  File Header  ************************************************************/
/**
* io_port.cc
*
* Elixir/Erlang Port ext. of tensor flow lite
* @author	   Shozo Fukuda
* @date	create Sat Sep 26 06:26:30 JST 2020
* System	   MINGW64/Windows 10<br>
*
**/
/**************************************************************************{{{*/

#include <iostream>
#include <string>
#include <memory>
using namespace std;

/***  Type ****************************************************************}}}*/
/**
* convert "unsigned short" <-> "char[2]"
**/
/**************************************************************************{{{*/
union Magic {
    unsigned int ui32;
    char C[4];
};

/***  Module Header  ******************************************************}}}*/
/**
* receive command packet from Elixir/Erlang
* @par DESCRIPTION
*   receive command packet and store it to "cmd_line"
*
* @retval res >  0  success
* @retval res == 0  termination
* @retval res <  0  error
**/
/**************************************************************************{{{*/
streamsize
rcv_packet_port(string& cmd_line)
{
    try {
        // receive packet size
        Magic len;
        /*+KNOWLEDGE:shoz:20/11/24:can't work "cin.get(len.C[3]).get(len.C[2]).." in WSL */
        len.C[3] = cin.get();
        len.C[2] = cin.get();
        len.C[1] = cin.get();
        len.C[0] = cin.get();

        // receive packet payload
        unique_ptr<char[]> buff(new char[len.ui32]);
        cin.read(buff.get(), len.ui32);

        // return received command line
        cmd_line.assign(buff.get(), len.ui32);
        return len.ui32;
    }
    catch(ios_base::failure) {
        return (cout.eof()) ? 0 : -1;
    }
    catch (std::bad_alloc& e) {
        cerr << e.what() << "@rcv_packet_port" << endl;
        return -1;
    }
}

/***  Module Header  ******************************************************}}}*/
/**
* send result packet to Elixir/Erlang
* @par DESCRIPTION
*   construct message packet and send it to stdout
*
* @return count of sent byte or error code
**/
/**************************************************************************{{{*/
streamsize
snd_packet_port(string result)
{
    try {
        Magic len = { static_cast<unsigned int>(result.size()) };
        (cout.put(len.C[3]).put(len.C[2]).put(len.C[1]).put(len.C[0]) << result).flush();
        return len.ui32;
    }
    catch(ios_base::failure) {
        return (cout.eof()) ? 0 : -1;
    }
}

/*** io_port.cc ********************************************************}}}*/
