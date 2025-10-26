import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.Naming;

public class HRServer {
    public static void main(String[] args) {
        try {
            HRInterface hr = new HRImpl();
            Registry registry = LocateRegistry.createRegistry(1099);
            Naming.rebind("HotelReservation", hr);
            System.out.println("HRServer is running...");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

