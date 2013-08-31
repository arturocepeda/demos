using System;
using System.IO;

namespace PathFinding
{
	public class ParserMeshGraph
	{
		public void LoadUnreachableNodesFromFile(MeshGraph meshGraph, string fileName)
		{
			string[] nodeList = LoadTextContents(fileName).Split(',');

			for(int i = 0; i < nodeList.Length; i++)
				meshGraph.SetUnreachableNode(Int32.Parse(nodeList[i]));
		}

		protected string LoadTextContents(string fileName)
		{
			TextReader tr = new StreamReader(fileName);
			string fileContents = tr.ReadToEnd();
			tr.Close();

			return fileContents;
		}
	}
}
