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
                contacts.name = Marshal.AllocHGlobal(UGrid.name_long_length);
                contacts.contact_name_id = Marshal.AllocHGlobal(UGrid.name_length * contacts.num_contacts);
                contacts.mesh_from_name = Marshal.AllocHGlobal(UGrid.name_long_length);
                contacts.mesh_to_name = Marshal.AllocHGlobal(UGrid.name_long_length);
                contacts.contact_name_long = Marshal.AllocHGlobal(UGrid.name_long_length * contacts.num_contacts);
                contacts.edges = Marshal.AllocHGlobal(contacts.num_contacts * 2 * Constants.intBytes);
                contacts.contact_type = Marshal.AllocHGlobal(contacts.num_contacts * Constants.intBytes);
            }

            public static void Free(this Contacts contacts)
            {
                if (contacts.name != IntPtr.Zero) Marshal.FreeHGlobal(contacts.name);
                if (contacts.contact_name_id != IntPtr.Zero) Marshal.FreeHGlobal(contacts.contact_name_id);
                if (contacts.mesh_from_name != IntPtr.Zero) Marshal.FreeHGlobal(contacts.mesh_from_name);
                if (contacts.mesh_to_name != IntPtr.Zero) Marshal.FreeHGlobal(contacts.mesh_to_name);
                if (contacts.contact_name_long != IntPtr.Zero) Marshal.FreeHGlobal(contacts.contact_name_long);
                if (contacts.edges != IntPtr.Zero) Marshal.FreeHGlobal(contacts.edges);
                if (contacts.contact_type != IntPtr.Zero) Marshal.FreeHGlobal(contacts.contact_type);
            }
        }
    }
}