
#include <iostream>
#include <cmath>
#include <regex>
#include <string.h>
using namespace std;

string binary(unsigned long n) //int to binary converter
{
   char result[(sizeof(unsigned long) * 8) + 1];
   unsigned index = sizeof(unsigned long) * 8;
   result[index] = '\0';

   do
      result[--index] = '0' + (n & 1);
   while (n >>= 1);

   return string(result + index);
}

//----------------------------------------------------------------------------------------------------
string ipToBinary(string ip[]) //ip to binary without formatting
{ 
   string output = "";
   for (int i = 0; i < 4; i++)
   {
      string out = binary(stoi(ip[i]));
      int aux = 8 - out.size();
      string out2 = "";
      for (int j = 0; j < aux; j++)
      {
         out2 += '0';
      }
      output += out2 + out;
   }
   return output;
}
//----------------------------------------------------------------------------------------------------
string BinaryToIp(string entrada, int alterer) //binary string to ip string with formatting
{ 

   string output = "";
   for (size_t i = 0; i < 4; i++)
   {
      if (i == 3)
         output += to_string(stoi(entrada.substr(8 * i, 8), nullptr, 2) + (alterer));
      else
         output += to_string(stoi(entrada.substr(8 * i, 8), nullptr, 2)) + ".";
   }
   return output;
}
//----------------------------------------------------------------------------------------------------
string networkAddress(string ip[], int CIDR) //calculates a network address for a given ip address
{ 

   string ipArray = ipToBinary(ip);
   for (int i = CIDR; i < 32; i++)
   {
      ipArray[i] = '0';
   }
   return ipArray;
}
//----------------------------------------------------------------------------------------------------
string broadcastAddress(string ip[], int CIDR) //calculates a broadcast address for a given ip address
{ 

   string ipArray = ipToBinary(ip);
   for (int i = CIDR; i < 32; i++)
   {
      ipArray[i] = '1';
   }
   return ipArray;
}
//----------------------------------------------------------------------------------------------------
string CIDRtoDecimal(int CIDR) //CIDR to decimal mask converter
{ 
   string output = "11111111111111111111111111111111";
   for (int i = CIDR; i < 32; i++)
   {
      output[i] = '0';
   }
   return output;
}
//----------------------------------------------------------------------------------------------------
int DecimalToCIDR(string mask[]) //Decimal mask to CIDR Converter
{
   int CIDR = 0;
   string aux = ipToBinary(mask);
   for (int j = 0; j < 32; j++)
   {
      if (aux[j] == '1')
         CIDR++;
   }
   return CIDR;
}
//----------------------------------------------------------------------------------------------------
int CIDRforIPWithClass(string firstOctet)//calculates a CIDR for a ip with class
{ 
   string input = binary(stoi(firstOctet, nullptr, 10));
   int aux = 8 - input.size();
   string output = "";
   if (aux)
   {  for (int j = 0; j < aux; j++)
         output += '0';
   }
   output += input;
   int j = 0;
   for (j; j < 8; j++)
         if(output[j]=='0')
            break;
   switch (j)
   {
      case 0:
         cout << "Class A \n";
         return 8;
      case 1:
         cout << "Class B \n";
         return 16;
      case 2:
         cout << "Class C \n";
         return 24;
      case 3:
         cout << "Class D \n";
         return -1;
      default:
         cout << "Class E \n";
         return -1;
   }
}
//-----------------------------------------------------------------------------------------------------
void callInfo(string ip[], int CIDR) //Executes the flow of tasks
{
   cout << "Number of hosts available: " << pow(2, (32 - CIDR)) - 2 << "\n";
   cout << "Network address: " << BinaryToIp(networkAddress(ip, CIDR), 0) << "\n";
   cout << "Broadcast address: " << BinaryToIp(broadcastAddress(ip, CIDR), 0) << "\n";
   cout << "First usable address: " << BinaryToIp(networkAddress(ip, CIDR), 1) << "\n";
   cout << "Last usable address: " << BinaryToIp(broadcastAddress(ip, CIDR), (-1)) << "\n";
}
//-----------------------------------------------------------------------------------------------------
void ipWithMaskDecimal(string ip[], string mask[]) 
{
   int CIDR = DecimalToCIDR(mask);

   cout << "CIDR MASK: " << CIDR << "\n";
   callInfo(ip, CIDR);
}
//------------------------------------------------------------------------------------------------------
void ipWithClass(string ip[])
{
   int CIDR = CIDRforIPWithClass(ip[0]);
   if(CIDR==(-1))
      cout<<"Not applicable \n";
   else
      callInfo(ip, CIDR);
}
//------------------------------------------------------------------------------------------------------
void ipWithMaskCIDR(string ip[], string CIDRstring)
{
   int CIDR = stoi(CIDRstring, nullptr, 10);

   cout << "Decimal Mask: " << BinaryToIp(CIDRtoDecimal(CIDR), 0) << "\n";
   callInfo(ip, CIDR);
}
//------------------------------------------------------------------------------------------------------
int main()
{
   string input = "191.10.20.30";
   cout<< "Please enter an input as given in one of the examples: 126.10.10.10 OR 152.168.1.75/28 OR 152.168.1.75/255.255.255.255 \nType here:";
   cin>>input;
   cout<<"\n";
   regex ipRxWithMaskDecimal("((\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3}))\\/((\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3}))");
   regex ipRxWithClass("(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3}$)");
   regex ipRxWithMaskCIDR("(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\/(\\d{1,2})");
   smatch matches;
   //end main declarations
   if (regex_match(input, ipRxWithMaskDecimal))
   {
      regex_search(input, matches, ipRxWithMaskDecimal);
      string mask[4] = {matches[7].str(), matches[8].str(), matches[9].str(), matches[10].str()};
      string ip[4] = {matches[2].str(), matches[3].str(), matches[4].str(), matches[5].str()};
      ipWithMaskDecimal(ip, mask);
   }
   else
   {
      if (regex_match(input, ipRxWithClass))
      {
         regex_search(input, matches, ipRxWithClass);
         string ip[4] = {matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()};
         ipWithClass(ip);
      }
      else
      {
         if (regex_match(input, ipRxWithMaskCIDR))
         {
            regex_search(input, matches, ipRxWithMaskCIDR);
            string mask = matches[5].str();
            string ip[4] = {matches[1].str(), matches[2].str(), matches[3].str(), matches[4].str()};
            ipWithMaskCIDR(ip, mask);
         }
         else
         {
            cout << "Invalid Expression, please give the input as given on the example: \n 152.168.1.75/28 or 152.168.1.75 or 152.168.1.75/255.255.255.255";
         }
      }
   }
   return main();
}
