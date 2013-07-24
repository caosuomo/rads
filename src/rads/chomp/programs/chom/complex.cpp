// Copyright (C) 2004 by William D Kalies. All rights reserved.
//
//-----------------------------------------------------------------------------
// complex.cc:
//-----------------------------------------------------------------------------
/*! \file complex.cc
    \brief Methods for class complex.

    Version 3.0

    Last modified by BK on 1/23/02.
*/

#include "common.h"
#include "complex.h"
#include "cell.h"
#include "edge.h"
#include "vertex.h"
#include "ncell.h"
#include "block.h"

#define CHL

void complex::Recurse(node* n, list<block*>* bl, int level)
{
   if (n==NULL)
     return;
   if (level==DIM)
     {
	bl->push_back((block *)n);
	return;
     }
   else
     {
	++level;
	Recurse(n->Left,bl,level);
	Recurse(n->Right,bl,level);
	return;
     }
}

void complex::Delete(node* n, int level)
{
   if ((n==NULL)||(n==cubes.Root))
     return;
   if (level==DIM)
     {
	delete (block*)n;
	return;
     }
   else
     {
	++level;
	Delete(n->Left,level);
	Delete(n->Right,level);
	delete n;
	return;
     }
}

block* complex::Merge(node* n)
{
   list<block*> sub_blocks;
   Recurse(n,&sub_blocks,0);

   list<block*>::iterator sbit=sub_blocks.begin();
//   if (sbit==sub_blocks.end())
//     exit(1);

   block* b=new block(BC(*sbit),this);
//    block* b=new block(*((*sbit)->bc),this);
//   assert(b->bdry_verts.Empty());
//   (b->bc)->SetAs(BC(*sbit));
   BC(b).SetAs(BC(*sbit));
   for(int i=0;i<DIM;++i)
     BC(b)++;
   int level=(cube_bits-BC(b).Bits())/DIM;
   for(int i=0;i<level*DIM;++i)
     BC(b)--;
   BC(b).Coordinates(min);
   for(int i=0;i<level*DIM;++i)
     BC(b)++;

   vertex* v;
   int vc[DIM];
   int flag=1;
   cell_iter bvit;
   cell_iter cit;
   cobdry_iter cbit;

   while(sbit!=sub_blocks.end())
     {
	bvit=(*sbit)->bdry_verts.Begin();
	while(bvit!=(*sbit)->bdry_verts.End())
	  {
             v=(vertex*)(*bvit);
	     v->bc->Coordinates(vc);
	     flag=1;
	     for (int i=0; i<DIM; ++i)
	       flag*=(((min[i]<vc[i])&&(vc[i]<min[i]+Power2(level)))||((vc[i]==0)||(vc[i]==Power2(cube_bits/DIM))));
             if (flag)
	       {
		  if (!(v->Interior()))
		    {
		       v->MarkInterior();
		       DeleteVertex(v);
		       b->cells[0].InsertUniqueSort(v);
		    }
	       }
	     else
	       b->bdry_verts.InsertUniqueSort(v);
	     ++bvit;
	  }
	for(int i=0;i<DIM;++i)
	   {
 	      cit=(*sbit)->cells[i].Begin();
	      while(cit!=(*sbit)->cells[i].End())
	         {
	            b->cells[i].InsertUniqueSort(Ptr(cit));
	            ++cit;
	         }
	   }
	++sbit;
     }

   cit=b->cells[0].Begin();
   while(cit!=b->cells[0].End())
     {
	cbit=((vertex*)(*cit))->Cobdry()->Begin();
//      cout << "size " << (*cit)->Cobdry()->Size() << endl;
	while(cbit!=((vertex*)(*cit))->Cobdry()->End())
	   {
	      ((edge*)Ptr(cbit))->MarkInterior();
//	      b->cells[1].InsertUniqueSort(Ptr(cbit));
	      if (b->cells[1].Find(Ptr(cbit))==b->cells[1].End())
		{
		   b->cells[1].InsertUniqueSort(Ptr(cbit));
		}
	      ++cbit;
	   }
	++cit;
     }
   
   cit=b->cells[1].Begin();
   while(cit!=b->cells[1].End())
     {
	cbit=((edge*)(*cit))->Cobdry()->Begin();
//      cout << "size " << (*cit)->Cobdry()->Size() << endl;
	while(cbit!=((edge*)(*cit))->Cobdry()->End())
	   {
	      ((ncell*)Ptr(cbit))->MarkInterior();
//	      b->cells[2].InsertUniqueSort(Ptr(cbit));
	      if (b->cells[2].Find(Ptr(cbit))==b->cells[2].End())
		{
		   b->cells[2].InsertUniqueSort(Ptr(cbit));
		}
	      ++cbit;
	   }
	++cit;
     }	
	
   for(int i=3;i<DIM;++i)
     {
	cit=b->cells[i-1].Begin();
	while(cit!=b->cells[i-1].End())
	  {
	     cbit=((ncell*)(*cit))->Cobdry()->Begin();
//             cout << "size " << (*cit)->Cobdry()->Size() << endl;
	     while(cbit!=((ncell*)(*cit))->Cobdry()->End())
	       {
		  ((ncell*)Ptr(cbit))->MarkInterior();
//		  b->cells[i].InsertUniqueSort(Ptr(cbit));
		  if (b->cells[i].Find(Ptr(cbit))==b->cells[i].End())
		    {
		       b->cells[i].InsertUniqueSort(Ptr(cbit));
		    }
		  ++cbit;
	       }
	     ++cit;
	  }
     }
   
   if (n==cubes.Root)
     cubes.Root=b;
   else
     {
       node* parent=n->Parent;
       if (parent->Left==n)
         parent->Left=b;
       else
         parent->Right=b;
       b->Parent=parent;	
     }	
   
   Delete(n,0);
   
   return(b);	
}

