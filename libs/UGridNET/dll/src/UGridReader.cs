using System;
using System.Collections.Generic;
using UGridNET.Extensions;
using System.Runtime.InteropServices;
using System.Diagnostics.CodeAnalysis;
using System.Linq;

namespace UGridNET
{
    public sealed class UGridReader : UGridBase
    {
        public UGridReader(string path) : base(path, FileMode.Read)
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

        private void ReadMesh1D()
        {
            int count = 0;
            Invoke(() => UGrid.ug_topology_get_count(fileID, TopologyType.Mesh1dTopology, ref count));
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    var mesh1D = new Mesh1D();
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
                    var mesh2D = new Mesh2D();
                    Invoke(() => UGrid.ug_mesh2d_inq(fileID, i, mesh2D));
                    mesh2D.Allocate();
                    Invoke(() => UGrid.ug_mesh2d_get(fileID, i, mesh2D));
                    mesh2DList.Add(mesh2D);
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
                    var contacts = new Contacts();
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
                    var network1D = new Network1D();
                    Invoke(() => UGrid.ug_network1d_inq(fileID, i, network1D));
                    network1D.Allocate();
                    Invoke(() => UGrid.ug_network1d_get(fileID, i, network1D));
                    network1DList.Add(network1D);
                }
            }
        }

        private Dictionary<string, string> GetVariableAttributes(string variableNameStr, bool isLongUGridString = false)
        {
            var result = new Dictionary<string, string>();

            byte[] variableName = isLongUGridString
                                      ? variableNameStr.GetBytes()
                                      : variableNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);

            // get number of attributes
            int attributesCount = 0;
            Invoke(() => UGrid.ug_variable_count_attributes(fileID, variableName, ref attributesCount));
            if (attributesCount == 0)
            {
                return result;
            }

            // get names of attributes
            var attributeNames = new byte[attributesCount * UGrid.name_long_length];
            Invoke(() => UGrid.ug_variable_get_attributes_names(fileID, variableName, attributeNames));
            List<string> dictionaryKeys = attributeNames.GetStringFromNullTerminatedArray().Tokenize(UGrid.name_long_length);

            // get values of attributes
            var attributeValues = new byte[attributesCount * UGrid.name_long_length];
            Invoke(() => UGrid.ug_variable_get_attributes_values(fileID, variableName, attributeValues));
            List<string> dictionaryValues = attributeValues.GetStringFromNullTerminatedArray().Tokenize(UGrid.name_long_length);

            // populate the dictionary with the name-value pairs
            for (int i = 0; i < attributesCount; i++)
            {
                result.Add(dictionaryKeys[i], dictionaryValues[i]);
            }

            return result;
        }

        public string GetConventions()
        {
            string attributeNameStr = "Conventions";
            byte[] attributeName = attributeNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            byte[] attributeValue = new byte[UGrid.name_long_length];
            Invoke(() => UGrid.ug_attribute_global_char_get(fileID, attributeName, attributeValue));
            return attributeValue.GetStringFromNullTerminatedArray(true);
        }

        public int GetEPSGCode()
        {
            string[] variableNames = { "projected_coordinate_system", "wgs84" };

            foreach (var name in variableNames)
            {
                try
                {
                    var attributes = GetVariableAttributes(name);
                    if (attributes.Count > 0)
                    {
                        if (attributes.TryGetValue("epsg", out string valueString) &&
                            int.TryParse(valueString, out int epsg))
                        {
                            return epsg;
                        }

                        throw new UGridNETException($"The variable {name} does not contain a valid 'epsg' attribute.");
                    }
                }
                catch
                {
                    // Ignore all other exceptions and try next
                }
            }

            throw new UGridNETException("Coordinate system not found or 'epsg' attribute missing.");
        }

        public Dictionary<string, string> GetEntityAttributesByIndex(TopologyType topologyType, int index)
        {
            IntPtr name;
            switch (topologyType)
            {
            case TopologyType.Mesh1dTopology:
                name = mesh1DList[index].name;
                break;
            case TopologyType.Mesh2dTopology:
                name = mesh2DList[index].name;
                break;
            case TopologyType.Network1dTopology:
                name = network1DList[index].name;
                break;
            case TopologyType.ContactsTopology:
                name = contactsList[index].name;
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(topologyType), "Invalid topology type.");
            }
            return GetVariableAttributes(Marshal.PtrToStringAnsi(name), true);
        }

        public Dictionary<string, string> GetEntityAttributesByName(TopologyType topologyType, string name)
        {
            int index;
            switch (topologyType)
            {
            case TopologyType.Mesh1dTopology:
                index = mesh1DList.FindIndex(item => GetName(item.name) == name);
                break;
            case TopologyType.Mesh2dTopology:
                index = mesh2DList.FindIndex(item => GetName(item.name) == name);
                break;
            case TopologyType.Network1dTopology:
                index = network1DList.FindIndex(item => GetName(item.name) == name);
                break;
            case TopologyType.ContactsTopology:
                index = contactsList.FindIndex(item => GetName(item.name) == name);
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(topologyType), "Invalid topology type.");
            }

            if (index == -1)
            {
                throw new InvalidOperationException($"No item found with name '{name}'");
            }

            return GetEntityAttributesByIndex(topologyType, index);
            string GetName(IntPtr intPtr) => Marshal.PtrToStringAnsi(intPtr)?.Trim();
        }

        [ExcludeFromCodeCoverage]
        private static void PrintAttributes(Dictionary<string, string> attributes)
        {
            foreach (var attribute in attributes)
            {
                Console.WriteLine($"{attribute.Key} : {attribute.Value}");
            }
        }
    }

}