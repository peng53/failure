using System;
using System.Data;
using Mono.Data.Sqlite;

namespace HierarchXml
{

	static class StringExtensions
	{
		public static string Abridged (this string value, int length)
		{
			if (value == null) {
				return null;
			}
			return value.Substring (0, Math.Min (value.Length, length));
		}
	}

	class MainClass
	{
		public static void Main (string [] args)
		{
			var dbConn = new Mono.Data.Sqlite.SqliteConnection ("Data Source=/mnt/ramdisk/test.db");
			dbConn.Open ();
			const string dbConnReadSql = "SELECT rowid, name FROM groups";
			var dbConnRead = dbConn.CreateCommand ();
			dbConnRead.CommandText = dbConnReadSql;

			const string for_each_group = "SELECT key,value FROM data WHERE gid=@GID";
			var dbConnEachGroup = dbConn.CreateCommand ();
			dbConnEachGroup.CommandText = for_each_group;

			IDataReader reader = dbConnRead.ExecuteReader ();
			while (reader.Read ()) {
				Console.WriteLine ($"{reader.GetInt32 (0)} {reader.GetString (1)}");
				dbConnEachGroup.Parameters.AddWithValue ("@GID", reader.GetInt32 (0));
				using (IDataReader data_reader = dbConnEachGroup.ExecuteReader ()){
					while (data_reader.Read ()) {
						Console.WriteLine ($"{string.Format("{0,-24}",data_reader.GetString (0).Abridged(24))} - {data_reader.GetString (1).Abridged(48)}");
					}
				}
			}

			reader.Dispose ();
			dbConnRead.Dispose ();
			dbConn.Clone ();
		}
	}
}
