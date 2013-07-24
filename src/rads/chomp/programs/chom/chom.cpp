// Copyright (C) 2004 by William D Kalies. All rights reserved.
//
//-----------------------------------------------------------------------------
// chom.cc:
//-----------------------------------------------------------------------------
/*! \file chom.cc
    \brief Main

    Version 2.0
 
    This file contains main.
    Last modified by BK on 12/18/01.
 */

#include "common.h"
#include "list.h"
#include "bitcodes.h"
#include "cell.h"
#include "edge.h"
#include "vertex.h"
#include "ncell.h"
#include "block.h"
#include "complex.h"

//#define PRINT_SIZE

int gen_flag[DIM+1];
int cube_bits=0;
int top_leaf=0;
int GEN_TOL=0;

int Power2(int exponent)
{
   int i,answer=1;
   for (i=0;i<exponent;answer*=2,i++);
   return(answer);
}

void PrepRead(ifstream* in)
{
   int dim;
   (*in) >> dim;
   if (dim!=DIM)
     {
	cout << "Wrong dimension!" << endl << endl;
	exit(1);
     }
   (*in) >> cube_bits;
   (*in) >> top_leaf;
  
   cout << "Reading " << top_leaf << " cubes with " << cube_bits << " bits... " << endl;
}


block* Read(complex* c, ifstream* in)
{
   int bit;
   block* b=new block(cube_bits,c);
   for (char j=0; j<cube_bits; ++j)
     {
	(*in) >> bit;
	BC(b)(cube_bits-j-1,bit);
     }
   b->CreateCells(c);
   return(b);
}

/*
void use()
{
   cout << endl;
   int mem=0;
   int total_mem=0;
   cout << "#vertices: " << vertex::counter << " total: " << vertex::max << endl;
   cout << "#edges: " << edge::counter << " total: " << edge::max << endl;
   cout << "#ncells: " << ncell::counter << " total: " << ncell::max << endl;
   cout << "#blocks: " << block::counter << " total: " << block::max << endl;
   cout << "#nodes: " << node::counter << " total: " << node::max << endl;
   cout << "#bitcodes: " << bitcode::counter << " total: " << bitcode::max << endl;
//   cout << "#bdry_elems: " << bdry_elem::counter << " total: " << bdry_elem::max << endl;
   mem+=vertex::counter*(sizeof(vertex)-sizeof(node)-sizeof(bitcode));
   mem+=edge::counter*sizeof(edge);
   mem+=ncell::counter*sizeof(ncell);
   mem+=block::counter*(sizeof(block)-sizeof(node)-sizeof(bitcode));
   mem+=node::counter*sizeof(node);
   mem+=bitcode::counter*sizeof(bitcode);
   mem+=bitcode::counter*cube_bits/8;
//   mem+=bdry_elem::counter*sizeof(bdry_elem);
   total_mem+=vertex::max*(sizeof(vertex)-sizeof(node)-sizeof(bitcode));
   total_mem+=edge::max*sizeof(edge);
   total_mem+=ncell::max*sizeof(ncell);
   total_mem+=block::max*(sizeof(block)-sizeof(node)-sizeof(bitcode));
   total_mem+=node::max*sizeof(node);
   total_mem+=bitcode::max*sizeof(bitcode);
   total_mem+=bitcode::max*cube_bits/8;
//   total_mem+=bdry_elem::max*sizeof(bdry_elem);
   cout << "Memory: " << mem << "  Total memory: " << total_mem << endl;
   cout << endl;
}
*/

void out_of_store()
{
   cerr << "Operator new failed: out of store.\n";
   exit(1);
}

