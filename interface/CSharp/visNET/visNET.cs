using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace visNET
{
    static class visNET
    {
        [DllImport("visLIB.dll", EntryPoint = "startup", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Startup();

        [DllImport("visLIB.dll", EntryPoint = "cleanup", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Cleanup();
    }
}
