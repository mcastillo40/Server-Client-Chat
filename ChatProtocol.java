import java.net.*;
import java.io.*;

public class ChatProtocol {
    BufferedReader stdIn = new BufferedReader( new InputStreamReader(System.in) );

    public String processInput(String cltInput) throws IOException {
        String theOutput = null;
        Boolean contains;

        try {
            if (cltInput != null){
                contains = cltInput.contains("\\quit");

                if (!contains) {
                    System.out.println(cltInput);

                    System.out.print("Server> ");
                    theOutput = stdIn.readLine();
                }
            } 
        }
        catch (IOException e) {
            System.err.println("ERROR: " + e);
            System.exit(1);
        } 
    
        return theOutput;
    }
}
