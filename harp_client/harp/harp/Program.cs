using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SimpleTCP;
// https://github.com/BrandonPotter/SimpleTCP


namespace harp
{
    class Program
    {

        enum eRaspCommands
        {
            RASP_SRV_RESTART,
            RASP_SRV_SHUTDOWN,
            RASP_SRV_DISCONNECT,
            RASP_SYS_REBOOT,
            RASP_SYS_SHUTDOWN,
            RASP_TKCTRL_START,
            RASP_TKCTRL_STOP,
            RASP_TKCTRL_LEFT,
            RASP_TKCTRL_RIGHT,
        };
        static readonly byte[][] rasp_cmds =  new byte[][]{
                                        new byte[4] { 0xAA, 0x00, 0x00, 0x5A }, //RASP_SRV_RESTART
                                        new byte[4] { 0xAA, 0x01, 0x00, 0x5A }, //RASP_SRV_SHUTDOWN
                                        new byte[4] { 0xAA, 0x02, 0x00, 0x5A }, //RASP_SRV_DISCONNECT

                                        new byte[4] { 0xAA, 0x20, 0x00, 0x5A }, //RASP_SYS_REBOOT
                                        new byte[4] { 0xAA, 0x21, 0x00, 0x5A }, //RASP_SYS_REBOOT

                                        new byte[4] { 0xAA, 0x40, 0x00, 0x5A }, //RASP_TKCTRL_START
                                        new byte[4] { 0xAA, 0x41, 0x00, 0x5A }, //RASP_TKCTRL_STOP
                                        new byte[4] { 0xAA, 0x42, 0x00, 0x5A }, //RASP_TKCTRL_LEFT
                                        new byte[4] { 0xAA, 0x43, 0x00, 0x5A }, //RASP_TKCTRL_RIGHT

                                    };

        static void Main(string[] args)
        {
            var client = new SimpleTcpClient().Connect("10.10.10.5", 3490);
            Console.WriteLine("Enter command ...");
            ConsoleKeyInfo key_input = Console.ReadKey();
            while (key_input.Key != ConsoleKey.Escape)
            {
                switch (key_input.Key)
                {
                    case ConsoleKey.W:
                        {
                            client.Write(rasp_cmds[(int)eRaspCommands.RASP_TKCTRL_START]);
                            break;
                        }
                    case ConsoleKey.S:
                        {
                            client.Write(rasp_cmds[(int)eRaspCommands.RASP_TKCTRL_STOP]);
                            break;
                        }
                    case ConsoleKey.A:
                        {
                            client.Write(rasp_cmds[(int)eRaspCommands.RASP_TKCTRL_LEFT]);
                            break;
                        }
                    case ConsoleKey.D:
                        {
                            client.Write(rasp_cmds[(int)eRaspCommands.RASP_TKCTRL_RIGHT]);
                            break;
                        }
                    default:
                        break;
                }

                key_input = Console.ReadKey();
            }

            client.Write(rasp_cmds[(int)eRaspCommands.RASP_SRV_DISCONNECT]);
            client.Disconnect();
        }
    }
}
