import java.io.*;
import java.net.*;

public class Server {
    public static void main(String[] args) {
        int port = 9090;
        try (ServerSocket serversocket = new ServerSocket(port)) {
            System.out.println("Server listening on port " + port);

            while (true) {
                Socket clientSocket = serversocket.accept();
                new Handler(clientSocket).start();
            }
        } catch (IOException e) {
            System.err.println("Error starting server: " + e.getMessage());
        }
    }
}

class Handler extends Thread {
    private final Socket socket;

    public Handler(Socket socket) {
        this.socket = socket;
    }

    

    @Override
    public void run() {
        try (BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {

            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                if (inputLine.equalsIgnoreCase("SHUTDOWN")) {
                    System.out.println("Closing server.");
                    System.exit(0);
                }

                try {
                    int number = Integer.parseInt(inputLine);
                    System.out.println("Received: " + number);
                    int incrementedNumber = number + 1;
                    System.out.println("Incremented: " + incrementedNumber);
                    out.println(incrementedNumber);
                } catch (NumberFormatException e) {
                    out.println("Invalid number");
                }
            }
        } catch (IOException e) {
            System.err.println("Error handling client connection: " + e.getMessage());
        } finally {
            try {
                socket.close();
            } catch (IOException e) {
                System.err.println("Error closing client socket: " + e.getMessage());
            }
        }
    }
}