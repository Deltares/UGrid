using System;
using System.Runtime.InteropServices;

namespace UGridNET
{
    namespace Extensions
    {
        internal static class ContactsExtensions
        {
            public static void Allocate(this Contacts contacts)
            {
                contacts.edges = Marshal.AllocHGlobal(contacts.num_contacts * 2 * Constants.intBytes);
                contacts.contact_type = Marshal.AllocHGlobal(contacts.num_contacts * Constants.intBytes);
            }

            public static void Free(this Contacts contacts)
            {
                if (contacts.edges != IntPtr.Zero) Marshal.FreeHGlobal(contacts.edges);
                if (contacts.contact_type != IntPtr.Zero) Marshal.FreeHGlobal(contacts.contact_type);
            }
        }
    }
}