
// Derived from class List
package com;

public class StackInheritance extends List{

   // construct stack
   public StackInheritance() 
   { 
      super( "stack" ); 
   }

   // add object to stack
   public synchronized void push( Object object )
   { 
      insertAtFront( object ); 
   }

   // remove object from stack
   public synchronized Object pop() throws EmptyListException
   { 
      return removeFromFront(); 
   }





}  // end class StackInheritance

