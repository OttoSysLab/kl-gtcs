/*=======================================================================================
 Program Nane  	: gtcs_tlg_decoder.c
 Subject 		: SARM Serial Port Communication Driver Process
---------------------------------------------------------------------------------------
 Compiler/IDE  	: gcc 4.6.3
 Library       	:
 Commands      	: Geany Build Command
  				: Compile : gcc -Wall -I"../inc" -c "%f"
  				: Buid 	  : gcc -Wall -o "%e" "%f" "sarm2pcb.c" "pcb2sarm.c" "../lib/sarmdb.c" "../lib/sarmcomm.c" "../lib/sarmutility.c" -I"../inc" -l"sqlite3" -l"X11" `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
 Programmer    	: Otto Chang
 Date	       	: 2019/08/06
=======================================================================================*/
#include "../include/gtcsmain.h"
// Set send data.
std::array<uint8_t,48> GetTelegramData()
{
    std::array<uint8_t,48> mcb_ctrl_telegram;
    #pragma region  
    mcb_ctrl_telegram[0] = 0x01;
    mcb_ctrl_telegram[1] = 0x00;
    mcb_ctrl_telegram[2] = 0x00;
    mcb_ctrl_telegram[3] = 0x00;
    mcb_ctrl_telegram[4] = 0x00;
    mcb_ctrl_telegram[5] = 0x00;
    mcb_ctrl_telegram[6] = 0x00;
    mcb_ctrl_telegram[7] = 0x00;
    mcb_ctrl_telegram[8] = 0x01;
    mcb_ctrl_telegram[9] = 0x24;
    mcb_ctrl_telegram[10] = 0x00;
    mcb_ctrl_telegram[11] = 0x00;
    mcb_ctrl_telegram[12] = 0xa0;
    mcb_ctrl_telegram[13] = 0x0f;
    mcb_ctrl_telegram[14] = 0x01;
    mcb_ctrl_telegram[15] = 0x00;
    mcb_ctrl_telegram[16] = 0xe8;
    mcb_ctrl_telegram[17] = 0x03;
    mcb_ctrl_telegram[18] = 0xe8;
    mcb_ctrl_telegram[19] = 0x03;
    mcb_ctrl_telegram[20] = 0x74;
    mcb_ctrl_telegram[21] = 0x01;
    mcb_ctrl_telegram[22] = 0xe8;
    mcb_ctrl_telegram[23] = 0x03;
    mcb_ctrl_telegram[24] = 0x00;
    mcb_ctrl_telegram[25] = 0x00;
    mcb_ctrl_telegram[26] = 0x00;
    mcb_ctrl_telegram[27] = 0x00;
    mcb_ctrl_telegram[28] = 0x08;
    mcb_ctrl_telegram[29] = 0x07;
    mcb_ctrl_telegram[30] = 0x00;
    mcb_ctrl_telegram[31] = 0x00;
    mcb_ctrl_telegram[32] = 0x00;
    mcb_ctrl_telegram[33] = 0x00;
    mcb_ctrl_telegram[34] = 0x00;
    mcb_ctrl_telegram[35] = 0x00;
    mcb_ctrl_telegram[36] = 0x00;
    mcb_ctrl_telegram[37] = 0x00;
    mcb_ctrl_telegram[38] = 0x00;
    mcb_ctrl_telegram[39] = 0x00;
    mcb_ctrl_telegram[40] = 0x00;
    mcb_ctrl_telegram[41] = 0x00;
    mcb_ctrl_telegram[42] = 0x00;
    mcb_ctrl_telegram[43] = 0x00;
    mcb_ctrl_telegram[44] = 0x46;   // DEV = 70
    mcb_ctrl_telegram[45] = 0xcf;   // DEV = 206
    mcb_ctrl_telegram[46] = 0xba;   // DEV = 186
    mcb_ctrl_telegram[47] = 0xbe;   // DEV = 190
    #pragma endregion

    return mcb_ctrl_telegram;
}
// // MCB communication.
// void mcbcommuniction()
// {
//     std::array<uint8_t,48> ptr_tlg_array = GetTelegramData(); 
//     std::cout << sizeof(ptr_tlg_array) <<std::endl;
//     for(int index=0;index<sizeof(ptr_tlg_array);index++){
//         std::cout <<"index = "<<std::to_string(index)<<" data = ";
//         std::cout << std::to_string(ptr_tlg_array[index])<<std::endl;
//     }
//     ComPort comm;
//     char com_name[] = "/dev/ttymxc3"; 
//     int com_num = comm.InitialComm(com_name);
//     while(true){
//         for(int index=0;index<48;index++){
//             comm.SendChar(com_num,ptr_tlg_array[index]);
//         }    
//         std::this_thread::sleep_for(std::chrono::milliseconds(20));
//         std::array<uint8_t,48> state_telegra_array = comm.ReadData(com_num);
//     }
//     // std::cout<<"Fuck mcbcommuniction"<<std::endl;
// }
// // TCP socket.
// void tcpsocket()
// {
//     // Socket.
//     TcpSocket mytcpserver;
//     char sockip[] = "127.0.0.1";
//     int sockport= 9000;    
//     mytcpserver.TcpSocketServer(sockip,sockport);
//     // std::cout<<"Fuck tcpsocket"<<std::endl;
// }
// main.
int main()
{
    // Set telegram.
    // std::thread thread_mcb = std::thread(mcbcommuniction);
    // std::thread thread_tcpsocket = std::thread(tcpsocket);
    // thread_mcb.join();
    // thread_tcpsocket.join();    
    // pthread_t threads[5];    
    // pthread_create(&threads[0],NULL,tcpsocket,(void *)0);

    GtcsAMSProtocol* gtcs_1 = GtcsAMSProtocol::getInstance();
    std::cout<<"GTCS_1 cmd300 str2s = "<<gtcs_1->amsBulletin.cmd300.str2<<std::endl;
    gtcs_1->amsBulletin.cmd300.str2 = "Fuck your family!!";
    GtcsAMSProtocol* gtcs_2 = GtcsAMSProtocol::getInstance();
    std::cout<<"GTCS_2 cmd300 str2 = "<<gtcs_2->amsBulletin.cmd300.str2<<std::endl;
    
    CMD300* prt_com300 = &gtcs_2->amsBulletin.cmd300;
    std::cout<<std::to_string(sizeof(gtcs_2->amsBulletin.cmd300))<<std::endl;
    std::cout<<"Prt str2= "<<prt_com300->str2<<std::endl;
    
    std::string* str_1 = (std::string *)(void *)prt_com300;
    std::cout << *str_1 <<std::endl;
    std::string* str_2 = (std::string *)(void *)(str_1+1);
    std::cout << *str_2 <<std::endl;
    std::string* str_3 = (std::string *)(void *)(str_2+1);
    std::cout << *str_3 <<std::endl;
    std::string* str_4 = (std::string *)(void *)(str_3+1);
    std::cout << *str_4 <<std::endl;
    std::string* str_5 = (std::string *)(void *)(str_4+1); 
    std::cout << *str_5 <<std::endl;
    return 0;
}