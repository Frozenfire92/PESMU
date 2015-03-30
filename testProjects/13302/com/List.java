// Fig. 19.3: List.java
// Class ListNode and class List definitions
package com;

// class to represent one node in a list
class ListNode {

   // package access members; List can access these directly
   Object data;    
   ListNode nextNode;

   // constructor to create a ListNode that refers to object 
   ListNode( Object object ) 
   { 
      this( object, null ); 
   }

   // constructor to create ListNode that refers to Object
   // and to next ListNode in List
   ListNode( Object object, ListNode node )
   {
      data = object;    
      nextNode = node;  
   }

   // return Object in this node
   Object getObject() 
   { 
      return data; 
   }

   // get next node
   ListNode getNext() 
   { 
      return nextNode; 
   }

}  // end class ListNode

// class List definition
public class List {
   private ListNode firstNode;
   private ListNode lastNode;
   private String name;  // String like "list" used in printing

   // construct an empty List with a name
   public List( String string )
   {
      name = string;
      firstNode = lastNode = null;
   }

   // construct empty List with "list" as the name
   public List() 
   { 
      this( "list" ); 
   }  

   // Insert Object at front of List. If List is empty, 
   // firstNode and lastNode will refer to same object.
   // Otherwise, firstNode refers to new node.
   public synchronized void insertAtFront( Object insertItem )
   {
      if ( isEmpty() )
         firstNode = lastNode = new ListNode( insertItem );

      else 
         firstNode = new ListNode( insertItem, firstNode );
   }

   // Insert Object at end of List. If List is empty,
   // firstNode and lastNode will refer to same Object.
   // Otherwise, lastNode's nextNode refers to new node.
   public synchronized void insertAtBack( Object insertItem )
   {
      if ( isEmpty() )
         firstNode = lastNode = new ListNode( insertItem );

      else 
         lastNode = lastNode.nextNode = 
            new ListNode( insertItem );
   }

   // remove first node from List
   public synchronized Object removeFromFront()
      throws EmptyListException
   {
      Object removeItem = null;

      // throw exception if List is empty
      if ( isEmpty() )
         throw new EmptyListException( name );

      // retrieve data being removed
      removeItem = firstNode.data;  

      // reset the firstNode and lastNode references
      if ( firstNode == lastNode )
         firstNode = lastNode = null;

      else
         firstNode = firstNode.nextNode;

      // return removed node data
      return removeItem;  
   }

   // Remove last node from List
   public synchronized Object removeFromBack()
      throws EmptyListException
   {
      Object removeItem = null;

      // throw exception if List is empty
      if ( isEmpty() )
         throw new EmptyListException( name );

      // retrieve data being removed
      removeItem = lastNode.data;  

      // reset firstNode and lastNode references
      if ( firstNode == lastNode )
         firstNode = lastNode = null;

      else {
 
         // locate new last node
         ListNode current = firstNode;

         // loop while current node does not refer to lastNode
         while ( current.nextNode != lastNode )
            current = current.nextNode;
   
         // current is new lastNode
         lastNode = current;
         current.nextNode = null;
      }

      // return removed node data
      return removeItem;
   }

   // return true if List is empty
   public synchronized boolean isEmpty()
   { 
      return firstNode == null; 
   }

   // output List contents
   public synchronized void print()
   {
      if ( isEmpty() ) {
         System.out.println( "Empty " + name );
         return;
      }

      System.out.print( "The " + name + " is: " );

      ListNode current = firstNode;

      // while not at end of list, output current node's data
      while ( current != null ) {
         System.out.print( current.data.toString() + " " );
         current = current.nextNode;
      }

      System.out.println( "\n" );
   }
 



}  // end class List
