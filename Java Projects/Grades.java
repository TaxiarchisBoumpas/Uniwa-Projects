import java.util.Scanner;
public class Grades 
{
	public static void main(String[] args) 
	{
		Scanner scanner = new Scanner(System.in);
		while(true)
		{
			system.out.print("Εισάγεται αριθμό μητρώου: ");
			String studentId = scanner.next();
			
			if (studentId.equals("000000")
			{
				break;
		    }
			double studentGrade = 0;
            int studentNumCourses = 0;
			
			while(true)
			{
				system.out.print("Εισάγεται μάθημα (ή 'end' για τερματισμό)");
				String course = scanner.next();
				
				if (course.equals("end"))
				{
					break;
				}
				system.out.print("enter grade: ");
				double grade = scanner.nextFloat();
				
				studentgrade +=grade;
				studentNumCourses++;
			}
			if (studentNumCourse>0)
			{
				double averagegrade = studentGrade / studenNumCourses;
				System.out.println("Μέση εξεταστική βαθμολογία για τον φοιτητή με αριθμό μητρώου " + studentId + ": " + averageGrade);
				System.out.println();
			}
		}	
		
	}
}	
    