using System;
using System.Collections.Generic;
using System.Linq;
using UGridNET.Extensions;
using System.Runtime.InteropServices;

namespace UGridNET
{

    public sealed class UGridReader : UGridBase
    {
        // requires using log4net;
        //private static readonly ILog log = LogManager.GetLogger(typeof(UGridReader));

        private bool disposed = false;

        public UGridReader(string path) : base(path, 0)
        {
            try
            {
                ReadMesh1D();
                ReadMesh2D();
                ReadContacts();
                ReadNetwork1D();
            }
            catch
            {
                // If initialization fails, dispose of any partially initialized resources
                Dispose();
                // Rethrow to inform the caller of the failure
                throw;
            }
        }

        ~UGridReader()
        {
            // Cleanup in case Dispose wasn't called
            Dispose(false);
        }

        public new void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
            base.Dispose();
        }

        protected override void Dispose(bool disposing)
        {
            if (!disposed)
            {
                if (disposing)
                {
                    // clean up
                }

                disposed = true;

                base.Dispose(disposing);
            }
        }


        private void ReadMesh1D()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.Mesh1dTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Mesh1D mesh1D = new Mesh1D();
                    Invoke(() => UGrid.ug_mesh1d_inq(fileID, i, mesh1D));
                    mesh1D.Allocate();
                    Invoke(() => UGrid.ug_mesh1d_get(fileID, i, mesh1D));
                    mesh1DList.Add(mesh1D);
                }
            }
        }

        private void ReadMesh2D()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.Mesh2dTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Mesh2D mesh2D = new Mesh2D();
                    Invoke(() => UGrid.ug_mesh2d_inq(fileID, i, mesh2D));
                    mesh2D.Allocate();
                    Invoke(() => UGrid.ug_mesh2d_get(fileID, i, mesh2D));
                    mesh2DList.Add(mesh2D);

                    //var name = mesh2D.name.CopyToArray<byte>(UGrid.name_long_length);
                    //Console.WriteLine(">>> name: {0}", name.GetString());

                    //var name = Marshal.PtrToStringAnsi(mesh2D.name);
                    //Console.WriteLine(">>> name: {0}", name);
                }
            }
        }

        private void ReadContacts()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.ContactsTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Contacts contacts = new Contacts();
                    Invoke(() => UGrid.ug_contacts_inq(fileID, i, contacts));
                    contacts.Allocate();
                    Invoke(() => UGrid.ug_contacts_get(fileID, i, contacts));
                    contactsList.Add(contacts);
                }
            }
        }
        private void ReadNetwork1D()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.Network1dTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    Network1D network1D = new Network1D();
                    Invoke(() => UGrid.ug_network1d_inq(fileID, i, network1D));
                    network1D.Allocate();
                    Invoke(() => UGrid.ug_network1d_get(fileID, i, network1D));
                    network1DList.Add(network1D);
                }
            }
        }

        private Dictionary<string, string> GetVariableAttributes(string variableNameStr, bool strIsPadded = false)
        {
            var variableName = variableNameStr.PadRightUpTo(UGrid.name_long_length).GetBytes();

            int attributesCount = 0;
            Invoke(() => UGrid.ug_variable_count_attributes(fileID, variableName, ref attributesCount));

            var attributeNames = new byte[attributesCount * UGrid.name_long_length];
            Invoke(() => UGrid.ug_variable_get_attributes_names(fileID, variableName, attributeNames));
            List<string> dictionaryKeys = attributeNames.GetString().SplitIntoSizedTokens(UGrid.name_long_length);

            var attributeValues = new byte[attributesCount * UGrid.name_long_length];
            Invoke(() => UGrid.ug_variable_get_attributes_values(fileID, variableName, attributeValues));
            List<string> dictionaryValues = attributeValues.GetString().SplitIntoSizedTokens(UGrid.name_long_length);

            var dictionary = new Dictionary<string, string>();
            for (int i = 0; i < attributesCount; i++)
            {
                dictionary.Add(dictionaryKeys[i], dictionaryValues[i]);
            }

            return dictionary;
        }

        public string GetConventions()
        {
            var attributeNameStr = "Conventions";
            var attributeName = attributeNameStr.PadRightUpTo(UGrid.name_long_length).GetBytes();
            var attributeValue = new byte[UGrid.name_long_length];
            Invoke(() => UGrid.ug_attribute_global_char_get(fileID, attributeName, attributeValue));
            return attributeValue.GetString();
        }

        public Dictionary<string, string> GetMesh2DAttributesByID(int topologyID)
        {
            var name = Marshal.PtrToStringAnsi(mesh2DList[topologyID].name);
            //Console.WriteLine("name: \"{0}\"", name);
            var dict = GetVariableAttributes(name, true);
            // foreach (var item in dict)
            // {
            //     Console.WriteLine($"Key: {item.Key}, Value: {item.Value}");
            // }
            return dict;
        }

        public Dictionary<string, string> GetMesh2DAttributesByName(string name)
        {
            int index = mesh2DList.FindIndex(item => Marshal.PtrToStringAnsi(item.name).TrimEnd() == name);
            if (index == -1)
            {
                throw new InvalidOperationException($"No item found with name '{name}'");
            }
            return GetMesh2DAttributesByID(index);
        }

        public string GetEPSGCode()
        {
            string variableName = "projected_coordinate_system";
            var attributes = GetVariableAttributes(variableName);
            return attributes["EPSG_code"];
        }

        public string GetMesh2DLongName()
        {
            string variableName = "mesh2d";
            var attributes = GetVariableAttributes(variableName);
            return attributes["long_name"];
        }

    }

}