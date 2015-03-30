
// Derived from class List
package com;

public class StackInheritance1 extends List1{

   // construct stack
   public StackInheritance1() 
   { 
      super( "stack1" ); 
   }

   // add object to stack
   public synchronized void push1( Object object1 )
   { 
      insertAtFront1( object1 ); 
   }

   // remove object from stack
   public synchronized Object pop1() throws EmptyListException
   { 
      return removeFromFront1(); 
   }



}  // end class StackInheritance

