import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.util.*;

public class HRImpl extends UnicastRemoteObject implements HRInterface {
    private Map<String, Integer> rooms;
    private Map<String, List<String>> bookings;
    private Map<String, List<String>> notifications;
   
    public HRImpl() throws RemoteException {
        rooms = new HashMap<>();
        bookings = new HashMap<>();
        notifications = new HashMap<>();
        
        rooms.put("A", 40);
        rooms.put("B", 35);
        rooms.put("C", 25);
        rooms.put("D", 30);
        rooms.put("E", 20);
    }
  
    public List<String> listRooms() throws RemoteException {
        List<String> availableRooms = new ArrayList<>();
        rooms.forEach((type, count) -> {
            String price;
            switch (type) {
                case "A":
                    price = "75 Ευρώ";
                    break;
                case "B":
                    price = "110 Ευρώ";
                    break;
	        case "C":
                    price = "120 Ευρώ";
                    break;
                case "D":
                    price = "150 Ευρώ";
                    break;
                case "E":
                    price = "200 Ευρώ";
                    break;
                default:
                    price = "Unknown";
            }
	    availableRooms.add(count + " δωμάτια τύπου " + type + " - τιμή: " + price + " τη βραδιά");
        });
        return availableRooms;
    }

     public String bookRoom(String type, int number, String name) throws RemoteException {
        if (rooms.get(type) >= number) {
            rooms.put(type, rooms.get(type) - number);
            bookings.computeIfAbsent(name, k -> new ArrayList<>()).add(type + " x" + number);
            return "Κράτηση επιτυχής για " + number + " δωμάτια τύπου " + type + " στο όνομα " + name;
        } else {
            return "Αποτυχία κράτησης. Διαθέσιμα δωμάτια: " + rooms.get(type);
        }
    }

     public List<String> listGuests() throws RemoteException {
        List<String> guestList = new ArrayList<>();
        bookings.forEach((name, bookedRooms) -> {
            int totalCost = bookedRooms.stream().mapToInt(room -> {
                String[] parts = room.split(" x");
                String type = parts[0];
                int count = Integer.parseInt(parts[1]);
                int cost = 0;
		switch (type) {
                    case "A":
                        cost = count * 75;
                        break;
                    case "B":
                        cost = count * 110;
                        break;
                    case "C":
                        cost = count * 120;
                        break;
                    case "D":
                        cost = count * 150;
                        break;
		    case "E":
                        cost = count * 200;
                        break;
                }
                return cost;
            }).sum();
            guestList.add(name + ": " + bookedRooms + " - συνολικό κόστος: " + totalCost + " Ευρώ");
        });
        return guestList;
    }

    public String cancelBooking(String type, int number, String name) throws RemoteException {
        List<String> userBookings = bookings.get(name);
        if (userBookings != null) {
        	// Υπολογισμός του συνολικού αριθμού δωματίων του συγκεκριμένου τύπου που έχει κρατήσει ο χρήστης
            int totalBooked = userBookings.stream()
                .filter(booking -> booking.startsWith(type + " x"))
                .mapToInt(booking -> Integer.parseInt(booking.split(" x")[1]))
                .sum();

                // Έλεγχος αν ο χρήστης προσπαθεί να ακυρώσει περισσότερα δωμάτια από αυτά που έχει κρατήσει
            if (number > totalBooked) {
                return "Αποτυχία ακύρωσης. Προσπαθείτε να ακυρώσετε περισσότερα δωμάτια από αυτά που έχετε κρατήσει.";
            }

            int roomsToCancel = number;
            boolean bookingFound = false;
             Iterator<String> iterator = userBookings.iterator();
            while (iterator.hasNext() && roomsToCancel > 0) {
                String booking = iterator.next();
                String[] parts = booking.split(" x");
                if (parts[0].equals(type)) {
                	int bookedCount = Integer.parseInt(parts[1]);
                    if (bookedCount <= roomsToCancel) {
                        roomsToCancel -= bookedCount;
                        iterator.remove();
                        rooms.put(type, rooms.get(type) + bookedCount);
                    } else {
                    	parts[1] = Integer.toString(bookedCount - roomsToCancel);
                        iterator.remove();
                        userBookings.add(parts[0] + " x" + parts[1]);
                        rooms.put(type, rooms.get(type) + roomsToCancel);
                        roomsToCancel = 0;
                    }
                    bookingFound = true;
                    }
            }
            if (bookingFound) {
                notifyClients(type);
                if (userBookings.isEmpty()) {
                    bookings.remove(name);
                }
                return "Ακύρωση επιτυχής για " + number + " δωμάτια τύπου " + type + " στο όνομα " + name + ". Υπόλοιπες κρατήσεις: " + userBookings;
            }
        }
        return "Αποτυχία ακύρωσης. Δεν βρέθηκαν τα αντίστοιχα δωμάτια.";
    }

    public void registerForNotification(String type, String name) throws RemoteException {
        notifications.computeIfAbsent(type, k -> new ArrayList<>()).add(name);
    }

    private void notifyClients(String type) throws RemoteException {
        List<String> clientsToNotify = notifications.get(type);
        if (clientsToNotify != null) {
	    for (String client : clientsToNotify) {
                System.out.println("Ειδοποίηση: Δωμάτια τύπου " + type + " είναι πλέον διαθέσιμα για τον " + client);
            }
            notifications.remove(type);
        }
    }
}

