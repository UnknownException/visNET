using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace visNET
{
    class TcpClient
    {
        private IntPtr _instance;

        private const string instanceMissing = "TcpClient instance is missing";

        [DllImport("visLIB.dll", EntryPoint = "TcpClientCreate", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr TcpClientCreate(IntPtr ip, ushort port);
        public TcpClient(string ip, ushort port)
        {
            IntPtr strPtr = Marshal.StringToHGlobalAnsi(ip);
            _instance = TcpClientCreate(strPtr, port);
            Marshal.FreeHGlobal(strPtr);
        }



        [DllImport("visLIB.dll", EntryPoint = "TcpClientDestroy", CallingConvention = CallingConvention.Cdecl)]
        private static extern void TcpClientDestroy(IntPtr instance);
        ~TcpClient()
        {
            if (_instance != null)
                TcpClientDestroy(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "TcpClientSend", CallingConvention = CallingConvention.Cdecl)]
        private static extern void TcpClientSend(IntPtr instance, IntPtr packet);
        public void Send(Packet packet)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            TcpClientSend(_instance, packet.ToPointer());
        }



        [DllImport("visLIB.dll", EntryPoint = "TcpClientReceive", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr TcpClientReceive(IntPtr instance);
        public List<Packet> Receive()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            throw new NotImplementedException();

            var receivedPackets = new List<Packet>();

            return receivedPackets;
        }



        [DllImport("visLIB.dll", EntryPoint = "TcpClientIsDisconnected", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool TcpClientIsDisconnected(IntPtr instance);
        public bool IsDisconnected()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return TcpClientIsDisconnected(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "TcpClientIsValid", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool TcpClientIsValid(IntPtr instance);
        public bool IsValid()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return TcpClientIsValid(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "TcpClientGetError", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr TcpClientGetError(IntPtr instance);
        public string GetError()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return Marshal.PtrToStringAnsi(TcpClientGetError(_instance));
        }
    }
}
