using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UGridNET.Extensions;

namespace UGridNET
{
    public sealed class UGridReader : UGridBase
    {
        public UGridReader(string path)
            : base(path, FileMode.Read)
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
            var count = 0;
            Invoke(() => UGrid.ug_topology_get_count(FileID, TopologyType.Mesh1dTopology, ref count));
            if (count <= 0)
            {
                return;
            }

            for (var i = 0; i < count; i++)
            {
                int index = i;
                var mesh1D = new Mesh1D();
                Invoke(() => UGrid.ug_mesh1d_inq(FileID, index, mesh1D));
                mesh1D.Allocate();
                Invoke(() => UGrid.ug_mesh1d_get(FileID, index, mesh1D));
                Meshes1D.Add(mesh1D);
            }
        }

        private void ReadMesh2D()
        {
            var count = 0;
            Invoke(() => UGrid.ug_topology_get_count(FileID, TopologyType.Mesh2dTopology, ref count));
            if (count <= 0)
            {
                return;
            }

            for (var i = 0; i < count; i++)
            {
                int index = i;
                var mesh2D = new Mesh2D();
                Invoke(() => UGrid.ug_mesh2d_inq(FileID, index, mesh2D));
                mesh2D.Allocate();
                Invoke(() => UGrid.ug_mesh2d_get(FileID, index, mesh2D));
                Meshes2D.Add(mesh2D);
            }
        }

        private void ReadContacts()
        {
            var count = 0;
            Invoke(() => UGrid.ug_topology_get_count(FileID, TopologyType.ContactsTopology, ref count));
            if (count <= 0)
            {
                return;
            }

            for (var i = 0; i < count; i++)
            {
                int index = i;
                var contacts = new Contacts();
                Invoke(() => UGrid.ug_contacts_inq(FileID, index, contacts));
                contacts.Allocate();
                Invoke(() => UGrid.ug_contacts_get(FileID, index, contacts));
                Contacts.Add(contacts);
            }
        }

        private void ReadNetwork1D()
        {
            var count = 0;
            Invoke(() => UGrid.ug_topology_get_count(FileID, TopologyType.Network1dTopology, ref count));
            if (count <= 0)
            {
                return;
            }

            for (var i = 0; i < count; i++)
            {
                int index = i;
                var network1D = new Network1D();
                Invoke(() => UGrid.ug_network1d_inq(FileID, index, network1D));
                network1D.Allocate();
                Invoke(() => UGrid.ug_network1d_get(FileID, index, network1D));
                Networks1D.Add(network1D);
            }
        }

        private bool IsVariablePresent(string variableNameStr)
        {
            byte[] variableName = variableNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            var exist = new[] { 0 };
            Invoke(() => UGrid.ug_variable_inq(FileID, variableName, exist));
            if (exist[0] == 0)
            {
                return false;
            }

            return true;
        }

        private Dictionary<string, string> GetVariableAttributes(string variableNameStr, bool isLongUGridString = false)
        {
            var result = new Dictionary<string, string>();

            byte[] variableName = isLongUGridString
                                      ? variableNameStr.GetBytes()
                                      : variableNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);

            // get number of attributes
            var attributesCount = 0;
            Invoke(() => UGrid.ug_variable_count_attributes(FileID, variableName, ref attributesCount));
            if (attributesCount == 0)
            {
                return result;
            }

            // get names of attributes
            var attributeNames = new byte[attributesCount * UGrid.name_long_length];
            Invoke(() => UGrid.ug_variable_get_attributes_names(FileID, variableName, attributeNames));
            List<string> dictionaryKeys = attributeNames.GetStringFromNullTerminatedArray().Tokenize(UGrid.name_long_length);

            // Get the maximum length of the attribute value strings
            var attributeMaxLength = 0;
            Invoke(() => UGrid.ug_variable_get_attributes_max_length(FileID, variableName, ref attributeMaxLength));

            // get values of attributes
            var attributeValues = new byte[attributesCount * attributeMaxLength];
            Invoke(() => UGrid.ug_variable_get_attributes_values(FileID, variableName, attributeMaxLength, attributeValues));
            List<string> dictionaryValues = attributeValues.GetStringFromNullTerminatedArray().Tokenize(attributeMaxLength);

            // populate the dictionary with the name-value pairs
            for (var i = 0; i < attributesCount; i++)
            {
                result.Add(dictionaryKeys[i], dictionaryValues[i]);
            }

            return result;
        }

        public string GetConventions()
        {
            var attributeNameStr = "Conventions";
            byte[] attributeName = attributeNameStr.GetRightPaddedNullTerminatedBytes(UGrid.name_long_length);
            var attributeValue = new byte[UGrid.name_long_length];
            Invoke(() => UGrid.ug_attribute_global_char_get(FileID, attributeName, attributeValue));
            return attributeValue.GetStringFromNullTerminatedArray(true);
        }

        public int GetEPSGCode()
        {
            string[] variableNames = { "projected_coordinate_system", "wgs84" };

            foreach (string name in variableNames)
            {
                if (!IsVariablePresent(name))
                {
                    continue;
                }

                Dictionary<string, string> attributes = GetVariableAttributes(name);
                if (attributes.Count > 0)
                {
                    if (attributes.TryGetValue("epsg", out string valueString) &&
                        int.TryParse(valueString, out int epsg))
                    {
                        return epsg;
                    }
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
                    name = Meshes1D[index].name;
                    break;
                case TopologyType.Mesh2dTopology:
                    name = Meshes2D[index].name;
                    break;
                case TopologyType.Network1dTopology:
                    name = Networks1D[index].name;
                    break;
                case TopologyType.ContactsTopology:
                    name = Contacts[index].name;
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
                    index = Meshes1D.FindIndex(item => GetName(item.name) == name);
                    break;
                case TopologyType.Mesh2dTopology:
                    index = Meshes2D.FindIndex(item => GetName(item.name) == name);
                    break;
                case TopologyType.Network1dTopology:
                    index = Networks1D.FindIndex(item => GetName(item.name) == name);
                    break;
                case TopologyType.ContactsTopology:
                    index = Contacts.FindIndex(item => GetName(item.name) == name);
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
    }
}