int complex::NewVertex(const bitcode& bc, cell** v)
{
//   assert((*v)==NULL);
   node* n=vertices.Find(bc);
   
   if (n==NULL)
     {
        (*v)=new vertex(bc);
	vert* vv=new vert(*v);
	vv->v=(*v);
	vertices.Insert(bc,vv);
	return(0);
     }
   else
     (*v)=((vert *)n)->v;
   return(1);
}

cell* complex::FindVertex(const bitcode& bc)
{
   node* n=vertices.Find(bc);
   if (n==NULL)
     return(NULL);
   else
     return(((vert *)n)->v);
}

void complex::Homology(ofstream* gout, ofstream* bout)
{
   int Betti[DIM];
   cell_iter cit;

   cout <<  endl << endl << "*** Homology ****" << endl;
   for (int i=0; i<DIM; ++i)
     {
	Betti[i]=0;
	cit=((block *)cubes.Root)->cells[i].Begin();
	while (cit!=((block *)cubes.Root)->cells[i].End())
	  {
	     ++(Betti[i]);
	     ++cit;
	  }
	cout << "Betti number " << i << " is " << Betti[i] << endl;
	if(i==0)
          (*bout) << Betti[i];
	else
	  (*bout) << " " << Betti[i];
     }
   (*bout) << endl;
   cout << "*****************" << endl << endl;

   bdry_list bl;
   bdry_iter bit;
   int coords[DIM];

   int gen_count=0;
   if (gen_flag[1])
     {
        ofstream gensize_out("gensize.dat");

//	edge* e;
        cit=((block *)cubes.Root)->cells[1].Begin();
        for(int gen=0; gen<Betti[1]; ++gen)
          {
//	     (*gout) << endl;
//	     (*gout) << "*** 1-Generator # " << gen+1 << " ***" << endl;
	     ((edge*)(*cit))->Generator(&bl,1);
	     ++gen_count;

	     if (bl.Size()<GEN_TOL)
	       {
		  cout << "Omitting 1d generator of size " << bl.Size() << " cells." << endl;
		  continue;
	       }
	     else
	       cout << "Writing 1d generator # " << gen_count << " of size " << bl.Size() << " cells." << endl;

             gensize_out << bl.Size() << endl;

	     bit=bl.Begin();
	     while(bit!=bl.End())
	       {
	          if ((*bit).Incidence()==0)
	            cout << "zero1:" << endl;
	          else
	            {
#ifdef CHL
	               (*gout) << gen_count << " ";
	               if ((*bit).Incidence() >= 0)
			 (*gout) << " ";
		       (*gout) << (*bit).Incidence() << " ";
		       ((edge*)Ptr(bit))->diagonal[0]->Coordinates(coords);
	               for (int i=0; i<DIM; ++i)
		         (*gout) << coords[i] << " ";
	               ((edge*)Ptr(bit))->diagonal[1]->Coordinates(coords);
		       for (int i=0; i<DIM; ++i)
		         (*gout) << coords[i] << " ";
		       (*gout) << endl;
#endif

#ifdef MATLAB
	               ((edge*)Ptr(bit))->diagonal[0]->Coordinates(coords);
	               for (int i=0; i<DIM; ++i)
		         (*gout) << coords[i] << " ";
	               ((edge*)Ptr(bit))->diagonal[1]->Coordinates(coords);
		       for (int i=0; i<DIM; ++i)
		         (*gout) << coords[i] << " ";
	               (*gout) << (*bit).Incidence() << " " << gen_count << " " << 1 << endl;
#endif

#ifdef COB
                       (*gout) << gen_count << " (" << (*bit).Incidence() << ") [(";
		       ((edge*)Ptr(bit))->diagonal[0]->Coordinates(coords);
		       for (int i=0; i<DIM-1; ++i)
		         (*gout) << coords[i] << ",";
                       (*gout) << coords[DIM-1] << "),(";
		       ((edge*)Ptr(bit))->diagonal[1]->Coordinates(coords);
		       for (int i=0; i<DIM-1; ++i)
		         (*gout) << coords[i] << ",";
		       (*gout) << coords[DIM-1] << ")]" << endl;
#endif

/*
		       e=(edge*)Ptr(bit);
	               e->diagonal[0]->Coordinates(coords);
	               for (int i=0; i<DIM; ++i)
		         (*gout) << coords[i] << " ";
	               e->diagonal[1]->Coordinates(coords);
		       for (int i=0; i<DIM; ++i)
		         (*gout) << coords[i] << " ";
	               (*gout) << (*bit).Incidence() << " " << gen_count << " " << 1 << endl;
*/
                       ++bit;
	            }
	       }
	     bl.Clear();
	     ++cit;
          }
     }
   
   for(int j=2;j<DIM;++j)
     {
        if (gen_flag[j])
          {   
//	     ncell* nc;
             cit=((block *)cubes.Root)->cells[j].Begin();
             for(int gen=0; gen<Betti[j]; ++gen)
               {
//	          (*gout) << endl; 
//	          (*gout) << "*** " << j << "-Generator # " << gen+1 << " ***" << endl;
	          ((ncell*)(*cit))->Generator(&bl,1);
		  ++gen_count;
		  
		  if (bl.Size()<GEN_TOL)
	            {
		       cout << "Omitting " << j << "d generator of size " << bl.Size() << " cells." << endl;
		       continue;
	            }
	          else
		    cout << "Writing " << j << "d generator # " << gen_count << " of size " << bl.Size() << " cells." << endl; 		  
		  
	          bit=bl.Begin();
	          while(bit!=bl.End())
	            {
	               if ((*bit).Incidence()==0)
	                 cout << "zero" << j << ":" << endl;
	               else
	                 {
#ifdef CHL		       
	                    (*gout) << gen_count << " ";
	                    if ((*bit).Incidence() >= 0)
			      (*gout) << " ";
			    (*gout) << (*bit).Incidence() << " ";
		            ((ncell*)Ptr(bit))->diagonal[0]->Coordinates(coords);
	                    for (int i=0; i<DIM; ++i)
		              (*gout) << coords[i] << " ";
	                    ((ncell*)Ptr(bit))->diagonal[1]->Coordinates(coords);
		            for (int i=0; i<DIM; ++i)
		              (*gout) << coords[i] << " ";
		            (*gout) << endl;
#endif			    
			    			    
#ifdef MATLAB			    
	                    ((ncell*)Ptr(bit))->diagonal[0]->Coordinates(coords);
	                    for (int i=0; i<DIM; ++i)
		              (*gout) << coords[i] << " ";
	                    ((ncell*)Ptr(bit))->diagonal[1]->Coordinates(coords);
		            for (int i=0; i<DIM; ++i)
		              (*gout) << coords[i] << " ";
	                    (*gout) << (*bit).Incidence() << " " << gen_count << " " << j << endl;
#endif
			    
#ifdef COB
			    (*gout) << gen_count << " (" << (*bit).Incidence() << ") [(";
			    ((ncell*)Ptr(bit))->diagonal[0]->Coordinates(coords);
	                    for (int i=0; i<DIM-1; ++i)
		              (*gout) << coords[i] << ",";
			    (*gout) << coords[DIM-1] << "),(";
	                    ((ncell*)Ptr(bit))->diagonal[1]->Coordinates(coords);
		            for (int i=0; i<DIM-1; ++i)
		              (*gout) << coords[i] << ",";
		            (*gout) << coords[DIM-1] << ")]" << endl;		       			    
#endif
			    
/*
			    nc=(ncell*)Ptr(bit);
	                    nc->diagonal[0]->Coordinates(coords);
	                    for (int i=0; i<DIM; ++i)
		              (*gout) << coords[i] << " ";
	                    nc->diagonal[1]->Coordinates(coords);
		            for (int i=0; i<DIM; ++i)
		              (*gout) << coords[i] << " ";
	                    (*gout) << (*bit).Incidence() << " " << gen_count << " " << j << endl;
*/
			    
	                    ++bit;
	                 }
	            }
	          bl.Clear();
	          ++cit;
	       }
          }
     }
}

