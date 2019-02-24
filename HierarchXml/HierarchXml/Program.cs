using System;
using System.Data;
using System.Collections.Generic;
using Mono.Data.Sqlite;

namespace HierarchXml
{

	static class StringExtensions
	{
		public static string Abridged (this string value, int length)
		{
			if (value == null)
			{
				return null;
			}
			return value.Substring (0, Math.Min (value.Length, length));
		}
	}

    class MainClass
    {
        public static void ApplyRow(IDataReader data, Action<IDataReader> action)
        {
            while (data.Read())
            {
                action(data);
            }
        }
        public static void Main(string[] args)
        {
            var bookmarks = new Bookmarks();
            using (var dbConn = new Mono.Data.Sqlite.SqliteConnection("Data Source=/mnt/ramdisk/test.db"))
            {
                dbConn.Open();

                var cmdGetGroups = dbConn.CreateCommand();
                cmdGetGroups.CommandText = "SELECT gid, name, IFNULL(pid,0) FROM rel JOIN groups ON rel.gid=groups.rowid WHERE depth=1";
                using (IDataReader groups = cmdGetGroups.ExecuteReader())
                {
                    ApplyRow(groups, (g) => bookmarks.GroupAdd(g.GetInt32(0), new Group { Id = g.GetInt32(0), Name = g.GetString(1), Pid = g.GetInt32(2) }));
                }
                var cmdGetRows = dbConn.CreateCommand();
                cmdGetRows.CommandText = "SELECT IFNULL(gid,0), key, value FROM data";
                using (IDataReader rows = cmdGetRows.ExecuteReader())
                {
                    ApplyRow(rows, (r) => bookmarks.LinkAdd(r.GetInt32(0), new LinkItem { Title = r.GetString(1), Target = r.GetString(2) }));
                }
            }

            foreach (var group in bookmarks.Groups)
            {
                Console.WriteLine($"{group.Key} = {group.Value.Id} | {group.Value.Name} / {group.Value.Pid}"); 
            }
            foreach (var group in bookmarks.Links)
            {
                Console.WriteLine(group.Key);
                foreach (var link in group.Value)
                {
                    Console.WriteLine($"\t{link.Title} {link.Target.Abridged(48)}");
                }
            }
        }
    }
}
