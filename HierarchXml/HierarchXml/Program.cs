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
		public static void Main (string [] args)
		{
            var bookmarks = new Bookmarks();
			using (var dbConn = new Mono.Data.Sqlite.SqliteConnection ("Data Source=/mnt/ramdisk/test.db"))
			{
				dbConn.Open ();
				const string dbConnReadSql = "SELECT rowid, name FROM groups";
				var dbConnRead = dbConn.CreateCommand ();
				dbConnRead.CommandText = dbConnReadSql;

				using (IDataReader reader = dbConnRead.ExecuteReader ()) 
                {
					const string for_each_group = "SELECT key,value FROM data WHERE gid=@GID";
					var dbConnEachGroup = dbConn.CreateCommand ();
					dbConnEachGroup.CommandText = for_each_group;

					while (reader.Read ())
					{
                        int gid = reader.GetInt32(0);
                        string name = reader.GetString(1);
                        bookmarks.GroupAdd(gid, new Group { Id = gid, Name = name });
                        Console.WriteLine ($"{gid} {name}");
						dbConnEachGroup.Parameters.AddWithValue ("@GID", gid);
						using (IDataReader data_reader = dbConnEachGroup.ExecuteReader ())
						{
							while (data_reader.Read ()) 
							{
                                bookmarks.LinkAdd(gid, new LinkItem { Title = data_reader.GetString(0), Target = data_reader.GetString(1) });
                                Console.WriteLine ($"{string.Format ("{0,-24}", data_reader.GetString (0).Abridged (24))} - {data_reader.GetString (1).Abridged (48)}");
							}
						}
					}
				}
			}
		}
	}
}
