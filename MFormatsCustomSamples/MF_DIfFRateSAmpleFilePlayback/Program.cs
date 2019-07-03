using System;
using System.Windows.Forms;

namespace SampleFilePlayback
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm(args));
		}

		static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e)
		{// All exceptions thrown by the main thread are handled over this method

			MessageBox.Show("Error occurred: " + e.Exception.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
		}

		static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
		{
			try
			{
				Exception ex = (Exception)e.ExceptionObject;
				MessageBox.Show("Fatal Error: " + ex.Message, "Fatal Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			finally
			{
				Application.Exit();
			}
		}
	}
}