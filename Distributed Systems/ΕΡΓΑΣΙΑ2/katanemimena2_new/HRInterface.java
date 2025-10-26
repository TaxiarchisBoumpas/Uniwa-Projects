import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.List;

public interface HRInterface extends Remote {
    List<String> listRooms() throws RemoteException;
    String bookRoom(String type, int number, String name) throws RemoteException;
    List<String> listGuests() throws RemoteException;
    String cancelBooking(String type, int number, String name) throws RemoteException;
    void registerForNotification(String type, String name) throws RemoteException;
}

