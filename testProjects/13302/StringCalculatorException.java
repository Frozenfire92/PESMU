// Fig. 14.1: DivideByZeroException.java
// Definition of class DivideByZeroException.
// Used to throw an exception when a
// divide-by-zero is attempted.
public class StringCalculatorException extends ArithmeticException {
             
   // no-argument constructor specifies default error message
   public StringCalculatorException() 
   {
      super( "Attempted to divide by zero" );
   }

   // constructor to allow customized error message
   public StringCalculatorException( String message )
   {
      super( message );
   }

}  // end class DivideByZeroException
