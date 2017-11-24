using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace AssemblyExtractor
{
    public class PropertyData
    {
        public string Name;
        public string Type;
    }

    public class FieldData
    {
        public bool IsPublic;
        public string Name;
        public string Type;
    }

    public class MethodData
    {
        public bool IsPublic;
        public string Name;
    }

    public class ExtractorData
    {
        public string Name;
        public string FullName;

        public List<MethodData> Methods = new List<MethodData>();
        public List<FieldData> Fields = new List<FieldData>();
        public List<PropertyData> Properties = new List<PropertyData>();
    }

    public class Extractor
    {
        public static List<ExtractorData> ExtractTypes(string assemblyName)
        {
            try
            {
                List<ExtractorData> data = new List<ExtractorData>();

                Assembly asm = Assembly.LoadFile(assemblyName);
                Type[] ts = asm.GetTypes();
                foreach (Type type in ts)
                {
                    ExtractorData ed = new ExtractorData();
                    ed.Name = type.Name;
                    ed.FullName = type.FullName;

                    MethodInfo[] mis = type.GetMethods(BindingFlags.Static | BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
                    foreach (MethodInfo mi in mis)
                    {
                        MethodData md = new MethodData();
                        md.IsPublic = mi.IsPublic;
                        md.Name = mi.Name;
                        ed.Methods.Add(md);
                    }

                    FieldInfo[] fis = type.GetFields(BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
                    foreach (FieldInfo fi in fis)
                    {
                        FieldData fd = new FieldData();
                        fd.IsPublic = fi.IsPublic;
                        fd.Type = fi.FieldType.ToString();
                        fd.Name = fi.Name;
                        ed.Fields.Add(fd);
                    }

                    PropertyInfo[] pis = type.GetProperties(BindingFlags.DeclaredOnly | BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic);
                    foreach (PropertyInfo pi in pis)
                    {
                        PropertyData pd = new PropertyData();
                        pd.Type = pi.PropertyType.ToString();
                        pd.Name = pi.Name;
                        ed.Properties.Add(pd);
                    }

                    data.Add(ed);
                }

                return data;
            }
            catch (Exception ex)
            {
                return null;
            }
        }
    }
}
