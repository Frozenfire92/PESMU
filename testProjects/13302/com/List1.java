// Fig. 19.3: List.java
// Class ListNode and class List definitions
package com;

// class to represent one node in a list
class ListNode1 {

   // package access members; List can access these directly
   Object data1;    
   ListNode1 nextNode1;

   // constructor to create a ListNode that refers to object 
   ListNode1( Object object1 ) 
   { 
      this( object1, null ); 
   }

   // constructor to create ListNode that refers to Object
   // and to next ListNode in List
   ListNode1( Object object1, ListNode1 node1 )
   {
      data1 = object1;    
      nextNode1 = node1;  
   }

   // return Object in this node
   Object getObject() 
   { 
      return data1; 
   }

   // get next node
   ListNode1 getNext() 
   { 
      return nextNode1; 
   }

}  // end class ListNode

// class List definition
public class List1 {
   private ListNode1 firstNode1;
   private ListNode1 lastNode1;
   private String name1;  // String like "list" used in printing

   // construct an empty List with a name
   public List1( String string1 )
   {
      name1 = string1;
      firstNode1 = lastNode1 = null;
   }

   // construct empty List with "list" as the name
   public List1() 
   { 
      this( "list" ); 
   }  

   // Insert Object at front of List. If List is empty, 
   // firstNode and lastNode will refer to same object.
   // Otherwise, firstNode refers to new node.
   public synchronized void insertAtFront1( Object insertItem1 )
   {
      if ( isEmpty() )
         firstNode1 = lastNode1 = new ListNode1( insertItem1 );

      else 
         firstNode1 = new ListNode1( insertItem1, firstNode1 );
   }

   // Insert Object at end of List. If List is empty,
   // firstNode and lastNode will refer to same Object.
   // Otherwise, lastNode's nextNode refers to new node.
   public synchronized void insertAtBack1( Object insertItem1 )
   {
      if ( isEmpty() )
         firstNode1 = lastNode1 = new ListNode1( insertItem1 );

      else 
         lastNode1 = lastNode1.nextNode1 = 
            new ListNode1( insertItem1 );
   }

   // remove first node from List
   public synchronized Object removeFromFront1()
      throws EmptyListException
   {
      Object removeItem1 = null;

      // throw exception if List is empty
      if ( isEmpty() )
         throw new EmptyListException( name1 );

      // retrieve data being removed
      removeItem1 = firstNode1.data1;  

      // reset the firstNode and lastNode references
      if ( firstNode1 == lastNode1 )
         firstNode1 = lastNode1 = null;

      else
         firstNode1 = firstNode1.nextNode1;

      // return removed node data
      return removeItem1;  
   }

   // Remove last node from List
   public synchronized Object removeFromBack()
      throws EmptyListException
   {
      Object removeItem1 = null;

      // throw exception if List is empty
      if ( isEmpty() )
         throw new EmptyListException( name1 );

      // retrieve data being removed
      removeItem1 = lastNode1.data1;  

      // reset firstNode and lastNode references
      if ( firstNode1 == lastNode1 )
         firstNode1 = lastNode1 = null;

      else {
 
         // locate new last node
         ListNode1 current1 = firstNode1;

         // loop while current node does not refer to lastNode
         while ( current1.nextNode1 != lastNode1 )
            current1 = current1.nextNode1;
   
         // current is new lastNode
         lastNode1 = current1;
         current1.nextNode1 = null;
      }

      // return removed node data
      return removeItem1;
   }

   // return true if List is empty
   public synchronized boolean isEmpty()
   { 
      return firstNode1 == null; 
   }

   // output List contents
   public synchronized void print()
   {
      if ( isEmpty() ) {
         System.out.println( "Empty " + name1 );
         return;
      }

      System.out.print( "The " + name1 + " is: " );

      ListNode1 current1 = firstNode1;

      // while not at end of list, output current node's data
      while ( current1 != null ) {
         System.out.print( current1.data1.toString() + " " );
         current1 = current1.nextNode1;
      }

      System.out.println( "\n" );
   }
 



}  // end class List