int main(int argc, char *argv[])
{
   time_t start_time=time(NULL);

   // Call out_of_store if new fails
   set_new_handler(&out_of_store);
   cout << endl;

   cout << "Dimension is " << DIM << endl << endl;

   for(int i=0; i<DIM+1; ++i)
     gen_flag[i]=0;

   ifstream* in=NULL;
   ofstream* gout=NULL;
   ofstream* marcio_out=NULL;

   int g;
   int t;
   int vflag=0;
   int i=0;
   while ((++i)<argc)            // scan options
     {
        if (argv[i][0]=='-')     // needs to start with '-'
	  {
	     switch (argv[i][1])
	       {
                 case 'i':       // input file name
		  if (++i<argc)
		    {
		       in=new ifstream(argv[i]);
		    }
		  break;

                 case 'b':       // betti numbers output file name
		  if (++i<argc)
		    {
		       marcio_out=new ofstream(argv[i]);
		    }
		  break;

		case 'o':       // generator output file name
		  if (++i<argc)
		    {
		       gout=new ofstream(argv[i]);
		    }
		  break;

		case 'g':        // generator
		  if (++i<argc)
		    {
		       g=atoi(argv[i]);
		       if ((g<DIM)&&(g>0))
			 {
			    cout << "Generators of dimension " << g << " will be output." << endl << endl;
		            gen_flag[g]=1;
			 }
		       else
			 cout << "Ignoring bad generator flag: " << g << endl << endl;
		    }
		  break;

		case 'v':
		  vflag=1;
		  break;

		case 't':        // generator
		  if (++i<argc)
		    {
		       t=atoi(argv[i]);
		       if (t>0)
			 {
			    cout << "Only generators of size no less than " << t << " will be output." << endl << endl;
		            GEN_TOL=t;
			 }
		       else
			 cout << "Ignoring bad generator tolerance: " << t << endl << endl;
		    }
		  break;

		default:
		  ++i;
		  break;
	       }
	  }
     }

   int gflag=0;
   for(int i=1; i<DIM; ++i)
     if (gen_flag[i])
       gflag=1;

   if (in==NULL)
     {
	cout << "No input file specified -- default is in.dat" << endl << endl;
        in=new ifstream("in.dat");
     }

   if (marcio_out==NULL)
     {
	cout << "No Betti number file specified -- default is hom.dat" << endl << endl;
        marcio_out=new ofstream("hom.dat");
     }

   if ((gout==NULL)&&(gflag))
     {
	cout << "No output file specified -- default is gen.dat" << endl << endl;
        gout=new ofstream("gen.dat");
     }

   block* nb=new block;
   complex c(nb);

   PrepRead(in);

   block* b;
   for (int i=0; i<top_leaf; ++i)
     {
//	cout << "i: " << i << endl;
	if ((!(i%10000))&&(i!=0))
	  {
	     if (vflag)
	       {
	          cout << "i: " << i;
//	          use();
	       }
	     else
	       cout << i << "\r";
	     flush(cout);
	  }
        b=Read(&c,in);
        c.InsertCube(b);
     }

   cout << endl;

   if (vflag)
     {
        cout << "Before final cube: " << endl;
//        use();
     }

   c.FinalCube();

   if (vflag)
     {
        cout << "After final cube: " << endl;
//        use();
     }

   c.Homology(gout, marcio_out);

   marcio_out->close();
   
//   c.PrintCubes();
//   c.PrintVerts();

#ifdef PRINT_SIZE

   // Print size of various types
   cout << "\n";
   cout << "Size of cell: " << sizeof(cell) << "\n";
   cout << "Size of vertex: " << sizeof(vertex) << "\n";
   cout << "Size of edge: " << sizeof(edge) << "\n";
   cout << "Size of ncell: " << sizeof(ncell) << "\n";
   cout << "Size of void*: " << sizeof(void*) << "\n";
   cout << "Size of bdry_elem: " << sizeof(bdry_elem) << "\n";
   cout << "Size of bdry_list: " << sizeof(bdry_list) << "\n";
   cout << "Size of cobdry_list: " << sizeof(cobdry_list) << "\n";
   cout << "Size of bitcode: " << sizeof(bitcode) << "\n";
   cout << "Size of block: " << sizeof(block) << "\n";
   cout << "Size of node: " << sizeof(node) << "\n";
   cout << "\n";

#endif

   time_t end_time=time(NULL);

   cout << endl << "Time used: " << difftime(end_time,start_time) << " seconds = " << difftime(end_time,start_time)/60 << " minutes." << endl;

   cout << "\n";
}
