import java.net.*;
import java.io.*;

public class DateServer
{
	public static void main(String[] args) {
		try {
			ServerSocket sock = new ServerSocket(5575);

			while(true) {
				Socket client = sock.accept();

				PrintWriter pout = new
					PrintWriter(client.getOutputStream(), true);
				String quote = "白日依山盡，黃河入海流。欲窮千里目，更上一層樓。";
				pout.println(quote);

				client.close();
			}
		}
		catch(IOException ioe) {
			System.err.println(ioe);
		}
	}
}
