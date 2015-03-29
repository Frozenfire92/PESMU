public class HelloWorld { 
   public static void main(String[] args) { 
      System.out.println("Hello, World!");
	  try{
		Thread.sleep(2000);	  	  
	  }
	  catch(InterruptedException e){

	  }
	  GoodbyeWorld.msg();
	  Gaben.msg();
   }
}