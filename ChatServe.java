import java.net.*;
import java.io.*;
import java.lang.*;

public class ChatServe {
    public static void main(String[] args) throws IOException
    { 
        int port = 0;
        Boolean listening = true; 

        if (args.length > 0) {
            port = Integer.parseInt(args[0]);

            System.out.println("Server Started on port: " + port);
                try (
                    ServerSocket servSocket = new ServerSocket(port);
                    
                ){
                    while(listening) {
                        Socket cltSocket = servSocket.accept();

                        PrintWriter out = new PrintWriter(cltSocket.getOutputStream(), true);
                        BufferedReader in = new BufferedReader( new InputStreamReader(cltSocket.getInputStream()) );
                        String inputLine, outputLine;

                        System.out.println("Client Connected");

                        // Initiate conversation with client
                        ChatProtocol cp = new ChatProtocol();

                        while ((inputLine = in.readLine()) != null) {

                            outputLine = cp.processInput(inputLine);

                            if (outputLine != null) {
                                if (outputLine.equals("\\quit")) {
                                    out.println("Server Left");

                                    // Quit system
                                    System.exit(0);
                                }

                                outputLine = "Server> " + outputLine;
                                out.println(outputLine);
                            }
                        }
                    }
            } catch (NumberFormatException e) {
                    System.err.println("Argument" + args[0] + " must be an integer.");
                    System.exit(1);
            }
        }
    }
}