bool complex::InsertCube(block* b)
{
   bitcode* bc=new bitcode(BC(b).Bits());
   bc->SetAs(BC(b));
   
   cubes.Insert(*bc,b);
   if (previous_cube==NULL)
     {
	previous_cube=new bitcode(cube_bits);
	previous_cube->SetAs(bc);
	delete bc;
        return true;	
     }

//   assert(previous_cube->Bits()==cube_bits);
   
   node* simplifiable_node;
   block* simplifiable_block;
   int levels=Ancestor(bc,previous_cube);  
   while (levels>=DIM)
     {
	for (int j=0; j<DIM; ++j)
	   (*previous_cube)++;
	levels-=DIM;
        simplifiable_node=cubes.Find(*previous_cube);
	simplifiable_block=Merge(simplifiable_node);
	Simplify(simplifiable_block);
      }
   
   delete previous_cube;
   previous_cube=new bitcode(cube_bits);
   previous_cube->SetAs(*bc);
   delete bc;
   return true;
}

void complex::FinalCube()
{
   int levels=previous_cube->Bits();
   node*  simplifiable_node;
   block* simplifiable_block;
   while (levels>=DIM)
     {
	for (int j=0; j<DIM; ++j)
	  (*previous_cube)++;
	levels-=DIM;
        simplifiable_node=cubes.Find(*previous_cube);
	simplifiable_block=Merge(simplifiable_node);
	Simplify(simplifiable_block);
      }
   delete previous_cube;
}

