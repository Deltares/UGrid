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
                try
                {
                    contacts.name = IntPtrHelpers.Allocate(UGrid.name_long_length);
                    contacts.contact_name_id = IntPtrHelpers.Allocate(UGrid.name_length * contacts.num_contacts);
                    contacts.mesh_from_name = IntPtrHelpers.Allocate(UGrid.name_long_length);
                    contacts.mesh_to_name = IntPtrHelpers.Allocate(UGrid.name_long_length);
                    contacts.contact_name_long = IntPtrHelpers.Allocate(UGrid.name_long_length * contacts.num_contacts);
                    contacts.edges = IntPtrHelpers.Allocate(contacts.num_contacts * 2 * IntPtrHelpers.Constants.intBytes);
                    contacts.contact_type = IntPtrHelpers.Allocate(contacts.num_contacts * IntPtrHelpers.Constants.intBytes);
                }
                catch
                {
                    // AllocHGlobal may throw OutOfMemoryException exception, clean up and re-throw
                    contacts.Free();
                    throw;
                }
            }

            public static void Free(this Contacts contacts)
            {
                IntPtrHelpers.Free(contacts.name);
                IntPtrHelpers.Free(contacts.contact_name_id);
                IntPtrHelpers.Free(contacts.mesh_from_name);
                IntPtrHelpers.Free(contacts.mesh_to_name);
                IntPtrHelpers.Free(contacts.contact_name_long);
                IntPtrHelpers.Free(contacts.edges);
                IntPtrHelpers.Free(contacts.contact_type);
            }
        }
    }
}