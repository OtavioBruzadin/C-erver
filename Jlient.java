import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class Jlient {
    public static void main(String[] args) {
        String serverAddress = "127.0.0.1";
        int port = 9002;

        try (
                Socket socket = new Socket(serverAddress, port);
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                Scanner scanner = new Scanner(System.in)
        ) {
            System.out.println("Conectado ao servidor " + serverAddress + " na porta " + port);

            while (true) {
                System.out.println("Digite a expressão (n1 operador n2) ou 'EXIT' para sair:");
                String input = scanner.nextLine();

                out.println(input);

                if (input.equalsIgnoreCase("EXIT")) {
                    String exitResponse = in.readLine();
                    if (exitResponse != null) {
                        System.out.println("Resposta do servidor: " + exitResponse);
                    }
                    break;
                }

                String response = in.readLine();
                if (response == null) {
                    System.out.println("Servidor encerrou a conexão.");
                    break;
                }
                System.out.println("Resposta do servidor: " + response);
            }
        } catch (Exception e) {
            System.err.println("Erro na conexão ou comunicação: " + e.getMessage());
            e.printStackTrace();
        }
        System.out.println("Cliente encerrado.");
    }
}