void complex::Simplify(block* n)
{
//   cout << "BEGIN : Size0: " << n->cells[0].Size() << "   Size1: " << n->cells[1].Size() << "   Size2: " << n->cells[2].Size()<< endl << endl;   

   cell_iter cit;
   
   ncell* b;
   ncell* c;
   int i;
   for(int j=0; j<DIM-2; ++j)
     {
	i=DIM-j-1;
	cit=n->cells[i].Begin();
        while (cit!=n->cells[i].End())
          {
	    b=(ncell*)Ptr(cit);
	    c=(ncell*)(b->FindInvertibleCobdryElement());
	     
//	    b->Cobdry()->Print(); 
//	    cout << c << endl;
	     
	    if (c!=NULL)
	      {
	         b->Reduce(c);
	         ++cit;
                 n->cells[i].Remove(b);
                 n->cells[i+1].Remove(c);

		 if (b->NoSave())
		   delete b;
		 else
		   {
		      delete b->Cobdry();//->Clear();
		      delete b->Bdry();//->Clear();
		   }
                 if (c->NoSave())
		   delete c;
		 else
		   {
		      delete c->Bdry();//->Clear();
		      delete c->Cobdry();//->Clear();
		   }
              }
            else
	      ++cit;
	  }
     }
   
   edge* be;
//   cout << "Size0: " << n->cells[0].Size() << "   Size1: " << n->cells[1].Size() << "   Size2: " << n->cells[2].Size() << endl << endl;
   cit=n->cells[1].Begin();
   while (cit!=n->cells[1].End())
     {
        be=(edge*)Ptr(cit);
	c=(ncell*)(be->FindInvertibleCobdryElement());
//	cout << "c: " << c << endl;
	if (c!=NULL)
	  {
	     be->Reduce(c);
	     ++cit;
             n->cells[1].Remove(be);
             n->cells[2].Remove(c);

	     if (be->NoSave())
	       delete be;
	     else
	       delete be->Cobdry();//->Clear();
             if (c->NoSave())
	       delete c;
	     else
	       {
    	          delete c->Bdry();//->Clear();
		  delete c->Cobdry();//->Clear();
	       }
          }
        else
	  ++cit;
      }
   
   edge* cbe;
   vertex* bv;
//   cout << "Size0: " << n->cells[0].Size() << "   Size1: " << n->cells[1].Size() << "   Size2: " << n->cells[2].Size() << endl << endl;   
   cit=n->cells[0].Begin();
   while (cit!=n->cells[0].End())
     {
        bv=(vertex*)Ptr(cit);
//	cout << bv << endl;
//	bv->Cobdry()->Print();
	cbe=(edge*)(bv->FindInvertibleCobdryElement());
	if (cbe!=NULL)
	  {
	     bv->Reduce(cbe);
	     ++cit;
             n->cells[0].Remove(bv);
             n->cells[1].Remove(cbe);

	     delete bv;

             if (cbe->NoSave())
	       delete cbe;
	     else
	       delete cbe->Cobdry();//->Clear();
          }
        else
	  ++cit;
      }   
//      cout << "END : Size0: " << n->cells[0].Size() << "   Size1: " << n->cells[1].Size() << "   Size2: " << n->cells[2].Size() << endl << endl;   
}
