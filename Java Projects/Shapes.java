class threedshape
{
	private double volume;
    private double surfacearea;
	public threedshape()
	{
        this.volume = 0.0;
        this.surfaceArea = 0.0;
    }
	
	public double calculatevolume();

    public double calculatesurfacearea();

    public double getvolume() 
	{
        return volume;
    }

    public double getsurfacearea() 
	{
        return surfacearea;
    }
	
}
class sphere extends threedshape
{
	private double aktina;
	
	sphere(double aktina)
	{
		this.aktina=aktina;
	}
	
	public double calculatevolume()
	{
		double volume = (4.0 / 3.0) * Math.PI * Math.pow(radius, 3);  //ογκος σφαιρας
        setVolume(volume);
        return volume;
	}
	
	public double calculatesurfacearea()
	{
		double surfaceArea = 4 * Math.PI * Math.pow(radius, 2);  //εμβαδον
        setSurfaceArea(surfaceArea);
        return surfaceArea;
	}
	private void setvolume(double volume)
	{
		this.volume=volume;
	}
	private void setsurfacearea(double surfacearea)
	{
		this.surfacearea=surfacearea;
	}
}

class cube extends threedshape
{
	private double side;
	public cube(double side)
	{
		this.side=side;
	}
	public double calculatevolume()
	{
		double volume=Math.pow(side,3);
		setvolume(volume);
		return volume;
	}
	public double calculatesurfacearea()
	{
		double surfacearea=6*Math.pow(side,2);
		setsurfacearea(surfacearea);
		return surfacearea;
	}
	private void setvolume(double volume)
	{
		this.volume=volume;
	}
	private void setsurfacearea(double surfacearea)
	{
		this.surfacearea=surfacearea;
	}
}

class rightcircularcylinder extends threedshape
{
	private double r;
	private double height;
	public rightcircularcylinder (double r, double height)
	{
		this.r=r;
		this.height=height;
	}
	public double calculatevolume()
	{
		double volume = Math.PI * Math.pow(radius, 2) * height;
		setvolume(volume);
		return volume;
	}
	public double calculatesurfacearea()
	{
		double surfaceArea = (2 * Math.PI * radius * height) + (2 * Math.PI * Math.pow(radius, 2));
        setSurfaceArea(surfaceArea);
        return surfaceArea;
	}
	private void setvolume(double volume)
	{
		this.volume=volume;
	}
	private void setsurfacearea(double surfacearea)
	{
		this.surfacearea=surfacearea;
	}
	
}

clas rectangularparallelepiped extends threedshape
{
	private double length, width, height;
	public rectangularparallelepiped (double length, double width, double height)
	{
		this.length=length;
		this.width=width;
		this.height=height;
	}
	public double calculatevolume()
	{
		double volume = length * width * height;
        setVolume(volume);
        return volume;
	}
	public double calculatesurfacearea()
	{
		double surfaceArea = 2 * ((length * width) + (width * height) + (height * length));
        setSurfaceArea(surfaceArea);
        return surfaceArea;
	}
	private void setvolume(double volume)
	{
		this.volume=volume;
	}
	private void setsurfacearea(double surfacearea)
	{
		this.surfacearea=surfacearea;
	}
	
}

class Shapes
{
	public static void main(String[] args) 
	{
        sphere sphere = new sphere(3.0);
        double spherevolume = sphere.calculatevolume();
        double spheresurfacearea = sphere.calculatesurfacearea();

        System.out.println("Sphere:");
        System.out.println("Volume: " + spherevolume);
        System.out.println("Surface Area: " + spheresurfacearea);

        System.out.println();

        rightcircularcylinder cylinder = new rightcircularcylinder(2.0, 5.0);
        double cylindervolume = cylinder.calculatevolume();
        double cylindersurfacearea = cylinder.calculatesurfacearea();

        System.out.println("Right Circular Cylinder:");
        System.out.println("Volume: " + cylindervolume);
        System.out.println("Surface Area: " + cylindersurfacearea);

        System.out.println();

        cube cube = new cube(4.0);
        double cubevolume = cube.calculatevolume();
        double cubesurfacearea = cube.calculatesurfacearea();

        System.out.println("Cube:");
        System.out.println("Volume: " + cubevolume);
        System.out.println("Surface Area: " + cubesurfacearea);

        System.out.println();

        rectangularparallelepiped parallelepiped = new rectangularparallelepiped(3.0, 4.0, 5.0);
        double parallelepipedvolume = parallelepiped.calculatevolume();
        double parallelepipedsurfacearea = parallelepiped.calculatesurfacearea();

        System.out.println("Rectangular Parallelepiped:");
        System.out.println("Volume: " + parallelepipedvolume);
        System.out.println("Surface Area: " + parallelepipedsurfacearea);
    }
}