import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.Naming;
import java.util.Scanner;

public class HRClient {
    public static void main(String[] args) {
        if (args.length == 0) {
            printUsage();
            return;
        }
	
	  try {
            String host = args[1];
            HRInterface hr = (HRInterface) Naming.lookup("//" + host + "/HotelReservation");

            switch (args[0]) {
                case "list":
                    hr.listRooms().forEach(System.out::println);
                    break;
                case "book":
			 String type = args[2];
                    int number = Integer.parseInt(args[3]);
                    String name = args[4];
                    String result = hr.bookRoom(type, number, name);
                    System.out.println(result);
		    if (result.startsWith("Αποτυχία κράτησης")) {
                        System.out.println("Θέλετε να ειδοποιηθείτε όταν τα δωμάτια γίνουν διαθέσιμα; (ναι/όχι)");
                        Scanner scanner = new Scanner(System.in);
                        String response = scanner.nextLine();
			if (response.equalsIgnoreCase("nai")) {
                            hr.registerForNotification(type, name);
                            System.out.println("Έχετε εγγραφεί στη λίστα ειδοποιήσεων.");
                        }
                    }
                    break;
		case "guests":
                    hr.listGuests().forEach(System.out::println);
                    break;
                case "cancel":
                    if (args.length < 5) {
                        System.out.println("Usage: java HRClient cancel <host> <type> <number> <name>");
                        return;
                    }
                    type = args[2];
                    number = Integer.parseInt(args[3]);
                    name = args[4];
		    result = hr.cancelBooking(type, number, name);
                    System.out.println(result);
                    break;
                default:
                    System.out.println("Invalid command");
                    printUsage();
                    break;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


private static void printUsage() {
        System.out.println("Usage: java HRClient <command> <host> [<args>]");
        System.out.println("Commands:");
        System.out.println("  list <host>                          - List all available rooms");
	System.out.println("  book <host> <type> <number> <name>   - Book rooms");
        System.out.println("  guests <host>                        - List all guests");
        System.out.println("  cancel <host> <type> <number> <name> - Cancel booking");
	System.out.println();
        System.out.println("Room Types and Prices:");
        System.out.println("  A - 75 Ευρώ");
        System.out.println("  B - 110 Ευρώ");
        System.out.println("  C - 120 Ευρώ");
        System.out.println("  D - 150 Ευρώ");
        System.out.println("  E - 200 Ευρώ");
    }


}

