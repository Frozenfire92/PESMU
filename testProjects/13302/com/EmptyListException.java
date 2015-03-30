
// Class EmptyListException definition
package com;

public class EmptyListException extends RuntimeException {

   // initialize an EmptyListException
   public EmptyListException( String name )
   {
      super( "The " + name + " is empty" );
   }

}  // end class EmptyListException
