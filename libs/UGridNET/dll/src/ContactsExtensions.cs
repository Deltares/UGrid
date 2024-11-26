namespace UGridNET
{
    namespace Extensions
    {
        internal static class ContactsExtensions
        {
            /// <summary> Allocates unmanaged memory for all required properties of the <see cref="Contacts"/> instance. </summary>
            /// <param name="contacts"> Instance of <see cref="Contacts"/>. </param>
            /// <remarks> Memory allocated by this method must be freed by <see cref="Free"> </remarks>
            public static void Allocate(this Contacts contacts)
            {
                try
                {
                    contacts.name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length);
                    contacts.contact_name_id = IntPtrHelpers.Allocate<byte>(UGrid.name_length * contacts.num_contacts);
                    contacts.mesh_from_name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length);
                    contacts.mesh_to_name = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length);
                    contacts.contact_name_long = IntPtrHelpers.Allocate<byte>(UGrid.name_long_length * contacts.num_contacts);
                    contacts.edges = IntPtrHelpers.Allocate<int>(contacts.num_contacts * 2);
                    contacts.contact_type = IntPtrHelpers.Allocate<int>(contacts.num_contacts);
                }
                catch
                {
                    // AllocHGlobal may throw OutOfMemoryException exception, clean up and re-throw
                    contacts.Free();
                    throw;
                }
            }

            /// <summary> Frees unmanaged memory allocated by <see cref="Allocate"/> for all required properties of the <see cref="Contacts"/> instance. </summary>
            /// <param name="contacts"> Instance of <see cref="Contacts"/>. </param>
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