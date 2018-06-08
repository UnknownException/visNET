using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace visNET
{
    class Packet
    {
        private IntPtr _instance;
        public IntPtr ToPointer()
        {
            return _instance;
        }

        private const string instanceMissing = "Packet instance is missing";

        [DllImport("visLIB.dll", EntryPoint = "PacketCreate", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr PacketCreate();
        public Packet()
        {
            _instance = PacketCreate();
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketDestroy", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketDestroy(IntPtr instance);
        ~Packet()
        {
            if (_instance != null)
                PacketDestroy(_instance);
        }



        /*
         * Write functions
         */



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteInt8", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteInt8(IntPtr instance, SByte n);
        public void WriteInt8(SByte n)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteInt8(_instance, n);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteInt16", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteInt16(IntPtr instance, Int16 n);
        public void WriteInt16(Int16 n)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteInt16(_instance, n);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteInt32", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteInt32(IntPtr instance, Int32 n);
        public void WriteInt32(Int32 n)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteInt32(_instance, n);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteInt64", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteInt64(IntPtr instance, Int64 n);
        public void WriteInt64(Int64 n)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteInt64(_instance, n);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteUInt8", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteUInt8(IntPtr instance, Byte n);
        public void WriteUInt8(Byte n)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteUInt8(_instance, n);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteUInt16", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteUInt16(IntPtr instance, UInt16 n);
        public void WriteUInt16(UInt16 n)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteUInt16(_instance, n);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteUInt32", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteUInt32(IntPtr instance, UInt32 n);
        public void WriteUInt32(UInt32 n)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteUInt32(_instance, n);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteUInt64", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteUInt64(IntPtr instance, UInt64 n);
        public void WriteUInt64(UInt64 n)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteUInt64(_instance, n);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteFloat", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteFloat(IntPtr instance, float f);
        public void WriteFloat(float f)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteFloat(_instance, f);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteDouble", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteDouble(IntPtr instance, double d);
        public void WriteDouble(double d)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteDouble(_instance, d);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketWriteBool", CallingConvention = CallingConvention.Cdecl)]
        private static extern void PacketWriteDouble(IntPtr instance, [MarshalAs(UnmanagedType.U1)]bool d);
        public void WriteBool(bool d)
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            PacketWriteDouble(_instance, d);
        }



        /*
         * Write functions
         */



        [DllImport("visLIB.dll", EntryPoint = "PacketReadInt8", CallingConvention = CallingConvention.Cdecl)]
        private static extern SByte PacketReadInt8(IntPtr instance);
        public SByte ReadInt8()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadInt8(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadInt16", CallingConvention = CallingConvention.Cdecl)]
        private static extern Int16 PacketReadInt16(IntPtr instance);
        public Int16 ReadInt16()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadInt16(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadInt32", CallingConvention = CallingConvention.Cdecl)]
        private static extern Int32 PacketReadInt32(IntPtr instance);
        public Int32 ReadInt32()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadInt32(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadInt64", CallingConvention = CallingConvention.Cdecl)]
        private static extern Int64 PacketReadInt64(IntPtr instance);
        public Int64 ReadInt64()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadInt64(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadUInt8", CallingConvention = CallingConvention.Cdecl)]
        private static extern Byte PacketReadUInt8(IntPtr instance);
        public Byte ReadUInt8()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadUInt8(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadUInt16", CallingConvention = CallingConvention.Cdecl)]
        private static extern UInt16 PacketReadUInt16(IntPtr instance);
        public UInt16 ReadUInt16()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadUInt16(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadUInt32", CallingConvention = CallingConvention.Cdecl)]
        private static extern UInt32 PacketReadUInt32(IntPtr instance);
        public UInt32 ReadUInt32()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadUInt32(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadUInt64", CallingConvention = CallingConvention.Cdecl)]
        private static extern UInt64 PacketReadUInt64(IntPtr instance);
        public UInt64 ReadUInt64()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadUInt64(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadFloat", CallingConvention = CallingConvention.Cdecl)]
        private static extern float PacketReadFloat(IntPtr instance);
        public float ReadFloat()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadFloat(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadDouble", CallingConvention = CallingConvention.Cdecl)]
        private static extern double PacketReadDouble(IntPtr instance);
        public double ReadDouble()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadDouble(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketReadBool", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool PacketReadBool(IntPtr instance);
        public bool ReadBool()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketReadBool(_instance);
        }



        /*
         * Flags
         */



        [DllImport("visLIB.dll", EntryPoint = "PacketIsReadable", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool PacketIsReadable(IntPtr instance);
        public bool IsReadable()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketIsReadable(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketIsValid", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool PacketIsValid(IntPtr instance);
        public bool IsValid()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketIsValid(_instance);
        }



        [DllImport("visLIB.dll", EntryPoint = "PacketIsWritable", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I1)]
        private static extern bool PacketIsWritable(IntPtr instance);
        public bool IsWritable()
        {
            if (_instance == null)
                throw new NullReferenceException(instanceMissing);

            return PacketIsWritable(_instance);
        }
    }
}
