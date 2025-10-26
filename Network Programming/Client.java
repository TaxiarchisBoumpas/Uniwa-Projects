import java.io.*;
import java.net.*;
import java.util.Random;

public class Client {
    public static void main(String[] args) {
        String host = "LocalHost";
        int port = 9090;
        Random random = new Random();

        for (int i = 0; i < 20; i++) {
            new Thread(() -> {
                try (Socket socket = new Socket(host, port);
                     BufferedReader reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                     PrintWriter writer = new PrintWriter(socket.getOutputStream(), true)) {

                    for (int j = 0; j < 5; j++) {
                        int number = random.nextInt(100);
                        System.out.println("Sending: " + number);
                        writer.println(number);
                        String response = reader.readLine();
                        System.out.println("Received: " + response);

                        if (response == null || !response.equals(String.valueOf(number + 1))) {
                            System.exit(1);
                        }

                        Thread.sleep(500);
                    }
                } catch (IOException | InterruptedException e) {
                    System.err.println("Thread interrupted: " + e.getMessage());
                }
            }).start();

            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                System.err.println("Main thread interrupted: " + e.getMessage());
            }
        }

        System.out.println("All threads completed!!!");
    }
}



