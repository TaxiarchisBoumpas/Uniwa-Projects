import java.util.Scanner;

class futurevalue
{
	public static void main (String args[])
	{
		System.out.print("Enter monthly investment: ");
		Scanner read = new Scanner(System.in);
		double inv = read.nextDouble();
		System.out.print("Enter yearly interest rate: ");
		double rate = read.nextDouble();
		System.out.print("Enter number of years: ");
		int years = read.nextInt();
		double futureinv=0;
		double monthlyinv=(rate/12)/100;
		double totalinv=monthlyinv*years*12;
		for (int i=0; i < years*12; i++)
		{
			futureinv = (futureinv + inv) * (1+monthlyinv);
		}
		System.out.println("The total investement is: "+ futureinv);
		
	}
}